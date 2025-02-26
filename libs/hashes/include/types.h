#ifndef HASHES_TYPES_H
#define HASHES_TYPES_H

#include <primitive-types/u256.h>
#include <stdint.h>

#define DEFINE_FIXED_U8_ARRAY(NAME, SIZE)                                                                              \
    typedef struct {                                                                                                   \
        uint8_t digest[SIZE];                                                                                          \
    } NAME;

DEFINE_FIXED_U8_ARRAY(h256, 32)

#endif
