#include <random.h>

uint64_t u64_random() {
    uint64_t randval;
    FILE *f;

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
