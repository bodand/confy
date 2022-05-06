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
 * \file test.bad_key.cpp
 * \brief Test functions for the bad_key class
 */

#include <exception>
#ifdef CPORTA
#  include <experimental/filesystem>
#  define filesystem experimental::filesystem
#else
#  include <filesystem>
#endif
#include <type_traits>

#include "visitor.hpp"

using namespace std::literals;

#include "gtest_lite.h"

namespace {
    struct base_type {
        virtual void
        accept(visitor_base&) = 0;
        virtual ~base_type() = default;
    };

    struct derived1 : base_type {
        int
        derived_code() { return 1; }
        void
        accept(visitor_base& v) override { v.visit(this); }
    };

    struct derived2 : base_type {
        int
        derived_code() { return 2; }
        void
        accept(visitor_base& v) override { v.visit(this); }
    };

    struct derived_thrower : base_type {
        int
        derived_code() { throw std::bad_alloc(); }
        void
        accept(visitor_base& v) override { v.visit(this); }
    };

    struct test_visitor : visitor<derived1, derived2, derived_thrower> {
        void
        do_visit(derived1& der) final { derived_1 += der.derived_code(); }
        void
        do_visit(derived2& der) final { derived_2 += der.derived_code(); }
        void
        do_visit(derived_thrower& der) final { derived_3 += der.derived_code(); }
        void
        reset() noexcept { derived_1 = derived_2 = derived_3 = 0; }

        int derived_1 = 0;
        int derived_2 = 0;
        int derived_3 = 0;
    };
}

void
test_visitors() {
    TEST(visitor, static_check) {
        EXPECT_TRUE((std::is_default_constructible<test_visitor>::value));
    }
    END

           // runtime checks
           test_visitor vtor;
    TEST(visitor, visit_derived1) {
        derived1 sut;
        EXPECT_NO_THROW(sut.accept(vtor));
        EXPECT_EQ(sut.derived_code(), vtor.derived_1);
        vtor.reset();
    }
    END

    TEST(visitor, visit_derived2) {
        derived2 sut;
        EXPECT_NO_THROW(sut.accept(vtor));
        EXPECT_EQ(sut.derived_code(), vtor.derived_2);
        vtor.reset();
    }
    END

    TEST(visitor, visit_throw) {
        derived_thrower sut;
        EXPECT_THROW(sut.accept(vtor), const std::bad_alloc&);
        EXPECT_EQ(0, vtor.derived_3);
        vtor.reset();
    }
    END

    TEST(vistitor, visitor_state) {
        derived1 sut;
        sut.accept(vtor);
        sut.accept(vtor);
        sut.accept(vtor);
        EXPECT_EQ(vtor.derived_1, sut.derived_code() * 3);
        vtor.reset();
    }
    END

    TEST(visitor, visitor_dynamic_dispatch) {
        derived1 der1;
        base_type* sut = &der1;
        sut->accept(vtor);
        EXPECT_EQ(1, vtor.derived_1);
        vtor.reset();
    }
    END
}
