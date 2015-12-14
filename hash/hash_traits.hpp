#pragma once

#include <memory>
#include <type_traits>

namespace hash {

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
