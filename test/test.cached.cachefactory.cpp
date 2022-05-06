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
 * \file test.cached.cachefactory.cpp
 * \brief Implements the tests for the caching cache_factory implementations
 */

#include <string>
#ifndef CPORTA
#  include <string_view>
#else
#  include <experimental/string_view>
#  define string_view experimental::string_view
#endif
#include <type_traits>

#include "cache_factory.hpp"
#include "cache_visitor_for.hpp"

using namespace std::literals;

#include "call_tuple.hpp"
#include "gtest_lite.h"

namespace {
    constexpr const char* const invalid_input = "#";
    template<class>
    constexpr const char* const valid_input = "42";
    template<>
    constexpr const char* const valid_input<float> = "4.2";
    template<>
    constexpr const char* const valid_input<double> = "4.2";
    template<>
    constexpr const char* const valid_input<long double> = "4.2";
    template<>
    constexpr const char* const valid_input<bool> = "1";

    template<class T>
    constexpr const auto valid_value = T(42);
    template<>
    constexpr const auto valid_value<float> = 4.2f;
    template<>
    constexpr const auto valid_value<double> = 4.2;
    template<>
    constexpr const auto valid_value<long double> = 4.2l;
    template<>
    constexpr const auto valid_value<bool> = true;

    template<class T, class I>
    struct do_test {
        explicit do_test(tlist<T, I>) { }

        void
        operator()() const {
            gtest_lite::test.begin(("cached_cachefactory.static_checks#" + std::to_string(I::value)).c_str());
            EXPECT_TRUE(std::is_default_constructible<cache_visitor_for<T>>::value);
            gtest_lite::test.end();

            gtest_lite::test.begin(("cached_cachefactory.construct#" + std::to_string(I::value)).c_str());
            cache_factory<T> sut;
            EXPECT_TRUE(sut.construct(invalid_input) == nullptr); // expect_eq requires copyable types
            EXPECT_TRUE(sut.construct(valid_input<T>) != nullptr);
            gtest_lite::test.end();

            gtest_lite::test.begin(("cache_vtor_for.T_validity#" + std::to_string(I::value)).c_str());
            cache_visitor_for<T> vtor;
            EXPECT_FALSE(vtor.valid());
            gtest_lite::test.end();

            auto value = sut.construct(valid_input<T>);
            value->accept(vtor);

            gtest_lite::test.begin(("cache_vtor_for.T_visitation#" + std::to_string(I::value)).c_str());
            EXPECT_TRUE(vtor.valid());
            if (std::is_integral<T>::value) {
                EXPECT_EQ(vtor.value(), valid_value<T>);
            } else {
                EXPECT_DOUBLE_EQ(vtor.value(), valid_value<T>);
            }
            gtest_lite::test.end();
        }
    };

    template<class... Args>
    do_test<Args...>
    make_test(tlist<Args...> typs) {
        return do_test<Args...>(typs);
    }
}

void
test_cached_cache_factory() {
    auto tests = std::make_tuple(
           make_test(tlist<char, int_t<1>>()),
           make_test(tlist<signed char, int_t<2>>()),
           make_test(tlist<unsigned char, int_t<3>>()),
           make_test(tlist<int, int_t<4>>()),
           make_test(tlist<unsigned, int_t<5>>()),
           make_test(tlist<short, int_t<6>>()),
           make_test(tlist<unsigned short, int_t<7>>()),
           make_test(tlist<long, int_t<8>>()),
           make_test(tlist<unsigned long, int_t<9>>()),
           make_test(tlist<long long, int_t<10>>()),
           make_test(tlist<unsigned long long, int_t<11>>()),
           make_test(tlist<float, int_t<12>>()),
           make_test(tlist<double, int_t<13>>()),
           make_test(tlist<long double, int_t<14>>()),
           make_test(tlist<bool, int_t<15>>()));
    call_tuple(tests);
}
