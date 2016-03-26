#pragma once

#include <cstdint>
#include <limits>

namespace hash {
    struct bitwise_operations {
        static
        constexpr uint32_t 
        ROTR(uint8_t n, uint32_t x) noexcept {
            return (x >> n) | (x << (std::numeric_limits<uint32_t>::digits - n));
        }
    };
} // namespace hash
