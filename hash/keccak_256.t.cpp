#define BOOST_TEST_MODULE keccak_test
#include <boost/test/included/unit_test.hpp>
#include <array>

#include "keccak_256.hpp"

BOOST_AUTO_TEST_SUITE(keccak)

BOOST_AUTO_TEST_CASE( theta_step_update_lanes )
{
    std::array<std::array<uint64_t, 5>, 5> a;
    hash::keccak_256::theta_step(a);
}

BOOST_AUTO_TEST_SUITE_END()
