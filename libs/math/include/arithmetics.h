#ifndef ARITHMETICS_H
#define ARITHMETICS_H

#include <primitive-types/biguint.h>

/**
 * Computes the least common multiple between two number via the euclidean algorithm
 * and the relation lcm(a,b) = |ab|/gcd(a,b)
 * https://en.wikipedia.org/wiki/Least_common_multiple
 */
void biguint_lcm(BigUint a, BigUint b, BigUint *out);

// computes the greatest common divisor between a and b via the euclidean algorithm
// https://en.wikipedia.org/wiki/Euclidean_algorithm
void biguint_gcd(BigUint a, BigUint b, BigUint *out);

typedef struct {
    BigUint rk;
    BigUint sk;
    int sk_sign; // -1 if negative, 1 if positive
    BigUint tk;
    int tk_sign; // -1 if negative, 1 if positive
} ExtendedEuclideanAlgorithm;

#define extended_euclidean_algorithm_new_heap(SIZE)                                                                    \
    (ExtendedEuclideanAlgorithm) {                                                                                     \
        .rk = biguint_new_heap(SIZE), .sk = biguint_new_heap(SIZE), .tk = biguint_new_heap(SIZE)                       \
    }

#define extended_euclidean_algorithm_new(SIZE)                                                                         \
    (ExtendedEuclideanAlgorithm) { .rk = biguint_new(SIZE), .sk = biguint_new(SIZE), .tk = biguint_new(SIZE) }

#define extended_euclidean_algorithm_free(str) biguint_free(&str.rk, &str.sk, &str.tk)

void biguint_extended_euclidean_algorithm(BigUint a, BigUint b, ExtendedEuclideanAlgorithm *out);

/**
 * Computes the modular inverse of a number `a` modulo `b` using the modular version of the Extended Euclidean
 * Algorithm.
 *
 * If such an inverse exists, it is stored in `out`. If `a` does not have an inverse modulo `b` (i.e., if `a` and `n`
 * are not coprime), `out` is set to zero.
 *
 * @param a The number for which the modular inverse is to be computed (BigUint).
 * @param n The modulus (BigUint). The inverse is computed modulo this value.
 * @param out Pointer to the BigUint where the result will be stored.
 *
 * @example
 * ```
 * BigUint a = biguint_new(3);
 * BigUint b = biguint_new(11);
 * BigUint inverse;
 * biguint_inverse_mod(a, b, &inverse);  // `inverse` is now 4, since 3 * 4 ≡ 1 mod 11
 *
 * BigUint c = biguint_new(2);
 * BigUint d = biguint_new(4);
 * biguint_inverse_mod(c, d, &inverse);  // `inverse` is now 0, since 2 and 4 are not coprime
 * ```
 *
 * https://en.wikipedia.org/wiki/Extended_Euclidean_algorithm#
 */
void biguint_inverse_mod(BigUint a, BigUint b, BigUint *out);

#endif
