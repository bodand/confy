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
 * \file type_id.hpp
 * \brief File that contains the type identifier class.
 *
 * File contains and implements the type_id class and all of its members. The class is mostly made
 * up of templates, so they are implemented here.
 */

#ifndef CONFY_TYPE_ID_HPP
#define CONFY_TYPE_ID_HPP

#include <cstdint>

/**
 * \brief The type identifier type
 *
 * A class that is used to identify a given type in constant time. It forms a bijection between a
 * class and a numeric id, which can then be trivially checked for equivalence.
 */
struct type_id {
    /**
     * \brief Named constructor for the type_id class.
     *
     * Creates a type_id object for the given T type. This type_id object will be bound to the type
     * and will compare equal to all other objects created for the same T type and compare unequal
     * to all other types.
     *
     * This is the only valid way to create a type_id class. If (somehow) a type_id object is not
     * created this way, the behavior of the program is undefined.
     *
     * \tparam T The type to create the object for.
     * \return The type_id object created for the type T.
     */
    template<class T>
    type_id
    id_of() noexcept;

    /**
     * \brief The equality operator for type_id objects
     *
     * Compares if two type_id objects have been created for the same type. The operational
     * complexity of this function is O(1).
     *
     * \param other The other type_id object to check equality to
     * \return Whether the two type_id objects have been created for the same type.
     */
    bool
    operator==(const type_id& other) const noexcept;
    /**
     * \brief The inequality operator for type_id objects
     *
     * Compares if two type_id objects have been created for different types. The operational
     * complexity of this function is O(1).
     *
     * \param other The other type_id object to check inequality to
     * \return Whether the two type_id object have been created for different types.
     */
    bool
    operator!=(const type_id& other) const noexcept;

private:
    /**
     * \brief Private constructor for type_id objects
     *
     * The actual constructor for the type_id type. It is hidden from the public eye as to not allow
     * anybody to create objects that have not been created through the \refitem id_of<T>()
     * function.
     *
     * \param val The integer id value to assign to this object. Refers to a singular
     */
    explicit type_id(std::uint_fast32_t val) noexcept;
};

#endif
