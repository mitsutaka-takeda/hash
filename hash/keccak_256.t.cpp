#define BOOST_TEST_MODULE keccak_test
#include <boost/test/included/unit_test.hpp>

#include <limits>
#include <rapidcheck/boost_test.h>
#include <rapidcheck/gen/Numeric.h>
#include <array>

#include "keccak_256.hpp"

BOOST_AUTO_TEST_SUITE(keccak)

BOOST_AUTO_TEST_CASE( theta_step_update_lanes )
{
    hash::keccak_256::state_type<64> a;
    hash::keccak_256::state_type<64> b;

    hash::keccak_256::theta_step<1600>(a);
    hash::keccak_256::rho_and_pi_steps<1600>(a, b);
    hash::keccak_256::chi_step<1600>(a, b);
    hash::keccak_256::iota_step<1600>(a, 10);
}

RC_BOOST_PROP( padding_length_should_be_multiple_of_first_argument, () ){
    auto const x = *rc::gen::inRange(uint32_t{1}, uint32_t{1600});
    auto const m = *rc::gen::inRange(int64_t{0}, std::numeric_limits<int64_t>::max());
    RC_ASSERT( ((m + std::get<1>(hash::keccak_256::pad(x,m))) % x) == 0 );
}

BOOST_AUTO_TEST_CASE( iota_step_takes_xor_with_a_round_constant ) {

    hash::keccak_256::state_type<1> A{{
            {{0, 0, 0, 0, 0}},
            {{0, 0, 0, 0, 0}},
            {{0, 0, 0, 0, 0}},
            {{0, 0, 0, 0, 0}},
            {{0, 0, 0, 0, 0}}
        }};

    uint64_t acc = 0;
    for(auto i = 0u; i < 23; ++i) {
        // When xor a constant with zero, the answer should be the accumulation of
        // round constants.
        hash::keccak_256::iota_step<25>(A, hash::keccak_256::RC[i]);
        acc ^= hash::keccak_256::RC[i];
        BOOST_TEST( std::bitset<1>(acc) == A[0][0] );
    }
}

BOOST_AUTO_TEST_SUITE_END()
