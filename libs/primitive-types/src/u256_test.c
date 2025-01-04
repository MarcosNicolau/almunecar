#include "./u256.h"
#include <assert.h>
#include <string.h>

void test_u256_overflow_add()
{
    u256 first = {{18446744073709551615ULL, 18446744073709551615ULL, 1099511627775ULL, 0}};
    u256 second = {{18446744073709551615ULL, 18446744073709551615ULL, 1099511627775ULL, 0}};
    u256_overflow_op result = u256_overflow_add(first, second);
    u256 expected_result = {{18446744073709551614ULL, 18446744073709551615ULL, 2199023255551ULL, 0}};

    assert(u256_cmp(result.res, expected_result) == 0);
    assert(result.overflow == 0);
}

void test_u256_overflow_add_with_overflow()
{
    u256 first = {{UINT64_MAX, UINT64_MAX, UINT64_MAX, UINT64_MAX}};
    u256 second = {{UINT64_MAX, UINT64_MAX, UINT64_MAX, UINT64_MAX}};
    u256_overflow_op result = u256_overflow_add(first, second);
    u256 expected_result = {{UINT64_MAX - 1, UINT64_MAX, UINT64_MAX, UINT64_MAX}};

    assert(u256_cmp(result.res, expected_result) == 0);
    assert(result.overflow == 1);
}

void test_u256_overflow_sub()
{
    u256 first = {{18446744073709551615ULL, 18446744073709551615ULL, 1099511627775ULL, 0}};
    u256 second = {{2919980651337220095ULL, 14019525496019259228ULL, 10995116277ULL, 0}};
    u256_overflow_op result = u256_overflow_sub(first, second);
    u256 expected_result = {{15526763422372331520ULL, 4427218577690292387ULL, 1088516511498ULL, 0}};

    assert(u256_cmp(result.res, expected_result) == 0);
    assert(result.overflow == 0);
}

void test_u256_overflow_sub_with_overflow()
{
    u256 first = {{0, 0, 0, 0}};
    u256 second = {{1, 1, 1, 1}};
    u256_overflow_op result = u256_overflow_sub(first, second);
    u256 expected_result = {{UINT64_MAX, UINT64_MAX - 1, UINT64_MAX - 1, UINT64_MAX - 1}};

    assert(u256_cmp(result.res, expected_result) == 0);
    assert(result.overflow == 1);
}

void test_u256_overflow_mul()
{
    u256 first = {{18446744073709551615ULL, 0, 0, 0}};
    u256 second = {{2919980651337220095ULL, 0, 0, 0}};
    u256_overflow_op result = u256_overflow_mul(first, second);
    u256 expected_result = {{15526763422372331521ULL, 2919980651337220094ULL, 0, 0}};

    assert(u256_cmp(result.res, expected_result) == 0);
    assert(result.overflow == 0);
}

void test_u256_overflow_mul_with_overflow()
{
    u256 first = {{18446744073709551615ULL, 18446744073709551615ULL, 1099511627775ULL, 0}};
    u256 second = {{2919980651337220095ULL, 14019525496019259228ULL, 10995116277ULL, 0}};
    u256_overflow_op result = u256_overflow_mul(first, second);
    u256 expected_result = {{15526763422372331521ULL, 4427218577690292387ULL, 17870282210899384074ULL, 14019525494141808257ULL}};

    assert(u256_cmp(result.res, expected_result) == 0);
    assert(result.overflow == 1);
}

void test_u256_bitand()
{
    u256 first = {{18446744073709551615ULL, 18446744073709551615ULL, 1099511627775ULL, 1ULL}};
    u256 second = {{2919980651337220095ULL, 14019525496019259228ULL, 10995116277ULL, 0}};
    u256 result = u256_bitand(first, second);
    u256 expected_result = {{2919980651337220095ULL, 14019525496019259228ULL, 10995116277ULL, 0}};

    assert(u256_cmp(result, expected_result) == 0);
}

void test_u256_bitor()
{
    u256 first = {{18446744073709551615ULL, 18446744073709551615ULL, 1099511627775ULL, 0}};
    u256 second = {{2919980651337220095ULL, 14019525496019259228ULL, 10995116277ULL, 1ULL}};
    u256 result = u256_bitor(first, second);
    u256 expected_result = {{18446744073709551615ULL, 18446744073709551615ULL, 1099511627775ULL, 1}};

    assert(u256_cmp(result, expected_result) == 0);
}

void test_u256_bitxor()
{
    u256 first = {{18446744073709551615ULL, 18446744073709551615ULL, 1099511627775ULL, 1}};
    u256 second = {{2919980651337220095ULL, 14019525496019259228ULL, 10995116277ULL, 1}};
    u256 result = u256_bitxor(first, second);
    u256 expected_result = {{15526763422372331520ULL, 4427218577690292387ULL, 1088516511498ULL, 0}};

    assert(u256_cmp(result, expected_result) == 0);
}

void test_u256_bitnot()
{
    u256 first = {{18446744073709551615ULL, 18446744073709551615ULL, 1099511627775ULL, 0}};
    u256 result = u256_bitnot(first);
    u256 expected_result = {{0, 0, 18446742974197923840ULL, 18446744073709551615ULL}};

    assert(u256_cmp(result, expected_result) == 0);
}

void test_u256_shl()
{
    u256 first = {{18446744073709551615ULL, 18446744073709551615ULL, 1099511627775ULL, 0}};
    int shift = 130;
    u256 result = u256_shl(first, shift);
    u256 expected_result = {{0, 0, 18446744073709551612ULL, 18446744073709551615ULL}};

    assert(u256_cmp(result, expected_result) == 0);
}

void test_u256_shr()
{
    u256 first = {{18446744073709551615ULL, 18446744073709551615ULL, 1099511627775ULL, 0}};
    int shift = 130;
    u256 result = u256_shr(first, shift);
    u256 expected_result = {{274877906943, 0, 0, 0}};

    assert(u256_cmp(result, expected_result) == 0);
}

void test_u256_divmod_with_rem()
{
    u256 first = {{18446744073709551615ULL, 18446744073709551615ULL, 1099511627775ULL, 0}};
    u256 second = {{2919980651337220095ULL, 14019525496019259228ULL, 10995116277ULL, 0}};
    u256_div_op result = u256_divmod(first, second);
    u256 expected_quot = {{100, 0, 0, 0}};
    u256 expected_rem = {{3149840045630816355, 0, 0, 0}};

    assert(u256_cmp(result.quot, expected_quot) == 0);
    assert(u256_cmp(result.rem, expected_rem) == 0);
}

void test_u256_divmod_without_rem()
{
    u256 first = {{18446744073709551615ULL, 18446744073709551615ULL, 1099511627775ULL, 0}};
    u256 second = {{3, 0, 0, 0}};
    u256_div_op result = u256_divmod(first, second);
    u256 expected_quot = {{6148914691236517205ULL, 6148914691236517205ULL, 366503875925ULL, 0}};
    u256 expected_rem = {{0, 0, 0, 0}};

    assert(u256_cmp(result.quot, expected_quot) == 0);
    assert(u256_cmp(result.rem, expected_rem) == 0);
}

void test_u256_from_string()
{
    u256 result = u256_from_dec_string("374144419156711147060143317175368453031918731001855");
    u256 expected_result = {{18446744073709551615ULL, 18446744073709551615ULL, 1099511627775ULL, 0}};

    assert(u256_cmp(result, expected_result) == 0);
}

void test_u256_to_string()
{
    char *result = u256_to_string((u256){{18446744073709551615ULL, 18446744073709551615ULL, 1099511627775ULL, 0}});
    char *expected_result = "374144419156711147060143317175368453031918731001855";

    assert(strcmp(result, expected_result) == 0);
}

int main(int argc, char **argv)
{
    test_u256_overflow_add();
    test_u256_overflow_add_with_overflow();
    test_u256_overflow_sub();
    test_u256_overflow_sub_with_overflow();
    test_u256_overflow_mul();
    test_u256_overflow_mul_with_overflow();
    test_u256_bitand();
    test_u256_bitor();
    test_u256_bitxor();
    test_u256_bitnot();
    test_u256_shl();
    test_u256_shr();
    test_u256_divmod_with_rem();
    test_u256_divmod_without_rem();
    test_u256_from_string();
    test_u256_to_string();

    return 0;
}
