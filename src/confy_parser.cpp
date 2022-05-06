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
 *
 * src/confy_parser.cpp --
 *   Implements confy_parser's functionality.
 */
#include "confy_parser.hpp"

#include "bad_syntax.hpp"

confy_parser::confy_parser(const std::filesystem::path& file) noexcept
     : _file(file) { }

std::optional<std::string>
confy_parser::next_line(std::istream& strm) const {
    while (strm) {
        std::string next_ln;
        std::getline(strm, next_ln);
        while (!next_ln.empty()
               && next_ln.find_last_of("\r\n") == next_ln.size() - 1) {
            next_ln.pop_back();
        }
        ++_ln_cnt;
        if (next_ln.empty()) continue;
        if (next_ln.front() == '#') continue;

        return {next_ln};
    }
    return std::nullopt;
}

std::pair<std::string, std::string>
confy_parser::parse_line(std::string_view ln) const {
    if (ln.empty() || !std::isalpha(ln.front())) throw bad_syntax({ln.data(), ln.size()}, _ln_cnt, 1, _file);
    std::string key;
    std::size_t i = 0;

    while (i < ln.size() && ln[i] != '=') {
        if (!std::isalnum(ln[i])) throw bad_syntax({ln.data(), ln.size()}, _ln_cnt, static_cast<int>(i + 1), _file);
        key.push_back(ln[i]);
        ++i;
    }
    if (i == ln.size()) throw bad_syntax({ln.data(), ln.size()}, _ln_cnt, static_cast<int>(i + 1), _file);
    ++i;

    if (ln.size() == i) return {key, ""};

    if (ln[i] == '\'' || ln[i] == '"') {
        auto value = ln.substr(i + 1);
        if (value.back() != ln[i]) throw bad_syntax({ln.data(), ln.size()}, _ln_cnt, static_cast<int>(ln.size() + 1), _file);
        auto str_value = value.substr(0, value.size() - 1);
        return {key, {str_value.data(), str_value.size()}};
    }

    std::string value;
    value.reserve(ln.size() - i);
    while (i < ln.size() && std::isalnum(ln[i])) {
        value.push_back(ln[i]);
        ++i;
    }
    if (i != ln.size()) throw bad_syntax({ln.data(), ln.size()}, _ln_cnt, static_cast<int>(i + 1), _file);

    return {key, value};
}
