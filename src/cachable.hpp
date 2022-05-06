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
 * \file cachable.hpp
 * \brief The file for the cachable concept.
 *
 * The header file defining the cachable concept, requiring all things that an object that is
 * cachable needs to have.
 *
 */

#ifndef CONFY_CACHABLE_HPP
#define CONFY_CACHABLE_HPP

#ifndef CPORTA
#  include <concepts>
#endif
#include <memory>
#include <string>
#include <type_traits>

#include "cache_factory.hpp"

#ifndef CPORTA

/**
 * \brief The cachable concept
 *
 * This concept is used to determine if a type fulfills the requirements to be cached.
 *
 * \tparam T The type to check.
 */
template<class T>
concept cachable =
       requires(cache_factory<T> c) {
           typename cache_factory<T>::cache_type;
           { c.construct(std::declval<const std::string&>()) } -> std::same_as<std::unique_ptr<cache>>;
       };

#else

// hacky non-C++20 implementation, officially this doesn't exist. Workaround JPorta
// only checks for the construct member fn. to exist

/**
 * \brief JPorta workaround cachable
 *
 * This is a workaround JPorta only being able to compile code in C++17 mode.
 * Implemements checking for the construct member function on the given T type.
 * Uses the classic SFINAE detection idiom.
 *
 * \tparam T The type to check.
 */
template<class T>
class jporta_cachable {
    using True = char;
    using False = char[2];

    template<class C>
    static auto
           test_impl(std::nullptr_t) -> decltype(std::declval<C>().construct(std::declval<const std::string&>()), True{});
    template<class>
    static False&
    test_impl(...)  ;

public:
    constexpr static bool value = sizeof(typename std::decay<decltype(test_impl<T>(nullptr))>::type) == sizeof(True);
};

/**
 * \brief JPorta workaround cachable helper
 *
 * Helper variable template for the workaround implementation.
 *
 * \tparam T The type to check.
 */
template<class T>
constexpr static bool cachable = jporta_cachable<cache_factory<T>>::value;

#endif

#endif
