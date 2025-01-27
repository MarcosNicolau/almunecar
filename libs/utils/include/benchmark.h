#ifndef TEST_H
#define TEST_H
#include <time.h>
#include <utils/macros.h>

#define benchmark(benchmark_fn, iterations, ...)                                                                       \
    do {                                                                                                               \
        printf("\n=============== %s (%d iterations) ===============\n", #benchmark_fn, iterations);                   \
        int ANONYMOUS_VARIABLE(benchmark_fn) = 0;                                                                      \
        double measures[iterations];                                                                                   \
        for (; ANONYMOUS_VARIABLE(benchmark_fn) < iterations; ANONYMOUS_VARIABLE(benchmark_fn)++) {                    \
            clock_t start_time = clock();                                                                              \
            benchmark_fn(__VA_ARGS__);                                                                                 \
            clock_t end_time = clock();                                                                                \
            double diff = (double)(end_time - start_time) / CLOCKS_PER_SEC;                                            \
            measures[ANONYMOUS_VARIABLE(benchmark_fn)] = diff;                                                         \
        }                                                                                                              \
        double sum = 0;                                                                                                \
        double avg = 0;                                                                                                \
        for (int ANONYMOUS_VARIABLE(benchmark_fn_i) = 0; ANONYMOUS_VARIABLE(benchmark_fn_i) < iterations;              \
             ANONYMOUS_VARIABLE(benchmark_fn_i)++) {                                                                   \
            sum += measures[ANONYMOUS_VARIABLE(benchmark_fn_i)];                                                       \
        }                                                                                                              \
        avg = sum / iterations;                                                                                        \
        printf("Total execution took: %f seconds\n", sum);                                                             \
        printf("Average execution took: %f seconds\n", avg);                                                           \
        printf("=============================\n");                                                                     \
    } while (0)

#define BEGIN_BENCHMARK()                                                                                              \
    printf("\n==============================\n");                                                                      \
    printf("Benchmark suite %s at %s\n", __FILE__, __func__);

#define END_BENCHMARK()                                                                                                \
    printf("\nBenchmark suite %s at %s", __FILE__, __func__);                                                          \
    printf("\n==============================\n");

#endif
