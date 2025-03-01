#include <elliptic-curves/secp256k1.h>
#include <primitive-types/biguint.h>
#include <utils/test.h>

void test_secp256k1_definition() {
    EllipticCurve secp256k1 = secp256k1();
    char *p_1 = biguint_to_dec_string(secp256k1.p);
    char *a_1 = biguint_to_dec_string(secp256k1.a);
    char *b_1 = biguint_to_dec_string(secp256k1.b);
    char *g_x_1 = biguint_to_dec_string(secp256k1.g_x);
    char *g_y_1 = biguint_to_dec_string(secp256k1.g_y);
    char *h_1 = biguint_to_dec_string(secp256k1.h);
    char *n_1 = biguint_to_dec_string(secp256k1.n);

    assert_that(strcmp(p_1, "115792089237316195423570985008687907853269984665640564039457584007908834671663") == 0);
    assert_that(strcmp(a_1, "0") == 0);
    assert_that(strcmp(b_1, "7") == 0);
    assert_that(strcmp(g_x_1, "55066263022277343669578718895168534326250603453777594175500187360389116729240") == 0);
    assert_that(strcmp(g_y_1, "32670510020758816978083085130507043184471273380659243275938904335757337482424") == 0);
    assert_that(strcmp(h_1, "1") == 0);
    assert_that(strcmp(n_1, "115792089237316195423570985008687907852837564279074904382605163141518161494337") == 0);
}

int main() {
    BEGIN_TEST()
    test(test_secp256k1_definition);
    END_TEST()

    return 0;
}
