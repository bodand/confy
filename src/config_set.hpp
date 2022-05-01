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

#include <filesystem>
#include <istream>
#include <string>
#include <string_view>

#include "parser.hpp"

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
    config_set(const std::filesystem::path& file);

    /**
     * \brief Reads the configuration from a stream
     *
     * Reads a configuration from a stream.
     * While this allows any generic istream to be used as an input, the name of the file will be
     * lost, therefore, this will result in lower-quality error diagnostics.
     *
     * \param strm The stream to read from
     */
    config_set(std::istream& strm);

    template<class T>
    [[nodiscard]] std::conditional_t<std::is_trivially_copyable_v<T>, T, const T&>
    get(std::string_view key) const;

    /**
     * \brief Getter for the size of the configuration set
     *
     * Returns the number of entries in the loaded configuration
     *
     * \return The size of the configuration set
     */
    [[nodiscard]] std::size_t
    size() const noexcept;
};

#endif
