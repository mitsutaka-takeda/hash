#pragma once

#include "hash/hash_append.hpp"

#include "hash/hash_algorithm_concept.hpp"

namespace hash {
    template <class H>
    // requires HashAlgorithm<H> && DefaultConstructible<H>
    struct uhash {
        using result_type = typename H::result_type;

        template <class T>
        result_type
        operator()(T const& t) const noexcept
        {
            H h;
            using hash::hash_append;
            hash_append(h, t);
            return static_cast<result_type>(h);
        }
    };
    
} // namespace hash
