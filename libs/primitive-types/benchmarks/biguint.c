#include <math/random.h>
#include <primitive-types/biguint.h>
#include <utils/benchmark.h>

void benchmark_divmod() {
    BigUint a = biguint_new(4);
    BigUint b = biguint_new(4);
    biguint_random(&a);
    biguint_random(&b);
    biguint_divmod(a, b, &a, &b);
}

int main() {
    BEGIN_BENCHMARK();
    benchmark(benchmark_divmod, 1000000);
    END_BENCHMARK();
}
