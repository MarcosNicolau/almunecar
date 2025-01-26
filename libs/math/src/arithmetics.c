#include <arithmetics.h>

void biguint_gcd(BigUint a, BigUint b, BigUint *out) {
    BigUint rem = biguint_new_heap(a.size);
    BigUint x = biguint_new_heap(a.size);
    BigUint y = biguint_new_heap(b.size);
    biguint_cpy(&x, a);
    biguint_cpy(&y, b);

    while (!biguint_is_zero(y)) {
        biguint_mod(x, y, &rem);
        biguint_cpy(&x, y);
        biguint_cpy(&y, rem);
    }

    biguint_cpy(out, x);
    biguint_free(&rem, &x, &y);
}

void biguint_lcm(BigUint a, BigUint b, BigUint *out) {
    if (biguint_is_zero(a) || biguint_is_zero(b)) {
        biguint_zero(out);
        return;
    }
    BigUint x = biguint_new_heap(a.size);
    BigUint y = biguint_new_heap(b.size);
    biguint_cpy(&x, a);
    biguint_cpy(&y, b);

    BigUint gcd = biguint_new_heap(out->size);
    biguint_gcd(x, y, &gcd);
    biguint_mul(&x, y);
    biguint_div(x, gcd, out);

    biguint_free(&gcd, &x, &y);
}

void biguint_extended_euclidean_algorithm(BigUint a, BigUint b, ExtendedEuclideanAlgorithm *out) {
    BigUint rp = biguint_new_heap(a.size); // r_0
    biguint_cpy(&rp, a);
    BigUint ri = biguint_new_heap(a.size); // r_1
    biguint_cpy(&ri, b);
    BigUint sp = biguint_new_heap(a.size); // s_0
    biguint_one(&sp);
    BigUint si = biguint_new_heap(a.size); // s_1
    biguint_zero(&si);
    BigUint tp = biguint_new_heap(a.size); // t_0
    biguint_zero(&tp);
    BigUint ti = biguint_new_heap(a.size); // t_1
    biguint_one(&ti);

    BigUint zero = biguint_new_heap(a.size);
    biguint_zero(&zero);

    BigUint quot = biguint_new_heap(a.size);
    BigUint r = biguint_new_heap(a.size);
    BigUint qr = biguint_new_heap(a.size);
    BigUint s = biguint_new_heap(a.size);
    BigUint qs = biguint_new_heap(a.size);
    BigUint t = biguint_new_heap(a.size);
    BigUint qt = biguint_new_heap(a.size);

    int overflow;
    out->sk_sign = 1;
    out->tk_sign = 1;

    while (!biguint_is_zero(ri)) {
        biguint_div(rp, ri, &quot);

        // r = r_{i-1} - q_i * r_i
        biguint_cpy(&qr, ri);
        biguint_mul(&qr, quot);
        biguint_cpy(&r, rp);
        biguint_overflow_sub(&r, qr);

        // s = s_{i-1} - q_i * s_i
        biguint_cpy(&qs, si);
        biguint_mul(&qs, quot);
        biguint_cpy(&s, sp);
        overflow = biguint_overflow_sub(&s, qs);
        out->sk_sign *= -1;

        // t = t_{i-1} - q_i * t_i
        biguint_cpy(&qt, ti);
        biguint_mul(&qt, quot);
        biguint_cpy(&t, tp);
        overflow = biguint_overflow_sub(&t, qt);
        out->tk_sign *= -1;

        // update values for next iteration
        biguint_cpy(&rp, ri);
        biguint_cpy(&sp, si);
        biguint_cpy(&tp, ti);
        biguint_cpy(&ri, r);
        biguint_cpy(&si, s);
        biguint_cpy(&ti, t);
    }

    biguint_cpy(&out->rk, rp);
    biguint_cpy(&out->sk, sp);
    biguint_cpy(&out->tk, tp);
    biguint_free(&rp, &ri, &sp, &si, &tp, &ti, &quot, &r, &qr, &s, &qs, &t, &qt);
}

void biguint_inverse_mod(BigUint a, BigUint n, BigUint *out) {
    ExtendedEuclideanAlgorithm alg = extended_euclidean_algorithm_new_heap(out->size);
    biguint_extended_euclidean_algorithm(a, n, &alg);

    BigUint one = biguint_new_heap(out->size);
    biguint_one(&one);

    if (biguint_cmp(alg.rk, one) > 0) {
        biguint_zero(out);
    } else {
        printf("SIGN %d\n", alg.sk_sign);
        if (alg.sk_sign == -1) {
            biguint_add(&alg.sk, n);
        }
        biguint_cpy(out, alg.sk);
    }

    biguint_free(&one);
    extended_euclidean_algorithm_free(alg);
}
