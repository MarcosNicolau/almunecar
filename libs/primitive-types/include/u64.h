#ifndef U64_H
#define U64_H

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

// Result of u64 operations with wrapped result in case of overflow and a boolean indicating if overflow occurred
// - res: returns the wrapped value in case of overflow
// - overflow: 1 if overflow happened, 0 if not.
typedef struct u64_overflow_op
{
    uint64_t res;
    int overflow;
} u64_overflow_op;

u64_overflow_op u64_overflow_add(uint64_t a, uint64_t b);
u64_overflow_op u64_overflow_sub(uint64_t a, uint64_t b);
u64_overflow_op u64_overflow_mul(uint64_t a, uint64_t b);

typedef struct u64_mul
{
    uint64_t res;
    uint64_t carry;
} u64_mul_op;

u64_mul_op u64_mul(uint64_t a, uint64_t b);

int u64_leading_zeros(uint64_t a);

#endif
