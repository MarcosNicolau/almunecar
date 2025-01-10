#ifndef SHA2_H
#define SHA2_H

#include <primitive-types/u256.h>
#include <stdint.h>

typedef struct {
    uint32_t h[8];
    uint8_t bytes[64];
    // keeps track of the bytes array size
    uint64_t bytes_size;
    // keeps track of the whole messages size (when bytes reaches 64 it
    // restarts)
    uint64_t total_size;
} sha256;

sha256 sha256_new();
void sha256_update(sha256 *, uint8_t *bytes, size_t size);
u256 sha256_finalize(sha256 *);

#endif
