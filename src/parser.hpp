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
 * \file parser.hpp
 * \brief Defines the parser concept
 *
 * Defines the formalized concept of a parser that config_set may use.
 */

#ifndef CONFY_PARSER_HPP
#define CONFY_PARSER_HPP

#ifdef CPORTA
#  ifndef USE_CXX17
#    define USE_CXX17
#  endif
#endif

#ifdef USE_CXX17
#  include <experimental/optional>
#  include <experimental/string_view>
#  define string_view experimental::string_view
#  define optional experimental::optional
#else
#  include <concepts>
#  include <optional>
#  include <string_view>
#endif

#include <istream>
#include <string>
#include <type_traits>

#ifndef USE_CXX17

/**
 * \brief The parser concept
 *
 * This concept is used to check whether a given type meets the formal requirements of being a
 * parser type.
 *
 * \tparam T The type to check.
 */
template<class T>
concept parser = requires(T t) {
                     { t.next_line(std::declval<std::istream&>()) } -> std::same_as<std::optional<std::string>>;
                     { t.parse_line(std::declval<std::string_view>()) } -> std::same_as<std::pair<std::string, std::string>>;
                 };

#endif

#endif
