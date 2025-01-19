#ifndef UINT_H
#define UINT_H

#include <assert.h>
#include <string.h>

#include "u64.h"

/**
 * Defines a new unsigned integer data type.
 *
 * The generated type is a structure named `NAME`, consisting of an
 * array of `WORDS` 64-bit unsigned integers (`uint64_t`). This
 * allows for representing large integers using multiple words.
 *
 * Example:
 *   If `WORDS` is 4 and `NAME` is u256, the resulting type is equivalent to:
 *       typedef struct {
 *           uint64_t parts[4];
 *       } u256;
 */
#define DEFINE_UINT_DATA_TYPE(NAME, WORDS)                                                                             \
    typedef struct {                                                                                                   \
        uint64_t parts[WORDS];                                                                                         \
    } NAME;

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
        uint64_t carry = 0;                                                                                            \
        NAME##_overflow_op op;                                                                                         \
        for (int i = 0; i < WORDS; i++) {                                                                              \
            u64_overflow_op addition = u64_overflow_add(a.parts[i], b.parts[i]);                                       \
            u64_overflow_op carry_addition = u64_overflow_add(addition.res, carry);                                    \
            op.res.parts[i] = carry_addition.res;                                                                      \
            carry = addition.overflow + carry_addition.overflow;                                                       \
        }                                                                                                              \
        op.overflow = (carry > 0);                                                                                     \
        return op;                                                                                                     \
    }

/**                                                                                                                    \
 * Subtracts one unsigned integer from another and detects overflow.                                                   \
 *                                                                                                                     \
 * Returns a structure containing the result and an overflow flag.                                                     \
 */                                                                                                                    \
#define DEFINE_UINT_OVERFLOW_SUB(NAME, WORDS)                                                                          \
    NAME##_overflow_op NAME##_overflow_sub(NAME a, NAME b) {                                                           \
        uint64_t carry = 0;                                                                                            \
        NAME##_overflow_op op;                                                                                         \
        for (int i = 0; i < WORDS; i++) {                                                                              \
            u64_overflow_op sub = u64_overflow_sub(a.parts[i], b.parts[i]);                                            \
            u64_overflow_op carry_sub = u64_overflow_sub(sub.res, carry);                                              \
            op.res.parts[i] = carry_sub.res;                                                                           \
            carry = sub.overflow + carry_sub.overflow;                                                                 \
        }                                                                                                              \
        op.overflow = (carry > 0);                                                                                     \
        return op;                                                                                                     \
    }

/**
 * Performs a bitwise AND operation.
 *
 * Returns the result of `a & b`.
 */
#define DEFINE_UINT_BITADD(NAME, WORDS)                                                                                \
    NAME NAME##_bitand(NAME a, NAME b) {                                                                               \
        NAME result = NAME##_zero();                                                                                   \
        for (int i = 0; i < WORDS; i++)                                                                                \
            result.parts[i] = a.parts[i] & b.parts[i];                                                                 \
        return result;                                                                                                 \
    }

/**
 * Performs a bitwise OR operation.
 *
 * Returns the result of `a | b`.
 */
#define DEFINE_UINT_BITOR(NAME, WORDS)                                                                                 \
    NAME NAME##_bitor(NAME a, NAME b) {                                                                                \
        NAME result = NAME##_zero();                                                                                   \
        for (int i = 0; i < WORDS; i++)                                                                                \
            result.parts[i] = a.parts[i] | b.parts[i];                                                                 \
        return result;                                                                                                 \
    }

/**                                                                                                                    \
 * Performs a bitwise XOR operation.                                                                                   \
 *                                                                                                                     \
 * Returns the result of `a ^ b`.                                                                                      \
 */                                                                                                                    \
#define DEFINE_UINT_BITXOR(NAME, WORDS)                                                                                \
    NAME NAME##_bitxor(NAME a, NAME b) {                                                                               \
        NAME result = NAME##_zero();                                                                                   \
        for (int i = 0; i < WORDS; i++)                                                                                \
            result.parts[i] = a.parts[i] ^ b.parts[i];                                                                 \
        return result;                                                                                                 \
    }
/**                                                                                                                    \
 * Performs a bitwise NOT operation.                                                                                   \
 *                                                                                                                     \
 * Returns the result of `~a`.                                                                                         \
 */                                                                                                                    \
#define DEFINE_UINT_BITNOT(NAME, WORDS)                                                                                \
    NAME NAME##_bitnot(NAME a) {                                                                                       \
        NAME result = NAME##_zero();                                                                                   \
        for (int i = 0; i < WORDS; i++)                                                                                \
            result.parts[i] = ~a.parts[i];                                                                             \
        return result;                                                                                                 \
    }
/** \
 * Multiplies two unsigned integers and detects overflow. \
 *                                                                                                       \
 * Returns a structure containing the result and an overflow flag. \
 */
#define DEFINE_UINT_OVERFLOW_MUL(NAME, WORDS)                                                                          \
    NAME##_overflow_op NAME##_overflow_mul(NAME a, NAME b) {                                                           \
        uint64_t result[WORDS * 2];                                                                                    \
        for (int i = 0; i < WORDS * 2; i++)                                                                            \
            result[i] = 0;                                                                                             \
        for (int i = 0; i < WORDS; i++) {                                                                              \
            uint64_t carry = 0;                                                                                        \
            for (int j = 0; j < WORDS; j++) {                                                                          \
                /* calculate result */                                                                                 \
                u64_mul_op mul = u64_mul(a.parts[j], b.parts[i]);                                                      \
                uint64_t current = result[i + j];                                                                      \
                u64_overflow_op addition = u64_overflow_add(mul.res, current);                                         \
                result[i + j] = addition.res;                                                                          \
                /* calculate carry */                                                                                  \
                uint64_t carry_current = result[i + j + 1];                                                            \
                u64_overflow_op carry_addition = u64_overflow_add(mul.carry + addition.overflow, carry);               \
                u64_overflow_op current_carry_addition = u64_overflow_add(carry_addition.res, carry_current);          \
                result[i + j + 1] = current_carry_addition.res;                                                        \
                carry = carry_addition.overflow | current_carry_addition.overflow;                                     \
            }                                                                                                          \
        }                                                                                                              \
        int overflow = 0;                                                                                              \
        for (int i = 4; i < WORDS * 2; i++) {                                                                          \
            if (result[i] != 0) {                                                                                      \
                overflow = 1;                                                                                          \
                break;                                                                                                 \
            }                                                                                                          \
        }                                                                                                              \
        NAME##_overflow_op op;                                                                                         \
        for (int i = 0; i < WORDS; i++)                                                                                \
            op.res.parts[i] = result[i];                                                                               \
        op.overflow = overflow;                                                                                        \
        return op;                                                                                                     \
    }

/** \
 * Calculates the number of significant bits in the unsigned integer. \
 *                                                                                                       \
 * Returns the number of bits required to represent `a`. \
 */
#define DEFINE_UINT_BITS(NAME, WORDS)                                                                                  \
    int NAME##_bits(NAME a) {                                                                                          \
        for (int i = 1; i < WORDS; i++) {                                                                              \
            if (a.parts[WORDS - i] > 0) {                                                                              \
                return 64 * (WORDS - i + 1) - u64_leading_zeros(a.parts[WORDS - i]);                                   \
            }                                                                                                          \
        }                                                                                                              \
        return 64 - u64_leading_zeros(a.parts[0]);                                                                     \
    }

/** \
 * Shifts the unsigned integer left by the specified number of bits. \
 *                                                                                                       \
 * Returns the result of shifting `a` by `shift` bits to the left. \
 */
#define DEFINE_UINT_SHL(NAME, WORDS)                                                                                   \
    NAME NAME##_shl(NAME a, int shift) {                                                                               \
        NAME result = NAME##_zero();                                                                                   \
        int shift_start = shift / 64;                                                                                  \
        int shift_mod = shift % 64;                                                                                    \
                                                                                                                       \
        for (int i = shift_start; i < WORDS; i++) {                                                                    \
            result.parts[i] = a.parts[i - shift_start] << shift_mod;                                                   \
        }                                                                                                              \
                                                                                                                       \
        /* calculate carry */                                                                                          \
        if (shift_mod > 0) {                                                                                           \
            for (int i = shift_start + 1; i < WORDS; i++) {                                                            \
                result.parts[i] += a.parts[i - shift_start - 1] >> (64 - shift_mod);                                   \
            }                                                                                                          \
        }                                                                                                              \
                                                                                                                       \
        return result;                                                                                                 \
    }

/** \
 * Shifts the unsigned integer right by the specified number of bits. \
 *                                                                                                       \
 * Returns the result of shifting `a` by `shift` bits to the right. \
 */
#define DEFINE_UINT_SHR(NAME, WORDS)                                                                                   \
    NAME NAME##_shr(NAME a, int shift) {                                                                               \
        NAME result = NAME##_zero();                                                                                   \
        int shift_start = shift / 64;                                                                                  \
        int shift_mod = shift % 64;                                                                                    \
                                                                                                                       \
        for (int i = shift_start; i < WORDS; i++) {                                                                    \
            result.parts[i - shift_start] = a.parts[i] >> shift_mod;                                                   \
        }                                                                                                              \
                                                                                                                       \
        /* calculate carry */                                                                                          \
        if (shift_mod > 0) {                                                                                           \
            for (int i = shift_start + 1; i < WORDS; i++) {                                                            \
                result.parts[i - shift_start - 1] += a.parts[i] << (64 - shift_mod);                                   \
            }                                                                                                          \
        }                                                                                                              \
                                                                                                                       \
        return result;                                                                                                 \
    }

/** \
 * Divides one unsigned integer by another, returning the quotient and
 * remainder.                        \
 *                                                                                                       \
 * Returns a structure containing the quotient and remainder of `a / b`. \
 */
#define DEFINE_UINT_DIV_MOD(NAME, WORDS)                                                                               \
    NAME##_div_op NAME##_divmod(NAME a, NAME b) {                                                                      \
        int a_bits = NAME##_bits(a);                                                                                   \
        int b_bits = NAME##_bits(b);                                                                                   \
        NAME quot = NAME##_zero();                                                                                     \
        NAME rem = a;                                                                                                  \
        assert(b_bits != 0);                                                                                           \
        if (a_bits < b_bits)                                                                                           \
            return (NAME##_div_op){.quot = quot, .rem = rem};                                                          \
                                                                                                                       \
        int shift = a_bits - b_bits;                                                                                   \
        NAME shift_copy = NAME##_shl(b, shift);                                                                        \
        while (1) {                                                                                                    \
            /* rem >= shift_copy */                                                                                    \
            if (NAME##_cmp(rem, shift_copy) >= 0) {                                                                    \
                quot.parts[shift / 64] |= ((uint64_t)1 << (uint64_t)(shift % 64));                                     \
                NAME##_overflow_op sub = NAME##_overflow_sub(rem, shift_copy);                                         \
                rem = sub.res;                                                                                         \
            }                                                                                                          \
            if (shift == 0)                                                                                            \
                break;                                                                                                 \
            shift -= 1;                                                                                                \
            shift_copy = NAME##_shr(shift_copy, 1);                                                                    \
        }                                                                                                              \
                                                                                                                       \
        return (NAME##_div_op){.quot = quot, .rem = rem};                                                              \
    }

#define DEFINE_UINT_ZERO(NAME, WORDS)                                                                                  \
    NAME NAME##_zero() {                                                                                               \
        NAME result;                                                                                                   \
        for (int i = 0; i < WORDS; i++)                                                                                \
            result.parts[i] = 0;                                                                                       \
        return result;                                                                                                 \
    }

/** \
 * Parses an unsigned integer from a decimal string. \
 *                                                                                                       \
 * Returns the unsigned integer represented by the string `str`. \
 */
#define DEFINE_UINT_FROM_DEC_STRING(NAME, WORDS)                                                                       \
    NAME NAME##_from_dec_string(char *str) {                                                                           \
        NAME result = NAME##_zero();                                                                                   \
        int len = strlen(str);                                                                                         \
        for (int i = 0; i < len; i++) {                                                                                \
            uint64_t digit = str[i] - '0';                                                                             \
            NAME##_overflow_op mul = NAME##_overflow_mul(result, NAME##_from_u64(10));                                 \
            NAME##_overflow_op addition = NAME##_overflow_add(mul.res, NAME##_from_u64(digit));                        \
            result = addition.res;                                                                                     \
        }                                                                                                              \
        return result;                                                                                                 \
    }

#define DEFINE_UINT_FROM_U64(NAME, WORDS)                                                                              \
    NAME NAME##_from_u64(uint64_t a) {                                                                                 \
        NAME result = NAME##_zero();                                                                                   \
        result.parts[0] = a;                                                                                           \
        return result;                                                                                                 \
    }

#define DEFINE_UINT_FROM_BYTES_BIG_ENDIAN(NAME, WORDS)                                                                 \
    NAME NAME##_from_bytes_big_endian(uint8_t bytes[32]) {                                                             \
        NAME result = NAME##_zero();                                                                                   \
        for (int i = WORDS - 1, j = 0; i >= 0; i--, j++) {                                                             \
            result.parts[i] = ((uint64_t)bytes[j * 8] << 56) | ((uint64_t)bytes[j * 8 + 1] << 48) |                    \
                              ((uint64_t)bytes[j * 8 + 2] << 40) | ((uint64_t)bytes[j * 8 + 3] << 32) |                \
                              ((uint64_t)bytes[j * 8 + 4] << 24) | ((uint64_t)bytes[j * 8 + 5] << 16) |                \
                              ((uint64_t)bytes[j * 8 + 6] << 8) | ((uint64_t)bytes[j * 8 + 7]);                        \
        }                                                                                                              \
        return result;                                                                                                 \
    }

#define DEFINE_UINT_GET_BYTES_BIG_ENDIAN(NAME, WORDS)                                                                  \
    void NAME##_get_bytes_big_endian(uint8_t buffer[32], NAME value) {                                                 \
        for (int i = WORDS - 1, j = 0; i >= 0; i--, j++) {                                                             \
            buffer[i * 8] = (value.parts[j] >> 56) & 0xFF;                                                             \
            buffer[i * 8 + 1] = (value.parts[j] >> 48) & 0xFF;                                                         \
            buffer[i * 8 + 2] = (value.parts[j] >> 40) & 0xFF;                                                         \
            buffer[i * 8 + 3] = (value.parts[j] >> 32) & 0xFF;                                                         \
            buffer[i * 8 + 4] = (value.parts[j] >> 24) & 0xFF;                                                         \
            buffer[i * 8 + 5] = (value.parts[j] >> 16) & 0xFF;                                                         \
            buffer[i * 8 + 6] = (value.parts[j] >> 8) & 0xFF;                                                          \
            buffer[i * 8 + 7] = value.parts[j] & 0xFF;                                                                 \
        }                                                                                                              \
    }

#define DEFINE_UINT_FROM_BYTES_LITTLE_ENDIAN(NAME, WORDS)                                                              \
    NAME NAME##_from_bytes_little_endian(uint8_t bytes[32]) {                                                          \
        NAME result = NAME##_zero();                                                                                   \
        for (int i = 0; i < WORDS; i++) {                                                                              \
            result.parts[i] = ((uint64_t)bytes[i * 8]) | ((uint64_t)bytes[i * 8 + 1] << 8) |                           \
                              ((uint64_t)bytes[i * 8 + 2] << 16) | ((uint64_t)bytes[i * 8 + 3] << 24) |                \
                              ((uint64_t)bytes[i * 8 + 4] << 32) | ((uint64_t)bytes[i * 8 + 5] << 40) |                \
                              ((uint64_t)bytes[i * 8 + 6] << 48) | ((uint64_t)bytes[i * 8 + 7] << 56);                 \
        }                                                                                                              \
        return result;                                                                                                 \
    }

#define DEFINE_UINT_GET_BYTES_LITTLE_ENDIAN(NAME, WORDS)                                                               \
    void NAME##_get_bytes_little_endian(uint8_t buffer[32], NAME value) {                                              \
        for (int i = 0; i < WORDS; i++) {                                                                              \
            buffer[i * 8] = value.parts[i] & 0xFF;                                                                     \
            buffer[i * 8 + 1] = (value.parts[i] >> 8) & 0xFF;                                                          \
            buffer[i * 8 + 2] = (value.parts[i] >> 16) & 0xFF;                                                         \
            buffer[i * 8 + 3] = (value.parts[i] >> 24) & 0xFF;                                                         \
            buffer[i * 8 + 4] = (value.parts[i] >> 32) & 0xFF;                                                         \
            buffer[i * 8 + 5] = (value.parts[i] >> 40) & 0xFF;                                                         \
            buffer[i * 8 + 6] = (value.parts[i] >> 48) & 0xFF;                                                         \
            buffer[i * 8 + 7] = (value.parts[i] >> 56) & 0xFF;                                                         \
        }                                                                                                              \
    }

#define DEFINE_UINT_ONE(NAME, WORDS)                                                                                   \
    NAME NAME##_one() {                                                                                                \
        NAME result = NAME##_zero();                                                                                   \
        result.parts[0] = 1;                                                                                           \
        return result;                                                                                                 \
    }

/**
 * Converts the unsigned integer to its decimal string representation.
 *
 * Returns a heap allocated string representation of the unsigned integer, make
 * sure to free the pointer after using it.
 */
#define DEFINE_UINT_TO_STRING(NAME, WORDS)                                                                             \
    char *NAME##_to_string(NAME a) {                                                                                   \
        char *result = malloc(WORDS * 20 + 1); /* multiply by 20, since each part can                                  \
                                                  take as much as 20 bytes*/                                           \
        int i = WORDS * 20 - 1;                                                                                        \
        NAME ten = NAME##_from_u64(10);                                                                                \
        while (1) {                                                                                                    \
            NAME##_div_op div = NAME##_divmod(a, ten);                                                                 \
            a = div.quot;                                                                                              \
            int digit = div.rem.parts[0] + '0';                                                                        \
            result[i] = digit;                                                                                         \
            if (NAME##_is_zero(a))                                                                                     \
                break;                                                                                                 \
            i -= 1;                                                                                                    \
        }                                                                                                              \
        char *dst = malloc(WORDS * 20 + 1 - i);                                                                        \
        int k = 0;                                                                                                     \
        for (int j = i; j < WORDS * 20 + 1; j++) {                                                                     \
            dst[k] = result[i + k];                                                                                    \
            k++;                                                                                                       \
        }                                                                                                              \
        free(result);                                                                                                  \
        return dst;                                                                                                    \
    }

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
    int NAME##_cmp(NAME a, NAME b) {                                                                                   \
        for (int i = WORDS - 1; i >= 0; i--) {                                                                         \
            uint64_t a_i = (uint64_t)a.parts[i];                                                                       \
            uint64_t b_i = (uint64_t)b.parts[i];                                                                       \
            if (a_i < b_i)                                                                                             \
                return -1;                                                                                             \
            else if (a_i > b_i)                                                                                        \
                return 1;                                                                                              \
        }                                                                                                              \
        return 0;                                                                                                      \
    }

/**                                                                                                                    \
 * Prints an array-like format of the internal                                                                         \
 * `parts` array of the `NAME` structure.                                                                              \
 */                                                                                                                    \
#define DEFINE_UINT_RAW_PRINTLN(NAME, WORDS)                                                                           \
    void NAME##_raw_println(NAME a) {                                                                                  \
        printf("[");                                                                                                   \
        for (int i = 0; i < WORDS; i++) {                                                                              \
            printf("%lu", a.parts[i]);                                                                                 \
            if (i != WORDS - 1)                                                                                        \
                printf(",");                                                                                           \
        }                                                                                                              \
        printf("]\n");                                                                                                 \
    }

/**                                                                                                                    \
 * Print the string representation of the structure followed by a newline.                                             \
 */                                                                                                                    \
#define DEFINE_UINT_PRINTLN(NAME, WORDS)                                                                               \
    void NAME##_println(NAME a) {                                                                                      \
        char *str = NAME##_to_string(a);                                                                               \
        printf("%s\n", str);                                                                                           \
        free(str);                                                                                                     \
    }

/**                                                                                                                    \
 * Print the string representation of the structure.                                                                   \
 */                                                                                                                    \
#define DEFINE_UINT_PRINT(NAME, WORDS)                                                                                 \
    void NAME##_print(NAME a) {                                                                                        \
        char *str = NAME##_to_string(a);                                                                               \
        printf("%s\n", str);                                                                                           \
        free(str);                                                                                                     \
    }
/**
 * Given the UINT `a`
 * Returns:
 * - 1 if `a` is zero.
 * - 0 otherwise.
 */
#define DEFINE_UINT_IS_ZERO(NAME)                                                                                      \
    int NAME##_is_zero(NAME a) { return NAME##_cmp(a, NAME##_zero()) == 0; }

#define DEFINE_UINT(NAME, WORDS)                                                                                       \
    DEFINE_UINT_DATA_TYPE(NAME, WORDS)                                                                                 \
    DEFINE_UINT_OVERFLOW_OP(NAME)                                                                                      \
    DEFINE_UINT_DIV_OP(NAME)                                                                                           \
    DEFINE_UINT_RAW_PRINTLN(NAME, WORDS)                                                                               \
    DEFINE_UINT_COMPARE(NAME, WORDS)                                                                                   \
    DEFINE_UINT_ZERO(NAME, WORDS)                                                                                      \
    DEFINE_UINT_ONE(NAME, WORDS)                                                                                       \
    DEFINE_UINT_IS_ZERO(NAME)                                                                                          \
    DEFINE_UINT_FROM_U64(NAME, WORDS)                                                                                  \
    DEFINE_UINT_FROM_BYTES_BIG_ENDIAN(NAME, WORDS)                                                                     \
    DEFINE_UINT_FROM_BYTES_LITTLE_ENDIAN(NAME, WORDS)                                                                  \
    DEFINE_UINT_GET_BYTES_BIG_ENDIAN(NAME, WORDS)                                                                      \
    DEFINE_UINT_GET_BYTES_LITTLE_ENDIAN(NAME, WORDS)                                                                   \
    DEFINE_UINT_OVERFLOW_ADD(NAME, WORDS)                                                                              \
    DEFINE_UINT_OVERFLOW_SUB(NAME, WORDS)                                                                              \
    DEFINE_UINT_OVERFLOW_MUL(NAME, WORDS)                                                                              \
    DEFINE_UINT_BITADD(NAME, WORDS)                                                                                    \
    DEFINE_UINT_BITOR(NAME, WORDS)                                                                                     \
    DEFINE_UINT_BITXOR(NAME, WORDS)                                                                                    \
    DEFINE_UINT_BITNOT(NAME, WORDS)                                                                                    \
    DEFINE_UINT_SHL(NAME, WORDS)                                                                                       \
    DEFINE_UINT_SHR(NAME, WORDS)                                                                                       \
    DEFINE_UINT_BITS(NAME, WORDS)                                                                                      \
    DEFINE_UINT_DIV_MOD(NAME, WORDS)                                                                                   \
    DEFINE_UINT_FROM_DEC_STRING(NAME, WORDS)                                                                           \
    DEFINE_UINT_TO_STRING(NAME, WORDS)                                                                                 \
    DEFINE_UINT_PRINT(NAME, WORDS)                                                                                     \
    DEFINE_UINT_PRINTLN(NAME, WORDS)

#endif
