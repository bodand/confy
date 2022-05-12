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
 * \file test.user_modes.cpp
 * \brief Provides tests cases for the user interaction functions
 */

#ifdef CPORTA
#  ifndef USE_CXX17
#    define USE_CXX17
#  endif
#endif

#ifdef USE_CXX17
#  include <experimental/filesystem>
#  include <experimental/string_view>
#  define filesystem experimental::filesystem
#  define string_view experimental::string_view
#else
#  include <filesystem>
#  include <string_view>
#endif
#include <string>
#include <type_traits>

#include "bad_key.hpp"
#include "bad_syntax.hpp"
#include "user_modes.hpp"

using namespace std::literals;

#include "capture_stdio.hpp"
#include "gtest_lite.h"

void
test_user_modes() {
    TEST(user_modes, inter_invalid_file) {
        EXPECT_THROW(interactive_mode("-invalid-"), const std::invalid_argument&);
    }
    END

    TEST(user_modes, inter_ill_file) {
        EXPECT_THROW(interactive_mode("broken1.confy"), const bad_syntax&);
    }
    END

    TEST(user_modes, inter_key_clash) {
        EXPECT_THROW(interactive_mode("key_clash.confy"), const bad_key&);
    }
    END

    TEST(user_modes, cli_invalid_file) {
        EXPECT_THROW(cli_mode("-invalid-", {}), const std::invalid_argument&);
    }
    END

    TEST(user_modes, cli_ill_file) {
        EXPECT_THROW(cli_mode("broken1.confy", {}), const bad_syntax&);
    }
    END

    TEST(user_modes, cli_key_clash) {
        EXPECT_THROW(cli_mode("key_clash.confy", {}), const bad_key&);
    }
    END

    TEST(user_modes, inter_empty_file) {
        for (auto&& file : {
                    "empty1.confy"s,
                    "empty2.confy"s,
                    "empty3.confy"s,
             }) {
            auto written = capture_stream<&std::cout>([&file] {
                feed_stream<&std::cin>("", [&file] {
                    EXPECT_EQ(interactive_mode(file), 0);
                });
            });
            EXPECT_TRUE(written.empty());
        }
    }
    END

    TEST(user_modes, cli_empty_file) {
        for (auto&& file : {
                    "empty1.confy"s,
                    "empty2.confy"s,
                    "empty3.confy"s,
             }) {
            auto written = capture_stream<&std::cout>([&file] {
                EXPECT_EQ(cli_mode(file, {}), 0);
            });
            EXPECT_TRUE(written.empty());
        }
    }
    END

    TEST(user_modes, inter_single_key) {
        for (auto&& data : {
                    std::make_pair("bare_words.confy"s, "bare"s),
                    std::make_pair("double-strings.confy"s, "some"s),
                    std::make_pair("ints.confy"s, "1"s),
                    std::make_pair("mixed.confy"s, "nothing"s),
                    std::make_pair("single-strings.confy"s, "some"s),
             }) {
            auto&& file = data.first;
            auto&& value = data.second;
            auto written = capture_stream<&std::cout>([file = file] {
                feed_stream<&std::cin>("key\n", [&file] {
                    int r;
                    EXPECT_NO_THROW(r = interactive_mode(file));
                    EXPECT_EQ(r, 0);
                });
            });
            EXPECT_EQ(written, value + "\n");
        }
    }
    END

    TEST(user_modes, cli_single_key) {
        for (auto&& data : {
                    std::make_pair("bare_words.confy"s, "bare"s),
                    std::make_pair("double-strings.confy"s, "some"s),
                    std::make_pair("ints.confy"s, "1"s),
                    std::make_pair("mixed.confy"s, "nothing"s),
                    std::make_pair("single-strings.confy"s, "some"s),
             }) {
            auto&& file = data.first;
            auto&& value = data.second;
            auto written = capture_stream<&std::cout>([file = file] {
                std::vector<std::string_view> keys{"key"};
                int r;
                EXPECT_NO_THROW(r = cli_mode(file, keys));
                EXPECT_EQ(r, 0);
            });
            EXPECT_EQ(written, value + "\n");
        }
    }
    END

    TEST(user_modes, inter_multi_key) {
        for (auto&& data : {
                    std::make_tuple("bare_words.confy"s, "bare"s, "word"s),
                    std::make_tuple("double-strings.confy"s, "some"s, "have space"s),
                    std::make_tuple("ints.confy"s, "1"s, "2"s),
                    std::make_tuple("mixed.confy"s, "nothing"s, "tests"s),
                    std::make_tuple("single-strings.confy"s, "some"s, "have space"s),
             }) {
            auto&& file = std::get<0>(data);
            auto&& key1 = std::get<1>(data);
            auto&& key2 = std::get<2>(data);
            auto written = capture_stream<&std::cout>([file = file] {
                feed_stream<&std::cin>("key\nkey2", [&file] {
                    int r;
                    EXPECT_NO_THROW(r = interactive_mode(file));
                    EXPECT_EQ(r, 0);
                });
            });
            EXPECT_EQ(written, key1 + "\n" + key2 + "\n");
        }
    }
    END

    TEST(user_modes, cli_multi_key) {
        for (auto&& data : {
                    std::make_tuple("bare_words.confy"s, "bare"s, "word"s),
                    std::make_tuple("double-strings.confy"s, "some"s, "have space"s),
                    std::make_tuple("ints.confy"s, "1"s, "2"s),
                    std::make_tuple("mixed.confy"s, "nothing"s, "tests"s),
                    std::make_tuple("single-strings.confy"s, "some"s, "have space"s),
             }) {
            auto&& file = std::get<0>(data);
            auto&& key1 = std::get<1>(data);
            auto&& key2 = std::get<2>(data);
            auto written = capture_stream<&std::cout>([file = file] {
                std::vector<std::string_view> keys{"key", "key2"};
                int r;
                EXPECT_NO_THROW(r = cli_mode(file, keys));
                EXPECT_EQ(r, 0);
            });
            EXPECT_EQ(written, key1 + "\n" + key2 + "\n");
        }
    }
    END

    TEST(user_modes, inter_invalid_key) {
        for (auto&& file : {
                    "bare_words.confy"s,
                    "double-strings.confy"s,
                    "ints.confy"s,
                    "mixed.confy"s,
                    "single-strings.confy"s,
             }) {
            std::ignore = capture_stream<&std::cout>([&file] {
                feed_stream<&std::cin>("doesntexist\n", [&file] {
                    try {
                        int r = interactive_mode(file);
                        EXPECT_EQ(r, 0);
                    } catch (...) { }
                });
            });
            auto written = capture_stream<&std::cout>([&file] {
                feed_stream<&std::cin>("doesntexist\n", [&file] {
                    EXPECT_THROW(interactive_mode(file), const std::out_of_range&);
                });
            });
            EXPECT_TRUE(written.empty());
        }
    }
    END

    TEST(user_modes, cli_invalid_key) {
        for (auto&& file : {
                    "bare_words.confy"s,
                    "double-strings.confy"s,
                    "ints.confy"s,
                    "mixed.confy"s,
                    "single-strings.confy"s,
             }) {
            std::vector<std::string_view> keys{"doesntexist"};
            std::ignore = capture_stream<&std::cout>([&keys, &file] {
                std::ignore = capture_stream<&std::cerr>([&keys, &file] { // silence stderr
                    int r = cli_mode(file, keys);
                    EXPECT_EQ(r, 2);
                });
            });
            auto written = capture_stream<&std::cout>([&keys, &file] {
                std::ignore = capture_stream<&std::cerr>([&keys, &file] { // silence stderr
                    EXPECT_NO_THROW(cli_mode(file, keys));
                });
            });
            EXPECT_TRUE(written.empty());
        }
    }
    END

    TEST(user_modes, inter_trailing_invalid_key) {
        for (auto&& data : {
                    std::make_pair("bare_words.confy"s, "bare"s),
                    std::make_pair("double-strings.confy"s, "some"s),
                    std::make_pair("ints.confy"s, "1"s),
                    std::make_pair("mixed.confy"s, "nothing"s),
                    std::make_pair("single-strings.confy"s, "some"s),
             }) {
            auto&& file = data.first;
            auto&& value = data.second;
            std::ignore = capture_stream<&std::cout>([file = file] {
                feed_stream<&std::cin>("key\nerroneous", [&file] {
                    try {
                        int r = interactive_mode(file);
                        EXPECT_EQ(r, 0);
                    } catch (...) { }
                });
            });
            auto written = capture_stream<&std::cout>([file = file] {
                feed_stream<&std::cin>("key\nerroneous", [&file] {
                    EXPECT_THROW(interactive_mode(file), const std::out_of_range&);
                });
            });
            EXPECT_EQ(written, value + "\n");
        }
    }
    END

    TEST(user_modes, cli_trailing_invalid_key) {
        for (auto&& data : {
                    std::make_pair("bare_words.confy"s, "bare"s),
                    std::make_pair("double-strings.confy"s, "some"s),
                    std::make_pair("ints.confy"s, "1"s),
                    std::make_pair("mixed.confy"s, "nothing"s),
                    std::make_pair("single-strings.confy"s, "some"s),
             }) {
            auto&& file = data.first;
            auto&& value = data.second;
            std::vector<std::string_view> keys{"key", "doesntexist"};
            std::ignore = capture_stream<&std::cout>([&keys, file = file] {
                int r = cli_mode(file, keys);
                EXPECT_EQ(r, 2);
            });
            auto written = capture_stream<&std::cout>([&keys, file = file] {
                EXPECT_NO_THROW(cli_mode(file, keys));
            });
            EXPECT_EQ(written, value + "\n");
        }
    }
    END
}
