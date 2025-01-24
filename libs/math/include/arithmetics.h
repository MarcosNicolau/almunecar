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
    BigUint tk;
} ExtendedEuclideanAlgorithm;

#define extended_euclidean_algorithm_new_heap(SIZE)                                                                    \
    (ExtendedEuclideanAlgorithm) {                                                                                     \
        .rk = biguint_new_heap(SIZE), .sk = biguint_new_heap(SIZE), .tk = biguint_new_heap(SIZE)                       \
    }

#define extended_euclidean_algorithm_new(SIZE)                                                                         \
    (ExtendedEuclideanAlgorithm) { .rk = biguint_new(SIZE), .sk = biguint_new(SIZE), .tk = biguint_new(SIZE) }

#define extended_euclidean_algorithm_free(str) biguint_free(&str.rk, &str.sk, &str.tk)

void biguint_extended_euclidean_algorithm(BigUint a, BigUint b, ExtendedEuclideanAlgorithm *out);

#endif
