#include <rsa.h>

#define e_const 65537

RSAKeyPair rsa_gen_key_pair(unsigned int bit_size) {
    int key_size = bit_size / 64;

    BigUint p = biguint_new_heap(key_size / 2);
    BigUint q = biguint_new_heap(key_size / 2);
    BigUint n = biguint_new_heap(key_size);
    biguint_random_prime(&p);
    biguint_random_prime(&q);
    biguint_cpy(&n, p);
    biguint_mul(&n, q);

    // calculating Carmichael's totient function via lcm(p-1, q-1)
    // https://en.wikipedia.org/wiki/Carmichael_function
    BigUint one = biguint_new_heap(key_size);
    biguint_one(&one);
    biguint_sub(&p, one);
    biguint_sub(&q, one);
    BigUint lambda_n;
    biguint_lcm(p, q, &lambda_n);

    // d = e^(-1) (mod lambda_n) (i.e `e` is the multiplicative inverse of `d` in Z_{lambda_n})

    // since lambda_n and e are coprime (1) we can calculate the gcd via the
    // extended euclidean which fullfills the Bezout identity (ax + by = gcd(a,b))
    // Because of (1) x (our d) is the multiplicative inverse of a mod b
    // and y is the multiplicative inverse of b mod
    // finally d = x = s_k computed from the euclidean algorithm
    BigUint e = biguint_new_heap(key_size);
    BigUint d = biguint_new_heap(key_size);
    biguint_from_u64(e_const, &e);
    ExtendedEuclideanAlgorithm alg = {
        .rk = biguint_new_heap(key_size), .sk = biguint_new_heap(key_size), .tk = biguint_new_heap(key_size)};
    biguint_extended_euclidean_algorithm(e, lambda_n, &alg);
    biguint_cpy(&d, alg.sk);

    RSAPublicKey pub_key = {.n = n, .e = e};
    RSAPrivateKey priv_key = {.n = n, .d = d};
    RSAKeyPair key_pair = {.pub = pub_key, .priv = priv_key, .bit_size = bit_size};

    biguint_free(&p, &q, &one, &alg.rk, &alg.sk, &alg.tk);

    return key_pair;
}

void rsa_key_pair_destroy(RSAKeyPair *key_pair) {
    biguint_free(&key_pair->priv.d, &key_pair->priv.n, &key_pair->pub.e, &key_pair->pub.n);
}
