#ifndef BIGUINT_H
#define BIGUINT_H

#include "u64.h"
#include <stdlib.h>
#include <utils/macros.h>

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
 * You must call `biguint_free_limbs` to release the memory after use to avoid memory leaks.
 *
 * @example
 * ```
 * BigUint num = biguint_new_heap(10);  // Allocate a BigUint with 10 limbs on the heap
 * biguint_free_limbs(&num);  // Don't forget to free the memory after use!
 * ```
 */
#define biguint_new_heap(SIZE)                                                                                         \
    (BigUint) { .size = (SIZE), .limbs = malloc(sizeof(uint64_t) * (SIZE)) }

/**
 * Frees the memory allocated for one or more BigUint variables.
 *
 * @param ... Variadic arguments of BigUint pointers to be freed.
 *
 * @example
 * ```
 * BigUint a = biguint_new(10);
 * BigUint b = biguint_new(20);
 * biguint_free(&a, &b);  // Frees memory for `a` and `b`
 * ```
 */
#define biguint_free(...)                                                                                              \
    BigUint *ANONYMOUS_VARIABLE(args)[] = {__VA_ARGS__};                                                               \
    for (size_t i = 0; i < sizeof(ANONYMOUS_VARIABLE(args)) / sizeof(ANONYMOUS_VARIABLE(args)[0]); i++)                \
        biguint_free_limbs(ANONYMOUS_VARIABLE(args)[i]);

/**
 * Creates a `BigUint` on the stack with a specified number of limbs, all initialized to 0.
 *
 * @param SIZE The number of limbs (64-bit integers) for the `BigUint`.
 *
 * @example
 * ```
 * BigUint num = biguint_new(5);  // Creates a BigUint with 5 limbs, all initialized to 0
 * ```
 */
#define biguint_new(SIZE)                                                                                              \
    (BigUint) { .size = (SIZE), .limbs = (uint64_t[SIZE]){0} }

/**
 * Creates a `BigUint` on the stack with a specified number of limbs and initializes them with given values.
 *
 * @param SIZE The number of limbs (64-bit integers) for the `BigUint`.
 * @param {1, 2, 3, ...} The values for initializing the limbs of the `BigUint`.
 *
 * @example
 * ```
 * BigUint num = biguint_new_with_limbs(3, {10, 20, 30});  // Creates a BigUint with 3 limbs, initialized to 10, 20,
 * and 30
 * ```
 */
#define biguint_new_with_limbs(SIZE, ...)                                                                              \
    (BigUint) { .size = (SIZE), .limbs = (uint64_t[SIZE])__VA_ARGS__ } // Initialize with custom values

/**
 * Creates a `BigUint` on the stack with a specified number of limbs and initializes them with given values.
 *
 * @param SIZE The number of limbs (64-bit integers) for the `BigUint`.
 * @param {1, 2, 3, ...} The values for initializing the limbs of the `BigUint`.
 *
 * @example
 * ```
 * BigUint num = biguint_new_with_limbs(3, {10, 20, 30});  // Creates a BigUint with 3 limbs, initialized to 10, 20,
 * and 30
 * ```
 */
#define biguint_new_from_limbs(SIZE, LIMBS)                                                                            \
    (BigUint) { .size = (SIZE), .limbs = LIMBS } // Initialize with custom values

/**
 * Sets the value of the BigUint to zero.
 *
 * @param out Pointer to the BigUint to be zeroed.
 *
 * @example
 * ```
 * BigUint num;
 * biguint_zero(&num);  // Set `num` to zero
 * ```
 */
void biguint_zero(BigUint *out);

/**
 * Sets the value of the BigUint to one.
 *
 * @param out Pointer to the BigUint to be set to one.
 *
 * @example
 * ```
 * BigUint num;
 * biguint_one(&num);  // Set `num` to one
 * ```
 */
void biguint_one(BigUint *out);

/**
 * Initializes a BigUint with a 64-bit unsigned integer value.
 *
 * @param val The 64-bit unsigned integer value.
 * @param out Pointer to the BigUint to store the value.
 *
 * @example
 * ```
 * BigUint num;
 * biguint_from_u64(12345, &num);  // Set `num` to the value 12345
 * ```
 */
void biguint_from_u64(uint64_t val, BigUint *out);

/**
 * Initializes a BigUint from a decimal string.
 *
 * @param str The decimal string to convert.
 * @param out Pointer to the BigUint to store the value.
 *
 * @example
 * ```
 * BigUint num;
 * biguint_from_dec_string("12345", &num);  // Convert the string "12345" into a BigUint
 * ```
 */
void biguint_from_dec_string(char *str, BigUint *out);

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
 * biguint_from_bytes_big_endian(bytes, &num);  // Convert the byte array to BigUint
 * ```
 */
void biguint_from_bytes_big_endian(uint8_t *bytes, BigUint *out);

/**
 * Retrieves the BigUint value as a byte array in big-endian format.
 *
 * @param value The BigUint value.
 * @param buffer The byte array to store the result.
 *
 * @example
 * ```
 * BigUint num = biguint_new(2);
 * uint8_t buffer[2];
 * biguint_get_bytes_big_endian(num, buffer);  // Store the value of `num` in big-endian format
 * ```
 */
void biguint_get_bytes_big_endian(BigUint value, uint8_t *buffer);

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
 * biguint_from_bytes_little_endian(bytes, &num);  // Convert the little-endian byte array to BigUint
 * ```
 */
void biguint_from_bytes_little_endian(uint8_t *bytes, BigUint *out);

/**
 * Retrieves the BigUint value as a byte array in little-endian format.
 *
 * @param value The BigUint value.
 * @param buffer The byte array to store the result.
 *
 * @example
 * ```
 * BigUint num = biguint_new(2);
 * uint8_t buffer[2];
 * biguint_get_bytes_little_endian(num, buffer);  // Store the value of `num` in little-endian format
 * ```
 */
void biguint_get_bytes_little_endian(BigUint value, uint8_t *buffer);

/**
 * Converts a BigUint to a decimal string.
 *
 * @param a The BigUint value to convert.
 * @return The decimal string representation of the BigUint.
 *
 * @example
 * ```
 * BigUint num = biguint_new(2);
 * char *str = biguint_to_dec_string(num);  // Convert the BigUint to a decimal string
 * ```
 */
char *biguint_to_dec_string(BigUint a);

/**
 * Copies the value of one BigUint to another.
 *
 * @param dst The destination BigUint.
 * @param src The source BigUint.
 *
 * @example
 * ```
 * BigUint src = biguint_new(1);
 * BigUint dst;
 * biguint_cpy(&dst, src);  // Copy the value of `src` to `dst`
 * ```
 */
void biguint_cpy(BigUint *dst, BigUint src);

/**
 * Returns the number of bits required to represent the BigUint.
 *
 * @param a The BigUint value.
 * @return The number of bits.
 *
 * @example
 * ```
 * BigUint num = biguint_new(3);
 * int bits = biguint_bits(num);  // Get the number of bits needed to represent `num`
 * ```
 */
int biguint_bits(BigUint a);

/**
 * Checks if the BigUint value is zero.
 *
 * @param a The BigUint value.
 * @return 1 if `a` is zero, 0 otherwise.
 *
 * @example
 * ```
 * BigUint num = biguint_new(1);
 * int isZero = biguint_is_zero(num);  // Check if `num` is zero
 * ```
 */
int biguint_is_zero(BigUint a);

/**
 * Compares two BigUint values.
 *
 * @param a The first BigUint value.
 * @param b The second BigUint value.
 * @return 0 if they are equal, a negative value if `a` < `b`, or a positive value if `a` > `b`.
 *
 * @example
 * ```
 * BigUint a = biguint_new(1);
 * BigUint b = biguint_new(1);
 * int comparison = biguint_cmp(a, b);  // Compare `a` and `b`
 * ```
 */
int biguint_cmp(BigUint a, BigUint b);

/**
 * Frees the memory allocated for the limbs of a BigUint that was allocated on the heap.
 *
 * @param a Pointer to the BigUint whose limbs should be freed.
 *
 * @example
 * ```
 * BigUint num = biguint_new_heap(5);
 * biguint_free_limbs(&num);  // Free the memory after use
 * ```
 */
void biguint_free_limbs(BigUint *a);

/**
 * Adds two BigUint values and stores the result in the first operand.
 *
 * @param a Pointer to the first BigUint (result stored here).
 * @param b The second BigUint to add.
 *
 * @example
 * ```
 * BigUint a = biguint_new(1);
 * BigUint b = biguint_new(1);
 * biguint_add(&a, b);  // Add `b` to `a`
 * ```
 */
void biguint_add(BigUint *a, BigUint b);

/**
 * Subtracts the second BigUint from the first and stores the result in the first operand.
 *
 * @param a Pointer to the first BigUint (result stored here).
 * @param b The second BigUint to subtract.
 *
 * @example
 * ```
 * BigUint a = biguint_new(1);
 * BigUint b = biguint_new(1);
 * biguint_sub(&a, b);  // Subtract `b` from `a`
 * ```
 */
void biguint_sub(BigUint *a, BigUint b);

/**
 * Multiplies two BigUint values and stores the result in the first operand.
 *
 * @param a Pointer to the first BigUint (result stored here).
 * @param b The second BigUint to multiply.
 *
 * @example
 * ```
 * BigUint a = biguint_new(1);
 * BigUint b = biguint_new(1);
 * biguint_mul(&a, b);  // Multiply `a` and `b`
 * ```
 */
void biguint_mul(BigUint *a, BigUint b);

/**
 * Checks for overflow when adding two BigUint values.
 *
 * @param a Pointer to the first BigUint (result stored here).
 * @param b The second BigUint to add.
 * @return 1 if overflow occurs, 0 otherwise.
 *
 * @example
 * ```
 * BigUint a = biguint_new(1);
 * BigUint b = biguint_new(1);
 * int overflow = biguint_overflow_add(&a, b);  // Check if adding `a` and `b` overflows
 * ```
 */
int biguint_overflow_add(BigUint *a, BigUint b);

/**
 * Computes the addition of a + b (mod m).
 *
 * @param a Pointer to the first BigUint (result stored here).
 * @param b The second BigUint to add.
 * @param m The third BigUint defining the modulus.
 *
 * @example
 * ```
 * BigUint a = biguint_new(1);
 * BigUint b = biguint_new(1);
 * BigUint m = biguint_new(1);
 * biguint_add_mod(&a, b, m);
 * ```
 */
void biguint_add_mod(BigUint *a, BigUint b, BigUint m);

/**
 * Checks for overflow when subtracting two BigUint values.
 *
 * @param a Pointer to the first BigUint (result stored here).
 * @param b The second BigUint to subtract.
 * @return 1 if overflow occurs, 0 otherwise.
 *
 * @example
 * ```
 * BigUint a = biguint_new(1);
 * BigUint b = biguint_new(1);
 * int overflow = biguint_overflow_sub(&a, b);  // Check if subtracting `b` from `a` overflows
 * ```
 */
int biguint_overflow_sub(BigUint *a, BigUint b);

/**
 * Computes the substraction of a - b (mod m).
 *
 * @param a Pointer to the first BigUint (result stored here).
 * @param b The second BigUint to substract.
 * @param m The third BigUint defining the modulus.
 *
 * @example
 * ```
 * BigUint a = biguint_new(1);
 * BigUint b = biguint_new(1);
 * BigUint m = biguint_new(1);
 * biguint_sub_mod(&a, b, m);
 * ```
 */
void biguint_sub_mod(BigUint *a, BigUint b, BigUint m);

/**
 * Checks for overflow when multiplying two BigUint values.
 *
 * @param a Pointer to the first BigUint (result stored here).
 * @param b The second BigUint to multiply.
 * @return 1 if overflow occurs, 0 otherwise.
 *
 * @example
 * ```
 * BigUint a = biguint_new(1);
 * BigUint b = biguint_new(1);
 * int overflow = biguint_overflow_mul(&a, b);  // Check if multiplying `a` and `b` overflows
 * ```
 */
int biguint_overflow_mul(BigUint *a, BigUint b);

/**
 * Computes the multiplication of a * b (mod m).
 *
 * @param a Pointer to the first BigUint (result stored here).
 * @param b The second BigUint to multiply.
 * @param m The third BigUint defining the modulus.
 *
 * @example
 * ```
 * BigUint a = biguint_new(1);
 * BigUint b = biguint_new(1);
 * BigUint m = biguint_new(1);
 * biguint_mul_mod(&a, b, m);
 * ```
 */
void biguint_mul_mod(BigUint *a, BigUint b, BigUint m);

/**
 * Computes the power of a BigUint to an exponent and checks for overflow.
 *
 * @param a Pointer to the base (BigUint).
 * @param exponent The exponent (BigUint).
 * @return Returns 1 if the operation overflows, 0 otherwise.
 *
 * @example
 * ```
 * BigUint base = biguint_new(2);
 * BigUint exponent = biguint_new(1000);
 * int has_overflowed = biguint_overflow_pow(&base, exponent);
 * ```
 */
int biguint_overflow_pow(BigUint *a, BigUint exponent);

/**
 * Computes the power of a BigUint raised to an exponent.
 *
 * @param a Pointer to the base (BigUint). The result is stored in this variable.
 * @param exponent The exponent (BigUint).
 *
 * @example
 * ```
 * BigUint base = biguint_new(2);
 * BigUint exponent = biguint_new(10);
 * biguint_pow(&base, exponent);  // `base` is now 1024
 * ```
 */
void biguint_pow(BigUint *a, BigUint exponent);

/**
 * Computes the power of a BigUint keeping the result in bounds over a mod m
 *
 * @param a Pointer to the base (BigUint). The result is stored in this variable.
 * @param m The modulus (BigUint). The result is computed modulo this value.
 * @param exponent The exponent (BigUint). The base `a` is raised to this power.
 *
 * @example
 * ```
 * BigUint base = biguint_new(2);
 * BigUint modulus = biguint_new(1000);
 * BigUint exponent = biguint_new(10);
 * biguint_pow_mod(&base, modulus, exponent);  // `base` is now (2^10) % 1000 = 24
 * ```
 */
void biguint_pow_mod(BigUint *a, BigUint exponent, BigUint m);

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
 * BigUint a = biguint_new(2);
 * BigUint b = biguint_new(2);
 * BigUint quot, rem;
 * biguint_divmod(a, b, &quot, &rem);  // Divide `a` by `b` and store the quotient and remainder
 * ```
 */
void biguint_divmod(BigUint a, BigUint b, BigUint *quot, BigUint *rem);

/**
 * Computes the quotient of one BigUint divided by another.
 *
 * @param a The dividend (BigUint).
 * @param b The divisor (BigUint).
 * @param out Pointer to store the quotient.
 *
 * @example
 * ```
 * BigUint a = biguint_new(10);
 * BigUint b = biguint_new(2);
 * BigUint quot;
 * biguint_div(a, b, &quot);  // Quotient `quot` will be 5
 * ```
 */
void biguint_div(BigUint a, BigUint b, BigUint *out);

/**
 * Computes the remainder of one BigUint divided by another.
 *
 * @param a The dividend (BigUint).
 * @param b The divisor (BigUint).
 * @param out Pointer to store the remainder.
 *
 * @example
 * ```
 * BigUint a = biguint_new(10);
 * BigUint b = biguint_new(3);
 * BigUint rem;
 * biguint_mod(a, b, &rem);  // Remainder `rem` will be 1
 * ```
 */
void biguint_mod(BigUint a, BigUint b, BigUint *out);

/**
 * Checks if a BigUint is even.
 *
 * @param a The BigUint to check.
 * @return Returns 1 if `a` is even, 0 otherwise.
 *
 * @example
 * ```
 * BigUint num = biguint_new(10);
 * if (biguint_is_even(num)) {
 *     printf("The number is even.\n");
 * } else {
 *     printf("The number is odd.\n");
 * }
 * ```
 */
int biguint_is_even(BigUint a);

/**
 * Performs a bitwise AND between two BigUint values.
 *
 * @param a Pointer to the first BigUint (result stored here).
 * @param b The second BigUint to perform AND with.
 *
 * @example
 * ```
 * BigUint a = biguint_new(1);
 * BigUint b = biguint_new(1);
 * biguint_bitand(&a, b);  // Perform bitwise AND between `a` and `b`
 * ```
 */
void biguint_bitand(BigUint *a, BigUint b);

/**
 * Performs a bitwise OR between two BigUint values.
 *
 * @param a Pointer to the first BigUint (result stored here).
 * @param b The second BigUint to perform OR with.
 *
 * @example
 * ```
 * BigUint a = biguint_new(1);
 * BigUint b = biguint_new(1);
 * biguint_bitor(&a, b);  // Perform bitwise OR between `a` and `b`
 * ```
 */
void biguint_bitor(BigUint *a, BigUint b);

/**
 * Performs a bitwise XOR between two BigUint values.
 *
 * @param a Pointer to the first BigUint (result stored here).
 * @param b The second BigUint to perform XOR with.
 *
 * @example
 * ```
 * BigUint a = biguint_new(1);
 * BigUint b = biguint_new(1);
 * biguint_bitxor(&a, b);  // Perform bitwise XOR between `a` and `b`
 * ```
 */
void biguint_bitxor(BigUint *a, BigUint b);

/**
 * Performs a bitwise NOT on a BigUint value.
 *
 * @param a Pointer to the BigUint to perform NOT on.
 *
 * @example
 * ```
 * BigUint a = biguint_new(1);
 * biguint_bitnot(&a);  // Perform bitwise NOT on `a`
 * ```
 */
void biguint_bitnot(BigUint *a);

/**
 * Performs a right shift (logical shift) on a BigUint value.
 *
 * @param a Pointer to the BigUint to shift.
 * @param shift The number of positions to shift.
 *
 * @example
 * ```
 * BigUint a = biguint_new(1);
 * biguint_shr(&a, 2);  // Perform a logical right shift by 2 positions
 * ```
 */
void biguint_shr(BigUint *a, int shift);

/**
 * Performs a left shift on a BigUint value.
 *
 * @param b Pointer to the BigUint to shift.
 * @param shift The number of positions to shift.
 *
 * @example
 * ```
 * BigUint a = biguint_new(1);
 * biguint_shl(&a, 2);  // Perform a left shift by 2 positions
 * ```
 */
void biguint_shl(BigUint *b, int shift);

/**
 * Debugging: Prints a raw representation of a BigUint value (binary).
 *
 * @param a The BigUint to print.
 *
 * @example
 * ```
 * BigUint a = biguint_new(1);
 * biguint_raw_println(a);  // Print raw binary representation of `a`
 * ```
 */
void biguint_raw_println(BigUint a);

/**
 * Debugging: Prints a raw representation of a BigUint value (binary, no newline).
 *
 * @param a The BigUint to print.
 *
 * @example
 * ```
 * BigUint a = biguint_new(1);
 * biguint_raw_print(a);  // Print raw binary representation of `a`
 * ```
 */
void biguint_raw_print(BigUint a);

/**
 * Debugging: Prints a human-readable representation of a BigUint value.
 *
 * @param a The BigUint to print.
 *
 * @example
 * ```
 * BigUint a = biguint_new(1);
 * biguint_println(a);  // Print a human-readable representation of `a`
 * ```
 */
void biguint_println(BigUint a);

/**
 * Debugging: Prints a human-readable representation of a BigUint value (no newline).
 *
 * @param a The BigUint to print.
 *
 * @example
 * ```
 * BigUint a = biguint_new(1);
 * biguint_print(a);  // Print a human-readable representation of `a`
 * ```
 */
void biguint_print(BigUint a);

#endif
