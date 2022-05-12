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
 * \file capture_stdio.hpp
 * \brief This file defines function templates that may be used to redirect std::cin/std::cout.
 *
 * This file defines the feed_stdin and capture_stdout function templates. These allow the
 * redirection of standard I/O streams for the duration of a single function call. These are used
 * for automated testing of functions dealing with user I/O.
 */

#ifndef CONFY_CAPTURE_STDIO_HPP
#define CONFY_CAPTURE_STDIO_HPP

#ifdef CPORTA
#  ifndef USE_CXX17
#    define USE_CXX17
#  endif
#endif

#include <ios>
#include <sstream>
#include <streambuf>
#ifdef USE_CXX17
#  include <experimental/string_view>
#  define string_view experimental::string_view
#else
#  include <string_view>
#endif

namespace {
    template<class Strm>
    struct stream_resetter {
        void
        operator()(std::streambuf* buf) const {
            Strm::value->rdbuf(buf);
        }
    };

    template<std::istream* IStrm>
    struct istream_v {
        static constexpr auto value = IStrm;
    };

    template<std::ostream* OStrm>
    struct ostream_v {
        static constexpr auto value = OStrm;
    };

    template<class Strm>
    using reset_stream = std::unique_ptr<std::streambuf, stream_resetter<Strm>>;
}

/**
 * \brief A function to feed input to an std::istream.
 *
 * A function that can be used to simulate as if the provided OStrm std::istream object was written
 * to.
 * It swaps the stream buffer under the stream allowing it to give it arbitrary data, as if it was
 * given to it from an outside source, e.g. STDIN.
 * This is useful in case of only `std::istream` objects that are not also `std::ostream`-s, since
 * one cannot just write data into an `std::istream`.
 *
 * The function is *not* thread safe.
 *
 * \tparam IStrm The std::istream object to feed data into. Passed as a pointer.
 * \tparam Fn The type of the function to execute while the stream is fed data. Perfect forwarded.
 * \tparam Args The type of arguments passed to the executed function. Perfect forwarded.
 * \param data The input data to appear in the stream.
 * \param fn  The function to execute during the redirected stream state.
 * \param args The of arguments passed to the executed function.
 */
template<std::istream* IStrm, class Fn, class... Args>
void
feed_stream(std::string_view data, Fn&& fn, Args&&... args) {
    std::istringstream ss({data.data(), data.size()});
    { // REDIRECTION SCOPE
        auto buf_buf = reset_stream<istream_v<IStrm>>(IStrm->rdbuf(ss.rdbuf()));
        std::forward<Fn>(fn)(std::forward<Args>(args)...);
    }
}

/**
 * \brief A function to capture output to an std::ostream.
 *
 * A function that can be used to capture all data output to the OStrm std::ostream object in string
 * format, as if it were to appear on wherever the stream was connected to.
 * It swaps the stream buffer under the stream allowing it to hijack all output. Note, that during
 * this function, the normal behavior of the stream may be compromised, for example, std::cout will
 * not write to STDOUT.
 * This is useful in case of only `std::ostream` objects that are not also `std::istream`-s, since
 * one cannot just read data from an `std::ostream`.
 *
 * The function is *not* thread safe.
 *
 * \tparam OStrm The std::ostream object to hijack. Passed as a pointer.
 * \tparam Fn The type of the function to execute while the stream is captured. Perfect forwarded.
 * \tparam Args The type of arguments passed to the executed function. Perfect forwarded.
 * \param fn The function to execute during the redirected stream state.
 * \param args The of arguments passed to the executed function.
 * \return The string data that would've been written to wherever the stream was connected.
 */
template<std::ostream* OStrm, class Fn, class... Args>
std::string
capture_stream(Fn&& fn, Args&&... args) {
    std::ostringstream ss;
    { // REDIRECTION SCOPE
        auto buf_buf = reset_stream<ostream_v<OStrm>>(OStrm->rdbuf(ss.rdbuf()));
        std::forward<Fn>(fn)(std::forward<Args>(args)...);
    }
    return ss.str();
}

#endif
