#pragma once

#include <memory>
#include <type_traits>

namespace hash {

	// http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2014/n3980.html
    // A type T is contiguously hashable if for all combinations of two values of a type,
    // say x and y, if x == y, then it must also be true that memcmp(addressof(x), addressof(y), sizeof(T)) == 0. 
    // I.e. if x == y, then x and y have the same bit pattern representation.
    template <typename T>
    struct is_contiguously_hashable;

    template <typename T>
    struct is_contiguously_hashable<std::enable_if<std::is_integral<T>::value, T> > : std::true_type {};

    template <typename HashaAlgorithm, typename T>
    inline
    std::enable_if_t<hash::is_contiguously_hashable<T>::value>
    hash_append(HashaAlgorithm& h, T const& t){
        h(std::addressof(t), sizeof(t));
    }

}  // namespace hash
