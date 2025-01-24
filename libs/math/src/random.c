#include <random.h>

uint64_t u64_random() {
    uint64_t randval;
    FILE *f;

    // use urandom https://sockpuppet.org/blog/2014/02/25/safely-generate-random-numbers/
    f = fopen("/dev/urandom", "r");
    fread(&randval, sizeof(randval), 1, f);
    fclose(f);

    return randval;
}

void biguint_random(BigUint *a) {
    for (int i = 0; i < a->size; i++) {
        a->limbs[i] = u64_random();
    }
}
