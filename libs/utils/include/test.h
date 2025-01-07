#ifndef TEST_H
#define TEST_H
#define TESTING
#define RED "\x1B[31m"
#define GRN "\x1B[32m"
#define RESET "\x1B[0m"

#include <setjmp.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define assert_that(expr) test_assert(expr, __FILE__, __LINE__)

static jmp_buf env;

#define test(test_fn, ...)                                          \
    do {                                                            \
        printf("\n=============== %s ===============\n", #test_fn); \
        if (setjmp(env) == 0) {                                     \
            test_fn(__VA_ARGS__);                                   \
            printf("%spassed%s\n", GRN, RESET);                     \
        }                                                           \
    } while (0)

#define BEGIN_TEST()                              \
    printf("\n==============================\n"); \
    printf("Test suite %s at %s\n", __FILE__, __func__);

#define END_TEST()                                       \
    printf("\nTest suite %s at %s", __FILE__, __func__); \
    printf("\n==============================\n");

void test_assert(int expression, char *file, int line) {
    if (expression == 0) {
        printf("%sfailed%s at %s:%d: \n", RED, RESET, file, line);
        char *fail_fast = getenv("FAIL_FAST");
        if (fail_fast != NULL && strcmp(fail_fast, "true") == 0) {
            exit(1);
        }
        longjmp(env, 1);
    }
}

#endif
