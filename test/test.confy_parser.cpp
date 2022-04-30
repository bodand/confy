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
 * \file test.confy_parser.cpp
 * \brief Test functions for the confy_parser class
 */

#include <exception>
#include <filesystem>
#include <iterator>
#include <optional>
#include <ranges>
#include <sstream>
#include <string>
#include <tuple>
#include <type_traits>

#include "confy_parser.hpp"

using namespace std::literals;

#include "bad_syntax.hpp"
#include "gtest_lite.h"

void
test_confy_parser() {
    TEST(confy_parser, construnction) {
        EXPECT_NO_THROW(confy_parser("xcolors.confy"));
        EXPECT_NO_THROW(confy_parser(""));
    }
    END

    TEST(confy_parser, null_lines) {
        confy_parser cf("test-file");
        for (auto&& inp : {""s, "\n\n\n"s, "#comment\n#comm"s}) {
            std::istringstream ss(inp);
            EXPECT_TRUE(cf.next_line(ss) == std::nullopt);
        }
    }
    END

    TEST(confy_parser, key_value_line) {
        confy_parser cf("test-file");
        std::istringstream liness("line=val");
        EXPECT_EQ(*cf.next_line(liness), "line=val");
    }
    END

    TEST(confy_parser, exceptions) {
        std::string file("test-file");
        for (auto&& [ln, errpos] : {
                    std::pair{""s, ":1:1"s},
                    {"#comment"s, ":1:1"s},
                    {" line=bad"s, ":1:1"s},
                    {"1=bad"s, ":1:1"s},
                    {"a,b=1"s, ":1:2"s},
                    {"a=,1"s, ":1:3"s},
                    {"a='b"s, ":1:5"s},
                    {"a=\"b"s, ":1:5"}}) {
            try {
                confy_parser cf(file);
                EXPECT_THROW_THROW(std::ignore = cf.parse_line(ln), const bad_syntax&);
            } catch (const bad_syntax& ex) {
                EXPECT_NE(ex.what(), nullptr);

                std::string str(ex.what());
                auto file_off = std::search(str.begin(), str.end(), file.begin(), file.end());
                EXPECT_FALSE(file_off == str.end());

                auto ln_col = ":1:1"s;
                auto ln_col_off = std::search(str.begin(), str.end(), ln_col.begin(), ln_col.end());
                EXPECT_FALSE(ln_col_off == str.end());
            }
        }
    }
    END

    TEST(confy_parser, valid) {
        for (auto&& ln : {
                    "key=value",
                    "key='value'",
                    R"(key="value")",
             }) {
            confy_parser cf("file");

            auto [key, value] = cf.parse_line(ln);
            EXPECT_EQ("key", key);
            EXPECT_EQ("value", value);
        }
    }
    END
}
