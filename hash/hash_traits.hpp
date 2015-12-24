#pragma once

#include <memory>
#include <type_traits>

namespace hash {

	// http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2014/n3980.html
    // A type T is contiguously hashable if for all combinations of two values of a type,
    // say x and y, if x == y, then it must also be true that memcmp(addressof(x), addressof(y), sizeof(T)) == 0.
    // I.e. if x == y, then x and y have the same bit pattern representation.
    template <typename, typename = void>
    struct is_contiguously_hashable;

    template <typename T>
    struct is_contiguously_hashable<T, std::enable_if_t<std::is_integral<T>::value> > : std::true_type {};

    template <typename T>
    using is_contiguously_hashable_t = typename is_contiguously_hashable<T>::type;

    template <typename T>
    constexpr bool is_contiguously_hashable_v = is_contiguously_hashable<T>::value;

}  // namespace hash
