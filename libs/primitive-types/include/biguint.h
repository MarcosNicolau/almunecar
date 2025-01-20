#ifndef BIG_UINT_H
#define BIG_UINT_H

#include "u64.h"
#include <stdlib.h>

typedef struct {
    uint64_t *limbs;
    int size;
} BigUint;

/**
 * Allocates a `BigUint` on the heap at runtime using `malloc`.
 *
 * @param SIZE The number of limbs (64-bit integers) for the `BigUint`.
 *
 * @note
 * You must call `big_uint_free_limbs` to release the memory after use to avoid memory leaks.
 *
 * @example
 * ```
 * BigUint num = big_uint_new_heap(10);  // Allocate a BigUint with 10 limbs on the heap
 * big_uint_free_limbs(&num);  // Don't forget to free the memory after use!
 * ```
 */
#define big_uint_new_heap(SIZE)                                                                                        \
    (BigUint) { .size = (SIZE), .limbs = malloc(sizeof(uint64_t) * (SIZE)) }

#define big_uint_new(SIZE)                                                                                             \
    (BigUint) { .size = (SIZE), .limbs = (uint64_t[SIZE]){0} }

#define big_uint_new_with_limbs(SIZE, ...)                                                                             \
    (BigUint) { .size = (SIZE), .limbs = (uint64_t[SIZE])__VA_ARGS__ }

/**
 * Conversions and initializers
 */
void big_uint_zero(BigUint *out);
void big_uint_one(BigUint *out);
void big_uint_from_u64(uint64_t, BigUint *out);
void big_uint_from_dec_string(char *str, BigUint *out);
void big_uint_from_bytes_big_endian(uint8_t *bytes, BigUint *out);
void big_uint_get_bytes_big_endian(BigUint value, uint8_t *buffer);
void big_uint_from_bytes_little_endian(uint8_t *bytes, BigUint *out);
void big_uint_get_bytes_little_endian(BigUint value, uint8_t *buffer);
char *big_uint_to_dec_string(BigUint a);
void big_uint_cpy(BigUint *dst, BigUint src);

/**
 * Util
 */
int big_uint_bits(BigUint a);
int big_uint_is_zero(BigUint a);
int big_uint_cmp(BigUint a, BigUint b);
void big_uint_free_limbs(BigUint *a);

/**
 * Operations and arithmetics
 */
void big_uint_add(BigUint *a, BigUint b);
void big_uint_sub(BigUint *a, BigUint b);
void big_uint_mul(BigUint *a, BigUint b);
int big_uint_overflow_add(BigUint *a, BigUint b);
int big_uint_overflow_sub(BigUint *a, BigUint b);
int big_uint_overflow_mul(BigUint *a, BigUint b);
void big_uint_divmod(BigUint a, BigUint b, BigUint *quot, BigUint *rem);
void big_uint_bitand(BigUint *a, BigUint b);
void big_uint_bitor(BigUint *a, BigUint b);
void big_uint_bitxor(BigUint *a, BigUint b);
void big_uint_bitnot(BigUint *a);
void big_uint_shr(BigUint *a, int shift);
void big_uint_shl(BigUint *b, int shift);

/**
 * Debugging
 */
void big_uint_raw_println(BigUint a);
void big_uint_raw_print(BigUint a);
void big_uint_println(BigUint a);
void big_uint_print(BigUint a);

#endif
