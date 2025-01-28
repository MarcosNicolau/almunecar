#include <math/random.h>
#include <primitive-types/biguint.h>
#include <utils/benchmark.h>

void benchmark_add() {
    BigUint a = biguint_new(16);
    BigUint b = biguint_new(16);
    biguint_random(&a);
    biguint_random(&b);
    biguint_add(&a, b);
}

void benchmark_sub() {
    BigUint a = biguint_new(16);
    BigUint b = biguint_new(16);
    biguint_random(&a);
    biguint_random(&b);
    biguint_sub(&a, b);
}

void benchmark_divmod() {
    BigUint a = biguint_new(16);
    BigUint b = biguint_new(16);
    BigUint c = biguint_new(16);
    BigUint d = biguint_new(16);
    biguint_random(&a);
    biguint_random(&b);
    biguint_divmod(a, b, &c, &d);
}

void benchmark_mul() {
    BigUint a = biguint_new(16);
    BigUint b = biguint_new(16);
    biguint_random(&a);
    biguint_random(&b);
    biguint_mul(&a, b);
}

void benchmark_pow() {
    BigUint a = biguint_new(16);
    BigUint b = biguint_new(16);
    biguint_random(&a);
    biguint_random(&b);
    biguint_pow(&a, b);
}

void benchmark_pow_mod() {
    BigUint a = biguint_new(16);
    BigUint b = biguint_new(16);
    BigUint m = biguint_new(16);
    biguint_random(&a);
    biguint_random(&b);
    biguint_random(&m);
    biguint_pow_mod(&a, b, m);
}

int main() {
    BEGIN_BENCHMARK();
    benchmark(benchmark_add, 1000000);
    benchmark(benchmark_sub, 1000000);
    benchmark(benchmark_divmod, 1000000);
    benchmark(benchmark_mul, 1000000);
    benchmark(benchmark_pow, 1000);
    benchmark(benchmark_pow_mod, 10);
    END_BENCHMARK();
}
