#pragma once

#include <vector>

#include "integral.hpp"

namespace hash {
    class sha_256 {
    public:
        using result_type = hash::uint256_t;
        
        void 
        operator()(void const* key, std::size_t len);

        explicit operator result_type();

    private:

        std::vector<uint32_t> H = {0x6a09e667, 0xbb67ae85, 0x3c6ef372, 0xa54ff53a, 0x510e527f, 0x9b05688c, 0x1f83d9ab, 0x5be0cd19};
    };
    
} // namespace hash
