#define BOOST_TEST_MODULE hash_traits_test
#include <boost/test/included/unit_test.hpp>
#include <rapidcheck/boost_test.h>

#include <cstring>

#include "hash/hash_traits.hpp"

BOOST_AUTO_TEST_SUITE(Integral_is_contiguously_hashable)

RC_BOOST_PROP(int_is_contiguously_hashable, (int a, int b)) {
    RC_PRE(a == b);

    RC_ASSERT(std::memcmp(std::addressof(a), std::addressof(b), sizeof(int)) == 0);
}

BOOST_AUTO_TEST_SUITE_END()
