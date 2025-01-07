#include <primitive-types/u256.h>

int main(int argc, char **argv)
{
    if (argc < 3)
    {
        printf("You must provide two decimal strings.");
        return 1;
    }

    u256 first = u256_from_dec_string(argv[1]);
    u256 second = u256_from_dec_string(argv[2]);

    u256_overflow_op res = u256_overflow_add(first, second);

    printf("RESULT:\n");
    u256_println(res.res);

    return 0;
}
