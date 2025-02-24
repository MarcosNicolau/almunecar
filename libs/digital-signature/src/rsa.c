#include <hashes/sha256.h>
#include <rsa.h>

#define e_const 65537

/**
 *
 * RSA Hash function oids
 *
 */

typedef struct {
    uint8_t oid[19]; // Maximum OID length from the given list
    int size;
    int hash_len;
    RSAHashes function;
    int supported; // If almunecar supports the hash function
} OidEntry;

const OidEntry oid_list[] = {
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

void rsa_encrypt(BigUint msg, RSAPublicKey pub, BigUint *cypher) { biguint_pow_mod(msg, pub.e, pub.n, cypher); };

void rsa_decrypt(BigUint cipher, RSAKeyPair key_pair, BigUint *out) {
    biguint_pow_mod(cipher, key_pair.priv.d, key_pair.pub.n, out);
};

void rsa_sign(BigUint msg, RSAKeyPair key_pair, BigUint *signature) {
    biguint_pow_mod(msg, key_pair.priv.d, key_pair.pub.n, signature);
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

    BigUint chipher = biguint_new_heap(limbs_size);
    rsa_encrypt(em, pub, &chipher);

    if (buf->size < k) {
        buf->array = realloc(buf->array, k);
        buf->size = k;
    }
    biguint_get_bytes_big_endian(chipher, buf->array);

    biguint_free(&em, &chipher);
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
    rsa_decrypt(cipher, key_pair, &em);

    // EM = 0x00 || 0x02 || PS || 0x00 || M.
    uint8_t *em_bytes = malloc(k);
    biguint_get_bytes_big_endian(em, em_bytes);

    biguint_free(&cipher, &em);
    int i = 0;
    if (em_bytes[i++] != 0x00) {
        return Err(RSADecryptResult, RSA_InvalidEncodedMessage);
    }
    if (em_bytes[i++] != 0x02) {
        return Err(RSADecryptResult, RSA_InvalidEncodedMessage);
    }
    int count = 0;
    uint8_t byte = em_bytes[i++];
    while (byte != 0 || count > k - 11) {
        byte = em_bytes[i++];
        count++;
    }
    if (count < 8) {
        return Err(RSADecryptResult, RSA_InvalidEncodedMessage);
    }
    if (byte != 0x00) {
        return Err(RSADecryptResult, RSA_InvalidEncodedMessage);
    }

    // the rest is the message
    int msg_size = cipher_bytes.size - count - 3;
    if (buf->size < msg_size + 1) {
        buf->array = realloc(buf->array, msg_size + 1);
        buf->size = msg_size;
    }
    for (int j = 0; j < msg_size; j++) {
        buf->array[j] = em_bytes[i++];
    }
    buf->array[msg_size] = '\0';

    free(em_bytes);

    return Ok(RSADecryptResult, {});
};

void hash_msg(RSAHashes hasher, UInt8Array msg, UInt8Array *hash) {
    switch (hasher) {
    case RSA_HASH_SHA256:
        sha256 hasher = sha256_new();
        sha256_update(&hasher, msg.array, msg.size);
        u256 msg_digest = sha256_finalize(&hasher);
        u256_get_bytes_big_endian(hash->array, msg_digest);
        break;
    };
}

// write hash function oid to the buffer, it assumes the buffer has enough space
// returns the size of the
int write_rsa_hash_with_oid(RSAHashes hasher, UInt8Array hash, UInt8Array *buf) {
    switch (hasher) {
    case RSA_HASH_SHA256:
        // 19 bytes for the oid and 32 for the hash
        int size = 51;
        buf->array = realloc(buf->array, size);
        buf->size = 51;
        int i = 0;
        for (; i < 19; i++)
            buf->array[i] = oid_list[3].oid[i];
        for (int j = 0; j < 32; j++)
            buf->array[i++] = hash.array[j];

        break;
    };
}

RSASignResult rsa_sign_PKCS1v15(UInt8Array msg_bytes, RSAKeyPair key_pair, RSAHashes hasher, UInt8Array *buf) {
    // k represents the length of n in bytes
    int k = (biguint_bits(key_pair.pub.n) + 7) / 8;
    int limbs_size = k / 8;

    UInt8Array msg_hash = {.array = (uint8_t[32]){}, .size = 32};
    hash_msg(hasher, msg_bytes, &msg_hash);

    UInt8Array t = {};
    write_rsa_hash_with_oid(hasher, msg_hash, &t);

    if (k < t.size + 11) {
        return Err(RSASignResult, RSA_MessageTooShort);
    }

    // EM = 0x00 || 0x01 || PS || 0x00 || T.
    uint8_t *em_bytes = malloc(k);
    int i = 0;
    em_bytes[i++] = 0x00;
    em_bytes[i++] = 0x01;
    for (; i < k - t.size - 1; i++)
        em_bytes[i] = 0xff;
    em_bytes[i++] = 0x00;
    for (int j = 0; j < t.size; j++)
        em_bytes[i++] = t.array[j];

    BigUint em = biguint_new_heap(limbs_size);
    biguint_from_bytes_big_endian(em_bytes, &em);

    BigUint signature = biguint_new_heap(limbs_size);
    rsa_sign(em, key_pair, &signature);

    if (buf->size < k) {
        buf->array = realloc(buf->array, k);
        buf->size = k;
    }
    biguint_get_bytes_big_endian(signature, buf->array);

    free(em_bytes);
    free(t.array);
    biguint_free(&signature, &em);

    return Ok(RSASignResult, {});
}

int try_identify_hasher_by_oid(uint8_t *bytes, int size, RSAHashes *hasher) {
    int num_oids = sizeof(oid_list) / sizeof(OidEntry);

    for (int i = 0; i < num_oids; i++) {
        if (size >= oid_list[i].size && memcmp(bytes, oid_list[i].oid, oid_list[i].size) == 0) {
            *hasher = oid_list[i].function;
            return 1;
        }
    }

    return 0;
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

    // EM = 0x00 || 0x01 || PS || 0x00 || T.
    uint8_t *em_bytes = malloc(k);
    biguint_get_bytes_big_endian(em, em_bytes);

    int i = 0;
    if (em_bytes[i++] != 0x00) {
        return Err(RSAVerificationResult, RSA_InvalidSignature);
    }
    if (em_bytes[i++] != 0x01) {
        return Err(RSAVerificationResult, RSA_InvalidSignature);
    }
    int count = 0;
    uint8_t byte = em_bytes[i++];
    while (byte == 0xff) {
        byte = em_bytes[i++];
        count++;
    }
    // TODO check this
    if (count < 8) {
        return Err(RSAVerificationResult, RSA_InvalidSignature);
    }
    if (byte != 0x00) {
        return Err(RSAVerificationResult, RSA_InvalidSignature);
    }

    RSAHashes hasher;

    int identified = 0;
    uint8_t hash_oid[19];
    int j = 0;
    while (identified == 0 && i < k && j < 19) {
        hash_oid[j++] = em_bytes[i++];
        identified = try_identify_hasher_by_oid(hash_oid, j, &hasher);
    }

    if (identified == 0) {
        return Err(RSAVerificationResult, RSA_InvalidSignature);
    }

    // the rest is the signature message hash
    int hash_size = oid_list[hasher].hash_len;
    if (hash_size == 0) {
        return Err(RSAVerificationResult, RSA_HashNotSupported);
    }

    uint8_t *decoded_msg_hash = malloc(hash_size);
    for (int j = 0; j < hash_size; j++)
        decoded_msg_hash[j] = em_bytes[i++];

    // hash original message and verify they are the same
    UInt8Array original_msg_hash = {.array = malloc(hash_size), .size = hash_size};
    hash_msg(hasher, msg, &original_msg_hash);

    free(em_bytes);
    biguint_free(&signature, &em);

    if (memcmp(original_msg_hash.array, decoded_msg_hash, hash_size) != 0) {
        free(original_msg_hash.array);
        free(decoded_msg_hash);
        return Err(RSAVerificationResult, RSA_InvalidSignature);
    }

    free(original_msg_hash.array);
    free(decoded_msg_hash);

    return Ok(RSAVerificationResult, {});
};
