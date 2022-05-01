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

#include <memory>
#include <string>
#include <string_view>

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
    [[nodiscard]] std::string_view
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
    [[nodiscard]] std::conditional_t<std::is_trivially_copyable_v<T>, T, const T&>
    get_as() const;
};

#endif
