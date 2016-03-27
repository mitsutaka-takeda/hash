#pragma once

#include <cstdint>
#include <limits>

namespace hash {
    struct bitwise_operations {

        template <typename T>
        static
        constexpr 
        T
        ROTR(uint8_t n, T x) noexcept;
    };
} // namespace hash

template <typename T>
constexpr 
T
hash::bitwise_operations::ROTR(uint8_t n, T x) noexcept {
    return (x >> n) | (x << (std::numeric_limits<uint32_t>::digits - n));
}
