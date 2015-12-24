#include "hash/sha_256.hpp"

#include <boost/convert.hpp>
#include <boost/convert/stream.hpp>
#include <boost/endian/conversion.hpp>

#include "hash/message_block.hpp"

namespace  {
    constexpr uint32_t Ch(uint32_t x, uint32_t y, uint32_t z){
        return (x & y) ^ ((~x) & z);
    }
    
    constexpr uint32_t Maj(uint32_t x, uint32_t y, uint32_t z) {
        return (x & y) ^ (x & z) ^ (y & z);
    }

    constexpr uint32_t SHR(uint8_t n, uint32_t x){
        return x >> n;
    }

    constexpr uint32_t ROTR(uint8_t n, uint32_t x){
        return (x >> n) | (x << (std::numeric_limits<uint32_t>::digits - n));
    }

    constexpr uint32_t Sigma_0_256(uint32_t x){
        return ROTR(2, x) ^ ROTR(13, x) ^ ROTR(22, x);
    }

    constexpr uint32_t Sigma_1_256(uint32_t x){
        return ROTR(6, x) ^ ROTR(11, x) ^ ROTR(25, x);
    }

    constexpr uint32_t sigma_0_256(uint32_t x){
        return ROTR(7, x) ^ ROTR(18, x) ^ SHR(3, x);
    }

    constexpr uint32_t sigma_1_256(uint32_t x){
        return ROTR(17, x) ^ ROTR(19, x) ^ SHR(10, x);
    }
} // namespace 

void
hash::sha_256::operator()(void const* key, std::size_t len) {
    static const std::array<uint32_t, 64> K = {
        {0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5, 0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,
         0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3, 0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,
         0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc, 0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
         0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7, 0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967,
         0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13, 0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
         0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3, 0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
         0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5, 0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3,
         0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208, 0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2}
    };

    // len*8 + 1 + k = 448 mod 512
    // =>
    // len * 8 + 8 * m = 8 * 56 mod 8 * 64
    //   where 1 + k = 8 * m
    // =>
    // len + m = 56 mod 64
    const auto m = (56 - len + 64) % 64;

    std::vector<char> padded_message(len + m + 8);
    std::copy(reinterpret_cast<char const*>(key), reinterpret_cast<char const*>(key)+len, padded_message.begin());
    padded_message[len] |= 0b10000000;
    const auto len_in_bits = static_cast<uint64_t>(len*8);
    std::copy(std::reverse_iterator<char const*>(reinterpret_cast<char const*>(&len_in_bits) + sizeof(uint64_t)),
              std::reverse_iterator<char const*>(reinterpret_cast<char const*>(&len_in_bits)),
              padded_message.begin() + static_cast<std::vector<char>::difference_type>(len + m));

    const auto msbs = message_block<512>::message_blocks(padded_message);

    std::array<uint32_t, 64> W;// message schedule

    for(auto i = 0ul, N = msbs.size();
        i < N;
        ++i){

        for(uint8_t t = 0; t < 16; ++t){
            W[t] = boost::endian::native_to_big(msbs[i][t]);
        }

        for(uint8_t t = 16; t < 64; ++t){
            W[t] = sigma_1_256(W[t-2]) + W[t-7] + sigma_0_256(W[t-15]) + W[t-16];
        }

        uint32_t
            a = H[0],
            b = H[1], 
            c = H[2],
            d = H[3],
            e = H[4],
            f = H[5], 
            g = H[6], 
            h = H[7];

        for(uint8_t t = 0; t < 64; ++t){
            uint32_t T1 = h + Sigma_1_256(e) + Ch(e, f, g) + K[t] + W[t];
            uint32_t T2 = Sigma_0_256(a) + Maj(a, b, c);
            h = g;
            g = f;
            f = e;
            e = d + T1;
            d = c;
            c = b;
            b = a;
            a = T1 + T2;
        }

        H[0] = a + H[0];
        H[1] = b + H[1];
        H[2] = c + H[2];
        H[3] = d + H[3];
        H[4] = e + H[4];
        H[5] = f + H[5];
        H[6] = g + H[6];
        H[7] = h + H[7];
    }
}

hash::sha_256::operator result_type() {
    std::string r = "0x";
    boost::cnv::cstream ccnv;
    ccnv(std::hex)(std::setw(sizeof(uint32_t)/2))(std::setfill('0'));
    for(auto h : H){
        r += boost::convert<std::string>(h, ccnv).value();
    }
    return result_type{r};
}
