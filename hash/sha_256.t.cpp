#define BOOST_TEST_MODULE sha_256_test
#include <boost/test/included/unit_test.hpp>

#include <rapidcheck/boost_test.h>

#include "hash/sha_256.hpp"

namespace  {
    
    constexpr
    std::size_t len(char const* str){
        std::size_t r = 0;
        while((*str) != '\0') {
            ++str;
            ++r;
        }
        return r;
    }
    
}

BOOST_AUTO_TEST_SUITE(sha_256)

BOOST_AUTO_TEST_CASE(sha_256_hashes_a_single_block_message) {
   
    using result_t = hash::sha_256::result_type;

    hash::sha_256 h;
    constexpr auto message = "abc";
    BOOST_TEST(len(message) == 3);
    h(message, len(message));

    BOOST_TEST(result_t{"0xba7816bf8f01cfea414140de5dae2223b00361a396177a9cb410ff61f20015ad"} == static_cast<result_t>(h));
}

BOOST_AUTO_TEST_CASE(sha_256_hashes_a_multiple_block_message) {
   
    using result_t = hash::sha_256::result_type;

    hash::sha_256 h;
    constexpr auto message = "abcdbcdecdefdefgefghfghighijhijkijkljklmklmnlmnomnopnopq";
    h(message, len(message));
    BOOST_TEST(result_t{"0x248d6a61d20638b8e5c026930c3e6039a33ce45964ff2167f6ecedd419db06c1"} == static_cast<result_t>(h));
}

BOOST_AUTO_TEST_CASE(sha_256_hashes_a_long_block_message) {
   
    using result_t = hash::sha_256::result_type;

    hash::sha_256 h;
    std::vector<char> const message(1000000, 'a');
    h(message.data(), message.size());
    BOOST_TEST(result_t{"0xcdc76e5c9914fb9281a1c7e284d73e67f1809a48a497200e046d39ccc7112cd0"} == static_cast<result_t>(h));
}

RC_BOOST_PROP(sha_256_hashes_a_same_message_to_the_same_hash_value, (std::string message)){
    hash::sha_256 h0, h1;

    using result_t = hash::sha_256::result_type;
    h0(message.data(), message.size());
    h1(message.data(), message.size());
    RC_ASSERT(static_cast<result_t>(h0) == static_cast<result_t>(h1));
}

BOOST_AUTO_TEST_SUITE_END()
