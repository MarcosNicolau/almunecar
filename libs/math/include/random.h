#ifndef RANDOM_H
#define RANDOM_H

#include <primitive-types/biguint.h>
#include <stdint.h>
#include <stdio.h>

/**
 * Generates a random number by reading bytes from /dev/urandom
 * See https://en.wikipedia.org/wiki//dev/random
 */
uint8_t u8_random();
uint64_t u64_random();
void biguint_random(BigUint *a);
void biguint_random_with_max_bits(BigUint *a, int max_bits);

#endif
