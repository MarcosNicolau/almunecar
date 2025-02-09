#ifndef RSA_H
#define RSA_H

#include <math/arithmetics.h>
#include <math/primes.h>
#include <math/random.h>
#include <primitive-types/biguint.h>
#include <utils/types.h>

typedef struct {
    BigUint n;
    BigUint e;
} RSAPublicKey;

typedef struct {
    BigUint d;
} RSAPrivateKey;

typedef struct {
    RSAPublicKey pub;
    RSAPrivateKey priv;
    unsigned int bit_size;
} RSAKeyPair;

typedef enum {
    RSA_MessageTooLong,
    RSA_MessageTooShort,
    RSA_InvalidEncodedMessage,
} RSAError;

DEFINE_RESULT(uint8_t *, RSAError, EncryptResult)
DEFINE_RESULT(uint8_t *, RSAError, DecryptResult)

#define rsa_key_pair_new(BIT_SIZE)                                                                                     \
    (RSAKeyPair) {                                                                                                     \
        .bit_size = BIT_SIZE, .pub = {.e = biguint_new(BIT_SIZE / 64), .n = biguint_new(BIT_SIZE / 64)}, .priv = {     \
            .d = biguint_new(BIT_SIZE / 64)                                                                            \
        }                                                                                                              \
    }

void rsa_gen_key_pair(RSAKeyPair *key_pair);
EncryptResult rsa_encrypt_msg_PKCS1v15(uint8_t *msg, int msg_size, RSAPublicKey pub);
DecryptResult rsa_decrypt_msg_PKCS1v15(RSAKeyPair key_pair, uint8_t *cipher, int cipher_size);
void rsa_sign_PKCS1v15(RSAPrivateKey *priv, void *msg, uint8_t *buffer);
/**
 * @returns
 *  - 1: valid signature
 *
 *  - 0: invalid signature
 */
int rsa_verify_signature_PKCS1v15(uint8_t *signature, uint8_t *msg, RSAPublicKey *pub);

#endif
