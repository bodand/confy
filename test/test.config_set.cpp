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
 * \file test.config_set.cpp
 * \brief Tests for the config_set class
 */

#ifdef CPORTA
#  ifndef USE_CXX17
#    define USE_CXX17
#  endif
#endif

#ifdef USE_CXX17
#  include <experimental/string_view>
#  define string_view experimental::string_view
#else
#  include <string_view>
#endif
#include <string>
#include <type_traits>

#include "bad_key.hpp"
#include "bad_syntax.hpp"
#include "config_set.hpp"
#include "confy_parser.hpp"

using namespace std::literals;

#include "gtest_lite.h"

void
test_config_set() {
    using confy_set = config_set<confy_parser>; // testing our implementation

    TEST(config_set, no_file) {
        try {
            EXPECT_THROW_THROW(confy_set cs("-invalid-"), const std::invalid_argument&);
        } catch (const std::exception& ex) {
            auto invalid = "-invalid-"s;
            auto sterr = std::string(ex.what());
            EXPECT_TRUE((std::search(sterr.begin(), sterr.end(), invalid.begin(), invalid.end()) != sterr.end()));
        }
    }
    END

    TEST(config_set, broken_files) {
        for (auto&& file : {
                    "broken1.confy"s,
                    "broken2.confy"s,
                    "broken3.confy"s,
                    "broken4.confy"s,
                    "broken5.confy"s,
                    "broken6.confy"s,
             }) {
            try {
                EXPECT_THROW_THROW(confy_set cs(file), const bad_syntax&);
            } catch (const std::exception& ex) {
                EXPECT_TRUE(ex.what() != nullptr);
                auto sterr = std::string(ex.what());
                EXPECT_TRUE((std::search(sterr.begin(), sterr.end(), file.begin(), file.end()) != sterr.end()));
            }
        }
    }
    END

    TEST(config_set, broken_streams) {
        for (auto& file : {
                    "broken1.confy"s,
                    "broken2.confy"s,
                    "broken3.confy"s,
                    "broken4.confy"s,
                    "broken5.confy"s,
                    "broken6.confy"s,
             }) {
            try {
                std::ifstream ifs(file);
                EXPECT_THROW_THROW(confy_set cs(ifs), const bad_syntax&);
            } catch (const std::exception& ex) {
                auto err_file = "<unknown file>"s;
                EXPECT_TRUE(ex.what() != nullptr);
                auto sterr = std::string(ex.what());
                EXPECT_TRUE((std::search(sterr.begin(), sterr.end(), err_file.begin(), err_file.end()) != sterr.end()));
            }
        }
    }
    END

    TEST(config_set, key_clash_file) {
        auto clash_file = "key_clash.confy"s;
        try {
            EXPECT_THROW_THROW(confy_set cs(clash_file), const bad_key&);
        } catch (const std::exception& ex) {
            EXPECT_TRUE(ex.what() != nullptr);
            auto sterr = std::string(ex.what());
            auto broken_key = "BROKEN"s;
            auto dup_key = "duplicate key"s;

            EXPECT_TRUE((std::search(sterr.begin(), sterr.end(), clash_file.begin(), clash_file.end()) != sterr.end()));
            EXPECT_TRUE((std::search(sterr.begin(), sterr.end(), broken_key.begin(), broken_key.end()) != sterr.end()));
            EXPECT_TRUE((std::search(sterr.begin(), sterr.end(), dup_key.begin(), dup_key.end()) != sterr.end()));
        }
    }
    END

    TEST(config_set, key_clash_stream) {
        auto clash_file = "key_clash.confy"s;
        try {
            std::ifstream ifs(clash_file);
            EXPECT_THROW_THROW(confy_set cs(ifs), const bad_key&);
        } catch (const std::exception& ex) {
            EXPECT_TRUE(ex.what() != nullptr);
            auto sterr = std::string(ex.what());

            auto err_file = "<unknown file>"s;
            auto broken_key = "BROKEN"s;
            auto dup_key = "duplicate key"s;

            EXPECT_TRUE((std::search(sterr.begin(), sterr.end(), err_file.begin(), err_file.end()) != sterr.end()));
            EXPECT_TRUE((std::search(sterr.begin(), sterr.end(), broken_key.begin(), broken_key.end()) != sterr.end()));
            EXPECT_TRUE((std::search(sterr.begin(), sterr.end(), dup_key.begin(), dup_key.end()) != sterr.end()));
        }
    }
    END

    TEST(config_set, empty_file) {
        for (std::filesystem::path file : {
                    "empty1.confy"s,
                    "empty2.confy"s,
                    "empty3.confy"s,
             }) {
            EXPECT_NO_THROW(confy_set cs(file));

            confy_set cs(file);
            EXPECT_TRUE(cs.size() == std::size_t{});
            EXPECT_THROW(std::ignore = cs.get<std::string_view>("no such key"), const std::out_of_range&);
        }
    }
    END


    TEST(config_set, empty_stream) {
        for (std::filesystem::path file : {
                    "empty1.confy"s,
                    "empty2.confy"s,
                    "empty3.confy"s,
             }) {
            {
                std::ifstream ifs(file);
                EXPECT_NO_THROW(confy_set cs(ifs));
            }

            std::ifstream ifs(file);
            confy_set cs(ifs);
            EXPECT_EQ(cs.size(), std::size_t{});
            EXPECT_THROW(std::ignore = cs.get<std::string_view>("no such key"), const std::out_of_range&);
        }
    }
    END

    TEST(config_set, bare_words_file) {
        std::filesystem::path file = "bare_words.confy"s;
        EXPECT_NO_THROW(confy_set cs(file));

        confy_set cs(file);
        EXPECT_EQ(cs.size(), std::size_t{2});
        EXPECT_THROW(std::ignore = cs.get<std::string_view>("no such key"), const std::out_of_range&);

        EXPECT_NO_THROW(std::ignore = cs.get<std::string>("key"));
        EXPECT_EQ(cs.get<std::string>("key"), "bare");

        EXPECT_NO_THROW(std::ignore = cs.get<std::string>("key2"));
        EXPECT_EQ(cs.get<std::string>("key2"), "word");
    }
    END

    TEST(config_set, bare_words_stream) {
        std::filesystem::path file = "bare_words.confy"s;
        {
            std::ifstream ifs(file);
            EXPECT_NO_THROW(confy_set cs(ifs));
        }

        std::ifstream ifs(file);
        confy_set cs(ifs);
        EXPECT_EQ(cs.size(), std::size_t{2});
        EXPECT_THROW(std::ignore = cs.get<std::string_view>("no such key"), const std::out_of_range&);

        EXPECT_NO_THROW(std::ignore = cs.get<std::string>("key"));
        EXPECT_EQ(cs.get<std::string>("key"), "bare");

        EXPECT_NO_THROW(std::ignore = cs.get<std::string>("key2"));
        EXPECT_EQ(cs.get<std::string>("key2"), "word");
    }
    END

    TEST(config_set, ints_file) {
        std::filesystem::path file = "ints.confy"s;
        EXPECT_NO_THROW(confy_set cs(file));

        confy_set cs(file);
        EXPECT_EQ(cs.size(), std::size_t{3});
        EXPECT_THROW(std::ignore = cs.get<std::string_view>("no such key"), const std::out_of_range&);

        EXPECT_NO_THROW(std::ignore = cs.get<std::string>("key"));
        EXPECT_EQ(cs.get<std::string>("key"), "1");

        EXPECT_NO_THROW(std::ignore = cs.get<int>("key"));
        EXPECT_EQ(cs.get<int>("key"), 1);

        EXPECT_NO_THROW(std::ignore = cs.get<long long>("keybig"));
        EXPECT_EQ(cs.get<long long>("keybig"), 8589934592LL);
    }
    END

    TEST(config_set, ints_stream) {
        std::filesystem::path file = "ints.confy"s;
        {
            std::ifstream ifs(file);
            EXPECT_NO_THROW(confy_set cs(ifs));
        }

        std::ifstream ifs(file);
        confy_set cs(ifs);
        EXPECT_EQ(cs.size(), std::size_t{3});
        EXPECT_THROW(std::ignore = cs.get<std::string_view>("no such key"), const std::out_of_range&);

        EXPECT_NO_THROW(std::ignore = cs.get<std::string>("key"));
        EXPECT_EQ(cs.get<std::string>("key"), "1");

        EXPECT_NO_THROW(std::ignore = cs.get<int>("key"));
        EXPECT_EQ(cs.get<int>("key"), 1);

        EXPECT_NO_THROW(std::ignore = cs.get<long long>("keybig"));
        EXPECT_EQ(cs.get<long long>("keybig"), 8589934592LL);
    }
    END

    TEST(config_set, sstrings_file) {
        std::filesystem::path file = "single-strings.confy"s;
        EXPECT_NO_THROW(confy_set cs(file));

        confy_set cs(file);
        EXPECT_EQ(cs.size(), std::size_t{4});
        EXPECT_THROW(std::ignore = cs.get<std::string_view>("no such key"), const std::out_of_range&);

        for (auto&& data : {
                    std::make_pair("key"s, "some"s),
                    std::make_pair("key2"s, "have space"s),
                    std::make_pair("key3"s, "some ******* symbols"s),
                    std::make_pair("key4"s, "#4ll_th3_th1ng5_w3_h4v3"s),
             }) {
            auto&& key = data.first;
            auto&& value = data.second;
            EXPECT_NO_THROW(std::ignore = cs.get<std::string_view>(key));
            EXPECT_EQ(cs.get<std::string>(key), value);
        }
    }
    END

    TEST(config_set, sstrings_stream) {
        std::filesystem::path file = "single-strings.confy"s;
        {
            std::ifstream ifs(file);
            EXPECT_NO_THROW(confy_set cs(ifs));
        }

        std::ifstream ifs(file);
        confy_set cs(ifs);
        EXPECT_EQ(cs.size(), std::size_t{4});
        EXPECT_THROW(std::ignore = cs.get<std::string_view>("no such key"), const std::out_of_range&);

        for (auto&& data : {
                    std::make_pair("key"s, "some"s),
                    std::make_pair("key2"s, "have space"s),
                    std::make_pair("key3"s, "some ******* symbols"s),
                    std::make_pair("key4"s, "#4ll_th3_th1ng5_w3_h4v3"s),
             }) {
            auto&& key = data.first;
            auto&& value = data.second;
            EXPECT_NO_THROW(std::ignore = cs.get<std::string_view>(key));
            EXPECT_EQ(cs.get<std::string>(key), value);
        }
    }
    END

    TEST(config_set, dstrings_file) {
        std::filesystem::path file = "double-strings.confy"s;
        EXPECT_NO_THROW(confy_set cs(file));

        confy_set cs(file);
        EXPECT_EQ(cs.size(), std::size_t{4});
        EXPECT_THROW(std::ignore = cs.get<std::string_view>("no such key"), const std::out_of_range&);

        for (auto&& data : {
                    std::make_pair("key"s, "some"s),
                    std::make_pair("key2"s, "have space"s),
                    std::make_pair("key3"s, "some ******* symbols"s),
                    std::make_pair("key4"s, "#4ll_th3_th1ng5_w3_h4v3"s),
             }) {
            auto&& key = data.first;
            auto&& value = data.second;
            EXPECT_NO_THROW(std::ignore = cs.get<std::string_view>(key));
            EXPECT_EQ(cs.get<std::string>(key), value);
        }
    }
    END

    TEST(config_set, dstrings_stream) {
        std::filesystem::path file = "double-strings.confy"s;
        {
            std::ifstream ifs(file);
            EXPECT_NO_THROW(confy_set cs(ifs));
        }

        std::ifstream ifs(file);
        confy_set cs(ifs);
        EXPECT_EQ(cs.size(), std::size_t{4});
        EXPECT_THROW(std::ignore = cs.get<std::string_view>("no such key"), const std::out_of_range&);

        for (auto&& data : {
                    std::make_pair("key"s, "some"s),
                    std::make_pair("key2"s, "have space"s),
                    std::make_pair("key3"s, "some ******* symbols"s),
                    std::make_pair("key4"s, "#4ll_th3_th1ng5_w3_h4v3"s),
             }) {
            auto&& key = data.first;
            auto&& value = data.second;
            EXPECT_NO_THROW(std::ignore = cs.get<std::string_view>(key));
            EXPECT_EQ(cs.get<std::string>(key), value);
        }
    }
    END

    TEST(config_set, mixed_file) {
        std::filesystem::path file = "mixed.confy"s;
        EXPECT_NO_THROW(confy_set cs(file));

        confy_set cs(file);
        EXPECT_EQ(cs.size(), std::size_t{6});
        EXPECT_THROW(std::ignore = cs.get<std::string_view>("no such key"), const std::out_of_range&);

        for (auto&& data : {
                    std::make_pair("project"s, "confy"s),
                    std::make_pair("version"s, "1"s),
                    std::make_pair("author"s, "Bodor Andras"s),
                    std::make_pair("license"s, "BSD 3-Clause"s),
             }) {
            auto&& key = data.first;
            auto&& value = data.second;
            EXPECT_NO_THROW(std::ignore = cs.get<std::string_view>(key));
            EXPECT_EQ(cs.get<std::string>(key), value);
        }
    }
    END

    TEST(config_set, mixed_stream) {
        std::filesystem::path file = "mixed.confy"s;
        {
            std::ifstream ifs(file);
            EXPECT_NO_THROW(confy_set cs(ifs));
        }

        std::ifstream ifs(file);
        confy_set cs(ifs);
        EXPECT_EQ(cs.size(), std::size_t{6});
        EXPECT_THROW(std::ignore = cs.get<std::string_view>("no such key"), const std::out_of_range&);

        for (auto&& data : {
                    std::make_pair("project"s, "confy"s),
                    std::make_pair("version"s, "1"s),
                    std::make_pair("author"s, "Bodor Andras"s),
                    std::make_pair("license"s, "BSD 3-Clause"s),
             }) {
            auto&& key = data.first;
            auto&& value = data.second;
            EXPECT_NO_THROW(std::ignore = cs.get<std::string_view>(key));
            EXPECT_EQ(cs.get<std::string>(key), value);
        }
    }
    END

    TEST(config_set, xcolors_file) {
        std::filesystem::path file = "xcolors.confy"s;
        EXPECT_NO_THROW(confy_set cs(file));

        confy_set cs(file);
        EXPECT_EQ(cs.size(), std::size_t{35});
        EXPECT_THROW(std::ignore = cs.get<std::string_view>("no such key"), const std::out_of_range&);
    }
    END

    TEST(config_set, xcolors_stream) {
        std::filesystem::path file = "xcolors.confy"s;
        {
            std::ifstream ifs(file);
            EXPECT_NO_THROW(confy_set cs(ifs));
        }

        std::ifstream ifs(file);
        confy_set cs(ifs);
        EXPECT_EQ(cs.size(), std::size_t{35});
        EXPECT_THROW(std::ignore = cs.get<std::string_view>("no such key"), const std::out_of_range&);
    }
    END
}
