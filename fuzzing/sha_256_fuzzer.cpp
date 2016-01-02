#include "hash/sha_256.hpp"
extern "C" int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size);

extern "C" 
int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size) {

    hash::sha_256 h;
    h(data, size);
    return 0;
}
