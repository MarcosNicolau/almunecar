#ifndef BIG_UINT_H
#define BIG_UINT_H

#include "u64.h"
#include <stdlib.h>

typedef struct {
    uint64_t *limbs; // Pointer to an array of 64-bit integers representing the large integer
    int size;        // Number of limbs (64-bit integers) used to represent the value
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

/**
 * Creates a `BigUint` on the stack with a specified number of limbs, all initialized to 0.
 *
 * @param SIZE The number of limbs (64-bit integers) for the `BigUint`.
 *
 * @example
 * ```
 * BigUint num = big_uint_new(5);  // Creates a BigUint with 5 limbs, all initialized to 0
 * ```
 */
#define big_uint_new(SIZE)                                                                                             \
    (BigUint) { .size = (SIZE), .limbs = (uint64_t[SIZE]){0} }

/**
 * Creates a `BigUint` on the stack with a specified number of limbs and initializes them with given values.
 *
 * @param SIZE The number of limbs (64-bit integers) for the `BigUint`.
 * @param {1, 2, 3, ...} The values for initializing the limbs of the `BigUint`.
 *
 * @example
 * ```
 * BigUint num = big_uint_new_with_limbs(3, {10, 20, 30});  // Creates a BigUint with 3 limbs, initialized to 10, 20,
 * and 30
 * ```
 */
#define big_uint_new_with_limbs(SIZE, ...)                                                                             \
    (BigUint) { .size = (SIZE), .limbs = (uint64_t[SIZE])__VA_ARGS__ } // Initialize with custom values

/**
 * Sets the value of the BigUint to zero.
 *
 * @param out Pointer to the BigUint to be zeroed.
 *
 * @example
 * ```
 * BigUint num;
 * big_uint_zero(&num);  // Set `num` to zero
 * ```
 */
void big_uint_zero(BigUint *out);

/**
 * Sets the value of the BigUint to one.
 *
 * @param out Pointer to the BigUint to be set to one.
 *
 * @example
 * ```
 * BigUint num;
 * big_uint_one(&num);  // Set `num` to one
 * ```
 */
void big_uint_one(BigUint *out);

/**
 * Initializes a BigUint with a 64-bit unsigned integer value.
 *
 * @param val The 64-bit unsigned integer value.
 * @param out Pointer to the BigUint to store the value.
 *
 * @example
 * ```
 * BigUint num;
 * big_uint_from_u64(12345, &num);  // Set `num` to the value 12345
 * ```
 */
void big_uint_from_u64(uint64_t val, BigUint *out);

/**
 * Initializes a BigUint from a decimal string.
 *
 * @param str The decimal string to convert.
 * @param out Pointer to the BigUint to store the value.
 *
 * @example
 * ```
 * BigUint num;
 * big_uint_from_dec_string("12345", &num);  // Convert the string "12345" into a BigUint
 * ```
 */
void big_uint_from_dec_string(char *str, BigUint *out);

/**
 * Initializes a BigUint from a byte array in big-endian format.
 *
 * @param bytes The byte array (big-endian) to convert.
 * @param out Pointer to the BigUint to store the value.
 *
 * @example
 * ```
 * uint8_t bytes[] = {0x01, 0x23};
 * BigUint num;
 * big_uint_from_bytes_big_endian(bytes, &num);  // Convert the byte array to BigUint
 * ```
 */
void big_uint_from_bytes_big_endian(uint8_t *bytes, BigUint *out);

/**
 * Retrieves the BigUint value as a byte array in big-endian format.
 *
 * @param value The BigUint value.
 * @param buffer The byte array to store the result.
 *
 * @example
 * ```
 * BigUint num = big_uint_new(2);
 * uint8_t buffer[2];
 * big_uint_get_bytes_big_endian(num, buffer);  // Store the value of `num` in big-endian format
 * ```
 */
void big_uint_get_bytes_big_endian(BigUint value, uint8_t *buffer);

/**
 * Initializes a BigUint from a byte array in little-endian format.
 *
 * @param bytes The byte array (little-endian) to convert.
 * @param out Pointer to the BigUint to store the value.
 *
 * @example
 * ```
 * uint8_t bytes[] = {0x23, 0x01};
 * BigUint num;
 * big_uint_from_bytes_little_endian(bytes, &num);  // Convert the little-endian byte array to BigUint
 * ```
 */
void big_uint_from_bytes_little_endian(uint8_t *bytes, BigUint *out);

/**
 * Retrieves the BigUint value as a byte array in little-endian format.
 *
 * @param value The BigUint value.
 * @param buffer The byte array to store the result.
 *
 * @example
 * ```
 * BigUint num = big_uint_new(2);
 * uint8_t buffer[2];
 * big_uint_get_bytes_little_endian(num, buffer);  // Store the value of `num` in little-endian format
 * ```
 */
void big_uint_get_bytes_little_endian(BigUint value, uint8_t *buffer);

/**
 * Converts a BigUint to a decimal string.
 *
 * @param a The BigUint value to convert.
 * @return The decimal string representation of the BigUint.
 *
 * @example
 * ```
 * BigUint num = big_uint_new(2);
 * char *str = big_uint_to_dec_string(num);  // Convert the BigUint to a decimal string
 * ```
 */
char *big_uint_to_dec_string(BigUint a);

/**
 * Copies the value of one BigUint to another.
 *
 * @param dst The destination BigUint.
 * @param src The source BigUint.
 *
 * @example
 * ```
 * BigUint src = big_uint_new(1);
 * BigUint dst;
 * big_uint_cpy(&dst, src);  // Copy the value of `src` to `dst`
 * ```
 */
void big_uint_cpy(BigUint *dst, BigUint src);

/**
 * Returns the number of bits required to represent the BigUint.
 *
 * @param a The BigUint value.
 * @return The number of bits.
 *
 * @example
 * ```
 * BigUint num = big_uint_new(3);
 * int bits = big_uint_bits(num);  // Get the number of bits needed to represent `num`
 * ```
 */
int big_uint_bits(BigUint a);

/**
 * Checks if the BigUint value is zero.
 *
 * @param a The BigUint value.
 * @return 1 if `a` is zero, 0 otherwise.
 *
 * @example
 * ```
 * BigUint num = big_uint_new(1);
 * int isZero = big_uint_is_zero(num);  // Check if `num` is zero
 * ```
 */
int big_uint_is_zero(BigUint a);

/**
 * Compares two BigUint values.
 *
 * @param a The first BigUint value.
 * @param b The second BigUint value.
 * @return 0 if they are equal, a negative value if `a` < `b`, or a positive value if `a` > `b`.
 *
 * @example
 * ```
 * BigUint a = big_uint_new(1);
 * BigUint b = big_uint_new(1);
 * int comparison = big_uint_cmp(a, b);  // Compare `a` and `b`
 * ```
 */
int big_uint_cmp(BigUint a, BigUint b);

/**
 * Frees the memory allocated for the limbs of a BigUint that was allocated on the heap.
 *
 * @param a Pointer to the BigUint whose limbs should be freed.
 *
 * @example
 * ```
 * BigUint num = big_uint_new_heap(5);
 * big_uint_free_limbs(&num);  // Free the memory after use
 * ```
 */
void big_uint_free_limbs(BigUint *a);

/**
 * Adds two BigUint values and stores the result in the first operand.
 *
 * @param a Pointer to the first BigUint (result stored here).
 * @param b The second BigUint to add.
 *
 * @example
 * ```
 * BigUint a = big_uint_new(1);
 * BigUint b = big_uint_new(1);
 * big_uint_add(&a, b);  // Add `b` to `a`
 * ```
 */
void big_uint_add(BigUint *a, BigUint b);

/**
 * Subtracts the second BigUint from the first and stores the result in the first operand.
 *
 * @param a Pointer to the first BigUint (result stored here).
 * @param b The second BigUint to subtract.
 *
 * @example
 * ```
 * BigUint a = big_uint_new(1);
 * BigUint b = big_uint_new(1);
 * big_uint_sub(&a, b);  // Subtract `b` from `a`
 * ```
 */
void big_uint_sub(BigUint *a, BigUint b);

/**
 * Multiplies two BigUint values and stores the result in the first operand.
 *
 * @param a Pointer to the first BigUint (result stored here).
 * @param b The second BigUint to multiply.
 *
 * @example
 * ```
 * BigUint a = big_uint_new(1);
 * BigUint b = big_uint_new(1);
 * big_uint_mul(&a, b);  // Multiply `a` and `b`
 * ```
 */
void big_uint_mul(BigUint *a, BigUint b);

/**
 * Checks for overflow when adding two BigUint values.
 *
 * @param a Pointer to the first BigUint (result stored here).
 * @param b The second BigUint to add.
 * @return 1 if overflow occurs, 0 otherwise.
 *
 * @example
 * ```
 * BigUint a = big_uint_new(1);
 * BigUint b = big_uint_new(1);
 * int overflow = big_uint_overflow_add(&a, b);  // Check if adding `a` and `b` overflows
 * ```
 */
int big_uint_overflow_add(BigUint *a, BigUint b);

/**
 * Checks for overflow when subtracting two BigUint values.
 *
 * @param a Pointer to the first BigUint (result stored here).
 * @param b The second BigUint to subtract.
 * @return 1 if overflow occurs, 0 otherwise.
 *
 * @example
 * ```
 * BigUint a = big_uint_new(1);
 * BigUint b = big_uint_new(1);
 * int overflow = big_uint_overflow_sub(&a, b);  // Check if subtracting `b` from `a` overflows
 * ```
 */
int big_uint_overflow_sub(BigUint *a, BigUint b);

/**
 * Checks for overflow when multiplying two BigUint values.
 *
 * @param a Pointer to the first BigUint (result stored here).
 * @param b The second BigUint to multiply.
 * @return 1 if overflow occurs, 0 otherwise.
 *
 * @example
 * ```
 * BigUint a = big_uint_new(1);
 * BigUint b = big_uint_new(1);
 * int overflow = big_uint_overflow_mul(&a, b);  // Check if multiplying `a` and `b` overflows
 * ```
 */
int big_uint_overflow_mul(BigUint *a, BigUint b);

/**
 * Divides one BigUint by another, storing the quotient and remainder.
 *
 * @param a The dividend (BigUint).
 * @param b The divisor (BigUint).
 * @param quot Pointer to store the quotient.
 * @param rem Pointer to store the remainder.
 *
 * @example
 * ```
 * BigUint a = big_uint_new(2);
 * BigUint b = big_uint_new(2);
 * BigUint quot, rem;
 * big_uint_divmod(a, b, &quot, &rem);  // Divide `a` by `b` and store the quotient and remainder
 * ```
 */
void big_uint_divmod(BigUint a, BigUint b, BigUint *quot, BigUint *rem);

/**
 * Performs a bitwise AND between two BigUint values.
 *
 * @param a Pointer to the first BigUint (result stored here).
 * @param b The second BigUint to perform AND with.
 *
 * @example
 * ```
 * BigUint a = big_uint_new(1);
 * BigUint b = big_uint_new(1);
 * big_uint_bitand(&a, b);  // Perform bitwise AND between `a` and `b`
 * ```
 */
void big_uint_bitand(BigUint *a, BigUint b);

/**
 * Performs a bitwise OR between two BigUint values.
 *
 * @param a Pointer to the first BigUint (result stored here).
 * @param b The second BigUint to perform OR with.
 *
 * @example
 * ```
 * BigUint a = big_uint_new(1);
 * BigUint b = big_uint_new(1);
 * big_uint_bitor(&a, b);  // Perform bitwise OR between `a` and `b`
 * ```
 */
void big_uint_bitor(BigUint *a, BigUint b);

/**
 * Performs a bitwise XOR between two BigUint values.
 *
 * @param a Pointer to the first BigUint (result stored here).
 * @param b The second BigUint to perform XOR with.
 *
 * @example
 * ```
 * BigUint a = big_uint_new(1);
 * BigUint b = big_uint_new(1);
 * big_uint_bitxor(&a, b);  // Perform bitwise XOR between `a` and `b`
 * ```
 */
void big_uint_bitxor(BigUint *a, BigUint b);

/**
 * Performs a bitwise NOT on a BigUint value.
 *
 * @param a Pointer to the BigUint to perform NOT on.
 *
 * @example
 * ```
 * BigUint a = big_uint_new(1);
 * big_uint_bitnot(&a);  // Perform bitwise NOT on `a`
 * ```
 */
void big_uint_bitnot(BigUint *a);

/**
 * Performs a right shift (logical shift) on a BigUint value.
 *
 * @param a Pointer to the BigUint to shift.
 * @param shift The number of positions to shift.
 *
 * @example
 * ```
 * BigUint a = big_uint_new(1);
 * big_uint_shr(&a, 2);  // Perform a logical right shift by 2 positions
 * ```
 */
void big_uint_shr(BigUint *a, int shift);

/**
 * Performs a left shift on a BigUint value.
 *
 * @param b Pointer to the BigUint to shift.
 * @param shift The number of positions to shift.
 *
 * @example
 * ```
 * BigUint a = big_uint_new(1);
 * big_uint_shl(&a, 2);  // Perform a left shift by 2 positions
 * ```
 */
void big_uint_shl(BigUint *b, int shift);

/**
 * Debugging: Prints a raw representation of a BigUint value (binary).
 *
 * @param a The BigUint to print.
 *
 * @example
 * ```
 * BigUint a = big_uint_new(1);
 * big_uint_raw_println(a);  // Print raw binary representation of `a`
 * ```
 */
void big_uint_raw_println(BigUint a);

/**
 * Debugging: Prints a raw representation of a BigUint value (binary, no newline).
 *
 * @param a The BigUint to print.
 *
 * @example
 * ```
 * BigUint a = big_uint_new(1);
 * big_uint_raw_print(a);  // Print raw binary representation of `a`
 * ```
 */
void big_uint_raw_print(BigUint a);

/**
 * Debugging: Prints a human-readable representation of a BigUint value.
 *
 * @param a The BigUint to print.
 *
 * @example
 * ```
 * BigUint a = big_uint_new(1);
 * big_uint_println(a);  // Print a human-readable representation of `a`
 * ```
 */
void big_uint_println(BigUint a);

/**
 * Debugging: Prints a human-readable representation of a BigUint value (no newline).
 *
 * @param a The BigUint to print.
 *
 * @example
 * ```
 * BigUint a = big_uint_new(1);
 * big_uint_print(a);  // Print a human-readable representation of `a`
 * ```
 */
void big_uint_print(BigUint a);

#endif
