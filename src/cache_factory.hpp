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
 * \file cache_factory.hpp
 * \brief Defines the cache factory classes
 *
 * This files is used to defines the built-in classes of the cache_factory types.
 */

#ifndef CONFY_CACHE_FACTORY_HPP
#define CONFY_CACHE_FACTORY_HPP

#include <limits>
#include <memory>
#include <string>
#ifdef USE_CXX17
#  include <experimental/string_view>
#  define string_view experimental::string_view
#else
#  include <string_view>
#endif

#include "caches.hpp"

/**
 * \brief Base of the cache_factory types.
 *
 * Type used as the base of the cache_factory types. This may be specialized by anyone to provide a
 * custom factory for their type.
 *
 * If this type can be cached, the followings are required (formalized in the cachable concept):
 * - must define the type of cache constructed,
 * - must define the `std::unique_ptr<cache> construct(const std::string&)` function, which takes
 * the stored string and converts it into the chosen type in the form of a cache object.
 *
 * If the type cannot be cached, the followings are required:
 * - must define the `T make(const std::string&) const` function, where T is the type to parse from
 * a string.
 *
 * \tparam T The type to provide support for in confy
 */
template<class T>
struct cache_factory { };

/**
 * \brief Non-caching specialization for the std::string type.
 *
 * This specialization of cache_factor for std::string implements the non-caching interface.
 * With this, one can request objects of type std::string from the configuration, but since the
 * storage works under our hand here, this needs not be cached.
 */
template<>
struct cache_factory<std::string> {
    /**
   * \brief Creates (trivially) a std::string object from a string.
   *
   * Arising from the storage implementation, this function implements a trivial getter
   * functionality.
   * Creates a std::string type object from the stored string.
   *
   * \param data The stored data in the key-value store.
   * \return The new object of type std::string.
   */
    std::string
    make(const std::string& data) const { return data; }
};

/**
 * \brief Non-caching specialization for the std::string_view type.
 *
 * This specialization of cache_factor for std::string_view implements the non-caching interface.
 * With this, one can request objects of type std::string_view from the configuration, but since the
 * storage works under our hand here, this needs not be cached.
 */
template<>
struct cache_factory<std::string_view> {
    /**
   * \brief Creates (trivially) a std::string_view object from a string.
   *
   * Arising from the storage implementation, this function implements a trivial getter
   * functionality.
   * Creates a std::string_view type object from the stored string.
   *
   * \param data The stored data in the key-value store.
   * \return The new object of type std::string_view.
   */
    std::string_view
    make(const std::string& data) const { return data; }
};

/**
 * \brief Non-caching specialization for the const char* type.
 *
 * This specialization of cache_factor for const char* implements the non-caching interface.
 * With this, one can request objects of type const char* from the configuration, but since the
 * storage works under our hand here, this needs not be cached.
 */
template<>
struct cache_factory<const char*> {
    /**
   * \brief Creates (trivially) a const char* object from a string.
   *
   * Arising from the storage implementation, this function implements a trivial getter
   * functionality.
   * Creates a const char* type object from the stored string.
   *
   * \param data The stored data in the key-value store.
   * \return The new object of type const char*.
   */
    const char*
    make(const std::string& data) const { return data.c_str(); }
};

/**
 * \brief Caching specialization for constructing signed char type objects.
 *
 * This specialization implements the caching interface for cache_factory<>.
 * Parses the passed-in string and constructs the new object.
 */
template<>
struct cache_factory<signed char> {
    /**
     * \brief The cache type constructed.
     *
     * When calling the construct member function, this is the concrete type of the cache returned.
     */
    using cache_type = schar_cache;

    /**
     * \brief The parser and type constructor function.
     *
     * This function is used to construct an object of the requested `signed char` type.
     * For this it parses the passed string and using the appropriate construction mechanism crates
     * the object.
     *
     * \param data The string stored as the value, parsed for the data.
     * \return The cache containing the parsed object, or `nullptr` if parsing couldn't succeed.
     */
    std::unique_ptr<cache>
    construct(const std::string& data) {
        char* end;
        auto act_value = std::strtol(data.c_str(), &end, 10);
        if (end == data.c_str()) { // couldn't parse anything
            return nullptr;
        }
        signed char used_value;
        if (act_value > std::numeric_limits<signed char>::max()) {
            used_value = std::numeric_limits<signed char>::max();
        } else if (act_value < std::numeric_limits<signed char>::min()) {
            used_value = std::numeric_limits<signed char>::min();
        } else {
            used_value = static_cast<signed char>(act_value);
        }
        return std::make_unique<cache_type>(std::move(used_value));
    }
};

/**
 * \brief Caching specialization for constructing unsigned char type objects.
 *
 * This specialization implements the caching interface for cache_factory<>.
 * Parses the passed-in string and constructs the new object.
 */
template<>
struct cache_factory<unsigned char> {
    /**
     * \brief The cache type constructed.
     *
     * When calling the construct member function, this is the concrete type of the cache returned.
     */
    using cache_type = uchar_cache;

    /**
     * \brief The parser and type constructor function.
     *
     * This function is used to construct an object of the requested `unsigned char` type.
     * For this it parses the passed string and using the appropriate construction mechanism crates
     * the object.
     *
     * \param data The string stored as the value, parsed for the data.
     * \return The cache containing the parsed object, or `nullptr` if parsing couldn't succeed.
     */
    std::unique_ptr<cache>
    construct(const std::string& data) {
        char* end;
        auto act_value = std::strtoul(data.c_str(), &end, 10);
        if (end == data.c_str()) { // couldn't parse anything
            return nullptr;
        }
        unsigned char used_value;
        if (act_value > std::numeric_limits<unsigned char>::max()) {
            used_value = std::numeric_limits<unsigned char>::max();
        } else if (act_value < std::numeric_limits<unsigned char>::min()) {
            used_value = std::numeric_limits<unsigned char>::min();
        } else {
            used_value = static_cast<unsigned char>(act_value);
        }
        return std::make_unique<cache_type>(std::move(used_value));
    }
};

/**
 * \brief Caching specialization for constructing char type objects.
 *
 * This specialization implements the caching interface for cache_factory<>.
 * Parses the passed-in string and constructs the new object.
 */
template<>
struct cache_factory<char> {
    /**
     * \brief The cache type constructed.
     *
     * When calling the construct member function, this is the concrete type of the cache returned.
     */
    using cache_type = char_cache;

    /**
     * \brief The parser and type constructor function.
     *
     * This function is used to construct an object of the requested `char` type.
     * For this it parses the passed string and using the appropriate construction mechanism crates
     * the object.
     *
     * \param data The string stored as the value, parsed for the data.
     * \return The cache containing the parsed object, or `nullptr` if parsing couldn't succeed.
     */
    std::unique_ptr<cache>
    construct(const std::string& data) {
        char* end;
        auto act_value = std::strtol(data.c_str(), &end, 10);
        if (end == data.c_str()) { // couldn't parse anything
            return nullptr;
        }
        char used_value;
        if (act_value > std::numeric_limits<char>::max()) {
            used_value = std::numeric_limits<char>::max();
        } else if (act_value < std::numeric_limits<char>::min()) {
            used_value = std::numeric_limits<char>::min();
        } else {
            used_value = static_cast<char>(act_value);
        }
        return std::make_unique<cache_type>(std::move(used_value));
    }
};

/**
 * \brief Caching specialization for constructing int type objects.
 *
 * This specialization implements the caching interface for cache_factory<>.
 * Parses the passed-in string and constructs the new object.
 */
template<>
struct cache_factory<int> {
    /**
     * \brief The cache type constructed.
     *
     * When calling the construct member function, this is the concrete type of the cache returned.
     */
    using cache_type = int_cache;

    /**
     * \brief The parser and type constructor function.
     *
     * This function is used to construct an object of the requested `int` type.
     * For this it parses the passed string and using the appropriate construction mechanism crates
     * the object.
     *
     * \param data The string stored as the value, parsed for the data.
     * \return The cache containing the parsed object, or `nullptr` if parsing couldn't succeed.
     */
    std::unique_ptr<cache>
    construct(const std::string& data) {
        char* end;
        auto act_value = std::strtol(data.c_str(), &end, 10);
        if (end == data.c_str()) { // couldn't parse anything
            return nullptr;
        }
        int used_value;
        if (act_value > std::numeric_limits<int>::max()) {
            used_value = std::numeric_limits<int>::max();
        } else if (act_value < std::numeric_limits<int>::min()) {
            used_value = std::numeric_limits<int>::min();
        } else {
            used_value = static_cast<int>(act_value);
        }
        return std::make_unique<cache_type>(std::move(used_value));
    }
};

/**
 * \brief Caching specialization for constructing unsigned type objects.
 *
 * This specialization implements the caching interface for cache_factory<>.
 * Parses the passed-in string and constructs the new object.
 */
template<>
struct cache_factory<unsigned> {
    /**
     * \brief The cache type constructed.
     *
     * When calling the construct member function, this is the concrete type of the cache returned.
     */
    using cache_type = uint_cache;

    /**
     * \brief The parser and type constructor function.
     *
     * This function is used to construct an object of the requested `unsigned` type.
     * For this it parses the passed string and using the appropriate construction mechanism crates
     * the object.
     *
     * \param data The string stored as the value, parsed for the data.
     * \return The cache containing the parsed object, or `nullptr` if parsing couldn't succeed.
     */
    std::unique_ptr<cache>
    construct(const std::string& data) {
        char* end;
        auto act_value = std::strtoul(data.c_str(), &end, 10);
        if (end == data.c_str()) { // couldn't parse anything
            return nullptr;
        }
        unsigned used_value;
        if (act_value > std::numeric_limits<unsigned>::max()) {
            used_value = std::numeric_limits<unsigned>::max();
        } else if (act_value < std::numeric_limits<unsigned>::min()) {
            used_value = std::numeric_limits<unsigned>::min();
        } else {
            used_value = static_cast<unsigned>(act_value);
        }
        return std::make_unique<cache_type>(std::move(used_value));
    }
};

/**
 * \brief Caching specialization for constructing long type objects.
 *
 * This specialization implements the caching interface for cache_factory<>.
 * Parses the passed-in string and constructs the new object.
 */
template<>
struct cache_factory<long> {
    /**
     * \brief The cache type constructed.
     *
     * When calling the construct member function, this is the concrete type of the cache returned.
     */
    using cache_type = long_cache;

    /**
     * \brief The parser and type constructor function.
     *
     * This function is used to construct an object of the requested `long` type.
     * For this it parses the passed string and using the appropriate construction mechanism crates
     * the object.
     *
     * \param data The string stored as the value, parsed for the data.
     * \return The cache containing the parsed object, or `nullptr` if parsing couldn't succeed.
     */
    std::unique_ptr<cache>
    construct(const std::string& data) {
        char* end;
        auto value = std::strtol(data.c_str(), &end, 10);
        if (end == data.c_str()) { // couldn't parse anything
            return nullptr;
        }
        return std::make_unique<cache_type>(std::move(value));
    }
};

/**
 * \brief Caching specialization for constructing unsigned long type objects.
 *
 * This specialization implements the caching interface for cache_factory<>.
 * Parses the passed-in string and constructs the new object.
 */
template<>
struct cache_factory<unsigned long> {
    /**
     * \brief The cache type constructed.
     *
     * When calling the construct member function, this is the concrete type of the cache returned.
     */
    using cache_type = ulong_cache;

    /**
     * \brief The parser and type constructor function.
     *
     * This function is used to construct an object of the requested `unsigned long` type.
     * For this it parses the passed string and using the appropriate construction mechanism crates
     * the object.
     *
     * \param data The string stored as the value, parsed for the data.
     * \return The cache containing the parsed object, or `nullptr` if parsing couldn't succeed.
     */
    std::unique_ptr<cache>
    construct(const std::string& data) {
        char* end;
        auto value = std::strtoul(data.c_str(), &end, 10);
        if (end == data.c_str()) { // couldn't parse anything
            return nullptr;
        }
        return std::make_unique<cache_type>(std::move(value));
    }
};

/**
 * \brief Caching specialization for constructing long long type objects.
 *
 * This specialization implements the caching interface for cache_factory<>.
 * Parses the passed-in string and constructs the new object.
 */
template<>
struct cache_factory<long long> {
    /**
     * \brief The cache type constructed.
     *
     * When calling the construct member function, this is the concrete type of the cache returned.
     */
    using cache_type = long_long_cache;

    /**
     * \brief The parser and type constructor function.
     *
     * This function is used to construct an object of the requested `long long` type.
     * For this it parses the passed string and using the appropriate construction mechanism crates
     * the object.
     *
     * \param data The string stored as the value, parsed for the data.
     * \return The cache containing the parsed object, or `nullptr` if parsing couldn't succeed.
     */
    std::unique_ptr<cache>
    construct(const std::string& data) {
        char* end;
        auto value = std::strtoll(data.c_str(), &end, 10);
        if (end == data.c_str()) { // couldn't parse anything
            return nullptr;
        }
        return std::make_unique<cache_type>(std::move(value));
    }
};

/**
 * \brief Caching specialization for constructing unsigned long long type objects.
 *
 * This specialization implements the caching interface for cache_factory<>.
 * Parses the passed-in string and constructs the new object.
 */
template<>
struct cache_factory<unsigned long long> {
    /**
     * \brief The cache type constructed.
     *
     * When calling the construct member function, this is the concrete type of the cache returned.
     */
    using cache_type = ulong_long_cache;

    /**
     * \brief The parser and type constructor function.
     *
     * This function is used to construct an object of the requested `unsigned long long` type.
     * For this it parses the passed string and using the appropriate construction mechanism crates
     * the object.
     *
     * \param data The string stored as the value, parsed for the data.
     * \return The cache containing the parsed object, or `nullptr` if parsing couldn't succeed.
     */
    std::unique_ptr<cache>
    construct(const std::string& data) {
        char* end;
        auto value = std::strtoull(data.c_str(), &end, 10);
        if (end == data.c_str()) { // couldn't parse anything
            return nullptr;
        }
        return std::make_unique<cache_type>(std::move(value));
    }
};

/**
 * \brief Caching specialization for constructing short type objects.
 *
 * This specialization implements the caching interface for cache_factory<>.
 * Parses the passed-in string and constructs the new object.
 */
template<>
struct cache_factory<short> {
    /**
     * \brief The cache type constructed.
     *
     * When calling the construct member function, this is the concrete type of the cache returned.
     */
    using cache_type = short_cache;

    /**
     * \brief The parser and type constructor function.
     *
     * This function is used to construct an object of the requested `short` type.
     * For this it parses the passed string and using the appropriate construction mechanism crates
     * the object.
     *
     * \param data The string stored as the value, parsed for the data.
     * \return The cache containing the parsed object, or `nullptr` if parsing couldn't succeed.
     */
    std::unique_ptr<cache>
    construct(const std::string& data) {
        char* end;
        auto act_value = std::strtol(data.c_str(), &end, 10);
        if (end == data.c_str()) { // couldn't parse anything
            return nullptr;
        }
        short used_value;
        if (act_value > std::numeric_limits<short>::max()) {
            used_value = std::numeric_limits<short>::max();
        } else if (act_value < std::numeric_limits<short>::min()) {
            used_value = std::numeric_limits<short>::min();
        } else {
            used_value = static_cast<short>(act_value);
        }
        return std::make_unique<cache_type>(std::move(used_value));
    }
};

/**
 * \brief Caching specialization for constructing unsigned short type objects.
 *
 * This specialization implements the caching interface for cache_factory<>.
 * Parses the passed-in string and constructs the new object.
 */
template<>
struct cache_factory<unsigned short> {
    /**
     * \brief The cache type constructed.
     *
     * When calling the construct member function, this is the concrete type of the cache returned.
     */
    using cache_type = ushort_cache;

    /**
     * \brief The parser and type constructor function.
     *
     * This function is used to construct an object of the requested `unsigned short` type.
     * For this it parses the passed string and using the appropriate construction mechanism crates
     * the object.
     *
     * \param data The string stored as the value, parsed for the data.
     * \return The cache containing the parsed object, or `nullptr` if parsing couldn't succeed.
     */
    std::unique_ptr<cache>
    construct(const std::string& data) {
        char* end;
        auto act_value = std::strtol(data.c_str(), &end, 10);
        if (end == data.c_str()) { // couldn't parse anything
            return nullptr;
        }
        unsigned short used_value;
        if (act_value > std::numeric_limits<unsigned short>::max()) {
            used_value = std::numeric_limits<unsigned short>::max();
        } else if (act_value < std::numeric_limits<unsigned short>::min()) {
            used_value = std::numeric_limits<unsigned short>::min();
        } else {
            used_value = static_cast<unsigned short>(act_value);
        }
        return std::make_unique<cache_type>(std::move(used_value));
    }
};

/**
 * \brief Caching specialization for constructing bool type objects.
 *
 * This specialization implements the caching interface for cache_factory<>.
 * Parses the passed-in string and constructs the new object.
 */
template<>
struct cache_factory<bool> {
    /**
     * \brief The cache type constructed.
     *
     * When calling the construct member function, this is the concrete type of the cache returned.
     */
    using cache_type = bool_cache;

    /**
     * \brief The parser and type constructor function.
     *
     * This function is used to construct an object of the requested `bool` type.
     * For this it parses the passed string and using the appropriate construction mechanism crates
     * the object.
     *
     * \param data The string stored as the value, parsed for the data.
     * \return The cache containing the parsed object, or `nullptr` if parsing couldn't succeed.
     */
    std::unique_ptr<cache>
    construct(const std::string& data) {
        char* end;
        auto act_value = std::strtol(data.c_str(), &end, 10);
        if (end == data.c_str()) { // couldn't parse anything
            return nullptr;
        }
        return std::make_unique<cache_type>(act_value != 0);
    }
};

/**
 * \brief Caching specialization for constructing float type objects.
 *
 * This specialization implements the caching interface for cache_factory<>.
 * Parses the passed-in string and constructs the new object.
 */
template<>
struct cache_factory<float> {
    /**
     * \brief The cache type constructed.
     *
     * When calling the construct member function, this is the concrete type of the cache returned.
     */
    using cache_type = float_cache;

    /**
     * \brief The parser and type constructor function.
     *
     * This function is used to construct an object of the requested `float` type.
     * For this it parses the passed string and using the appropriate construction mechanism crates
     * the object.
     *
     * \param data The string stored as the value, parsed for the data.
     * \return The cache containing the parsed object, or `nullptr` if parsing couldn't succeed.
     */
    std::unique_ptr<cache>
    construct(const std::string& data) {
        char* end;
        auto act_value = std::strtof(data.c_str(), &end);
        if (end == data.c_str()) { // couldn't parse anything
            return nullptr;
        }
        return std::make_unique<cache_type>(std::move(act_value));
    }
};

/**
 * \brief Caching specialization for constructing double type objects.
 *
 * This specialization implements the caching interface for cache_factory<>.
 * Parses the passed-in string and constructs the new object.
 */
template<>
struct cache_factory<double> {
    /**
     * \brief The cache type constructed.
     *
     * When calling the construct member function, this is the concrete type of the cache returned.
     */
    using cache_type = double_cache;

    /**
     * \brief The parser and type constructor function.
     *
     * This function is used to construct an object of the requested `double` type.
     * For this it parses the passed string and using the appropriate construction mechanism crates
     * the object.
     *
     * \param data The string stored as the value, parsed for the data.
     * \return The cache containing the parsed object, or `nullptr` if parsing couldn't succeed.
     */
    std::unique_ptr<cache>
    construct(const std::string& data) {
        char* end;
        auto act_value = std::strtod(data.c_str(), &end);
        if (end == data.c_str()) { // couldn't parse anything
            return nullptr;
        }
        return std::make_unique<cache_type>(std::move(act_value));
    }
};

/**
 * \brief Caching specialization for constructing long double type objects.
 *
 * This specialization implements the caching interface for cache_factory<>.
 * Parses the passed-in string and constructs the new object.
 */
template<>
struct cache_factory<long double> {
    /**
     * \brief The cache type constructed.
     *
     * When calling the construct member function, this is the concrete type of the cache returned.
     */
    using cache_type = long_double_cache;

    /**
     * \brief The parser and type constructor function.
     *
     * This function is used to construct an object of the requested `long double` type.
     * For this it parses the passed string and using the appropriate construction mechanism crates
     * the object.
     *
     * \param data The string stored as the value, parsed for the data.
     * \return The cache containing the parsed object, or `nullptr` if parsing couldn't succeed.
     */
    std::unique_ptr<cache>
    construct(const std::string& data) {
        char* end;
        auto act_value = std::strtold(data.c_str(), &end);
        if (end == data.c_str()) { // couldn't parse anything
            return nullptr;
        }
        return std::make_unique<cache_type>(std::move(act_value));
    }
};

#endif
