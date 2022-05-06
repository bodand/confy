/* -- confy project --
 *
 * Copyright (c) 2022 András Bodor <bodand@pm.me>
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * - Redistributions of source code must retain the above copyright notice, this
 *   list of conditions and the following disclaimer.
 *
 * - Redistributions in binary form must reproduce the above copyright notice,
 *   this list of conditions and the following disclaimer in the documentation
 *   and/or other materials provided with the distribution.
 *
 * - Neither the name of the copyright holder nor the names of its contributors
 *   may be used to endorse or promote products derived from this software
 *   without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 * src/config_set.hpp --
 *   XXX file description missing
 */

/**
 * \file config_set.hpp
 * \brief Defines the main entry into the library: config_set
 *
 * The config_set main library API is defined in this file.
 */

#ifndef CONFY_CONFIG_SET_HPP
#define CONFY_CONFIG_SET_HPP

#ifdef CPORTA
#  include <experimental/filesystem>
#  include <experimental/string_view>
#  define string_view experimental::string_view
#  define filesystem experimental::filesystem
#else
#  include <filesystem>
#  include <string_view>
#endif

#include <cstring>
#include <fstream>
#include <istream>
#include <numeric>
#include <string>
#include <vector>

#include "bad_key.hpp"
#include "config.hpp"
#include "parser.hpp"

#ifdef CPORTA
#  define parser class
#endif

/**
 * \brief The class collecting a file's worth of configurations
 *
 * The main API of the library.
 * Provides a way to read in a configuration from a file, or a generic stream.
 * After reading and parsing, the key-value entries will be made available to the user for query in
 * a read-only fashion.
 *
 * \tparam P The type of the parser object to parse configuration with
 */
template<parser P>
struct config_set {
    /**
     * \brief Reads the configuration from a file
     *
     * Reads a configuration from a file on disk.
     * When using this constructor, error messages will contain the name of the file given here, for
     * easier interpretation of error diagnostics.
     *
     * \param file The configuration file
     */
    explicit config_set(const std::filesystem::path& file)
         : _file(file) {
        std::ifstream ifs(file);
        if (!ifs.is_open())
            throw std::invalid_argument("invalid_file " + _file.string());
        parse_stream(ifs);
    }

    /**
     * \brief Reads the configuration from a stream
     *
     * Reads a configuration from a stream.
     * While this allows any generic istream to be used as an input, the name of the file will be
     * lost, therefore, this will result in lower-quality error diagnostics.
     *
     * \param strm The stream to read from
     */
    explicit config_set(std::istream& strm) {
        parse_stream(strm);
    }

    template<class T>
    auto
    get(std::string_view key) const {
        const auto& cfg = _configs;

        return callback_binary_search(
               _configs,
               [&key](const config& cfg) {
                   return std::strcmp(cfg.get_key().data(), key.data());
               },
               [&cfg](std::size_t, std::size_t middle, std::size_t) -> decltype(auto) {
                   return cfg[middle].template get_as<T>();
               },
               [&key](auto&&...) -> T {
                   throw std::out_of_range("invalid key looked up: " + std::string(key));
               });
    }

    /**
     * \brief Getter for the size of the configuration set
     *
     * Returns the number of entries in the loaded configuration
     *
     * \return The size of the configuration set
     */
    std::size_t
    size() const noexcept { return _configs.size(); }

private:
    void
    parse_stream(std::istream& strm) {
        auto parse = P(_file);
        std::optional<std::string> maybe_next_ln;
        while ((maybe_next_ln = parse.next_line(strm))) {
            auto next_ln = maybe_next_ln.value();
            auto conf = parse.parse_line(next_ln);
            emplace_config(std::move(conf.first), std::move(conf.second));
        }
    }

    /**
     * \brief Universal callback-based binary search
     *
     * A callback oriented binary search. Searches the given vec vector and calls succ if it
     * successfully finds an equal value, according to the given unary predicate cmpr.
     * If it doesn't find anything searched, it calls fail.
     * In all cases returns the return values of the callbacks, so they must have an equal return
     * type.
     *
     * \tparam T The type of the container to search.
     * \tparam CmprFn The type of the comparison unary predicate.
     * \tparam SuccFn The type of the success function.
     * \tparam FailFn The type of the failure function.
     * \param vec The container to search in. Requires to have random access support.
     * \param cmpr The unary predicate, that provides three way comparison.
     * \param succ The function called if the search succeeds.
     * \param fail The function called if the search fails.
     * \return The return value of the called completion function.
     */
    template<class T, class CmprFn, class SuccFn, class FailFn>
    static auto
    callback_binary_search(const T& vec,
                           CmprFn&& cmpr,
                           SuccFn&& succ,
                           FailFn&& fail) {
        std::size_t begin = 0;
        std::size_t middle;
        std::size_t end = vec.size();
        while (begin != end) {
#ifndef CPORTA
            middle = std::midpoint(begin, end);
#else
            middle = (end + begin) / 2;
#endif
            auto dir = std::forward<CmprFn>(cmpr)(vec[middle]);
            if (dir == 0) {
                return std::forward<SuccFn>(succ)(begin, middle, end);
            } else if (dir < 0) {
                begin = middle + 1;
            } else if (dir > 0) {
                end = middle;
            }
        }
        return std::forward<FailFn>(fail)(begin, middle, end);
    }

    void
    emplace_config(std::string&& name, std::string&& value) {
        callback_binary_search(
               _configs,
               [&name](const config& cfg) {
                   // C++20: cfg.get_key() <=> name;
                   return std::strcmp(cfg.get_key().data(), name.c_str());
               },
               [&name, this](auto...) {
                   throw bad_key(name, _file);
               },
               [this, &name, &value](std::size_t, std::size_t, std::size_t end) {
                   _configs.emplace(std::next(_configs.begin(), static_cast<std::ptrdiff_t>(end)), name, value);
               });
    }

    std::filesystem::path _file{}; ///< The currently used file's path
    std::vector<config> _configs;  ///< The set of configurations stored
};

#endif
