#pragma once

#include <array>
#include <bitset>
#include <functional>
#include <range/v3/action/transform.hpp>
#include <range/v3/algorithm/for_each.hpp>
#include <range/v3/numeric/accumulate.hpp>
#include <range/v3/view/zip.hpp>

#include "integral.hpp"
#include "bitwise_operations.hpp"

namespace hash {
    // Implementation of KECCAK-256 algorithm
    // http://keccak.noekeon.org/Keccak-implementation-3.2.pdf
    class keccak_256 {
    public:
        using result_type = hash::uint256_t;

        template <uint8_t w>
        using lane_type = std::bitset<w>;
        // A lane with th length w. w is related to the width of
        // permutation b with b = 25w. Since b is in {25, 50, 100, 200, 400, 800, 1600},
        // w is in {1, 2, 4, 8, 16, 32, 64}.

        template <uint8_t w>
        using state_type = std::array<std::array<lane_type<w>, 5>, 5>;
        // A state matrix with the lane_type<w>. The template parameter w is
        // the length of the lane.

        static
        std::array<uint64_t, 24> const RC;
        // The round constants of KECCAK-f round.

        template <uint32_t r, uint32_t c>
        static
        result_type
        KECCAK(const void* M);
        // Compute KECCAK[r, c] sponge function where, r is the bitrate, and c is the capacity.

        template <uint32_t b>
        static
        state_type<b/25>&
        KECCAK_f(state_type<b/25>& A) noexcept;
        // Returns the KECCAK-f permutations of A described in the KECCAK implementation overview version 3.2.
        // http://keccak.noekeon.org/Keccak-implementation-3.2.pdf
        // The template parameter b is the width of permutation in bits and it must be one of
        // {25, 50, 100, 200, 400, 800, 1600}.

        template <uint32_t b>
        static
        state_type<b/25>&
        Round(
            state_type<b/25>,
            uint64_t RCi
            ) noexcept;

        template <uint32_t b>
        static
        void
        theta_step(state_type<b/25>& A) noexcept;

        template <uint32_t b>
        static
        void
        rho_and_pi_steps(
            state_type<b/25> const & A,
            state_type<b/25>& B
            ) noexcept;

        template <uint32_t b>
        static
        void
        chi_step(
            state_type<b/25>& A,
            state_type<b/25> const & B
            ) noexcept;

        template <uint32_t b>
        static
        void
        iota_step(
            state_type<b/25>& A,
            uint64_t RCi
            ) noexcept;
    };
} // namespace hash

std::array<uint64_t, 24> const
hash::keccak_256::RC = {{
        0x00000000'00000001,
        0x00000000'00008082,
        0x80000000'0000808A,
        0x80000000'80008000,
        0x00000000'0000808B,
        0x00000000'80000001,
        0x80000000'80008081,
        0x80000000'00008009,
        0x00000000'0000008A,
        0x00000000'00000088,
        0x00000000'80008009,
        0x00000000'8000000A,
        0x00000000'8000808B,
        0x80000000'0000008B,
        0x80000000'00008089,
        0x80000000'00008003,
        0x80000000'00008002,
        0x80000000'00000080,
        0x00000000'0000800A,
        0x80000000'8000000A,
        0x80000000'80008081,
        0x80000000'00008080,
        0x00000000'80000001,
        0x80000000'80008008
    }};

template <uint32_t Bitrate, uint32_t Capacity>
hash::keccak_256::result_type
hash::keccak_256::KECCAK(const void* /*M*/) {
    // Padding

    // Initialization
    state_type<Bitrate + Capacity> S{};

    // Absorbing Phase

    // Squeezing Phase
    return 0;
}

template <uint32_t b>
inline
hash::keccak_256::state_type<b/25>&
hash::keccak_256::KECCAK_f(hash::keccak_256::state_type<b/25> & A) noexcept {
    constexpr auto n = 12 + 2 * std::log2(b/25);
    for(int i = 0; i < n; ++i){
        A = Round<b>(A, RC[i]);
    }
    return A;
}

template <uint32_t b>
inline
hash::keccak_256::state_type<b/25>&
hash::keccak_256::Round(
    hash::keccak_256::state_type<b/25> A,
    uint64_t RCi
    ) noexcept {

    theta_step(A);
    state_type<b/25> B;
    rho_and_pi_steps(A, B);
    chi_step(A, B);
    iota_step(A, RCi);
}

template <uint32_t b>
inline
void
hash::keccak_256::theta_step(hash::keccak_256::state_type<b/25>& A) noexcept {
    constexpr auto w = b/25;
    static_assert(w == b/25, "w must be b/25.");

    std::array<hash::keccak_256::lane_type<w>, 5> C;
    ranges::v3::for_each(
        A,
        [x = 0u, &C]
        (auto const& row) mutable {
            C[x] = ranges::v3::accumulate(row, hash::keccak_256::lane_type<w>{}, std::bit_xor<>());
            ++x;
        });

    std::array<hash::keccak_256::lane_type<w>, 5> D;
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
                [dx = std::get<1>(ax_and_dx)](auto const& axy) { return axy ^ dx; });
        });
}

template <uint32_t b>
inline
void
hash::keccak_256::rho_and_pi_steps(
    hash::keccak_256::state_type<b/25> const & A,
    hash::keccak_256::state_type<b/25>& B
    ) noexcept {
    std::array<std::array<uint8_t, 5>, 5> const r = {{
        {{0,  36, 3,  41, 18}},
        {{1,  44, 10, 45, 2}},
        {{62, 6,  43, 15, 61}},
        {{28, 55, 25, 21, 56}},
        {{27, 20, 39, 8,  14}}
        }};

    ranges::v3::for_each(
        ranges::v3::view::zip(A, r),
        [x = 0u, &B](auto ax_and_rx) mutable{
            ranges::v3::for_each(
                ranges::v3::view::zip(std::get<0>(ax_and_rx),
                                      std::get<1>(ax_and_rx)),
                [y = 0u, &B, x]
                (auto axy_and_rxy) mutable {
                    B[y][(2*x + 3*y) % 5] = hash::bitwise_operations::ROTR(
                        std::get<1>(axy_and_rxy),
                        std::get<0>(axy_and_rxy)
                        );
                    ++y;
                });
            ++x;
        });
}

template <uint32_t b>
inline
void
hash::keccak_256::chi_step(
    hash::keccak_256::state_type<b/25>& A,
    hash::keccak_256::state_type<b/25> const & B
    ) noexcept {
    ranges::v3::for_each(
        A,
        [x = 0u, &B] (auto& ax) mutable {
            ranges::v3::for_each(
                ax,
                [x, y = 0u, &B] (auto& axy) mutable {
                    axy = B[x][y] ^ (~B[(x+1)%5][y] & B[(x+2)%5][y]);
                    ++y;
                });
            ++x;
        });
}

template <uint32_t b>
inline
void
hash::keccak_256::iota_step(
    hash::keccak_256::state_type<b/25>& A,
    uint64_t RCi
    ) noexcept {
    A[0][0] ^= RCi;
}
