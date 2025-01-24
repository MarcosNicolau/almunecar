#include <math/arithmetics.h>
#include <utils/test.h>

void test_biguint_gcd_inner(int size, char *a, char *b, char *expected) {
    BigUint x = biguint_new_heap(size);
    BigUint y = biguint_new_heap(size);
    BigUint z = biguint_new_heap(size);
    BigUint result = biguint_new_heap(size);
    biguint_from_dec_string(a, &x);
    biguint_from_dec_string(b, &y);
    biguint_from_dec_string(expected, &z);

    biguint_gcd(x, y, &result);

    assert_that(biguint_cmp(result, z) == 0);
    biguint_free(&result, &x, &y, &z);
}

void test_biguint_gcd() {
    test_biguint_gcd_inner(4, "17", "19", "1");
    test_biguint_gcd_inner(4, "987654321", "987654321", "987654321");
    test_biguint_gcd_inner(4, "12345678901234567890", "1", "1");
    test_biguint_gcd_inner(4, "462", "1071", "21");
    test_biguint_gcd_inner(4, "10000000000000000000", "5000000000000000000", "5000000000000000000");
    test_biguint_gcd_inner(4, "999999999989", "888888888887", "1");
    test_biguint_gcd_inner(4, "987654321012345678901234567890", "12345678901234567890", "90");
}

void test_biguint_lcm_inner(int size, char *a, char *b, char *expected) {
    BigUint x = biguint_new_heap(size);
    BigUint y = biguint_new_heap(size);
    BigUint z = biguint_new_heap(size);
    BigUint result = biguint_new_heap(size);
    biguint_from_dec_string(a, &x);
    biguint_from_dec_string(b, &y);
    biguint_from_dec_string(expected, &z);

    biguint_lcm(x, y, &result);

    assert_that(biguint_cmp(result, z) == 0);
    biguint_free(&result, &x, &y, &z);
}

void test_biguint_lcm() {
    test_biguint_lcm_inner(4, "17", "19", "323");
    test_biguint_lcm_inner(4, "100", "50", "100");
    test_biguint_lcm_inner(4, "462", "1071", "23562");
    test_biguint_lcm_inner(4, "12345678901234567890", "1", "12345678901234567890");
    test_biguint_lcm_inner(4, "987654321012345678901234567890", "12345678901234567890",
                           "135480701251502988873986011750021168887500211690");
}

void test_biguint_extended_euclidean_algorithm_inner(int size, char *a, char *b, char *rk, char *sk, char *tk) {
    BigUint x = biguint_new_heap(size);
    BigUint y = biguint_new_heap(size);
    biguint_from_dec_string(a, &x);
    biguint_from_dec_string(b, &y);
    ExtendedEuclideanAlgorithm result = extended_euclidean_algorithm_new_heap(size);
    biguint_extended_euclidean_algorithm(x, y, &result);

    BigUint expected_rk = biguint_new_heap(size);
    BigUint expected_sk = biguint_new_heap(size);
    BigUint expected_tk = biguint_new_heap(size);

    biguint_from_dec_string(rk, &expected_rk);
    biguint_from_dec_string(sk, &expected_sk);
    biguint_from_dec_string(tk, &expected_tk);

    assert_that(biguint_cmp(result.rk, expected_rk) == 0);
    assert_that(biguint_cmp(result.sk, expected_sk) == 0);
    assert_that(biguint_cmp(result.tk, expected_tk) == 0);

    biguint_free(&x, &y, &expected_rk, &expected_sk, &expected_tk);
    extended_euclidean_algorithm_free(result);
}

void test_biguint_extended_euclidean_algorithm() {
    // note: sk = -1 but since we are working with unsigend integers it overflows
    test_biguint_extended_euclidean_algorithm_inner(
        4, "161", "28", "7", "115792089237316195423570985008687907853269984665640564039457584007913129639935", "6");
}

int main() {
    BEGIN_TEST()
    test(test_biguint_gcd);
    test(test_biguint_lcm);
    test(test_biguint_extended_euclidean_algorithm);
    END_TEST()

    return 0;
}
