#pragma once

#include <type_traits>
#include <vector>

#include "hash_traits.hpp"

namespace hash {
    namespace detail {
        template <typename H, typename T, typename Alloc>
        // requires HashAlgorithm<H>
        void
        hash_append_impl(H& h, std::vector<T, Alloc> const& v, std::true_type){
            h(std::addressof(v.data()), v.size() * sizeof(T));
            hash_append(h, v.size());
        }

        template <typename H, typename T, typename Alloc>
        // requires HashAlgorithm<H>
        void
        hash_append_impl(H& h, std::vector<T, Alloc> const& v, std::false_type){
            for(auto&& e: v){
                hash_append(h, e);
            }
            hash_append(h, v.size());
        }
    } // namespace detail

    template <typename H, typename T>
    // requires HashAlgorithm<H>
    inline
    std::enable_if_t<hash::is_contiguously_hashable_v<T> >
    hash_append(H& h, T const& t){
        h(std::addressof(t), sizeof(t));
    }

    template <typename H, typename T, typename Alloc>
    // requires HashAlgorithm<H>
    void
    hash_append(H& h, std::vector<T, Alloc> const& v){
        hash_append_impl(h, v, hash::is_contiguously_hashable<T>{});
    }

} // namespace hash
