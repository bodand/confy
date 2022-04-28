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
 * \file visitor.hpp
 *
 * \brief Defines the base types of the visitor schema provided for visiting caches.
 *
 * Contains the base definitions for the classes visitor_base, typed_visitor<class T>, and
 * visitor<class... Ts>. These types form the base of the dynamic visitor pattern used by the
 * project to perform cache lookups, almost at a single function call cost.
 */

#ifndef CONFY_VISITOR_HPP
#define CONFY_VISITOR_HPP

#include "type_id.hpp"

/**
 * \brief The dynamic visitor pattern's base class.
 *
 * The root of the dynamic visitor's class hierarchy tree. Classes that may be visited need to be
 * able to accept this type of object (polymorphically) to be visited.
 * This is implemented in the cache class, for example.
 *
 * This class just passes down the implementation requirements into the class hierarchy through an
 * internal type-erased interface.
 */
struct visitor_base {
    /**
     * \brief The function called to visit an object.
     *
     * This wrapper function provides universal visitability to all objects, provided they were
     * implemented in the hierarchy below.
     * Takes a T*, usually a `this` pointer, and performs the visitation on it.
     *
     * Internally calculates the `type_id` value for the objects type, and casts the pointer down to
     * `void*`.
     * Passes these two values to the implementation functions, which will call the appropriate
     * `do_visit` functions in the hierarchy.
     *
     * \tparam T The type of the visited object.
     * \param visited The visited object
     */
    template<class T>
    void
    visit(T* visited);

    /**
     * \brief Defaulted virtual destructor
     *
     * A virtual destructor to allow subtyping. Defaulted for it does nothing.
     */
    virtual ~visitor_base() noexcept = default;

protected:
    /**
     * \brief Internal type-erased interface using runtime-type information.
     *
     * This function is internal to the hierarchy and is used to implement the actual visitation.
     * The type erased object to be visited is passed through with a void-pointer and a `type_id`
     * object for providing runtime type information to figure out which type the type erased object
     * is.
     * This is done using the one way implemented bijection type, `type_id`.
     *
     * \param erased_visited The visited object, cast back to void pointer.
     * \param tid The type_id object of the actual type of the visited object.
     */
    virtual void
    visit_typeless(void* erased_visited, type_id tid) = 0;
};

/**
 * \brief A visitor for a concrete T type
 *
 * A type in the dynamic visitor hierarchy.
 * Directly descending from the root, it provides the implementation for a singular object to be
 * visited using the `do_visit` pure virtual function.
 *
 * \tparam T The type to provide visitation for.
 */
template<class T>
struct typed_visitor : virtual visitor_base {
    /**
     * \brief Visitation callback for type T.
     *
     * This is the implementation point for specific visitors.
     * Each concrete visitor implementation shall implement this function for all types they wish
     * to be able to visit, and if the type visited matches, this function gets called.
     *
     * \param visited The visited object of type T.
     */
    virtual void
    do_visit(T& visited) = 0;

    /**
     * \brief Defaulted destructor.
     *
     * The defaulted constructor that does nothing.
     * May not throw.
     */
    ~typed_visitor() noexcept override = default;
};

/**
 * \brief Direct base class of concrete visitors
 *
 * This class combines multiple type_visitor objects, providing the ability to visit multiple types
 * of objects.
 * It inherits from `typed_visitor` classes for all types in the input Ts template parameter pack.
 *
 * \tparam Ts Parameter pack holding all types the concrete visitor will be able to handle.
 */
template<class... Ts>
struct visitor : typed_visitor<Ts>... {
    /**
     * \brief Defaulted destructor.
     *
     * The defaulted constructor that does nothing.
     * May not throw.
     */
    ~visitor() noexcept override = default;

protected:
    /**
     * \brief Implemented typeless visitor internal.
     *
     * This class is the one in the hierarchy that implements the type erased visitor internal
     * function.
     * Iteratively checks with each type it is supposed to be able to handle and if it finds a
     * matching T in Ts that has the same `type_id` as provided, it casts the type erased value
     * to that pointer, then calls the `do_visit` function with the proper parameters.
     *
     * \param erased_visited The type erased object to visit.
     * \param tid The type_id value of the object's actual type.
     */
    void
    visit_typeless(void* erased_visited, type_id tid) final;
};

#endif
