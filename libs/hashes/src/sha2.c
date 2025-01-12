#include <sha256.h>

/**
 *
 * Constants, operations and functions
 *
 */
static const uint32_t k[64] = {
    0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5, 0x3956c25b, 0x59f111f1,
    0x923f82a4, 0xab1c5ed5, 0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3,
    0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174, 0xe49b69c1, 0xefbe4786,
    0x0fc19dc6, 0x240ca1cc, 0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
    0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7, 0xc6e00bf3, 0xd5a79147,
    0x06ca6351, 0x14292967, 0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13,
    0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85, 0xa2bfe8a1, 0xa81a664b,
    0xc24b8b70, 0xc76c51a3, 0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
    0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5, 0x391c0cb3, 0x4ed8aa4a,
    0x5b9cca4f, 0x682e6ff3, 0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208,
    0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2};

static const uint32_t h[8] = {0x6a09e667, 0xbb67ae85, 0x3c6ef372, 0xa54ff53a,
                              0x510e527f, 0x9b05688c, 0x1f83d9ab, 0x5be0cd19};

// equivalent to a left rotation by (w-n). Here w = 32
uint32_t rotr(uint32_t x, int n) { return (x >> n) | (x << (32 - n)); };
uint32_t ch(uint32_t x, uint32_t y, uint32_t z) { return (x & y) ^ (~x & z); };
uint32_t major(uint32_t x, uint32_t y, uint32_t z) {
    return (x & y) ^ (x & z) ^ (y & z);
};
uint32_t Sigma0(uint32_t x) { return rotr(x, 2) ^ rotr(x, 13) ^ rotr(x, 22); };
uint32_t Sigma1(uint32_t x) { return rotr(x, 6) ^ rotr(x, 11) ^ rotr(x, 25); }
uint32_t sigma0(uint32_t x) { return rotr(x, 7) ^ rotr(x, 18) ^ (x >> 3); };
uint32_t sigma1(uint32_t x) { return rotr(x, 17) ^ rotr(x, 19) ^ (x >> 10); };

void sha256_process(sha256 *hash) {
    uint32_t w[64];
    memset(w, 0, 64);

    // we need to fit the 64 entries bytes into 16 entries
    // so each entry needs to have a size of 4 bytes
    for (int i = 0, j = 0; i < 16; i++, j += 4) {
        // This operation merges the first 4 bytes starting from the jth
        // position
        w[i] = (hash->bytes[j] << 24) | (hash->bytes[j + 1] << 16) |
               (hash->bytes[j + 2] << 8) | (hash->bytes[j + 3]);
    }

    for (int i = 16; i < 64; i++) {
        uint32_t s0 = sigma0(w[i - 15]);
        uint32_t s1 = sigma1(w[i - 2]);
        w[i] = w[i - 16] + s0 + w[i - 7] + s1;
    }

    uint32_t a = hash->h[0];
    uint32_t b = hash->h[1];
    uint32_t c = hash->h[2];
    uint32_t d = hash->h[3];
    uint32_t e = hash->h[4];
    uint32_t f = hash->h[5];
    uint32_t g = hash->h[6];
    uint32_t h = hash->h[7];

    for (int i = 0; i < 64; i++) {
        uint32_t S1 = Sigma1(e);
        uint32_t ch_ = ch(e, f, g);
        uint32_t t1 = h + S1 + ch_ + k[i] + w[i];
        uint32_t S0 = Sigma0(a);
        uint32_t maj = major(a, b, c);
        uint32_t t2 = S0 + maj;

        h = g;
        g = f;
        f = e;
        e = d + t1;
        d = c;
        c = b;
        b = a;
        a = t1 + t2;
    }

    hash->h[0] += a;
    hash->h[1] += b;
    hash->h[2] += c;
    hash->h[3] += d;
    hash->h[4] += e;
    hash->h[5] += f;
    hash->h[6] += g;
    hash->h[7] += h;
}

void sha256_apply_padding(sha256 *hash) {
    // if its less than 56 bytes, then we need to pad the message to make it a
    // multiple of 512, this consists of:
    // - appending 1 bit to the end
    // - appending k 0 bits where k is the minimum number such that L + 1 + K +
    // 64 = 512b
    if (hash->bytes_size < 56) {
        hash->bytes[hash->bytes_size] = 0x80; // 10000000
        int k_zeros_to_add = 56 - (hash->bytes_size + 1);
        for (int i = 1; i < k_zeros_to_add; i++) {
            hash->bytes[hash->bytes_size + i] = 0x00;
        }
    }

    // if the hash is larger than 64 bytes, then we pad the message construct a
    // new message block and then we create another message block with the total
    // size length in bits.
    if (hash->bytes_size > 56) {
        hash->bytes[hash->bytes_size] = 0x80; // 10000000
        int k_zeros_to_add = 64 - (hash->bytes_size + 1);
        for (int i = 1; i < k_zeros_to_add; i++) {
            hash->bytes[hash->bytes_size + i] = 0x00;
        }
        sha256_process(hash);
        memset(hash->bytes, 0, 56);
    }

    // Finally, set the last 64 bits to the total message length in bits
    uint64_t bitlen = hash->total_size * 8 + hash->bytes_size * 8;
    hash->bytes[56] = bitlen >> 56;
    hash->bytes[57] = bitlen >> 48;
    hash->bytes[58] = bitlen >> 40;
    hash->bytes[59] = bitlen >> 32;
    hash->bytes[60] = bitlen >> 24;
    hash->bytes[61] = bitlen >> 16;
    hash->bytes[62] = bitlen >> 8;
    hash->bytes[63] = bitlen;
    sha256_process(hash);
}

sha256 sha256_new() {
    sha256 hash = {.bytes_size = 0, .total_size = 0};
    for (int i = 0; i < 8; i++)
        hash.h[i] = h[i];
    memset(hash.bytes, 0, 64);
    return hash;
}

void sha256_update(sha256 *hash, uint8_t *bytes, size_t size) {
    for (size_t i = 0; i < size; i++) {
        hash->bytes[hash->bytes_size++] = bytes[i];
        // We reach 64 bytes of data size (512 bits), we can construct a new
        // message block and restart the data
        if (hash->bytes_size == 64) {
            sha256_process(hash);
            hash->bytes_size = 0;
            hash->total_size += 64;
        }
    }
}

u256 sha256_finalize(sha256 *hash) {
    sha256_apply_padding(hash);
    // SHA-2 algorithm defines the hash values in big-endian format, but our
    // implementation stores the hash values in little-endian format.
    // we need to convert the hash to big-endian byte order
    uint8_t bytes[32];
    for (int j = 0; j < 8; ++j) {
        for (int i = 0; i < 4; ++i) {
            bytes[j * 4 + i] = (hash->h[j] >> (24 - i * 8)) & 0xFF;
        }
    }
    u256 digest = u256_from_bytes_big_endian(bytes);
    return digest;
};
