#ifndef RANDOM_H
#define RANDOM_H

#include <primitive-types/biguint.h>
#include <stdint.h>
#include <stdio.h>

/**
 * Generates a random number by reading bytes from /dev/urandom
 * See https://en.wikipedia.org/wiki//dev/random
 */
uint64_t u64_random();
void biguint_random(BigUint *a);

#endif
