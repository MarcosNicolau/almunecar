#include <hashes/sha256.h>
#include <rsa.h>

#define e_const 65537

/**
 * Rsa builds upon the fact that it is easy to find three larger integers e,d,n such that for any another integer
 * m 0 <= m < n:
 *                                                  (m^e)^d = m (mod n)
 *
 * However, when given e and n, it is not feasible to derive d.
 *
 * e and n form the Public key and d defines the Private key.
 * m is defined as the message.
 *
 * From this principle, rsa protocol defines the following operations:
 * 1. Key generation
 * 2. Encryption/Decryption of messages
 * 3. Signing/Verifying messages recipients
 */

typedef struct {
    uint8_t oid[19]; // Maximum OID length from the given list
    int oid_size;
    int hash_len;
    RSAHashes function;
    int supported; // If almunecar supports the hash function
} RSAHashEntry;

const RSAHashEntry hash_list[] = {
    {{0x30, 0x20, 0x30, 0x0c, 0x06, 0x08, 0x2a, 0x86, 0x48, 0x86, 0xf7, 0x0d, 0x02, 0x02, 0x05, 0x00, 0x04, 0x10},
     18,
     16,
     RSA_HASH_MD2,
     0},
    {{0x30, 0x20, 0x30, 0x0c, 0x06, 0x08, 0x2a, 0x86, 0x48, 0x86, 0xf7, 0x0d, 0x02, 0x05, 0x05, 0x00, 0x04, 0x10},
     18,
     16,
     RSA_HASH_MD5,
     0},
    {{0x30, 0x21, 0x30, 0x09, 0x06, 0x05, 0x2b, 0x0e, 0x03, 0x02, 0x1a, 0x05, 0x00, 0x04, 0x14},
     15,
     32,
     RSA_HASH_SHA1,
     0},
    {{0x30, 0x31, 0x30, 0x0d, 0x06, 0x09, 0x60, 0x86, 0x48, 0x01, 0x65, 0x03, 0x04, 0x02, 0x01, 0x05, 0x00, 0x04, 0x20},
     19,
     32,
     RSA_HASH_SHA256,
     1},
    {{0x30, 0x41, 0x30, 0x0d, 0x06, 0x09, 0x60, 0x86, 0x48, 0x01, 0x65, 0x03, 0x04, 0x02, 0x02, 0x05, 0x00, 0x04, 0x30},
     19,
     48,
     RSA_HASH_SHA384,
     0},
    {{0x30, 0x51, 0x30, 0x0d, 0x06, 0x09, 0x60, 0x86, 0x48, 0x01, 0x65, 0x03, 0x04, 0x02, 0x03, 0x05, 0x00, 0x04, 0x40},
     19,
     64,
     RSA_HASH_SHA512,
     0}};

void hash_msg(RSAHashes hasher, UInt8Array msg, UInt8Array *hash);
int try_identify_hasher_by_oid(uint8_t *bytes, int size, RSAHashes *hasher);

// Generating a key pair consists of:
// 1. generating two random prime number p,q
// 2. computing n as n = p*q
// 3. computing Carmichael's totient of n (lambda_n)
// 4. picking an e such that 1 < e < lambda_n and gcd(e, lambda_n) = 1
// 5. finding d as the multiplicative inverse of e
// 6. releasing the public key as n,e and the private key as n,d
void rsa_gen_key_pair(RSAKeyPair *key_pair) {
    int key_limbs_size = key_pair->bit_size / 64;

    // prime numbers have to be half the size of the desired key to prevent multiplication overflows
    BigUint p = biguint_new_heap(key_limbs_size / 2);
    BigUint q = biguint_new_heap(key_limbs_size / 2);
    biguint_random_prime(&p);
    biguint_random_prime(&q);

    BigUint n = biguint_new_heap(key_limbs_size);
    biguint_mul(p, q, &n);

    // Carmichael's totient (lambda) of n outputs the smallest integer m, such that for every integer coprime to n, it
    // holds that:
    //                                          (a^m = 1 (mod n))
    // because n = p*q => lambda(n) = lcm(lambda(p) * lambda(q)), since p,q are prime then lambda(p) = p - 1 and
    // lambda(q) = q - 1 hance lambda(n) = lcm(p - 1, q - 1)
    //
    // https://en.wikipedia.org/wiki/Carmichael_function
    BigUint one = biguint_new_heap(key_limbs_size);
    biguint_one(&one);
    biguint_sub(p, one, &p);
    biguint_sub(q, one, &q);

    BigUint lambda_n = biguint_new_heap(key_limbs_size);
    biguint_lcm(p, q, &lambda_n);

    // Now we need to compute the private exponent d as
    // d = e^(-1) (mod lambda_n)
    // (i.e e is the multiplicative inverse of `d` in Z_{lambda_n} so d*e = 1 (mod lambda_n))
    //
    // Bezout identity states that given two integers a,b that are coprime then there exist two integers x, y such that:
    //                             ax + by = gcd(a,b) or ax = gcd(a, b) (mod b)
    //
    // Because lambda_n and e are comprime => gcd(e, lambda_n) = 1, so ax = 1 (mod n)
    // so if we can compute x we get d.
    //
    // Using the extended euclidean algorithm we can compute both x,y obtaining d
    BigUint e = biguint_new_heap(key_limbs_size);
    BigUint d = biguint_new_heap(key_limbs_size);
    biguint_from_u64(e_const, &e);
    biguint_inverse_mod(e, lambda_n, &d);

    biguint_cpy(&key_pair->pub.n, n);
    biguint_cpy(&key_pair->pub.e, e);
    biguint_cpy(&key_pair->priv.d, d);

    biguint_free(&p, &q, &n, &one, &lambda_n, &e, &d);
}

RSAEncryptResult rsa_encrypt_msg_PKCS1v15(UInt8Array msg, RSAPublicKey pub, UInt8Array *buf) {
    // k represents the length of n in bytes
    int k = (biguint_bits(pub.n) + 7) / 8;
    int limbs_size = k / 8;

    // message too long
    if (msg.size > k - 11) {
        return Err(RSAEncryptResult, RSA_MessageTooLong);
    }

    uint8_t *ps = malloc((k - msg.size - 3));
    for (int j = 0; j < k - msg.size - 3; j++) {
        uint8_t rand = 0;
        while (rand == 0) {
            rand = u8_random();
        }
        ps[j] = rand;
    }
    // EM = 0x00 || 0x02 || PS || 0x00 || M.
    uint8_t *em_bytes = malloc(k);
    int i = 0;
    em_bytes[i++] = 0x00;
    em_bytes[i++] = 0x02;
    for (int j = 0; j < (k - msg.size - 3); j++) {
        em_bytes[i++] = ps[j];
    }
    em_bytes[i++] = 0x00;
    for (int j = 0; j < msg.size; j++) {
        em_bytes[i++] = msg.array[j];
    }

    BigUint em = biguint_new_heap(limbs_size);
    biguint_from_bytes_big_endian(em_bytes, &em);

    BigUint cipher = biguint_new_heap(limbs_size);
    biguint_pow_mod(em, pub.e, pub.n, &cipher);

    buf->array = realloc(buf->array, k);
    buf->size = k;
    biguint_get_bytes_big_endian(cipher, buf->array);

    biguint_free(&em, &cipher);
    free(ps);
    free(em_bytes);

    return Ok(RSAEncryptResult, {});
}

RSADecryptResult rsa_decrypt_msg_PKCS1v15(RSAKeyPair key_pair, UInt8Array cipher_bytes, UInt8Array *buf) {
    // k represents the length of n in bytes
    int k = (biguint_bits(key_pair.pub.n) + 7) / 8;
    int limbs_size = k / 8;

    if (cipher_bytes.size > k) {
        return Err(RSADecryptResult, RSA_MessageTooLong);
    }

    if (cipher_bytes.size < 11) {
        return Err(RSADecryptResult, RSA_MessageTooShort);
    }

    BigUint cipher = biguint_new_heap(limbs_size);
    biguint_from_bytes_big_endian(cipher_bytes.array, &cipher);

    BigUint em = biguint_new_heap(limbs_size);
    biguint_pow_mod(cipher, key_pair.priv.d, key_pair.pub.n, &em);

    // EM = 0x00 || 0x02 || PS || 0x00 || M.
    uint8_t *em_bytes = malloc(k);
    biguint_get_bytes_big_endian(em, em_bytes);
    biguint_free(&cipher, &em);

    int i = 0;
    if (em_bytes[i++] != 0x00) {
        free(em_bytes);
        return Err(RSADecryptResult, RSA_InvalidEncodedMessage);
    }
    if (em_bytes[i++] != 0x02) {
        free(em_bytes);
        return Err(RSADecryptResult, RSA_InvalidEncodedMessage);
    }
    int ps_len = 0;
    uint8_t byte = em_bytes[i++];
    while (byte != 0 || ps_len > k - 11) {
        byte = em_bytes[i++];
        ps_len++;
    }
    if (ps_len < 8) {
        free(em_bytes);
        return Err(RSADecryptResult, RSA_InvalidEncodedMessage);
    }
    if (byte != 0x00) {
        free(em_bytes);
        return Err(RSADecryptResult, RSA_InvalidEncodedMessage);
    }

    // the rest is the message
    int msg_size = cipher_bytes.size - ps_len - 3;
    buf->array = realloc(buf->array, msg_size + 1);
    buf->size = msg_size;
    for (int j = 0; j < msg_size; j++) {
        buf->array[j] = em_bytes[i++];
    }
    buf->array[msg_size] = '\0';

    free(em_bytes);

    return Ok(RSADecryptResult, {});
};

RSASignResult rsa_sign_PKCS1v15(UInt8Array msg_bytes, RSAKeyPair key_pair, RSAHashes hasher, UInt8Array *buf) {
    // k represents the length of n in bytes
    int k = (biguint_bits(key_pair.pub.n) + 7) / 8;
    int limbs_size = k / 8;

    RSAHashEntry hash_entry = hash_list[hasher];
    if (hash_entry.supported == 0) {
        return Err(RSASignResult, RSA_HashNotSupported);
    }

    UInt8Array msg_hash = {.array = malloc(hash_entry.hash_len), .size = hash_entry.hash_len};
    hash_msg(hasher, msg_bytes, &msg_hash);

    int t_len = hash_entry.oid_size + hash_entry.hash_len;
    uint8_t *t = malloc(hash_entry.oid_size + hash_entry.hash_len);

    int m = 0;
    for (; m < hash_entry.oid_size; m++)
        t[m] = hash_entry.oid[m];
    for (int j = 0; j < hash_entry.hash_len; j++)
        t[m++] = msg_hash.array[j];

    free(msg_hash.array);

    if (k < t_len + 11) {
        free(t);
        return Err(RSASignResult, RSA_MessageTooShort);
    }

    // EM = 0x00 || 0x01 || PS || 0x00 || T.
    uint8_t *em_bytes = malloc(k);
    int i = 0;
    em_bytes[i++] = 0x00;
    em_bytes[i++] = 0x01;
    for (; i < k - t_len - 1; i++)
        em_bytes[i] = 0xff;
    em_bytes[i++] = 0x00;
    for (int j = 0; j < t_len; j++)
        em_bytes[i++] = t[j];

    BigUint em = biguint_new_heap(limbs_size);
    biguint_from_bytes_big_endian(em_bytes, &em);

    BigUint signature = biguint_new_heap(limbs_size);
    biguint_pow_mod(em, key_pair.priv.d, key_pair.pub.n, &signature);

    buf->array = realloc(buf->array, k);
    buf->size = k;
    biguint_get_bytes_big_endian(signature, buf->array);

    free(t);
    free(em_bytes);
    biguint_free(&signature, &em);

    return Ok(RSASignResult, {});
}

RSAVerificationResult rsa_verify_signature_PKCS1v15(UInt8Array msg, UInt8Array signature_bytes, RSAPublicKey pub) {
    // k represents the length of n in bytes
    int k = (biguint_bits(pub.n) + 7) / 8;
    int limbs_size = k / 8;

    if (signature_bytes.size < k) {
        return Err(RSAVerificationResult, RSA_InvalidSignature);
    }

    BigUint signature = biguint_new_heap(limbs_size);
    biguint_from_bytes_big_endian(signature_bytes.array, &signature);

    BigUint em = biguint_new_heap(limbs_size);
    biguint_pow_mod(signature, pub.e, pub.n, &em);
    biguint_free(&signature);

    // EM = 0x00 || 0x01 || PS || 0x00 || T.
    uint8_t *em_bytes = malloc(k);
    biguint_get_bytes_big_endian(em, em_bytes);
    biguint_free(&em);

    int i = 0;
    if (em_bytes[i++] != 0x00) {
        free(em_bytes);
        return Err(RSAVerificationResult, RSA_InvalidSignature);
    }
    if (em_bytes[i++] != 0x01) {
        free(em_bytes);
        return Err(RSAVerificationResult, RSA_InvalidSignature);
    }

    int ps_len = 0;
    uint8_t byte = em_bytes[i++];
    while (byte == 0xff) {
        byte = em_bytes[i++];
        ps_len++;
    }
    if (ps_len < 8) {
        free(em_bytes);
        return Err(RSAVerificationResult, RSA_InvalidSignature);
    }

    if (byte != 0x00) {
        free(em_bytes);
        return Err(RSAVerificationResult, RSA_InvalidSignature);
    }

    RSAHashes hasher;

    int identified = 0;
    uint8_t hash_oid[19]; // 19 is the max hash_oid size
    int j = 0;
    while (identified == 0 && i < k && j < 19) {
        hash_oid[j++] = em_bytes[i++];
        identified = try_identify_hasher_by_oid(hash_oid, j, &hasher);
    }

    if (identified == 0) {
        free(em_bytes);
        return Err(RSAVerificationResult, RSA_InvalidSignature);
    }

    RSAHashEntry hash_entry = hash_list[hasher];
    if (hash_entry.supported == 0) {
        free(em_bytes);
        return Err(RSAVerificationResult, RSA_HashNotSupported);
    }

    // the rest is the signature message hash
    int hash_size = hash_entry.hash_len;
    uint8_t *decoded_msg_hash = malloc(hash_size);
    for (int j = 0; j < hash_size; j++)
        decoded_msg_hash[j] = em_bytes[i++];

    // hash original message and verify they are the same
    UInt8Array original_msg_hash = {.array = malloc(hash_size), .size = hash_size};
    hash_msg(hasher, msg, &original_msg_hash);

    int cmp = memcmp(original_msg_hash.array, decoded_msg_hash, hash_size) != 0;
    free(em_bytes);
    free(original_msg_hash.array);
    free(decoded_msg_hash);

    if (cmp != 0)
        return Err(RSAVerificationResult, RSA_InvalidSignature);

    return Ok(RSAVerificationResult, {});
};

void hash_msg(RSAHashes hasher, UInt8Array msg, UInt8Array *hash) {
    switch (hasher) {
    case RSA_HASH_SHA256: {
        sha256 sha_hasher = sha256_new();
        sha256_update(&sha_hasher, msg.array, msg.size);
        u256 msg_digest = sha256_finalize(&sha_hasher);
        u256_get_bytes_big_endian(hash->array, msg_digest);
        return;
    }
    default:
        return;
    };
}

int try_identify_hasher_by_oid(uint8_t *bytes, int size, RSAHashes *hasher) {
    int num_oids = sizeof(hash_list) / sizeof(RSAHashEntry);

    for (int i = 0; i < num_oids; i++) {
        if (size >= hash_list[i].oid_size && memcmp(bytes, hash_list[i].oid, hash_list[i].oid_size) == 0) {
            *hasher = hash_list[i].function;
            return 1;
        }
    }

    return 0;
}
