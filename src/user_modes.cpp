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
 * \file user_modes.cpp
 * \brief Implements the user_modes.hpp file's user interaction functions
 */

#include "user_modes.hpp"

#include <iostream>

#include "config_set.hpp"
#include "confy_parser.hpp"

int
interactive_mode(const std::filesystem::path& cfg_file) {
    config_set<confy_parser> conf(cfg_file);

    for (;;) {
        try {
            std::string key;
            if (!std::getline(std::cin, key)) return 0;

            std::cout << conf.get<std::string_view>(key) << "\n";
        } catch (const std::invalid_argument& ex) {
            /* NOP, invalid keys are ignored here */
        }
    }
}


int
cli_mode(const std::filesystem::path& cfg_file, cli_keys_t keys) {
    config_set<confy_parser> conf(cfg_file);

    try {
        for (const auto& key : keys) {
            std::cout << conf.get<std::string_view>(key) << "\n";
        }
        return 0;
    } catch (const std::out_of_range& ex) {
        std::cerr << ex.what();
    }
    return 2;
}
