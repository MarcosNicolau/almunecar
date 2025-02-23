#ifndef TYPES_H
#define TYPES_H

#include <stdint.h>

// Result like return type like rust
// https://gist.github.com/f0rki/9b2c2b73d46ccdad2b39ab79b3a5517f
#define DEFINE_RESULT(T, E, NAME)                                                                                      \
    typedef struct {                                                                                                   \
        int success;                                                                                                   \
        union {                                                                                                        \
            T result;                                                                                                  \
            E error;                                                                                                   \
        };                                                                                                             \
    } NAME;

#define Err(NAME, E)                                                                                                   \
    (NAME) { .success = 0, .error = E }

#define Ok(NAME, R)                                                                                                    \
    (NAME) { .success = 1, .result = R }

typedef struct {
    uint8_t *array;
    int size;
} UInt8Array;

#endif
