#define BOOST_TEST_MODULE message_block_test
#include <boost/test/included/unit_test.hpp>

#include <rapidcheck/boost_test.h>

#include "hash/message_block.hpp"

namespace  {
    using sha_256_message_block = hash::message_block<512>;
} // namespace 

BOOST_AUTO_TEST_SUITE(message_block)

BOOST_AUTO_TEST_CASE(message_block_calculate_the_size_of_paddings) {
    BOOST_TEST(423 == sha_256_message_block::padding_size(25));
    BOOST_TEST(511 == sha_256_message_block::padding_size(449));
    BOOST_TEST(0   == sha_256_message_block::padding_size(448));
}

RC_BOOST_PROP(int_is_contiguously_hashable, (std::size_t x)) {
    constexpr const auto size = sha_256_message_block::BlockSize;
    RC_ASSERT(((x + sha_256_message_block::padding_size(x) + 64u) % size) == 0);
}

BOOST_AUTO_TEST_SUITE_END()
