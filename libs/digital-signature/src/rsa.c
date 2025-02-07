#include <rsa.h>

#define e_const 65537

/**
 * Rsa builds upon the fact that it is easy to find three larger integers e,d,n such that for any another integer
 * m 0 <= m < n:
 *                                                  (m^e)^d = m (mod n)
 *
 * However, when given e and n, it is not feasible to derive d.
 *
 * e and n form the Public key and d defines the Private key.
 * m is defined as the message.
 *
 * From this principle, rsa protocol defines the following operations:
 * 1. Key generation
 * 2. Encryption/Decryption of messages
 * 3. Signing/Verifying messages recipients
 */

// Generating a key pair consists of:
// 1. generating two random prime number p,q
// 2. computing n as n = p*q
// 3. computing Carmichael's totient of n (lambda_n)
// 4. picking an e such that 1 < e < lambda_n and gcd(e, lambda_n) = 1
// 5. finding d as the multiplicative inverse of e
// 6. releasing the public key as n,e and the private key as n,d
void rsa_gen_key_pair(RSAKeyPair *key_pair) {
    int key_limbs_size = key_pair->bit_size / 64;

    // prime numbers have to be half the size of the desired key to prevent multiplication overflows
    BigUint p = biguint_new_heap(key_limbs_size / 2);
    BigUint q = biguint_new_heap(key_limbs_size / 2);
    biguint_random_prime(&p);
    biguint_random_prime(&q);

    BigUint n = biguint_new_heap(key_limbs_size);
    biguint_mul(p, q, &n);

    // Carmichael's totient (lambda) of n outputs the smallest integer m, such that for every integer coprime to n, it
    // holds that:
    //                                          (a^m = 1 (mod n))
    // because n = p*q => lambda(n) = lcm(lambda(p) * lambda(q)), since p,q are prime then lambda(p) = p - 1 and
    // lambda(q) = q - 1 hance lambda(n) = lcm(p - 1, q - 1)
    //
    // https://en.wikipedia.org/wiki/Carmichael_function
    BigUint one = biguint_new_heap(key_limbs_size);
    biguint_one(&one);
    biguint_sub(p, one, &p);
    biguint_sub(q, one, &q);

    BigUint lambda_n = biguint_new_heap(key_limbs_size);
    biguint_lcm(p, q, &lambda_n);

    // Now we need to compute the private exponent d as
    // d = e^(-1) (mod lambda_n)
    // (i.e e is the multiplicative inverse of `d` in Z_{lambda_n} so d*e = 1 (mod lambda_n))
    //
    // Bezout identity states that given two integers a,b that are coprime then there exist two integers x, y such that:
    //                             ax + by = gcd(a,b) or ax = gcd(a, b) (mod b)
    //
    // Because lambda_n and e are comprime => gcd(e, lambda_n) = 1, so ax = 1 (mod n)
    // so if we can compute x we get d.
    //
    // Using the extended euclidean algorithm we can compute both x,y obtaining d
    BigUint e = biguint_new_heap(key_limbs_size);
    BigUint d = biguint_new_heap(key_limbs_size);
    biguint_from_u64(e_const, &e);
    biguint_inverse_mod(e, lambda_n, &d);

    biguint_cpy(&key_pair->pub.n, n);
    biguint_cpy(&key_pair->pub.e, e);
    biguint_cpy(&key_pair->priv.d, d);

    biguint_free(&p, &q, &n, &one, &lambda_n, &e, &d);
}
