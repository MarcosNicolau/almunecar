#include <math/primes.h>
#include <math/random.h>
#include <utils/benchmark.h>

void benchmark_random_prime(int size) {
    BigUint a = biguint_new_heap(size);
    biguint_random_prime(&a);
    biguint_free(&a);
}

void benchmark_is_prime_solovay_strassen(int size, char *prime) {
    BigUint a = biguint_new_heap(size);
    biguint_from_dec_string(prime, &a);
    biguint_is_prime_solovay_strassen(a);
    biguint_free(&a);
}

void benchmark_jacobi(int size, char *prime) {
    BigUint p = biguint_new_heap(size);
    BigUint a = biguint_new_heap(size);
    biguint_from_dec_string(prime, &p);
    biguint_random(&a);
    jacobi(a, p);
    biguint_free(&a, &p);
}

int main() {
    BEGIN_BENCHMARK()
    benchmark("random_prime 256 bits", benchmark_random_prime, 1, 4);
    benchmark("random_prime 512 bits", benchmark_random_prime, 1, 8);
    benchmark("random_prime 1024 bits", benchmark_random_prime, 1, 16);
    benchmark("is_prime_solovay_strassen 512 bits prime", benchmark_is_prime_solovay_strassen, 1, 8,
              "34335733933145862804940350952130198968391666739716830607881089259566479256360992225995345130785490553890"
              "25695338868874287109369850868158680127720763571503");
    benchmark("is_prime_solovay_strassen 1024 bits prime", benchmark_is_prime_solovay_strassen, 1, 16,
              "24655650060360753080142862709006690867636082763996432687872619014553026794142593984738962973795417850400"
              "85414073637172246986341398241070222037495540411160258675361723321157569314242853171336423647307914642357"
              "9483846036052104816190148409971239514167619589698929804939809242854000598245576619523371634111874143");
    benchmark("jacobi 512 bits prime", benchmark_jacobi, 1, 8,
              "34335733933145862804940350952130198968391666739716830607881089259566479256360992225995345130785490553890"
              "25695338868874287109369850868158680127720763571503");
    benchmark("jacobi 1024 bits prime", benchmark_jacobi, 1, 16,
              "24655650060360753080142862709006690867636082763996432687872619014553026794142593984738962973795417850400"
              "85414073637172246986341398241070222037495540411160258675361723321157569314242853171336423647307914642357"
              "9483846036052104816190148409971239514167619589698929804939809242854000598245576619523371634111874143");

    END_BENCHMARK()

    return 0;
}
