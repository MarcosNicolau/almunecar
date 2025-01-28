#include <random.h>

static FILE *urandom_file = NULL;

uint64_t u64_random() {
    uint64_t randval;

    if (urandom_file == NULL) {
        // use urandom https://sockpuppet.org/blog/2014/02/25/safely-generate-random-numbers/
        urandom_file = fopen("/dev/urandom", "r");
    }
    fread(&randval, sizeof(randval), 1, urandom_file);

    return randval;
}

void biguint_random(BigUint *a) {
    for (int i = 0; i < a->size; i++) {
        a->limbs[i] = u64_random();
    }
}

void biguint_random_with_max_bits(BigUint *a, int max_bits) {
    if (max_bits <= 0 || max_bits > a->size * 64) {
        return biguint_zero(a);
    }

    for (int i = 0; i < a->size; i++) {
        a->limbs[i] = u64_random();
    }

    int last_limb_index = (max_bits - 1) / 64;
    int bit_offset = max_bits % 64;

    if (bit_offset != 0) {
        a->limbs[last_limb_index] &= ((uint64_t)1 << bit_offset) - 1;
    }

    for (int i = last_limb_index + 1; i < a->size; i++) {
        a->limbs[i] = 0;
    }
}
