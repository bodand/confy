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
 * \file test.uncached.cachefactory.cpp
 * \brief Implements the tests for the non-caching cache_factory implementations
 */

#include <string>
#include <string_view>
#include <type_traits>

#include "cache_factory.hpp"

using namespace std::literals;

#include "call_tuple.hpp"
#include "gtest_lite.h"

namespace {
    template<class T>
    concept has_make = requires(T t) {
                           { t.make(std::string("")) };
                       };
    template<class T>
    concept const_makeable = requires(const T t) {
                                 { t.make(std::declval<const std::string&>()) };
                             };
    template<class T, class U>
    concept makeable_rettype = requires(const T t) {
                                   { t.make(std::declval<const std::string&>()) } -> std::same_as<U>;
                               };

    template<class T, int I>
    struct do_test {
        explicit do_test(tlist<T, int_t<I>>) { }

        void
        operator()() const {
            gtest_lite::test.begin(("uncached_cachefactory.static_checks#" + std::to_string(I)).c_str());
            EXPECT_TRUE(has_make<cache_factory<T>>);
            EXPECT_TRUE(const_makeable<cache_factory<T>>);
            EXPECT_TRUE((makeable_rettype<cache_factory<T>, T>) );
            gtest_lite::test.end();

            gtest_lite::test.begin(("uncached_cachefactory.make#" + std::to_string(I)).c_str());
            cache_factory<T> sut;
            std::string data = "<data>";
            EXPECT_EQ(sut.make(data), data);
            gtest_lite::test.end();
        }
    };
}

void
test_uncached_cache_factory() {
    auto tests = std::tuple{
           do_test(tlist<std::string, int_t<1>>()),
           do_test(tlist<std::string_view, int_t<2>>()),
           do_test(tlist<const char*, int_t<3>>()),
    };
    call_tuple(tests);
}
