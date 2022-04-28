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
 * \file bad_syntax.hpp
 * \brief Defines the bad_syntax exception type
 *
 * This file defines the exception type `bad_syntax`.
 */

#ifndef CONFY_BAD_SYNTAX_HPP
#define CONFY_BAD_SYNTAX_HPP

#include <exception>
#include <filesystem>
#include <string>

/**
 * \brief The exception type thrown on syntax errors.
 *
 * This is the type of exception thrown when a syntax error is encountered in a configuration file.
 * The exception keeps track of the ill-formed configuration file, the line that was ill-formed, and
 * the line and column numbers of the offending character(s).
 */
struct bad_syntax : std::exception {
    /**
     * \brief The exception constructor for the bad_syntax type.
     *
     * Constructs the exception type. Takes the ill-formed file, the line and column of the
     * offending character, and the file containing the ill-formed line.
     * Using these an error message is built which is displayed using the what() function.
     *
     * \param line The ill-formed line in the configuration. Used in the error message.
     * \param ln The line number of the unparseable character.
     * \param col The column number of the unparseable character.
     * \param file The file currently parsed which turned out to be ill-formed.
     */
    bad_syntax(std::string line, int ln, int col, std::filesystem::path file) noexcept;

    /**
     * \brief The error message getter
     *
     * Returns the stored error message.
     * The message is returned in a C-string referencing this objects internal buffer, so it is only
     * valid as the object is alive. DO NOT FREE!
     *
     * \return The error message in a C-string.
     */
    [[nodiscard]] const char*
    what() const noexcept override;
};

#endif
