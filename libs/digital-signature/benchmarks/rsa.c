#include <digital-signature/rsa.h>
#include <math/random.h>
#include <utils/benchmark.h>

#define DEFINE_KEY_GEN_BENCHMARKS(BIT_SIZE)                                                                            \
    void benchmark_rsa_key_generation##BIT_SIZE() {                                                                    \
        RSAKeyPair key_pair = rsa_key_pair_new(BIT_SIZE);                                                              \
        rsa_gen_key_pair(&key_pair);                                                                                   \
    }

DEFINE_KEY_GEN_BENCHMARKS(256)
DEFINE_KEY_GEN_BENCHMARKS(512)
DEFINE_KEY_GEN_BENCHMARKS(1024)

int main() {
    BEGIN_BENCHMARK();
    benchmark(benchmark_rsa_key_generation256, 1);
    benchmark(benchmark_rsa_key_generation512, 1);
    benchmark(benchmark_rsa_key_generation1024, 1);
    END_BENCHMARK();
}
