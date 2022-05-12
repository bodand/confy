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
 * \brief Defines the confy language parser type
 *
 * This file contains the definition of the confy_parser class, which provides the confy language
 * parser to the other parts of the library.
 * Can be swapped with anything to easily change the grammar the compiled binary uses.
 */

#ifndef CONFY_CONFY_PARSER_HPP
#define CONFY_CONFY_PARSER_HPP

#ifdef CPORTA
#  ifndef USE_CXX17
#    define USE_CXX17
#  endif
#endif

#ifdef USE_CXX17
#  include <experimental/filesystem>
#  include <experimental/optional>
#  include <experimental/string_view>
#  define filesystem experimental::filesystem
#  define string_view experimental::string_view
#  define optional experimental::optional
#  define nullopt experimental::nullopt
#else
#  include <filesystem>
#  include <optional>
#  include <string_view>
#endif
#include <string>
#include <utility>

/**
 * \brief The confy grammar parser.
 *
 * This class implements the confy language grammar.
 * It is used by the config_set class to parse the provided file.
 *
 * May be replaced by any class with an equivalent interface, that conforms to the parser concept
 * and config_set will be happy to use it.
 */
struct confy_parser {
    /**
     * \brief The confy_parser constructor.
     *
     * Constructs a confy_parser object and saves the file reference.
     * This reference is not copied, and MUST LIVE THROUGHOUT THE LIFE OF THE PARSER OBJECT.
     * It is used as a diagnostic when errors occur.
     *
     * \param file The file we are currently reading. May be empty.
     */
    confy_parser(const std::filesystem::path& file) noexcept;

    /**
     * \brief Parses the stream and returns the next valid line.
     *
     * Goes through a given stream until it encounters a non-empty or comment line, or the end of
     * the stream.
     * If it found a key-value line, it returns that.
     * If it reached the end of file mark, it returns `std::nullopt`.
     *
     * \param strm The stream to read from.
     * \return The next non-empty line, or `std::nullopt` if already at EOF.
     */
    std::optional<std::string>
    next_line(std::istream& strm) const;

    /**
     * \brief Parses a key-value line into a key and a value.
     *
     * Takes a line that is assumed to be a key and value pair.
     * It takes a valid C ASCII identifier without spaces on the left side of an equals sign, then
     * any of the following:
     * - an alphanumeric stream without spaces as the value; or
     * - a single quote closed by another on the end of the line, anything between is the value; or
     * - a double quote closed by another on the end of the line, anything between is the value.
     *
     * If any strict requirement is not met, an exception is thrown.
     *
     * \param ln The line to parse
     * \return The key and value packed into a pair
     */
    std::pair<std::string, std::string>
    parse_line(std::string_view ln) const;

private:
    mutable int _ln_cnt = 1;            ///< The current line count
    const std::filesystem::path& _file; ///< The file
};

#endif
