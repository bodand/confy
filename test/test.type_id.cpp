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
 * \file test.type_id.cpp
 * \brief Test functions for the type_id classes
 */

#include <type_traits>

#include "gtest_lite.h"
#include "type_id.hpp"

#ifdef CPORTA
#  ifndef USE_CXX17
#    define USE_CXX17
#  endif
#endif

#ifndef USE_CXX17
namespace {
    template<class T>
    concept const_equality_comparable = requires(const T t) {
                                            { t == std::declval<T>() } -> std::same_as<bool>;
                                            { std::declval<T>() == t } -> std::same_as<bool>;
                                        };
    template<class T>
    concept const_inequality_comparable = requires(const T t) {
                                              { t != std::declval<T>() } -> std::same_as<bool>;
                                              { std::declval<T>() != t } -> std::same_as<bool>;
                                          };
}
#endif

void
test_type_id() {
    using T = int;
    using U = unsigned short int;

    TEST(type_id, injection) {
        EXPECT_TRUE(type_id::id_of<T>() == type_id::id_of<T>());
        EXPECT_TRUE(type_id::id_of<T>() != type_id::id_of<U>());
    }
    END

    // static checks
    TEST(type_id, static_checks) {
#ifndef USE_CXX17
        EXPECT_TRUE(const_equality_comparable<type_id>);
        EXPECT_TRUE(const_inequality_comparable<type_id>);
#endif
        EXPECT_TRUE(noexcept(std::declval<type_id>() == std::declval<type_id>()));
        EXPECT_TRUE(noexcept(std::declval<type_id>() != std::declval<type_id>()));
    }
    END
}
