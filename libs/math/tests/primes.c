#include <math/primes.h>
#include <utils/test.h>

void test_is_prime_inner(int size, char *prime, int is_prime) {
    BigUint p = biguint_new_heap(size);
    biguint_from_dec_string(prime, &p);

    assert_that(biguint_is_prime(p) == is_prime);

    biguint_free(&p);
}

void test_is_prime() {
    test_is_prime_inner(4, "13", 1);
    test_is_prime_inner(4, "2147483647", 1);
    test_is_prime_inner(4, "98764321261", 1);
    test_is_prime_inner(4, "86979627671220575743356597306088825369450358524981474414865226602524911075691", 1);
    test_is_prime_inner(4, "86979627671220575743356597306088825369450358524981474414865226602524911076043", 1);

    // 62837329 = 7927^2 and 7927 is the 1001th prime number
    // so it should be identified as a non-prime by the solovay_strassen test
    test_is_prime_inner(4, "62837329", 0);
    test_is_prime_inner(4, "115792089237316195423570985008687907853269984665640564039457584007913129639746", 0);
}

void test_random_prime_works() {
    BigUint a = biguint_new_with_limbs(4, {0});
    biguint_random_prime(&a);

    assert_that(biguint_is_prime(a) == 1);
}

int main() {
    BEGIN_TEST()
    test(test_random_prime_works);
    test(test_is_prime);
    END_TEST()

    return 0;
}
