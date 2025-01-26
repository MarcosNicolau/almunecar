#include <digital-signature/rsa.h>
#include <utils/test.h>

void test_key_generation() {
    RSAKeyPair key_pair = rsa_key_pair_new(512);
    rsa_gen_key_pair(&key_pair);

    biguint_println(key_pair.pub.n);
    biguint_println(key_pair.pub.e);
    biguint_println(key_pair.priv.d);
}

int main() {
    BEGIN_TEST()
    test(test_key_generation);
    END_TEST()

    return 0;
}
