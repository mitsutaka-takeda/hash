#pragma once

#include <cstdint>
#include <vector>

#include <iostream>

namespace hash {
    // A chunk of memory of the size 512 bits.
    template <std::size_t BlockSizeInBits>
    struct message_block {
        using const_iterator = std::vector<char>::const_iterator;
        constexpr static std::size_t BlockSizeInByte = BlockSizeInBits/8;
        constexpr 
        message_block(const_iterator begin_) 
            : begin(begin_)
            {}

        // Access underlying memory as a value of uint32_t.
        // @pre idx < BlockSizeInBits/sizeof(uint32_t)
        // @post return *(reinterpret_cast<uint32_t*>(&(*begin)) + idx)
        uint32_t operator[](uint8_t idx) const {
            return *(reinterpret_cast<uint32_t const*>(&(*begin)) + idx);
        }

        static
        std::vector<message_block>
        message_blocks(std::vector<char>& padded_message) {
            const auto s = padded_message.size()/BlockSizeInByte;
            std::vector<message_block> r;
            r.reserve(s);
            for(auto b = padded_message.cbegin(), e = padded_message.cend();
                b != e;
                b += BlockSizeInByte) {
                r.push_back(b);
            }
            return r;
        }

        const_iterator begin;
    };
} // namespace hash
