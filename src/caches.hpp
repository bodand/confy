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
 * \file caches.hpp
 * \brief The file declares and defines the built-in cache types
 *
 * This file contains all the declarations and definitions of the built-in cache classes, for all
 * fundamental types.
 */

#ifndef CONFY_CACHES_HPP
#define CONFY_CACHES_HPP

#include <utility>

#include "visitor.hpp"

/**
 * \brief The base cache class.
 *
 * The base class of the cache hierarchy. However, all caches that are to be implemented need not
 * derive from this, use the provided convenience wrapper, visitable_cache.
 */
struct cache {
    /**
     * \brief Accepts a visitor and lets it visit us.
     *
     * The function is called to begin a visitation by a visitor.
     * Pure virtual, all subclasses need to implement this so that overload resolution can select
     * the appropriate function to be called on the visitor.
     * This, however, is really boilerplate-y.
     * Derive instead from visitable_cache<D>, where D is your class, this will take care of this
     * function for you.
     *
     * \param vtor The visitor to let visit us
     */
    virtual void
    accept(visitor_base& vtor) = 0;
    /**
     * \brief Defaulted virtual destructor
     *
     * A defaulted virtual destructor to allow subclassing.
     */
    virtual ~cache() noexcept = default;
};

/**
 * \brief Helper function to ease subtyping of cache.
 *
 * A wrapper class around cache, which, using CRTP, implements the accept function for all classes,
 * without needing to think about it.
 *
 * \tparam D The class deriving from this class
 */
template<class D>
struct visitable_cache : cache {
    /**
     * \brief Function called to initiate visitation
     *
     * This function implements the base class's accept functionality using CRTP and the knowledge
     * it gives us: the actual type of our children.
     * The visitor will get called on the child object, as if implemented manually.
     *
     * \param vtor The visitor to let visit us
     */
    void
    accept(visitor_base& vtor) override {
        auto self = static_cast<D*>(this);
        vtor.visit(self);
    }

    /**
     * \brief Defaulted destructor.
     *
     * The defaulted constructor that does nothing.
     * May not throw.
     */
    ~visitable_cache() noexcept override = default;
};

/**
 *  \brief Signed character cache
 *
 *  This cache type is responsible for caching a signed char typed value.
 */
struct schar_cache : visitable_cache<schar_cache> {
    /**
     * \brief Cache constructor
     *
     * Constructs a cache object from the value it is meant to store.
     * For this class this is a `signed char`.
     * The value is to be moved in, or be a literal.
     *
     * \param data The value to store in the cache. Moved.
     */
    schar_cache(signed char&& data) noexcept : _data(std::move(data)) { }

    /**
     * \brief Cache getter
     *
     * Returns a const pointer to the stored value in the cache.
     *
     * \return The stored value.
     */
    const signed char*
    get_value_ptr() const { return &_data; }

private:
    signed char _data;
};

/**
 *  \brief Unsigned character cache
 *
 *  This cache type is responsible for caching an unsigned char typed value.
 */
struct uchar_cache : visitable_cache<uchar_cache> {
    /**
     * \brief Cache constructor
     *
     * Constructs a cache object from the value it is meant to store.
     * For this class this is a `unsigned char`.
     * The value is to be moved in, or be a literal.
     *
     * \param data The value to store in the cache. Moved.
     */
    uchar_cache(unsigned char&& data) noexcept : _data(std::move(data)) { }

    /**
     * \brief Cache getter
     *
     * Returns a const pointer to the stored value in the cache.
     *
     * \return The stored value.
     */
    const unsigned char*
    get_value_ptr() const { return &_data; }

private:
    unsigned char _data;
};

/**
 *  \brief Character cache
 *
 *  This cache type is responsible for caching a char typed value.
 */
struct char_cache : visitable_cache<char_cache> {
    /**
     * \brief Cache constructor
     *
     * Constructs a cache object from the value it is meant to store.
     * For this class this is a `char`.
     * The value is to be moved in, or be a literal.
     *
     * \param data The value to store in the cache. Moved.
     */
    char_cache(char&& data) noexcept : _data(std::move(data)) { }

    /**
     * \brief Cache getter
     *
     * Returns a const pointer to the stored value in the cache.
     *
     * \return The stored value.
     */
    const char*
    get_value_ptr() const { return &_data; }

private:
    char _data;
};

/**
 *  \brief Integer cache
 *
 *  This cache type is responsible for caching an int typed value.
 */
struct int_cache : visitable_cache<int_cache> {
    /**
     * \brief Cache constructor
     *
     * Constructs a cache object from the value it is meant to store.
     * For this class this is a `int`.
     * The value is to be moved in, or be a literal.
     *
     * \param data The value to store in the cache. Moved.
     */
    int_cache(int&& data) noexcept : _data(std::move(data)) { }

    /**
     * \brief Cache getter
     *
     * Returns a const pointer to the stored value in the cache.
     *
     * \return The stored value.
     */
    const int*
    get_value_ptr() const { return &_data; }

private:
    int _data;
};

/**
 *  \brief Unsigned integer cache
 *
 *  This cache type is responsible for caching an unsigned typed value.
 */
struct uint_cache : visitable_cache<uint_cache> {
    /**
     * \brief Cache constructor
     *
     * Constructs a cache object from the value it is meant to store.
     * For this class this is a `unsigned`.
     * The value is to be moved in, or be a literal.
     *
     * \param data The value to store in the cache. Moved.
     */
    uint_cache(unsigned&& data) noexcept : _data(std::move(data)) { }

    /**
     * \brief Cache getter
     *
     * Returns a const pointer to the stored value in the cache.
     *
     * \return The stored value.
     */
    const unsigned*
    get_value_ptr() const { return &_data; }

private:
    unsigned _data;
};

/**
 *  \brief Long integer cache
 *
 *  This cache type is responsible for caching a long typed value.
 */
struct long_cache : visitable_cache<long_cache> {
    /**
     * \brief Cache constructor
     *
     * Constructs a cache object from the value it is meant to store.
     * For this class this is a `long`.
     * The value is to be moved in, or be a literal.
     *
     * \param data The value to store in the cache. Moved.
     */
    long_cache(long&& data) noexcept : _data(std::move(data)) { }

    /**
     * \brief Cache getter
     *
     * Returns a const pointer to the stored value in the cache.
     *
     * \return The stored value.
     */
    const long*
    get_value_ptr() const { return &_data; }

private:
    long _data;
};

/**
 *  \brief Unsigned long integer cache
 *
 *  This cache type is responsible for caching an unsigned long typed value.
 */
struct ulong_cache : visitable_cache<ulong_cache> {
    /**
     * \brief Cache constructor
     *
     * Constructs a cache object from the value it is meant to store.
     * For this class this is a `unsigned long`.
     * The value is to be moved in, or be a literal.
     *
     * \param data The value to store in the cache. Moved.
     */
    ulong_cache(unsigned long&& data) noexcept : _data(std::move(data)) { }

    /**
     * \brief Cache getter
     *
     * Returns a const pointer to the stored value in the cache.
     *
     * \return The stored value.
     */
    const unsigned long*
    get_value_ptr() const { return &_data; }

private:
    unsigned long _data;
};

/**
 *  \brief Long long integer cache
 *
 *  This cache type is responsible for caching a long long typed value.
 */
struct long_long_cache : visitable_cache<long_long_cache> {
    /**
     * \brief Cache constructor
     *
     * Constructs a cache object from the value it is meant to store.
     * For this class this is a `long long`.
     * The value is to be moved in, or be a literal.
     *
     * \param data The value to store in the cache. Moved.
     */
    long_long_cache(long long&& data) noexcept : _data(std::move(data)) { }

    /**
     * \brief Cache getter
     *
     * Returns a const pointer to the stored value in the cache.
     *
     * \return The stored value.
     */
    const long long*
    get_value_ptr() const { return &_data; }

private:
    long long _data;
};

/**
 *  \brief Unsigned long long integer cache
 *
 *  This cache type is responsible for caching an unsigned long long typed value.
 */
struct ulong_long_cache : visitable_cache<ulong_long_cache> {
    /**
     * \brief Cache constructor
     *
     * Constructs a cache object from the value it is meant to store.
     * For this class this is a `unsigned long long`.
     * The value is to be moved in, or be a literal.
     *
     * \param data The value to store in the cache. Moved.
     */
    ulong_long_cache(unsigned long long&& data) noexcept : _data(std::move(data)) { }

    /**
     * \brief Cache getter
     *
     * Returns a const pointer to the stored value in the cache.
     *
     * \return The stored value.
     */
    const unsigned long long*
    get_value_ptr() const { return &_data; }

private:
    unsigned long long _data;
};

/**
 *  \brief Short integer cache
 *
 *  This cache type is responsible for caching a short typed value.
 */
struct short_cache : visitable_cache<short_cache> {
    /**
     * \brief Cache constructor
     *
     * Constructs a cache object from the value it is meant to store.
     * For this class this is a `short`.
     * The value is to be moved in, or be a literal.
     *
     * \param data The value to store in the cache. Moved.
     */
    short_cache(short&& data) noexcept : _data(std::move(data)) { }

    /**
     * \brief Cache getter
     *
     * Returns a const pointer to the stored value in the cache.
     *
     * \return The stored value.
     */
    const short*
    get_value_ptr() const { return &_data; }

private:
    short _data;
};

/**
 *  \brief Unsigned short integer cache
 *
 *  This cache type is responsible for caching an unsigned short typed value.
 */
struct ushort_cache : visitable_cache<ushort_cache> {
    /**
     * \brief Cache constructor
     *
     * Constructs a cache object from the value it is meant to store.
     * For this class this is a `unsigned short`.
     * The value is to be moved in, or be a literal.
     *
     * \param data The value to store in the cache. Moved.
     */
    ushort_cache(unsigned short&& data) noexcept : _data(std::move(data)) { }

    /**
     * \brief Cache getter
     *
     * Returns a const pointer to the stored value in the cache.
     *
     * \return The stored value.
     */
    const unsigned short*
    get_value_ptr() const { return &_data; }

private:
    unsigned short _data;
};

/**
 *  \brief Boolean logical value cache
 *
 *  This cache type is responsible for caching a bool typed value.
 */
struct bool_cache : visitable_cache<bool_cache> {
    /**
     * \brief Cache constructor
     *
     * Constructs a cache object from the value it is meant to store.
     * For this class this is a `bool`.
     * The value is to be moved in, or be a literal.
     *
     * \param data The value to store in the cache. Moved.
     */
    bool_cache(bool&& data) noexcept : _data(std::move(data)) { }

    /**
     * \brief Cache getter
     *
     * Returns a const pointer to the stored value in the cache.
     *
     * \return The stored value.
     */
    const bool*
    get_value_ptr() const { return &_data; }

private:
    bool _data;
};

/**
 *  \brief Floating point cache
 *
 *  This cache type is responsible for caching a float typed value.
 */
struct float_cache : visitable_cache<float_cache> {
    /**
     * \brief Cache constructor
     *
     * Constructs a cache object from the value it is meant to store.
     * For this class this is a `float`.
     * The value is to be moved in, or be a literal.
     *
     * \param data The value to store in the cache. Moved.
     */
    float_cache(float&& data) noexcept : _data(std::move(data)) { }

    /**
     * \brief Cache getter
     *
     * Returns a const pointer to the stored value in the cache.
     *
     * \return The stored value.
     */
    const float*
    get_value_ptr() const { return &_data; }

private:
    float _data;
};

/**
 *  \brief Double precision floating point cache
 *
 *  This cache type is responsible for caching a double typed value.
 */
struct double_cache : visitable_cache<double_cache> {
    /**
     * \brief Cache constructor
     *
     * Constructs a cache object from the value it is meant to store.
     * For this class this is a `double`.
     * The value is to be moved in, or be a literal.
     *
     * \param data The value to store in the cache. Moved.
     */
    double_cache(double&& data) noexcept : _data(std::move(data)) { }

    /**
     * \brief Cache getter
     *
     * Returns a const pointer to the stored value in the cache.
     *
     * \return The stored value.
     */
    const double*
    get_value_ptr() const { return &_data; }

private:
    double _data;
};

/**
 *  \brief Long double precision floating point cache
 *
 *  This cache type is responsible for caching a long double typed value.
 */
struct long_double_cache : visitable_cache<long_double_cache> {
    /**
     * \brief Cache constructor
     *
     * Constructs a cache object from the value it is meant to store.
     * For this class this is a `long double`.
     * The value is to be moved in, or be a literal.
     *
     * \param data The value to store in the cache. Moved.
     */
    long_double_cache(long double&& data) noexcept : _data(std::move(data)) { }

    /**
     * \brief Cache getter
     *
     * Returns a const pointer to the stored value in the cache.
     *
     * \return The stored value.
     */
    const long double*
    get_value_ptr() const { return &_data; }

private:
    long double _data;
};


#endif
