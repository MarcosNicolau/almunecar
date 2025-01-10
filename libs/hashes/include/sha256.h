#ifndef SHA2_H
#define SHA2_H

#include <primitive-types/u256.h>
#include <stdint.h>

typedef struct {
    uint32_t h[8];
    uint8_t bytes[64];
    uint64_t size;
} sha256;

sha256 sha256_new();
void sha256_update(sha256 *, uint8_t *bytes, unsigned int size);
u256 sha256_finalize(sha256 *);

#endif
