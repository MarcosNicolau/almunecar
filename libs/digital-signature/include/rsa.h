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

/**
 * List of RSA-supported and recommended hash algorithms.
 *
 * Note: This enumeration includes widely used hash functions for RSA signatures.
 * However, this library **only supports RSA_HASH_SHA256** for signing and verification.
 * Attempting to use any other hash will result in RSA_HashNotSupported.
 */
typedef enum { RSA_HASH_MD2, RSA_HASH_MD5, RSA_HASH_SHA1, RSA_HASH_SHA256, RSA_HASH_SHA384, RSA_HASH_SHA512 } RSAHashes;

typedef enum {
    RSA_MessageTooLong,
    RSA_MessageTooShort,
    RSA_InvalidEncodedMessage,
    RSA_InvalidSignature,
    RSA_HashNotSupported
} RSAError;

DEFINE_RESULT(struct {}, RSAError, RSAEncryptResult)
DEFINE_RESULT(struct {}, RSAError, RSADecryptResult)
DEFINE_RESULT(struct {}, RSAError, RSASignResult)
DEFINE_RESULT(struct {}, RSAError, RSAVerificationResult)

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
 * @return         RSAEncryptResult containing the encrypted message or an error.
 */
RSAEncryptResult rsa_encrypt_msg_PKCS1v15(UInt8Array msg, RSAPublicKey pub, UInt8Array *cipher);

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
 * @return         RSADecryptResult containing the decrypted message or an error.
 */
RSADecryptResult rsa_decrypt_msg_PKCS1v15(RSAKeyPair key_pair, UInt8Array cipher, UInt8Array *msg);

/**
 * Signs a message using the RSA PKCS1 v1.5 padding scheme.
 *
 * If `signature->array` is NULL or `signature->size` is too small, the buffer will be allocated or reallocated as
 * needed. The caller is responsible for freeing the allocated memory.
 *
 * WARNING: Only RSA_HASH_SHA256 is supported. Using any other hash algorithm will result in RSA_HasNotSupported.
 *
 * @param msg       The message to sign.
 * @param key_pair  The RSA key pair containing the private key.
 * @param hash      The hash algorithm to use for signing. **Only RSA_HASH_SHA256 is supported.**
 * @param signature A pointer to a UInt8Array where the signature will be stored.
 *                  If NULL, a new buffer will be allocated.
 * @return          RSASignResult containing the signature or an error.
 */
RSASignResult rsa_sign_PKCS1v15(UInt8Array msg, RSAKeyPair key_pair, RSAHashes hash, UInt8Array *signature);

/**
 * Verifies an RSA signature using the PKCS1 v1.5 padding scheme.
 *
 * This function checks whether the given signature is valid for the provided message and public key.
 *
 * WARNING: Only RSA_HASH_SHA256 is supported. Using any other hash algorithm will result in RSA_HasNotSupported.
 *
 * @param msg       The original message.
 * @param signature The signature to verify.
 * @param pub       The RSA public key.
 * @return          RSAVerificationResult indicating whether the signature is valid or an error.
 */
RSAVerificationResult rsa_verify_signature_PKCS1v15(UInt8Array msg, UInt8Array signature, RSAPublicKey pub);

#endif
