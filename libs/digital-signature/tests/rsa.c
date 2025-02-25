#include <digital-signature/rsa.h>
#include <hashes/sha256.h>
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
    RSAEncryptResult res = rsa_encrypt_msg_PKCS1v15(msg_bytes, key_pair.pub, &cipher);
    assert_that(res.success == 1);

    UInt8Array decrypted_msg = {};
    RSADecryptResult decrypt_result = rsa_decrypt_msg_PKCS1v15(key_pair, cipher, &decrypted_msg);
    assert_that(decrypt_result.success == 1);

    assert_that(strcmp((char *)decrypted_msg.array, msg) == 0);

    free(cipher.array);
    free(decrypted_msg.array);
}

void test_encrypt_decrypt_large_msg() {
    RSAKeyPair key_pair = rsa_key_pair_new(512);
    rsa_gen_key_pair(&key_pair);

    // Creating a message too large for RSA (512-bit key, PKCS1 padding leaves ~53 bytes for data)
    char *msg = "This message is too long for a 512-bit RSA key with PKCS1 padding!";
    UInt8Array msg_bytes = {.array = (uint8_t *)msg, .size = strlen(msg)};

    UInt8Array cipher = {};
    RSAEncryptResult res = rsa_encrypt_msg_PKCS1v15(msg_bytes, key_pair.pub, &cipher);
    assert_that(res.success == 0);

    free(cipher.array);
}

void test_decrypt_with_wrong_key() {
    RSAKeyPair key_pair1 = rsa_key_pair_new(512);
    rsa_gen_key_pair(&key_pair1);

    RSAKeyPair key_pair2 = rsa_key_pair_new(512);
    rsa_gen_key_pair(&key_pair2);

    char *msg = "This message is encrypted!";
    UInt8Array msg_bytes = {.array = (uint8_t *)msg, .size = strlen(msg)};

    UInt8Array cipher = {};
    RSAEncryptResult res = rsa_encrypt_msg_PKCS1v15(msg_bytes, key_pair1.pub, &cipher);
    assert_that(res.success == 1);

    UInt8Array decrypted_msg = {};
    RSADecryptResult decrypt_result = rsa_decrypt_msg_PKCS1v15(key_pair2, cipher, &decrypted_msg);
    assert_that(decrypt_result.success == 0);

    free(cipher.array);
    free(decrypted_msg.array);
}

void test_signing_msg_is_valid() {
    RSAKeyPair key_pair = rsa_key_pair_new(512);
    rsa_gen_key_pair(&key_pair);

    char *msg = "Hello this is my secret message!";
    UInt8Array msg_bytes = {.array = (uint8_t *)msg, .size = strlen(msg)};

    UInt8Array signature = {};
    RSASignResult res = rsa_sign_PKCS1v15(msg_bytes, key_pair, RSA_HASH_SHA256, &signature);
    assert_that(res.success == 1);

    RSAVerificationResult verification = rsa_verify_signature_PKCS1v15(msg_bytes, signature, key_pair.pub);
    assert_that(verification.success == 1);
}

void test_signature_tampering() {
    RSAKeyPair key_pair = rsa_key_pair_new(512);
    rsa_gen_key_pair(&key_pair);

    char *msg = "Original message!";
    UInt8Array msg_bytes = {.array = (uint8_t *)msg, .size = strlen(msg)};

    UInt8Array signature = {};
    RSASignResult res = rsa_sign_PKCS1v15(msg_bytes, key_pair, RSA_HASH_SHA256, &signature);
    assert_that(res.success == 1);

    // Tamper signature
    if (signature.size > 0) {
        signature.array[0] ^= 0xFF;
    }

    RSAVerificationResult verification = rsa_verify_signature_PKCS1v15(msg_bytes, signature, key_pair.pub);
    assert_that(verification.success == 0);

    free(signature.array);
}

int main() {
    BEGIN_TEST()
    test(test_key_generation);
    test(test_encrypt_decrypt_msg);
    test(test_encrypt_decrypt_large_msg);
    test(test_decrypt_with_wrong_key);
    test(test_signing_msg_is_valid);
    test(test_signature_tampering);
    END_TEST()

    return 0;
}
