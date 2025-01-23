#ifndef UINT_H
#define UINT_H

#include <assert.h>
#include <string.h>

#include "biguint.h"

/**
 * ==============================================================================
 * Via macros we define wrappers over big uints for useful sizes such as u256
 * for elliptic curve operations, and other applications.
 * The advantage of using these macros include:
 *
 * - **Static Allocation**: The data is statically allocated at compile-time,
 *   eliminating runtime memory allocation.
 * - **Simplified API**: You don't need to worry about memory allocation or
 *   manually tracking the size of the numbers.
 * - **Easier Reasoning**: The macros help to avoid pitfalls of dynamic memory
 *   allocation and provide a clean, simple interface.
 * - **More Functional API**: These macros encourage functional programming
 *   practices where operations are performed in a predictable manner.
 * ==============================================================================
 */

/**
 * Defines a new unsigned integer data type.
 *
 * The generated type is a structure named `NAME`, consisting of an
 * array of `WORDS` 64-bit unsigned integers (`uint64_t`). This
 * allows for representing large integers using multiple words.
 */
#define DEFINE_UINT_DATA_TYPE(NAME, WORDS)                                                                             \
    typedef struct {                                                                                                   \
        uint64_t limbs[WORDS];                                                                                         \
    } NAME;

/**
 * Converts a custom uint to a `BigUint` structure.
 *
 * This macro converts a type like `u256` to a `BigUint`, which has a dynamically
 * allocated limbs array. The number of limbs is automatically calculated based
 * on the size of the input limbs array.
 *
 * @example
 * ```
 * u256 my_u256 = ...;  // Assume my_u256 is initialized.
 * BigUint my_biguint = uint_to_biguint(my_u256);  // Converts u256 to BigUint.
 * ```
 */
#define uint_to_biguint(a)                                                                                             \
    (BigUint) { .size = sizeof(a.limbs) / sizeof(uint64_t), .limbs = a.limbs }

/**
 * Defines a function to initialize a custom type (`NAME`) from an array of limbs.
 *
 * The function takes a `limbs` array and its size as input and initializes a new
 * instance of the `NAME` type, copying the elements from the input array into
 * the `limbs` field of the structure.
 */
#define DEFINE_UINT_FROM_LIMBS(NAME, WORDS)                                                                            \
    NAME NAME##_from_limbs(uint64_t limbs[WORDS], int limbs_size) {                                                    \
        NAME result = NAME##_zero();                                                                                   \
        int limit;                                                                                                     \
        if (limbs_size < WORDS)                                                                                        \
            limit = limbs_size;                                                                                        \
        else                                                                                                           \
            limit = WORDS;                                                                                             \
        for (int i = 0; i < limit; i++) {                                                                              \
            result.limbs[i] = limbs[i];                                                                                \
        }                                                                                                              \
        return result;                                                                                                 \
    }

/**
 * Converts a `BigUint` structure into a custom integer type.
 *
 * This function converts a `BigUint` into a custom integer type like `u256`, using
 * the limbs from the `BigUint` structure. The resulting custom type is initialized
 * with the limbs from the BigUint.
 * ```
 */
#define DEFINE_UINT_FROM_BIGUINT(NAME, WORDS)                                                                          \
    NAME NAME##_from_biguint(BigUint a) {                                                                              \
        u256 result = u256_from_limbs(a.limbs, a.size);                                                                \
        return result;                                                                                                 \
    }

/**                                                                       \
 * Defines a structure for operations that detect overflow.              \
 *                                                                        \
 * The generated type is a structure named `NAME##_overflow_op`,         \
 * containing:                                                           \
 *   - `res`: A field of type `NAME`, representing the result of the     \
 *     operation.                                                        \
 *   - `overflow`: An integer flag indicating whether an overflow        \
 *     occurred (1 if true, 0 if false).                                 \
 */
#define DEFINE_UINT_OVERFLOW_OP(NAME)                                                                                  \
    typedef struct {                                                                                                   \
        NAME res;                                                                                                      \
        int overflow;                                                                                                  \
    } NAME##_overflow_op;

/**                                                                      \
 * Defines a structure for storing the result of a division operation.   \
 *                                                                        \
 * The generated type is a structure named `NAME##_div_op`, containing:  \
 *   - `quot`: A field of type `NAME`, representing the quotient.         \
 *   - `rem`: A field of type `NAME`, representing the remainder.         \
 *                                                                        \
 */
#define DEFINE_UINT_DIV_OP(NAME)                                                                                       \
    typedef struct {                                                                                                   \
        NAME quot;                                                                                                     \
        NAME rem;                                                                                                      \
    } NAME##_div_op;

/**
 * Adds two unsigned integers and detects overflow.
 *
 * Returns a structure containing the result and an overflow flag.
 */
#define DEFINE_UINT_OVERFLOW_ADD(NAME, WORDS)                                                                          \
    NAME##_overflow_op NAME##_overflow_add(NAME a, NAME b) {                                                           \
        BigUint result = uint_to_biguint(a);                                                                           \
        int overflow = biguint_overflow_add(&result, uint_to_biguint(b));                                              \
        return (NAME##_overflow_op){.res = NAME##_from_biguint(result), .overflow = overflow};                         \
    }

/**                                                                                                                    \
 * Subtracts one unsigned integer from another and detects overflow.                                                   \
 *                                                                                                                     \
 * Returns a structure containing the result and an overflow flag.                                                     \
 */                                                                                                                    \
#define DEFINE_UINT_OVERFLOW_SUB(NAME, WORDS)                                                                          \
    NAME##_overflow_op NAME##_overflow_sub(NAME a, NAME b) {                                                           \
        BigUint result = uint_to_biguint(a);                                                                           \
        int overflow = biguint_overflow_sub(&result, uint_to_biguint(b));                                              \
        return (NAME##_overflow_op){.res = NAME##_from_biguint(result), .overflow = overflow};                         \
    }

/**
 * Performs a bitwise AND operation.
 *
 * Returns the result of `a & b`.
 */
#define DEFINE_UINT_BITAND(NAME, WORDS)                                                                                \
    NAME NAME##_bitand(NAME a, NAME b) {                                                                               \
        BigUint result = uint_to_biguint(a);                                                                           \
        biguint_bitand(&result, uint_to_biguint(b));                                                                   \
        return NAME##_from_biguint(result);                                                                            \
    }

/**
 * Performs a bitwise OR operation.
 *
 * Returns the result of `a | b`.
 */
#define DEFINE_UINT_BITOR(NAME, WORDS)                                                                                 \
    NAME NAME##_bitor(NAME a, NAME b) {                                                                                \
        BigUint result = uint_to_biguint(a);                                                                           \
        biguint_bitor(&result, uint_to_biguint(b));                                                                    \
        return NAME##_from_biguint(result);                                                                            \
    }

/**                                                                                                                    \
 * Performs a bitwise XOR operation.                                                                                   \
 *                                                                                                                     \
 * Returns the result of `a ^ b`.                                                                                      \
 */                                                                                                                    \
#define DEFINE_UINT_BITXOR(NAME, WORDS)                                                                                \
    NAME NAME##_bitxor(NAME a, NAME b) {                                                                               \
        BigUint result = uint_to_biguint(a);                                                                           \
        biguint_bitxor(&result, uint_to_biguint(b));                                                                   \
        return NAME##_from_biguint(result);                                                                            \
    }
/**                                                                                                                    \
 * Performs a bitwise NOT operation.                                                                                   \
 *                                                                                                                     \
 * Returns the result of `~a`.                                                                                         \
 */                                                                                                                    \
#define DEFINE_UINT_BITNOT(NAME, WORDS)                                                                                \
    NAME NAME##_bitnot(NAME a) {                                                                                       \
        BigUint result = uint_to_biguint(a);                                                                           \
        biguint_bitnot(&result);                                                                                       \
        return NAME##_from_biguint(result);                                                                            \
    }

/** \
 * Multiplies two unsigned integers and detects overflow. \
 *                                                                                                       \
 * Returns a structure containing the result and an overflow flag. \
 */
#define DEFINE_UINT_OVERFLOW_MUL(NAME, WORDS)                                                                          \
    NAME##_overflow_op NAME##_overflow_mul(NAME a, NAME b) {                                                           \
        BigUint result = uint_to_biguint(a);                                                                           \
        int overflow = biguint_overflow_mul(&result, uint_to_biguint(b));                                              \
        return (NAME##_overflow_op){.res = NAME##_from_biguint(result), .overflow = overflow};                         \
    }

/** \
 * Calculates the power of two unsigned integers and detects overflow. \
 *                                                                                                       \
 * Returns a structure containing the result and an overflow flag. \
 */
#define DEFINE_UINT_OVERFLOW_POW(NAME, WORDS)                                                                          \
    NAME##_overflow_op NAME##_overflow_pow(NAME a, NAME exponent) {                                                    \
        BigUint result = uint_to_biguint(a);                                                                           \
        int overflow = biguint_overflow_pow(&result, uint_to_biguint(exponent));                                       \
        return (NAME##_overflow_op){.res = NAME##_from_biguint(result), .overflow = overflow};                         \
    }

/** \
 * Calculates the number of significant bits in the unsigned integer. \
 *                                                                                                       \
 * Returns the number of bits required to represent `a`. \
 */
#define DEFINE_UINT_BITS(NAME, WORDS)                                                                                  \
    int NAME##_bits(NAME a) { return biguint_bits(uint_to_biguint(a)); }

/** \
 * Shifts the unsigned integer left by the specified number of bits. \
 *                                                                                                       \
 * Returns the result of shifting `a` by `shift` bits to the left. \
 */
#define DEFINE_UINT_SHL(NAME, WORDS)                                                                                   \
    NAME NAME##_shl(NAME a, int shift) {                                                                               \
        BigUint result = uint_to_biguint(a);                                                                           \
        biguint_shl(&result, shift);                                                                                   \
        return NAME##_from_biguint(result);                                                                            \
    }

/** \
 * Shifts the unsigned integer right by the specified number of bits. \
 *                                                                                                       \
 * Returns the result of shifting `a` by `shift` bits to the right. \
 */
#define DEFINE_UINT_SHR(NAME, WORDS)                                                                                   \
    NAME NAME##_shr(NAME a, int shift) {                                                                               \
        BigUint result = uint_to_biguint(a);                                                                           \
        biguint_shr(&result, shift);                                                                                   \
        return NAME##_from_biguint(result);                                                                            \
    }

/** \
 * Divides one unsigned integer by another, returning the quotient and
 * remainder.                        \
 *                                                                                                       \
 * Returns a structure containing the quotient and remainder of `a / b`. \
 */
#define DEFINE_UINT_DIV_MOD(NAME, WORDS)                                                                               \
    NAME##_div_op NAME##_divmod(NAME a, NAME b) {                                                                      \
        BigUint quot = biguint_new(WORDS);                                                                             \
        BigUint rem = biguint_new(WORDS);                                                                              \
        biguint_divmod(uint_to_biguint(a), uint_to_biguint(b), &quot, &rem);                                           \
        return (NAME##_div_op){.quot = NAME##_from_biguint(quot), .rem = NAME##_from_biguint(rem)};                    \
    }

/** \
 * Divides one unsigned integer by another, returning the remainder only
 */
#define DEFINE_UINT_DIV(NAME, WORDS)                                                                                   \
    NAME NAME##_div(NAME a, NAME b) {                                                                                  \
        BigUint quot = biguint_new(WORDS);                                                                             \
        biguint_div(uint_to_biguint(a), uint_to_biguint(b), &quot);                                                    \
        return NAME##_from_biguint(quot);                                                                              \
    }

/** \
 * Divides one unsigned integer by another, returning the remainder only
 */
#define DEFINE_UINT_MOD(NAME, WORDS)                                                                                   \
    NAME NAME##_mod(NAME a, NAME b) {                                                                                  \
        BigUint rem = biguint_new(WORDS);                                                                              \
        biguint_mod(uint_to_biguint(a), uint_to_biguint(b), &rem);                                                     \
        return NAME##_from_biguint(rem);                                                                               \
    }

/** \
 * Returns:
 * - 1 if number is even
 * - 0 if its odd
 */
#define DEFINE_UINT_IS_EVEN(NAME, WORDS)                                                                               \
    int NAME##_is_even(NAME a) { return biguint_is_even(uint_to_biguint(a)); }

#define DEFINE_UINT_ZERO(NAME, WORDS)                                                                                  \
    NAME NAME##_zero() {                                                                                               \
        NAME result = {0};                                                                                             \
        return result;                                                                                                 \
    }

/** \
 * Parses an unsigned integer from a decimal string. \
 *                                                                                                       \
 * Returns the unsigned integer represented by the string `str`. \
 */
#define DEFINE_UINT_FROM_DEC_STRING(NAME, WORDS)                                                                       \
    NAME NAME##_from_dec_string(char *str) {                                                                           \
        BigUint result = biguint_new(WORDS);                                                                           \
        biguint_from_dec_string(str, &result);                                                                         \
        return NAME##_from_biguint(result);                                                                            \
    }

#define DEFINE_UINT_FROM_U64(NAME, WORDS)                                                                              \
    NAME NAME##_from_u64(uint64_t a) {                                                                                 \
        BigUint result = biguint_new(WORDS);                                                                           \
        biguint_from_u64(a, &result);                                                                                  \
        return NAME##_from_biguint(result);                                                                            \
    }

#define DEFINE_UINT_FROM_BYTES_BIG_ENDIAN(NAME, WORDS)                                                                 \
    NAME NAME##_from_bytes_big_endian(uint8_t *bytes) {                                                                \
        BigUint result = biguint_new(WORDS);                                                                           \
        biguint_from_bytes_big_endian(bytes, &result);                                                                 \
        return NAME##_from_biguint(result);                                                                            \
    }

#define DEFINE_UINT_GET_BYTES_BIG_ENDIAN(NAME, WORDS)                                                                  \
    void NAME##_get_bytes_big_endian(uint8_t *buffer, NAME value) {                                                    \
        biguint_get_bytes_big_endian(uint_to_biguint(value), buffer);                                                  \
    }

#define DEFINE_UINT_FROM_BYTES_LITTLE_ENDIAN(NAME, WORDS)                                                              \
    NAME NAME##_from_bytes_little_endian(uint8_t bytes[32]) {                                                          \
        BigUint result = biguint_new(WORDS);                                                                           \
        biguint_from_bytes_little_endian(bytes, &result);                                                              \
        return NAME##_from_biguint(result);                                                                            \
    }

#define DEFINE_UINT_GET_BYTES_LITTLE_ENDIAN(NAME, WORDS)                                                               \
    void NAME##_get_bytes_little_endian(uint8_t *buffer, NAME value) {                                                 \
        biguint_get_bytes_little_endian(uint_to_biguint(value), buffer);                                               \
    }

#define DEFINE_UINT_ONE(NAME, WORDS)                                                                                   \
    NAME NAME##_one() {                                                                                                \
        NAME result = NAME##_zero();                                                                                   \
        result.limbs[0] = 1;                                                                                           \
        return result;                                                                                                 \
    }

/**
 * Converts the unsigned integer to its decimal string representation.
 *
 * Returns a heap allocated string representation of the unsigned integer, make
 * sure to free the pointer after using it.
 */
#define DEFINE_UINT_TO_STRING(NAME, WORDS)                                                                             \
    char *NAME##_to_dec_string(NAME a) { return biguint_to_dec_string(uint_to_biguint(a)); }

/**
 * @def DEFINE_UINT_COMPARE(NAME, WORDS)
 *
 * Defines a comparison function for a custom integer type.
 *
 * Example:
 * @code
 * DEFINE_UINT_COMPARE(u256, 4);
 * // Generates:
 * // int u256_cmp(u256 a, u256 b) {
 * //   ...
 * // }
 * @endcode
 *
 * @note The generated function compares `a` and `b`:
 * - Returns `-1` if `a < b`
 * - Returns `0` if `a == b`
 * - Returns `1` if `a > b`
 */
#define DEFINE_UINT_COMPARE(NAME, WORDS)                                                                               \
    int NAME##_cmp(NAME a, NAME b) { return biguint_cmp(uint_to_biguint(a), uint_to_biguint(b)); }

/**                                                                                                                    \
 * Prints an array-like format of the internal                                                                         \
 * `parts` array of the `NAME` structure.                                                                              \
 */                                                                                                                    \
#define DEFINE_UINT_RAW_PRINTLN(NAME, WORDS)                                                                           \
    void NAME##_raw_println(NAME a) { biguint_raw_println(uint_to_biguint(a)); }

/**                                                                                                                    \
 * Print the string representation of the structure followed by a newline.                                             \
 */                                                                                                                    \
#define DEFINE_UINT_RAW_PRINT(NAME, WORDS)                                                                             \
    void NAME##_raw_print(NAME a) { biguint_raw_print(uint_to_biguint(a)); }

/**                                                                                                                    \
 * Print the string representation of the structure.                                                                   \
 */                                                                                                                    \
#define DEFINE_UINT_PRINTLN(NAME, WORDS)                                                                               \
    void NAME##_println(NAME a) { biguint_println(uint_to_biguint(a)); }

/**                                                                                                                    \
 * Print the string representation of the structure.                                                                   \
 */                                                                                                                    \
#define DEFINE_UINT_PRINT(NAME, WORDS)                                                                                 \
    void NAME##_print(NAME a) { biguint_print(uint_to_biguint(a)); }
/**
 * Given the UINT `a`
 * Returns:
 * - 1 if `a` is zero.
 * - 0 otherwise.
 */
#define DEFINE_UINT_IS_ZERO(NAME)                                                                                      \
    int NAME##_is_zero(NAME a) { return biguint_is_zero(uint_to_biguint(a)); }

#define DEFINE_UINT(NAME, WORDS)                                                                                       \
    DEFINE_UINT_DATA_TYPE(NAME, WORDS)                                                                                 \
    DEFINE_UINT_OVERFLOW_OP(NAME)                                                                                      \
    DEFINE_UINT_DIV_OP(NAME)                                                                                           \
    DEFINE_UINT_ZERO(NAME, WORDS)                                                                                      \
    DEFINE_UINT_FROM_LIMBS(NAME, WORDS)                                                                                \
    DEFINE_UINT_FROM_BIGUINT(NAME, WORDS)                                                                              \
    DEFINE_UINT_OVERFLOW_ADD(NAME, WORDS)                                                                              \
    DEFINE_UINT_COMPARE(NAME, WORDS)                                                                                   \
    DEFINE_UINT_RAW_PRINTLN(NAME, WORDS)                                                                               \
    DEFINE_UINT_RAW_PRINT(NAME, WORDS)                                                                                 \
    DEFINE_UINT_ONE(NAME, WORDS)                                                                                       \
    DEFINE_UINT_IS_ZERO(NAME)                                                                                          \
    DEFINE_UINT_FROM_U64(NAME, WORDS)                                                                                  \
    DEFINE_UINT_FROM_BYTES_BIG_ENDIAN(NAME, WORDS)                                                                     \
    DEFINE_UINT_FROM_BYTES_LITTLE_ENDIAN(NAME, WORDS)                                                                  \
    DEFINE_UINT_GET_BYTES_BIG_ENDIAN(NAME, WORDS)                                                                      \
    DEFINE_UINT_GET_BYTES_LITTLE_ENDIAN(NAME, WORDS)                                                                   \
    DEFINE_UINT_OVERFLOW_SUB(NAME, WORDS)                                                                              \
    DEFINE_UINT_OVERFLOW_MUL(NAME, WORDS)                                                                              \
    DEFINE_UINT_BITAND(NAME, WORDS)                                                                                    \
    DEFINE_UINT_BITOR(NAME, WORDS)                                                                                     \
    DEFINE_UINT_BITXOR(NAME, WORDS)                                                                                    \
    DEFINE_UINT_BITNOT(NAME, WORDS)                                                                                    \
    DEFINE_UINT_SHL(NAME, WORDS)                                                                                       \
    DEFINE_UINT_SHR(NAME, WORDS)                                                                                       \
    DEFINE_UINT_BITS(NAME, WORDS)                                                                                      \
    DEFINE_UINT_DIV_MOD(NAME, WORDS)                                                                                   \
    DEFINE_UINT_DIV(NAME, WORDS)                                                                                       \
    DEFINE_UINT_MOD(NAME, WORDS)                                                                                       \
    DEFINE_UINT_IS_EVEN(NAME, WORDS)                                                                                   \
    DEFINE_UINT_OVERFLOW_POW(NAME, WORDS)                                                                              \
    DEFINE_UINT_FROM_DEC_STRING(NAME, WORDS)                                                                           \
    DEFINE_UINT_TO_STRING(NAME, WORDS)                                                                                 \
    DEFINE_UINT_PRINT(NAME, WORDS)                                                                                     \
    DEFINE_UINT_PRINTLN(NAME, WORDS)

#endif
