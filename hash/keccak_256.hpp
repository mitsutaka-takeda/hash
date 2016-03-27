#pragma once

#include <array>
#include <functional>
#include <range/v3/action/transform.hpp>
#include <range/v3/algorithm/for_each.hpp>
#include <range/v3/numeric/accumulate.hpp>
#include <range/v3/view/zip.hpp>

#include "bitwise_operations.hpp"

namespace hash {
    // Implementation of KECCAK-256 algorithm
    // http://keccak.noekeon.org/Keccak-implementation-3.2.pdf
    class keccak_256 {
    public:
        static
        void
        theta_step(std::array<std::array<uint64_t, 5>, 5>& A) noexcept;
    };
} // namespace hash

void
hash::keccak_256::theta_step(std::array<std::array<uint64_t, 5>, 5>& A) noexcept {
    std::array<uint64_t, 5> C;
    ranges::v3::for_each(
        A,
        [x = 0u, &C]
        (auto const& row) mutable {
            C[x] = ranges::v3::accumulate(row, uint64_t{0}, std::bit_xor<>());
            ++x;
        });

    std::array<uint64_t, 5> D;
    assert(D.size() == 5);
    ranges::v3::for_each(
        D,
        [x = 0u, &C] (auto& d) mutable{
            d = C[(x-1) % 5] ^ hash::bitwise_operations::ROTR(1, C[(x+1) % 5]);
            ++x;
        });

    ranges::v3::for_each(
        ranges::v3::view::zip(A, D),
        [](auto ax_and_dx){
            ranges::v3::action::transform(
                std::get<0>(ax_and_dx),
                [dx = std::get<1>(ax_and_dx)](auto const& a) { return dx*a; });
        });
}
