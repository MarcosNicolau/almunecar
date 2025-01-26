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

void test_biguint_inverse_mod_inner(int size, char *a, char *n, char *expected) {
    BigUint x = biguint_new_heap(size);
    BigUint y = biguint_new_heap(size);
    biguint_from_dec_string(a, &x);
    biguint_from_dec_string(n, &y);
    BigUint result = biguint_new_heap(size);
    biguint_inverse_mod(x, y, &result);

    BigUint expected_inverse = biguint_new_heap(size);
    biguint_from_dec_string(expected, &expected_inverse);

    assert_that(biguint_cmp(result, expected_inverse) == 0);
    biguint_free(&x, &y, &result, &expected_inverse);
}

void test_biguint_inverse_mod() {
    test_biguint_inverse_mod_inner(4, "13", "28", "13");
    // if numbers are not comprime, then we expect the output to be zero
    test_biguint_inverse_mod_inner(4, "14", "28", "0");

    test_biguint_inverse_mod_inner(4, "1234567890123456789012345678901234567890123456789012345678901234",
                                   "115792089237316195423570985008687907853269984665640564039457584007913129639747",
                                   "84596425267770677564935741578703644467687671628434139713891761663354215487081");

    test_biguint_inverse_mod_inner(4, "57896044618658097711785492504343953926634992332820282019728792003956564819967",
                                   "115792089237316195423570985008687907853269984665640564039457584007913129639747",
                                   "85450846602939224430228855247053108469258063550044908221631799963058887113824");

    test_biguint_inverse_mod_inner(4, "57896044618658097711785492504343953926634992332820282019728792003956564819968",
                                   "115792089237316195423570985008687907853269984665640564039457584007913129639747",
                                   "72293473703721222539584001222355413368708244394421092892359761444093911626932");

    test_biguint_inverse_mod_inner(
        4, "1", "115792089237316195423570985008687907853269984665640564039457584007913129639747", "1");

    test_biguint_inverse_mod_inner(4, "115792089237316195423570985008687907853269984665640564039457584007913129639746",
                                   "115792089237316195423570985008687907853269984665640564039457584007913129639747",
                                   "115792089237316195423570985008687907853269984665640564039457584007913129639746");

    test_biguint_inverse_mod_inner(4, "115792089237316195423570985008687907853269984665640564039457584007913129639747",
                                   "115792089237316195423570985008687907853269984665640564039457584007913129639747",
                                   "0");
}

int main() {
    BEGIN_TEST()
    test(test_biguint_gcd);
    test(test_biguint_lcm);
    test(test_biguint_extended_euclidean_algorithm);
    test(test_biguint_inverse_mod);
    END_TEST()

    return 0;
}
