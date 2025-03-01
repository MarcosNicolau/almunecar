#include <utils/test.h>

void test_void() {}

int main() {
    BEGIN_TEST()
    test(test_void);
    END_TEST()

    return 0;
}
