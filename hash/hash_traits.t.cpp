#define BOOST_TEST_MODULE hash_traits_test
#include <boost/test/included/unit_test.hpp>
#include <rapidcheck/boost_test.h>

#include <cstring>

#include "hash/hash_traits.hpp"

BOOST_AUTO_TEST_SUITE(Integral_is_contiguously_hashable)

namespace  {
    template <typename Candidate>
    void
    check_contiguously_hashability(Candidate x) {
        static_assert(hash::is_contiguously_hashable_v<Candidate>,
                      "Candidate must be declared to continuously hashable.");
        const auto y = x;
        RC_ASSERT(x == y);
        RC_ASSERT(std::memcmp(std::addressof(x), std::addressof(y), sizeof(Candidate)) == 0);
    }
}

RC_BOOST_PROP(int_is_contiguously_hashable, (int a)) {
    check_contiguously_hashability(a);
}

RC_BOOST_PROP(int8_t_is_contiguously_hashable, (int8_t a)) {
    check_contiguously_hashability(a);
}

RC_BOOST_PROP(int16_t_is_contiguously_hashable, (int16_t a)) {
    check_contiguously_hashability(a);
}


BOOST_AUTO_TEST_SUITE_END()
