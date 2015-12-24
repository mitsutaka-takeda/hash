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

// BOOST_AUTO_TEST_CASE(sha_256_hashes_a_multiple_block_message) {
   
//     using result_t = hash::sha_256::result_type;

//     hash::sha_256 h;
//     constexpr auto message = "abcdbcdecdefdefgefghfghighijhijkijkljklmklmnlmnomnopnopq";
//     h(message, len(message));

//     BOOST_TEST(result_t{"0xd6a61d20638b8e5c026930c3e6039a33ce45964ff2167f6ecedd419db06c1"} == static_cast<result_t>(h));
// }

BOOST_AUTO_TEST_SUITE_END()
