#include <digital-signature/rsa.h>
#include <math/random.h>
#include <utils/test.h>

void test_key_generation() {
#define limbs_size 8
#define size_in_bits 8 * 64
    RSAKeyPair key_pair = rsa_key_pair_new(size_in_bits);
    rsa_gen_key_pair(&key_pair);

    // verify rsa premise: (m^e)^d = m (mod n)
    BigUint msg = biguint_new(limbs_size);
    biguint_random(&msg);

    BigUint left_side = biguint_new(limbs_size);
    biguint_pow_mod(msg, key_pair.pub.e, key_pair.pub.n, &left_side);
    biguint_pow_mod(left_side, key_pair.priv.d, key_pair.pub.n, &left_side);

    BigUint right_side = biguint_new(limbs_size);
    biguint_mod(msg, key_pair.pub.n, &right_side);

    assert_that(biguint_cmp(left_side, right_side) == 0);
}

void test_encrypt_decrypt_msg() {
    RSAKeyPair key_pair = rsa_key_pair_new(512);
    rsa_gen_key_pair(&key_pair);

    char *msg = "Hello this is my secret message!";
    UInt8Array msg_bytes = {.array = (uint8_t *)msg, .size = strlen(msg)};

    UInt8Array cipher = {};
    EncryptResult res = rsa_encrypt_msg_PKCS1v15(msg_bytes, key_pair.pub, &cipher);
    assert_that(res.success == 1);

    UInt8Array decrypted_msg = {};
    DecryptResult decrypt_result = rsa_decrypt_msg_PKCS1v15(key_pair, cipher, &decrypted_msg);
    assert_that(decrypt_result.success == 1);

    assert_that(strcmp((char *)decrypted_msg.array, msg) == 0);

    free(cipher.array);
    free(decrypted_msg.array);
}

int main() {
    BEGIN_TEST()
    test(test_key_generation);
    test(test_encrypt_decrypt_msg);
    END_TEST()

    return 0;
}
