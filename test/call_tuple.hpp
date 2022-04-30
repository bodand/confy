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
 * test/inputs/type_iteration.hpp --
 *   XXX file descritption missing
 */

#ifndef CONFY_TYPE_ITERATION_HPP
#define CONFY_TYPE_ITERATION_HPP

#include <type_traits>

/**
 * \brief A trivial type list
 *
 * A type-list type.
 *
 * \tparam ... The elements in the type.
 */
template<class...>
struct tlist { };

/**
 * \brief Basic integer to type level evaulator
 * \tparam I The integer to put into type computation
 */
template<int I>
struct int_t {
    constexpr auto const static value = I;
};

/**
 * \brief Implementation function for call_tuple.
 *
 * Calls each element in the tuple using operator().
 *
 * \tparam T The type of the tuple.
 * \tparam Is Numbers 0..<size of tuple>.
 * \param t The tuple to call each value of
 */
template<class T, std::size_t... Is>
void
call_tuple_impl(const T& t, std::index_sequence<Is...>) {
    using std::get; // two-step ADL dance
    (((get<Is>(t)()), void()), ...);
}

/**
 * \brief Calls each elem in a tuple
 *
 * Goes through a given t tuple, and calls each element like a function.
 *
 * \tparam T The type of the tuple.
 * \param t The tuple to call each element of.
 */
template<class T>
void
call_tuple(const T& t) {
    call_tuple_impl(t, std::make_index_sequence<std::tuple_size_v<T>>());
}

#endif
