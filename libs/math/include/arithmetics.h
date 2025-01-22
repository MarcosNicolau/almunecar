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
void biguint_greatest_common_divisor(BigUint a, BigUint b, BigUint *out);

typedef struct {
    BigUint rk;
    BigUint sk;
    BigUint tk;
} ExtendedEuclideanAlgorithm;

void biguint_extended_euclidean_algorithm(BigUint a, BigUint b, ExtendedEuclideanAlgorithm *out);

#endif
