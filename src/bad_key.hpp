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
 */

/**
 * \file bad_key.hpp
 * \brief Defines the bad_key exception type
 *
 * This file defines the exception type bad_key.
 */

#ifndef CONFY_BAD_KEY_HPP
#define CONFY_BAD_KEY_HPP

#ifdef CPORTA
#  ifndef USE_CXX17
#    define USE_CXX17
#  endif
#endif

#ifdef USE_CXX17
#  include <experimental/filesystem>
#  define filesystem experimental::filesystem
#else
#  include <filesystem>
#endif
#include <exception>
#include <string>

/**
 * \brief The exception type throw when a duplicate key is encountered.
 *
 * This type is the exception type thrown when there key -> value projection would be ambiguous,
 * i.e. there would be a key from two distinct entries.
 * The exception keeps track of the ill-formed file that caused the exception, and the key.
 */
struct bad_key : std::exception {
    /**
     * \brief The constructor for the bad_key type
     *
     * Constructs the exception type. Takes the exception causing key as a string, and the
     * ill-formed file to produce a meaningful error message.
     * This message is built at construction.
     *
     * \param key The offending duplicate key
     * \param file The file containing the ill-formed confy configuration
     */
    bad_key(std::string key, std::filesystem::path file) noexcept;

    /**
     * \brief The error message of the exception
     *
     * Returns the stored error message. The message is returned as a C-string, and is a pointer to
     * the internal string buffer containing the message. Do not free!
     *
     * \return The nicely formatted error message. Its lifetime is bound to the lifetime of the
     * exception object.
     */
    const char*
    what() const noexcept override;

private:
    std::string _buf;            ///< Buffer containing the error message
    std::string _key;            ///< The erroneous key
    std::filesystem::path _file; ///< The ill-formed file's path
};

#endif
