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
 * \file main.cpp
 * \brief The main entry point to a non-test build
 */

#ifdef CPORTA
#  ifndef USE_CXX17
#    define USE_CXX17
#  endif
#  undef NO_CONFY_TESTING
#endif

#ifndef NO_CONFY_TESTING
#  define main not_main_anymore
#endif

#include <exception>
#include <iostream>
#ifdef USE_CXX17
#  include <experimental/string_view>
#  define string_view experimental::string_view
#else
#  include <string_view>
#endif
#include <vector>

#include "user_modes.hpp"

int
main(int argc, char** argv) try {
    if (argc >= 3) {
        std::vector<std::string_view> args(argv + 2,
                                           argv + argc);
        return cli_mode(argv[1], args);
    }

    std::filesystem::path cfg_file;
    if (argc == 2) {
        cfg_file = argv[1];
    } else {
        std::cout << "Which configuration file would you like to use?\n";
        std::string file_buf;
        std::getline(std::cin, file_buf);
        cfg_file = file_buf;
        if (!std::filesystem::exists(cfg_file)) return 1;
    }
    return interactive_mode(cfg_file);
} catch (const std::exception& ex) {
    std::cerr << ex.what() << "\n";
    return -1;
}
