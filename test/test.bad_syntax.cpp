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
 * \file test.bad_syntax.cpp
 * \brief Test functions for the bad_syntax class
 */

#include <exception>
#include <string>
#include <type_traits>

#include "bad_syntax.hpp"

using namespace std::literals;

#include "gtest_lite.h"

void
test_bad_syntax() {
    TEST(bad_syntax, static_checks) {
        EXPECT_TRUE((std::is_base_of<std::exception, bad_syntax>::value) );
        using fs_path = std::filesystem::path; // gtest_lite macros do not expand their parameters
        EXPECT_TRUE((std::is_nothrow_constructible<bad_syntax, std::string, int, int, fs_path>::value) );
    }
    END

    TEST(bad_syntax, path_nonempty) {
        auto sut = bad_syntax("erratic line", 42, 69, std::filesystem::path("some/file/in/dirs.txt"));

        auto err = sut.what();
        EXPECT_TRUE(nullptr != err);
        if (!err) return;

        auto err_str = std::string(err);
        for (auto&& exp : {
                    "erratic line"s,
                    "^--HERE"s,
                    "some/file/in/dirs.txt"s,
                    "42"s,
                    "69"s,
             }) {
            // EXPECT_NE doesn't compile here
            EXPECT_TRUE((std::search(err_str.begin(), err_str.end(), exp.begin(), exp.end())) != err_str.end());
        }
    }
    END

    TEST(bad_syntax, empty_path) {
        auto sut = bad_syntax("erratic line", 42, 69, std::filesystem::path());

        auto err = sut.what();
        EXPECT_TRUE(nullptr != err);
        if (!err) return;

        auto err_str = std::string(err);
        for (auto&& exp : {
                    "erratic line"s,
                    "^--HERE"s,
                    "<unknown file>"s,
                    "42"s,
                    "69"s,

             }) {
            // EXPECT_NE doesn't compile here
            EXPECT_TRUE((std::search(err_str.begin(), err_str.end(), exp.begin(), exp.end())) != err_str.end());
        }
    }
    END
}
