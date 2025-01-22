#include <math/random.h>
#include <primes.h>

void biguint_random_prime(BigUint *a) {
    while (!biguint_is_primen(*a)) {
        biguint_random(a);
    }
};

// Verifies if a number is prime by dividing it by the first 1000 primes
// If it passes the initial test, then we run a more strong and probable primality test
int big_uint_is_prime(BigUint a) {
    BigUint p = biguint_new_heap(a.size);
    BigUint rem = biguint_new_heap(a.size);

    for (int i = 0; i < PRIMES_LENGTH; i++) {
        // if a < p and we are at this point, a is 100% prime
        if (biguint_cmp(a, p) < 0) {
            biguint_free(&p, &rem) return 1;
        }

        biguint_from_u64(PRIMES[i], &p);
        biguint_mod(a, p, &rem);

        // found a factor
        if (biguint_is_zero(rem)) {
            biguint_free(&p, &rem) return 0;
        }
    }

    biguint_free(&p, &rem);
    return biguint_is_prime_solovay_strassen(a);
}

// Verifies if a number is prime using via Solovay–Strassen primality test
// this is a probabilistic method that uses the following relation:
//                  (a^((p-1)/2) mod p = jacobi(a, p).
// where p is an odd prime, a belongs to {1...p-1} and is not divisible by p.
//
// If p is prime, then the relation always holds
// If p is composite, there is a 1/2 chance for the relation to be true.
// so if we compute 100 numbers between {1...,n-1} the probability of it being a comprime
// is reduced to the point were it is negligible (1 in (1/2)^100).
//
// @reference
//    - https://en.wikipedia.org/wiki/Solovay%E2%80%93Strassen_primality_test
//    - https://en.wikipedia.org/wiki/Legendre_symbol
//    - https://web.archive.org/web/20230127011251/http://people.csail.mit.edu/rivest/Rsapaper.pdf (page 9)
int biguint_is_prime_solovay_strassen(BigUint p) {
    BigUint a = biguint_new_heap(p.size);
    BigUint rem = biguint_new_heap(p.size);
    BigUint exponent = biguint_new_heap(p.size);
    BigUint num = biguint_new_heap(p.size);
    biguint_cpy(&exponent, p);
    biguint_one(&num);
    biguint_sub(&exponent, num);
    biguint_from_u64(2, &num);
    biguint_div(exponent, num, &exponent);

    for (int i = 0; i < 100; i++) {
        biguint_random(&a);
        biguint_mod(a, p, &rem);
        // check that a is not divisible by p and a < p
        if (biguint_is_zero(rem) && biguint_cmp(a, p) < 0) {
            // don't count the iteration
            i--;
            continue;
        }

        int j = jacobi(a, p);
        biguint_pow(&a, exponent);
        biguint_mod(a, p, &rem);

        if (rem.limbs[0] != j)
            return 0;
    }

    biguint_free(&a, &rem, &exponent, &num);

    return 1;
};

/**
 * Jacobi symbol is a generalization over the legendre symbol function.
 * It is defined as the product of the legendre symbol of a over each prime factor
 *    jacobi(a, n) = legendre(a, p_1)^p_1_k * legendre(a, p_2)^p_2_k * ... * legendre(a, p_n2)^p_n_k
 * Where p_1^p_1_k * p_2^p_2_k * ... * p_n^p_n_k = a
 *
 * Note that if n is an odd prime number, then jacobi(a, n) = legendre(a, n)
 *
 * It states that given an integer a and an odd integer n, yields:
 * - 0: if a = 0 (mod n)
 * - 1: if a != 0 (mod n) and a is the solution to the quadratic residue of some x mod p
 * - - 1: if a != 0 (mod n) and there isn't a quadratic residue of some x mod p such that a is the solution
 *
 *
 * To calculelate it we are recursively doing:
 * jacobi(a, n) = {
 *      if a = 1 => 1
 *      if a is even => jacobi(a/2, n) * (-1)^(n^2 - 1) / 8
 *      else => jacobi(n mod a, a) * (-1)^((a - 1) * (n - 1)) / 4
 * }
 *
 * https://en.wikipedia.org/wiki/Jacobi_symbol
 * https://en.wikipedia.org/wiki/Legendre_symbol
 * https://en.wikipedia.org/wiki/Quadratic_residue
 */
int jacobi(BigUint a, BigUint n) {
    BigUint rem = biguint_new_heap(a.size);
    // a mod (n)
    biguint_mod(a, n, &rem);
    if (biguint_is_zero(rem)) {
        biguint_free(&rem);
        return 0;
    }

    BigUint num = biguint_new_heap(a.size);
    biguint_one(&num);

    if (biguint_cmp(a, num) == 0) {
        biguint_free(&num);
        return 1;
    }

    BigUint exponent = biguint_new_heap(a.size);
    BigUint exponent_two = biguint_new_heap(a.size);
    BigUint exp_result = biguint_new_heap(a.size);
    BigUint next = biguint_new_heap(a.size);

    int result;

    if (biguint_is_even(a)) {
        biguint_cpy(&exponent, n);
        biguint_pow(&exponent, 2);
        biguint_from_u64(1, &num);
        biguint_sub(&exponent, num);
        biguint_from_u64(8, &num);
        biguint_div(exponent, num, &exp_result);

        // (-1)^n = { - 1 if n is even, - -1 if n is odd }
        int calc;
        if (biguint_is_even(exp_result))
            calc = 1;
        else
            calc = -1;

        biguint_from_u64(2, &num);
        biguint_div(a, num, &next);

        result = jacobi(next, n);

    } else {
        biguint_cpy(&exponent, a);
        biguint_sub(&exponent, num);

        biguint_cpy(&exponent_two, n);
        biguint_sub(&exponent_two, num);

        biguint_mul(&exponent, exponent_two);
        biguint_from_u64(4, &num);
        biguint_div(exponent, num, &exp_result);

        // (-1)^n = { - 1 if n is even, - -1 if n is odd }
        int calc;
        if (biguint_is_even(exp_result))
            calc = 1;
        else
            calc = -1;

        biguint_mod(n, a, &next);

        result = jacobi(next, a);
    }

    biguint_free(&exponent, &exponent_two, &exp_result, &next, &num);

    return result;
}
