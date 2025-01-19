#include <rsa.h>

/**
 * Operations are all defined over uints of 4096, even for smaller keysizes
 * The size restriction is done by checking the uint bit size + mod operations
 */
DEFINE_UINT(u4096, 64)
DEFINE_UINT(u2048, 32)

#define e 65537

RSAKeyPair rsa_gen_key_pair(unsigned int bit_size) {
    // This is a naive implementation that isn't really safe, see:
    // -
    // https://nvlpubs.nist.gov/nistpubs/SpecialPublications/NIST.SP.800-90Ar1.pdf
    // -
    // https://web.archive.org/web/20230127011251/http://people.csail.mit.edu/rivest/Rsapaper.pdf
    // (page 9)
    u4096 p = u4096_from_limbs(u2048_random_prime().parts, 32);
    u4096 q = u4096_from_limbs(u2048_random_prime().parts, 32);
    // since bit_size(p, q) is <= 2048, there can't be an overflow
    u4096 n = u4096_overflow_mul(p, q).res;

    // calculating Carmichael's totient function via lcm(p-1, q-1)
    // https://en.wikipedia.org/wiki/Carmichael_function
    u4096 lambda_n =
        least_common_divisor(u4096_overflow_sub(p, u4096_one()).res,
                             u4096_overflow_sub(q, u4096_one()).res);

    // d = e^(-1) (mod lambda_n) (i.e `e` is the multiplicative inverse of `d`
    // in Z_{lambda_n})

    // since lambda_n and e are coprime (1) we can calculate the gcd via the
    // extended euclidean which fullfills the Bezout identity (ax + by =
    // gcd(a,b))
    // Because of (1) x (our d) is the multiplicative inverse of a mod b and y
    // is the multiplicative inverse of b mod
    // finally d = x = s_k computed from the eucledian algorithm
    u4096 d = extended_euclidean_algorithm(u4096_from_u64(e), lambda_n).sk;

    RSAPublicKey pub_key = {.n = n, u4096_from_u64(e)};
    RSAPrivateKey priv_key = {.n = n, .d = d};
    RSAKeyPair key_pair = {
        .pub = pub_key, .priv = priv_key, .bit_size = bit_size};

    return key_pair;
}

// obtains the least common divisor between a & b by the relation
// lcm(a,b) = |ab|/gcd(a,b)
u4096 least_common_divisor(u4096 a, u4096 b) {
    if (u4096_is_zero(a) || u4096_is_zero(b)) {
        return u4096_zero();
    }

    u4096 result = u4096_one();
    u4096 ab = u4096_overflow_mul(a, b).res;
    u4096 gcd = greatest_common_divisor(a, b);

    u4096 lcm = u4096_divmod(a, b).quot;

    return lcm;
}

// computes the greatest commond divisor between a and b
// via the euclidean algorithm
// https://en.wikipedia.org/wiki/Euclidean_algorithm
u4096 greatest_common_divisor(u4096 a, u4096 b) {
    u4096 zero = u4096_zero();

    while (u4096_cmp(b, zero) != 0) {
        u4096 rem = u4096_divmod(a, b).rem;
        a = b;
        b = rem;
    }
    return a;
}

typedef struct {
    u4096 rk;
    u4096 sk;
    u4096 tk;
} ExtendedEuclideanAlgorithm;

ExtendedEuclideanAlgorithm extended_euclidean_algorithm(u4096 a, u4096 b) {
    u4096 rp = a;            // r_0
    u4096 ri = b;            // r_1
    u4096 sp = u4096_one();  // s_0
    u4096 si = u4096_zero(); // s_1
    u4096 tp = u4096_zero(); // t_0
    u4096 ti = u4096_one();  // t_1

    u4096 zero = u4096_zero();

    while (u4096_cmp(ri, zero) != 0) {
        u4096_div_op div = u4096_divmod(rp, ri);
        u4096 q = div.quot;

        // r = r_{i-1} - q_i * r_i
        u4096 r = u4096_overflow_sub(rp, u4096_overflow_mul(q, ri).res).res;
        // s = s_{i-1} - q_i * s_i
        u4096 s = u4096_overflow_sub(sp, u4096_overflow_mul(q, si).res).res;
        // t = t_{i-1} - q_i * t_i
        u4096 t = u4096_overflow_sub(tp, u4096_overflow_mul(q, ti).res).res;

        // update values for next iteration
        rp = ri;
        sp = si;
        tp = ti;
        ri = r;
        si = s;
        ti = t;
    }

    return (ExtendedEuclideanAlgorithm){.rk = rp, .sk = sp, .tk = tp};
}
