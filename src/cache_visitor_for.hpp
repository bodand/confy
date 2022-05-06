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

#ifndef CONFY_CACHE_VISITOR_FOR_HPP
#define CONFY_CACHE_VISITOR_FOR_HPP

#include "cachable.hpp"
#include "visitor.hpp"

#ifdef CPORTA
#  define cachable class
#endif

/**
 * \brief Concrete cache visitor
 *
 * Concrete implementation for the dynamic visitor.
 * Can visit a specific type's cache type.
 * This type must be a cachabe type, as the concept specifies it.
 *
 * This type's cache is the only type the visitor will visit, therefore, it implicitly checks a type
 * equivalence.
 * The result of the visitation is queryable using the valid() and value() member functions.
 *
 * If T is a user provided type and its cache is not provided by the library, the cache_factory<T>
 * specialization must be made available before the compiler finds this class.
 *
 * \tparam T The type whose cache is to be visited.
 */
template<cachable T>
struct cache_visitor_for final : visitor<typename cache_factory<T>::cache_type> {
    /**
     * \brief The visited cache type
     *
     * The type of the cache the object will be visiting. Queried from the cache_factory
     * specialization, thus this specialization must be made available before this to the compiler.
     */
    using cache_type = typename cache_factory<T>::cache_type;

    /**
     * \brief The visitation's behavior function
     *
     * Implements the behavior if the type matches during the visitation.
     * It queries the cache value from the cache object, and stores the returned pointer.
     *
     * \param visited The visited cache object.
     */
    void
    do_visit(cache_type& visited) final {
        _data = visited.get_value_ptr();
    }

    /**
     * \brief Check whether the last visitation succeeded
     *
     * A validity function to query the current state of the visitor. If this returns true, the
     * value from the last visitation is stored, and may be retrieved with value().
     *
     * \return The success of the last visitation operation
     */
    bool
    valid() const noexcept { return _data != nullptr; }
    /**
     * \brief Returns the last visitations value.
     *
     * Precondition: `valid() == true`.
     *
     * If the preconditions are met, and this is a visitor that had a valid last visitation, then
     * this function returns a const reference to the object of type T stored in the cache.
     *
     * \return A const reference to the cache's value.
     */
    const T&
    value() const noexcept { return *_data; }

private:
    const T* _data = nullptr; ///< The pointer to the stored data
};

#endif
