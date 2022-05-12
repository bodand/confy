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
 * src/config.hpp --
 *   XXX file description missing
 */

/**
 * \file config.hpp
 * \brief A single configuration entry
 *
 * This file defines the config class, which stores a single key-value entry.
 */

#ifndef CONFY_CONFIG_HPP
#define CONFY_CONFIG_HPP

#if defined(CPORTA) && !defined(USE_CXX17)
#  define USE_CXX17
#endif

#include <memory>
#include <stdexcept>
#include <string>
#ifdef USE_CXX17
#  include <experimental/string_view>
#  define string_view experimental::string_view
#else
#  include <string_view>
#endif

#include "cache_factory.hpp"
#include "cache_visitor_for.hpp"
#include "caches.hpp"

#ifdef cachable
#  undef cachable
#endif
#include "cachable.hpp"

/**
 * \brief Key-value config entry
 *
 * A class that stores a single key-value entry in the system.
 */
struct config {
    /**
     * \brief Constructs a key-value entry
     *
     * Takes a key and value pair, and creates a valid config entry for storage.
     *
     * \param name The key part of the entry
     * \param value The value part of the entry
     */
    config(std::string name, std::string value);

    /**
     * \brief Returns the key
     *
     * A getter for the key part of the configuration entry.
     *
     * \return The name of the entry
     */
    std::string_view
    get_key() const noexcept;

    /**
     * \brief Get the value of the entry
     *
     * A getter for the configuration entry.
     * The value is parsed into the requested type.
     * Caching is implemented, so multiple queries to the same type will not calculate the process
     * again, if asked in direct succession.
     *
     * \tparam T The type to parse the value into
     * \return The parsed value
     */
    template<class T>
    auto
    get_as() const {
        return get_as_impl<T, cachable<T>>::get(_value, _cache);
    }

private:
    template<class T, bool>
    struct get_as_impl;

    template<class T>
    struct get_as_impl<T, false> {
        static auto
        get(const std::string& value, std::unique_ptr<cache>&) {
            auto cf = cache_factory<T>();
            return cf.make(value);
        }
    };

    template<class T>
    struct get_as_impl<T, true> {
        static auto
        get(const std::string& value, std::unique_ptr<cache>& _cache) {
            if (_cache) {
                cache_visitor_for<T> vtor;
                _cache->accept(vtor);
                if (vtor.valid()) return vtor.value();
            }
            auto cf = cache_factory<T>();
            _cache = cf.construct(value);
            if (!_cache) throw std::invalid_argument("requested type couldn't be constructed");

            cache_visitor_for<T> vtor;
            _cache->accept(vtor);
            if (!vtor.valid()) throw std::runtime_error("unknown error occurred fetching config");
            return vtor.value();
        }
    };

    std::string _name;                     ///< The name, or key, of the config entry stored
    std::string _value;                    ///< The value of the config entry
    mutable std::unique_ptr<cache> _cache; ///< The cache used to speed up conversions to types
};

#endif
