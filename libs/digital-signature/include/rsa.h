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

DEFINE_RESULT(struct {}, RSAError, EncryptResult)
DEFINE_RESULT(struct {}, RSAError, DecryptResult)

#define rsa_key_pair_new(BIT_SIZE)                                                                                     \
    (RSAKeyPair) {                                                                                                     \
        .bit_size = BIT_SIZE, .pub = {.e = biguint_new(BIT_SIZE / 64), .n = biguint_new(BIT_SIZE / 64)}, .priv = {     \
            .d = biguint_new(BIT_SIZE / 64)                                                                            \
        }                                                                                                              \
    }

/**
 * Generates an RSA key pair with the given parameters.
 *
 * The key pair must be initialized before calling this function.
 * This function populates the `key_pair` with a newly generated RSA public and private key.
 *
 * Example usage:
 *
 * RSAKeyPair key_pair = rsa_key_pair_new(size_in_bits);
 *
 * rsa_gen_key_pair(&key_pair);
 *
 * @param key_pair A pointer to an RSAKeyPair structure to store the generated keys.
 */
void rsa_gen_key_pair(RSAKeyPair *key_pair);

/**
 * Encrypts a message using RSA PKCS1 v1.5 padding scheme.
 *
 * If `cipher->array` is NULL or `cipher->size` is too small, the buffer will be allocated or reallocated as needed.
 * The caller is responsible for freeing the allocated memory.
 *
 * @param msg      The message to encrypt.
 * @param pub      The RSA public key.
 * @param cipher   A pointer to a UInt8Array where the encrypted data will be stored.
 *                 If NULL, a new buffer will be allocated.
 * @return         EncryptResult containing the encrypted message or an error.
 */
EncryptResult rsa_encrypt_msg_PKCS1v15(UInt8Array msg, RSAPublicKey pub, UInt8Array *cipher);

/**
 * Decrypts an RSA encrypted with PKCS1 v1.5 padding scheme.
 *
 * If `msg->array` is NULL or `msg->size` is too small, the buffer will be allocated or reallocated as needed.
 * The caller is responsible for freeing the allocated memory.
 *
 * @param key_pair The RSA key pair for decryption.
 * @param cipher   The encrypted message.
 * @param msg      A pointer to a UInt8Array where the decrypted message will be stored.
 *                 If NULL, a new buffer will be allocated.
 * @return         DecryptResult containing the decrypted message or an error.
 */
DecryptResult rsa_decrypt_msg_PKCS1v15(RSAKeyPair key_pair, UInt8Array cipher, UInt8Array *msg);

void rsa_sign_PKCS1v15(RSAPrivateKey *priv, void *msg, uint8_t *buffer);
/**
 * @returns
 *  - 1: valid signature
 *
 *  - 0: invalid signature
 */
int rsa_verify_signature_PKCS1v15(uint8_t *signature, uint8_t *msg, RSAPublicKey *pub);

#endif
