#include <primitive-types/u256.h>
#include <string.h>
#include <utils/test.h>

void test_u256_overflow_add() {
    u256 first = {{18446744073709551615ULL, 18446744073709551615ULL, 1099511627775ULL, 0}};
    u256 second = {{18446744073709551615ULL, 18446744073709551615ULL, 1099511627775ULL, 0}};
    u256_overflow_op result = u256_overflow_add(first, second);
    u256 expected_result = {{18446744073709551614ULL, 18446744073709551615ULL, 2199023255551ULL, 0}};

    assert_that(u256_cmp(result.res, expected_result) == 0);
    assert_that(result.overflow == 0);
}

void test_u256_overflow_add_with_overflow() {
    u256 first = {{UINT64_MAX, UINT64_MAX, UINT64_MAX, UINT64_MAX}};
    u256 second = {{UINT64_MAX, UINT64_MAX, UINT64_MAX, UINT64_MAX}};
    u256_overflow_op result = u256_overflow_add(first, second);
    u256 expected_result = {{UINT64_MAX - 1, UINT64_MAX, UINT64_MAX, UINT64_MAX}};

    assert_that(u256_cmp(result.res, expected_result) == 0);
    assert_that(result.overflow == 1);
}

void test_u256_overflow_sub() {
    u256 first = {{18446744073709551615ULL, 18446744073709551615ULL, 1099511627775ULL, 0}};
    u256 second = {{2919980651337220095ULL, 14019525496019259228ULL, 10995116277ULL, 0}};
    u256_overflow_op result = u256_overflow_sub(first, second);
    u256 expected_result = {{15526763422372331520ULL, 4427218577690292387ULL, 1088516511498ULL, 0}};

    assert_that(u256_cmp(result.res, expected_result) == 0);
    assert_that(result.overflow == 0);
}

void test_u256_overflow_sub_with_overflow() {
    u256 first = {{0, 0, 0, 0}};
    u256 second = {{1, 1, 1, 1}};
    u256_overflow_op result = u256_overflow_sub(first, second);
    u256 expected_result = {{UINT64_MAX, UINT64_MAX - 1, UINT64_MAX - 1, UINT64_MAX - 1}};

    assert_that(u256_cmp(result.res, expected_result) == 0);
    assert_that(result.overflow == 1);
}

void test_u256_overflow_mul() {
    u256 first = {{18446744073709551615ULL, 0, 0, 0}};
    u256 second = {{2919980651337220095ULL, 0, 0, 0}};
    u256_overflow_op result = u256_overflow_mul(first, second);
    u256 expected_result = {{15526763422372331521ULL, 2919980651337220094ULL, 0, 0}};

    assert_that(u256_cmp(result.res, expected_result) == 0);
    assert_that(result.overflow == 0);
}

void test_u256_overflow_mul_with_overflow() {
    u256 first = {{18446744073709551615ULL, 18446744073709551615ULL, 1099511627775ULL, 0}};
    u256 second = {{2919980651337220095ULL, 14019525496019259228ULL, 10995116277ULL, 0}};
    u256_overflow_op result = u256_overflow_mul(first, second);
    u256 expected_result = {
        {15526763422372331521ULL, 4427218577690292387ULL, 17870282210899384074ULL, 14019525494141808257ULL}};

    assert_that(u256_cmp(result.res, expected_result) == 0);
    assert_that(result.overflow == 1);
}

void test_u256_bitand() {
    u256 first = {{18446744073709551615ULL, 18446744073709551615ULL, 1099511627775ULL, 1ULL}};
    u256 second = {{2919980651337220095ULL, 14019525496019259228ULL, 10995116277ULL, 0}};
    u256 result = u256_bitand(first, second);
    u256 expected_result = {{2919980651337220095ULL, 14019525496019259228ULL, 10995116277ULL, 0}};

    assert_that(u256_cmp(result, expected_result) == 0);
}

void test_u256_bitor() {
    u256 first = {{18446744073709551615ULL, 18446744073709551615ULL, 1099511627775ULL, 0}};
    u256 second = {{2919980651337220095ULL, 14019525496019259228ULL, 10995116277ULL, 1ULL}};
    u256 result = u256_bitor(first, second);
    u256 expected_result = {{18446744073709551615ULL, 18446744073709551615ULL, 1099511627775ULL, 1}};

    assert_that(u256_cmp(result, expected_result) == 0);
}

void test_u256_bitxor() {
    u256 first = {{18446744073709551615ULL, 18446744073709551615ULL, 1099511627775ULL, 1}};
    u256 second = {{2919980651337220095ULL, 14019525496019259228ULL, 10995116277ULL, 1}};
    u256 result = u256_bitxor(first, second);
    u256 expected_result = {{15526763422372331520ULL, 4427218577690292387ULL, 1088516511498ULL, 0}};

    assert_that(u256_cmp(result, expected_result) == 0);
}

void test_u256_bitnot() {
    u256 first = {{18446744073709551615ULL, 18446744073709551615ULL, 1099511627775ULL, 0}};
    u256 result = u256_bitnot(first);
    u256 expected_result = {{0, 0, 18446742974197923840ULL, 18446744073709551615ULL}};

    assert_that(u256_cmp(result, expected_result) == 0);
}

void test_u256_shl() {
    u256 first = {{18446744073709551615ULL, 18446744073709551615ULL, 1099511627775ULL, 0}};
    int shift = 130;
    u256 result = u256_shl(first, shift);
    u256 expected_result = {{0, 0, 18446744073709551612ULL, 18446744073709551615ULL}};

    assert_that(u256_cmp(result, expected_result) == 0);
}

void test_u256_shr() {
    u256 first = {{18446744073709551615ULL, 18446744073709551615ULL, 1099511627775ULL, 0}};
    int shift = 130;
    u256 result = u256_shr(first, shift);
    u256 expected_result = {{274877906943, 0, 0, 0}};

    assert_that(u256_cmp(result, expected_result) == 0);
}

void test_u256_divmod_with_rem() {
    u256 first = {{18446744073709551615ULL, 18446744073709551615ULL, 1099511627775ULL, 0}};
    u256 second = {{2919980651337220095ULL, 14019525496019259228ULL, 10995116277ULL, 0}};
    u256_div_op result = u256_divmod(first, second);
    u256 expected_quot = {{100, 0, 0, 0}};
    u256 expected_rem = {{3149840045630816355, 0, 0, 0}};

    assert_that(u256_cmp(result.quot, expected_quot) == 0);
    assert_that(u256_cmp(result.rem, expected_rem) == 0);
}

void test_u256_divmod_without_rem() {
    u256 first = {{18446744073709551615ULL, 18446744073709551615ULL, 1099511627775ULL, 0}};
    u256 second = {{3, 0, 0, 0}};
    u256_div_op result = u256_divmod(first, second);
    u256 expected_quot = {{6148914691236517205ULL, 6148914691236517205ULL, 366503875925ULL, 0}};
    u256 expected_rem = {{0, 0, 0, 0}};

    assert_that(u256_cmp(result.quot, expected_quot) == 0);
    assert_that(u256_cmp(result.rem, expected_rem) == 0);
}

void test_u256_from_string() {
    u256 result = u256_from_dec_string("374144419156711147060143317175368453031918731001855");
    u256 expected_result = {{18446744073709551615ULL, 18446744073709551615ULL, 1099511627775ULL, 0}};

    assert_that(u256_cmp(result, expected_result) == 0);
}

void test_u256_to_string() {
    char *result = u256_to_string((u256){{18446744073709551615ULL, 18446744073709551615ULL, 1099511627775ULL, 0}});
    char *expected_result = "374144419156711147060143317175368453031918731001855";

    assert_that(strcmp(result, expected_result) == 0);
}

void test_u256_from_u64() {
    u256 result = u256_from_u64(9223372036854775808ULL);
    u256 expected_result = {{9223372036854775808ULL, 0, 0, 0}};
    assert_that(u256_cmp(result, expected_result) == 0);
}

void test_u256_from_bytes_little_endian() {
    uint8_t bytes[32];
    memset(bytes, 0, 32);
    for (int i = 0; i < 21; i++) {
        bytes[i] = 255;
    }
    u256 result = u256_from_bytes_little_endian(bytes);
    u256 expected_result = {{18446744073709551615ULL, 18446744073709551615ULL, 1099511627775ULL, 0}};

    assert_that(u256_cmp(result, expected_result) == 0);
}

void test_u256_get_bytes_little_endian() {
    u256 number = u256_from_dec_string("374144419156711147060143317175368453031918731001855");
    uint8_t buffer[32];
    u256_get_bytes_little_endian(buffer, number);
    uint8_t expected_result[32] = {255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
                                   255, 255, 255, 255, 255, 0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0};

    for (int i = 0; i < 32; i++) {
        assert_that(expected_result[i] == buffer[i]);
    }
}

void test_u256_from_bytes_big_endian() {
    uint8_t bytes[32];
    memset(bytes, 0, 32);
    for (int i = 0; i < 21; i++) {
        bytes[i] = 255;
    }
    u256 result = u256_from_bytes_big_endian(bytes);
    u256 expected_result = {{0, 18446744073692774400ULL, 18446744073709551615ULL, 18446744073709551615ULL}};

    assert_that(u256_cmp(result, expected_result) == 0);
}

void test_u256_get_bytes_big_endian() {
    u256 number = u256_from_dec_string("374144419156711147060143317175368453031918731001855");
    uint8_t buffer[32];
    u256_get_bytes_big_endian(buffer, number);
    uint8_t expected_result[32] = {0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   255, 255, 255, 255, 255,
                                   255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255};

    for (int i = 0; i < 32; i++) {
        assert_that(expected_result[i] == buffer[i]);
    }
}

int main() {
    BEGIN_TEST();
    test(test_u256_overflow_add);
    test(test_u256_overflow_add_with_overflow);
    test(test_u256_overflow_sub);
    test(test_u256_overflow_sub_with_overflow);
    test(test_u256_overflow_mul);
    test(test_u256_overflow_mul_with_overflow);
    test(test_u256_bitand);
    test(test_u256_bitor);
    test(test_u256_bitxor);
    test(test_u256_bitnot);
    test(test_u256_shl);
    test(test_u256_shr);
    test(test_u256_divmod_with_rem);
    test(test_u256_divmod_without_rem);
    test(test_u256_from_string);
    test(test_u256_to_string);
    test(test_u256_from_u64);
    test(test_u256_from_bytes_little_endian);
    test(test_u256_get_bytes_little_endian);
    test(test_u256_from_bytes_big_endian);
    test(test_u256_get_bytes_big_endian);
    END_TEST();

    return 0;
}
