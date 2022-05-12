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
 * \file test.cache.cpp
 * \brief Tests the cache implementations
 */

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
#include <type_traits>

#include "caches.hpp"

using namespace std::literals;

#include "call_tuple.hpp"
#include "gtest_lite.h"

namespace {
#ifndef USE_CXX17
    template<class T>
    concept const_get_value_ptr = requires(const T t) {
                                      { t.get_value_ptr() };
                                  };
#endif

    template<class C, class T, class I>
    struct do_test {
        explicit do_test(tlist<C, T, I>) { }

        void
        operator()() const {
            /* the TEST macro doesn't suffice in these repeated list places,
             * just like Google test is garbage, needless macro spam seemingly yet again didn't lead
             * us to anywhere we wanted to go to; just use Boost.uT */
            gtest_lite::test.begin(("cache.static_checks#" + std::to_string(I::value)).c_str());
            {
                EXPECT_TRUE((std::is_constructible<C, T&&>::value));
                EXPECT_TRUE((std::is_constructible<C, decltype(std::move(std::declval<T>()))>::value));
#ifndef USE_CXX17
                EXPECT_TRUE(const_get_value_ptr<C>);
#endif
                EXPECT_TRUE(std::is_const<std::remove_pointer_t<decltype(std::declval<C>().get_value_ptr())>>::value);
            }
            gtest_lite::test.end();

            gtest_lite::test.begin(("cache.literal_value#" + std::to_string(I::value)).c_str());
            {
                auto lit = C(static_cast<T&&>(1));
                EXPECT_TRUE(lit.get_value_ptr() != nullptr);
                EXPECT_NO_THROW(lit.get_value_ptr());
            }
            gtest_lite::test.end();

            gtest_lite::test.begin(("cache.moved_value#" + std::to_string(I::value)).c_str());
            {
                T val = 0;
                auto mv = C(std::move(val));
                EXPECT_TRUE(mv.get_value_ptr() != nullptr);
                EXPECT_NO_THROW(mv.get_value_ptr());
                EXPECT_TRUE(mv.get_value_ptr() != &val);
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
test_caches() {
    auto tests = std::make_tuple(
           make_test(tlist<char_cache, char, int_t<1>>()),
           make_test(tlist<uchar_cache, unsigned char, int_t<2>>()),
           make_test(tlist<schar_cache, signed char, int_t<3>>()),
           make_test(tlist<short_cache, short, int_t<4>>()),
           make_test(tlist<ushort_cache, unsigned short, int_t<5>>()),
           make_test(tlist<int_cache, int, int_t<6>>()),
           make_test(tlist<uint_cache, unsigned, int_t<7>>()),
           make_test(tlist<long_cache, long, int_t<8>>()),
           make_test(tlist<ulong_cache, unsigned long, int_t<9>>()),
           make_test(tlist<long_long_cache, long long, int_t<10>>()),
           make_test(tlist<ulong_long_cache, unsigned long long, int_t<11>>()),
           make_test(tlist<float_cache, float, int_t<12>>()),
           make_test(tlist<double_cache, double, int_t<13>>()),
           make_test(tlist<long_double_cache, long double, int_t<14>>()),
           make_test(tlist<bool_cache, bool, int_t<15>>()));
    call_tuple(tests);
}
