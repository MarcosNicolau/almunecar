#include <math/random.h>
#include <utils/test.h>

void test_random_biguint_works() {
    BigUint a = biguint_new_with_limbs(4, {0});
    biguint_random(&a);

    for (int i = 0; i < 4; i++) {
        assert_that(!biguint_is_zero(a));
    }
}

int main() {
    BEGIN_TEST()
    test(test_random_biguint_works);
    END_TEST()

    return 0;
}
