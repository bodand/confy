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
 * src/user_modes.hpp --
 *   XXX file descritption missing
 */

/**
 * \file user_modes.hpp
 * \brief Declares the two functions handling user interaction
 */

#ifndef CONFY_USER_MODES_HPP
#define CONFY_USER_MODES_HPP

#ifdef CPORTA
#  ifndef USE_CXX17
#    define USE_CXX17
#  endif
#endif

#ifdef USE_CXX17
#  include <experimental/filesystem>
#  include <experimental/string_view>
#  define string_view experimental::string_view
#  define filesystem experimental::filesystem
#else
#  include <filesystem>
#  include <span>
#  include <string_view>
#endif

/**
 * \brief The interactive user mode function
 *
 * This function implements the functionality of a user-interactive session.
 *
 * \param cfg_file The configuration file to use
 * \return Exit code
 */
int
interactive_mode(const std::filesystem::path& cfg_file);

#ifdef USE_CXX17
using cli_keys_t = const std::vector<std::string_view>&;
#else
using cli_keys_t = std::span<std::string_view>;
#endif

/**
 * \brief The command line user mode function
 *
 * This function implements the mode, where the user provides the keys they want on the command line
 * and we just print the values back to them without further interaction.
 *
 * \param cfg_file The configuration file to use
 * \param keys The set of keys to read
 * \return Exit code
 */
int
cli_mode(const std::filesystem::path& cfg_file, cli_keys_t keys);

#endif
