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
 * \file test_main.cpp
 * \brief The test entry point
 *
 * This file defines the main function to be used while testing.
 */

#ifdef CPORTA
#  ifndef USE_CXX17
#    define USE_CXX17
#  endif
#  undef NO_CONFY_TESTING
#endif

#ifndef NO_CONFY_TESTING
#  define test_main main
#endif

/**
 * \brief bad_key class tests
 */
void
test_bad_key();
void
test_bad_syntax();
void
test_caches();
void
test_cached_cache_factory();
void
test_config_set();
void
test_confy_parser();
void
test_type_id();
void
test_uncached_cache_factory();
void
test_user_modes();
void
test_visitors();

int
test_main() {
    test_bad_key();
    test_bad_syntax();
    test_caches();
    test_cached_cache_factory();
    test_config_set();
    test_confy_parser();
    test_type_id();
    test_uncached_cache_factory();
    test_user_modes();
    test_visitors();

    return 0;
}
