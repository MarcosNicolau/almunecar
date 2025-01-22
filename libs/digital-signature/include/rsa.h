#ifndef RSA_H
#define RSA_H

#include <math/arithmetics.h>
#include <math/primes.h>
#include <primitive-types/biguint.h>

typedef struct {
    BigUint n;
    BigUint e;
} RSAPublicKey;

typedef struct {
    BigUint d;
    BigUint n;
} RSAPrivateKey;

typedef struct {
    RSAPublicKey pub;
    RSAPrivateKey priv;
    unsigned int bit_size;
} RSAKeyPair;

RSAKeyPair rsa_gen_key_pair(unsigned int bit_size);
void rsa_key_pair_destroy(RSAKeyPair *key_pair);
void rsa_encrypt_msg_PKCS1v15(uint8_t *msg, RSAPrivateKey priv, uint8_t *buffer);
void rsa_decrypt_msg_PKCS1v15(RSAPublicKey *pub, uint8_t *msg, uint8_t *buffer);
void rsa_sign_PKCS1v15(RSAPrivateKey *priv, void *msg, uint8_t *buffer);
/**
 * @returns
 *  - 1: valid signature
 *
 *  - 0: invalid signature
 */
int rsa_verify_signature_PKCS1v15(uint8_t *signature, uint8_t *msg, RSAPublicKey *pub);

#endif
