#include <primitive-types/biguint.h>
#include <string.h>
#include <utils/test.h>

void test_big_uint_overflow_add() {
    BigUint first = big_uint_new_with_limbs(4, {18446744073709551615ULL, 18446744073709551615ULL, 1099511627775ULL, 0});
    BigUint second =
        big_uint_new_with_limbs(4, {18446744073709551615ULL, 18446744073709551615ULL, 1099511627775ULL, 0});
    BigUint expected_result =
        big_uint_new_with_limbs(4, {18446744073709551614ULL, 18446744073709551615ULL, 2199023255551ULL, 0});

    int overflow = big_uint_overflow_add(&first, second);

    assert_that(big_uint_cmp(first, expected_result) == 0);
    assert_that(overflow == 0);
}

void test_big_uint_overflow_add_with_overflow() {
    BigUint first = big_uint_new_with_limbs(4, {UINT64_MAX, UINT64_MAX, UINT64_MAX, UINT64_MAX});
    BigUint second = big_uint_new_with_limbs(4, {UINT64_MAX, UINT64_MAX, UINT64_MAX, UINT64_MAX});
    BigUint expected_result = big_uint_new_with_limbs(4, {UINT64_MAX - 1, UINT64_MAX, UINT64_MAX, UINT64_MAX});

    int overflow = big_uint_overflow_add(&first, second);

    assert_that(big_uint_cmp(first, expected_result) == 0);
    assert_that(overflow == 1);
}

void test_big_uint_overflow_sub() {
    BigUint first = big_uint_new_with_limbs(4, {18446744073709551615ULL, 18446744073709551615ULL, 1099511627775ULL, 0});
    BigUint second = big_uint_new_with_limbs(4, {2919980651337220095ULL, 14019525496019259228ULL, 10995116277ULL, 0});
    BigUint expected_result =
        big_uint_new_with_limbs(4, {15526763422372331520ULL, 4427218577690292387ULL, 1088516511498ULL, 0});

    int overflow = big_uint_overflow_sub(&first, second);

    assert_that(big_uint_cmp(first, expected_result) == 0);
    assert_that(overflow == 0);
}

void test_big_uint_overflow_sub_with_overflow() {
    BigUint first = big_uint_new_with_limbs(4, {0, 0, 0, 0});
    BigUint second = big_uint_new_with_limbs(4, {1, 1, 1, 1});
    BigUint expected_result = big_uint_new_with_limbs(4, {UINT64_MAX, UINT64_MAX - 1, UINT64_MAX - 1, UINT64_MAX - 1});
    int overflow = big_uint_overflow_sub(&first, second);

    assert_that(big_uint_cmp(first, expected_result) == 0);
    assert_that(overflow == 1);
}

void test_big_uint_overflow_mul() {
    BigUint first = big_uint_new_with_limbs(4, {18446744073709551615ULL, 0, 0, 0});
    BigUint second = big_uint_new_with_limbs(4, {2919980651337220095ULL, 0, 0, 0});
    BigUint expected_result = big_uint_new_with_limbs(4, {15526763422372331521ULL, 2919980651337220094ULL, 0, 0});
    int overflow = big_uint_overflow_mul(&first, second);

    assert_that(big_uint_cmp(first, expected_result) == 0);
    assert_that(overflow == 0);
}

void test_big_uint_overflow_mul_with_overflow() {
    BigUint first = big_uint_new_with_limbs(4, {18446744073709551615ULL, 18446744073709551615ULL, 1099511627775ULL, 0});
    BigUint second = big_uint_new_with_limbs(4, {2919980651337220095ULL, 14019525496019259228ULL, 10995116277ULL, 0});
    BigUint expected_result = big_uint_new_with_limbs(
        4, {15526763422372331521ULL, 4427218577690292387ULL, 17870282210899384074ULL, 14019525494141808257ULL});
    int overflow = big_uint_overflow_mul(&first, second);

    assert_that(big_uint_cmp(first, expected_result) == 0);
    assert_that(overflow == 1);
}

void test_big_uint_bitand() {
    BigUint first =
        big_uint_new_with_limbs(4, {18446744073709551615ULL, 18446744073709551615ULL, 1099511627775ULL, 1ULL});
    BigUint second = big_uint_new_with_limbs(4, {2919980651337220095ULL, 14019525496019259228ULL, 10995116277ULL, 0});
    BigUint expected_result =
        big_uint_new_with_limbs(4, {2919980651337220095ULL, 14019525496019259228ULL, 10995116277ULL, 0});
    big_uint_bitand(&first, second);

    assert_that(big_uint_cmp(first, expected_result) == 0);
}

void test_big_uint_bitor() {
    BigUint first = big_uint_new_with_limbs(4, {18446744073709551615ULL, 18446744073709551615ULL, 1099511627775ULL, 0});
    BigUint second =
        big_uint_new_with_limbs(4, {2919980651337220095ULL, 14019525496019259228ULL, 10995116277ULL, 1ULL});
    BigUint expected_result =
        big_uint_new_with_limbs(4, {18446744073709551615ULL, 18446744073709551615ULL, 1099511627775ULL, 1});
    big_uint_bitor(&first, second);

    assert_that(big_uint_cmp(first, expected_result) == 0);
}

void test_big_uint_bitxor() {
    BigUint first = big_uint_new_with_limbs(4, {18446744073709551615ULL, 18446744073709551615ULL, 1099511627775ULL, 1});
    BigUint second = big_uint_new_with_limbs(4, {2919980651337220095ULL, 14019525496019259228ULL, 10995116277ULL, 1});
    BigUint expected_result =
        big_uint_new_with_limbs(4, {15526763422372331520ULL, 4427218577690292387ULL, 1088516511498ULL, 0});
    big_uint_bitxor(&first, second);

    assert_that(big_uint_cmp(first, expected_result) == 0);
}

void test_big_uint_bitnot() {
    BigUint first = big_uint_new_with_limbs(4, {18446744073709551615ULL, 18446744073709551615ULL, 1099511627775ULL, 0});
    BigUint expected_result = big_uint_new_with_limbs(4, {0, 0, 18446742974197923840ULL, 18446744073709551615ULL});
    big_uint_bitnot(&first);

    assert_that(big_uint_cmp(first, expected_result) == 0);
}

void test_big_uint_shl() {
    BigUint first = big_uint_new_with_limbs(4, {18446744073709551615ULL, 18446744073709551615ULL, 1099511627775ULL, 0});
    int shift = 130;
    BigUint expected_result = big_uint_new_with_limbs(4, {0, 0, 18446744073709551612ULL, 18446744073709551615ULL});
    big_uint_shl(&first, shift);

    assert_that(big_uint_cmp(first, expected_result) == 0);
}

void test_big_uint_shr() {
    BigUint first = big_uint_new_with_limbs(4, {18446744073709551615ULL, 18446744073709551615ULL, 1099511627775ULL, 0});
    int shift = 130;
    BigUint expected_result = big_uint_new_with_limbs(4, {274877906943, 0, 0, 0});
    big_uint_shr(&first, shift);

    assert_that(big_uint_cmp(first, expected_result) == 0);
}

void test_big_uint_divmod_with_rem() {
    BigUint first = big_uint_new_with_limbs(4, {18446744073709551615ULL, 18446744073709551615ULL, 1099511627775ULL, 0});
    BigUint second = big_uint_new_with_limbs(4, {2919980651337220095ULL, 14019525496019259228ULL, 10995116277ULL, 0});
    BigUint quot = big_uint_new_with_limbs(4, {0});
    BigUint rem = big_uint_new_with_limbs(4, {0});
    BigUint expected_quot = big_uint_new_with_limbs(4, {100, 0, 0, 0});
    BigUint expected_rem = big_uint_new_with_limbs(4, {3149840045630816355, 0, 0, 0});
    big_uint_divmod(first, second, &quot, &rem);

    assert_that(big_uint_cmp(quot, expected_quot) == 0);
    assert_that(big_uint_cmp(rem, expected_rem) == 0);
}

void test_big_uint_divmod_without_rem() {
    BigUint first = big_uint_new_with_limbs(4, {18446744073709551615ULL, 18446744073709551615ULL, 1099511627775ULL, 0});
    BigUint second = big_uint_new_with_limbs(4, {3, 0, 0, 0});
    BigUint quot = big_uint_new_with_limbs(4, {0});
    BigUint rem = big_uint_new_with_limbs(4, {0});
    BigUint expected_quot =
        big_uint_new_with_limbs(4, {6148914691236517205ULL, 6148914691236517205ULL, 366503875925ULL, 0});
    BigUint expected_rem = big_uint_new_with_limbs(4, {0, 0, 0, 0});
    big_uint_divmod(first, second, &quot, &rem);

    assert_that(big_uint_cmp(quot, expected_quot) == 0);
    assert_that(big_uint_cmp(rem, expected_rem) == 0);
}

void test_big_uint_from_string() {
    BigUint result = big_uint_new_with_limbs(4, {0});
    BigUint expected_result =
        big_uint_new_with_limbs(4, {18446744073709551615ULL, 18446744073709551615ULL, 1099511627775ULL, 0});
    big_uint_from_dec_string("374144419156711147060143317175368453031918731001855", &result);

    assert_that(big_uint_cmp(result, expected_result) == 0);
}

void test_big_uint_to_string() {
    BigUint number =
        big_uint_new_with_limbs(4, {18446744073709551615ULL, 18446744073709551615ULL, 1099511627775ULL, 0});
    char *result = big_uint_to_dec_string(number);
    char *expected_result = "374144419156711147060143317175368453031918731001855";

    assert_that(strcmp(result, expected_result) == 0);
    free(result);
}

void test_big_uint_from_u64() {
    BigUint result = big_uint_new_with_limbs(4, {0});
    big_uint_from_u64(9223372036854775808ULL, &result);
    BigUint expected_result = big_uint_new_with_limbs(4, {9223372036854775808ULL, 0, 0, 0});
    assert_that(big_uint_cmp(result, expected_result) == 0);
}

void test_big_uint_from_bytes_little_endian() {
    uint8_t bytes[32];
    memset(bytes, 0, 32);
    for (int i = 0; i < 21; i++) {
        bytes[i] = 255;
    }
    BigUint result = big_uint_new_with_limbs(4, {0});
    BigUint expected_result =
        big_uint_new_with_limbs(4, {18446744073709551615ULL, 18446744073709551615ULL, 1099511627775ULL, 0});
    big_uint_from_bytes_little_endian(bytes, &result);

    assert_that(big_uint_cmp(result, expected_result) == 0);
}

void test_big_uint_get_bytes_little_endian() {
    BigUint number = big_uint_new_with_limbs(4, {0});
    big_uint_from_dec_string("374144419156711147060143317175368453031918731001855", &number);

    uint8_t buffer[32];
    big_uint_get_bytes_little_endian(number, buffer);
    uint8_t expected_result[32] = {255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
                                   255, 255, 255, 255, 255, 0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0};

    for (int i = 0; i < 32; i++) {
        assert_that(expected_result[i] == buffer[i]);
    }
}

void test_big_uint_from_bytes_big_endian() {
    uint8_t bytes[32];
    memset(bytes, 0, 32);
    for (int i = 0; i < 21; i++) {
        bytes[i] = 255;
    }
    BigUint result = big_uint_new_with_limbs(4, {0});
    big_uint_from_bytes_big_endian(bytes, &result);
    BigUint expected_result =
        big_uint_new_with_limbs(4, {0, 18446744073692774400ULL, 18446744073709551615ULL, 18446744073709551615ULL});

    assert_that(big_uint_cmp(result, expected_result) == 0);
}

void test_big_uint_get_bytes_big_endian() {
    BigUint number = big_uint_new(4);
    big_uint_from_dec_string("374144419156711147060143317175368453031918731001855", &number);
    uint8_t buffer[32];
    big_uint_get_bytes_big_endian(number, buffer);
    uint8_t expected_result[32] = {0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   255, 255, 255, 255, 255,
                                   255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255};

    for (int i = 0; i < 32; i++) {
        assert_that(expected_result[i] == buffer[i]);
    }
}

int main() {
    BEGIN_TEST();
    test(test_big_uint_overflow_add);
    test(test_big_uint_overflow_add_with_overflow);
    test(test_big_uint_overflow_sub);
    test(test_big_uint_overflow_sub_with_overflow);
    test(test_big_uint_overflow_mul);
    test(test_big_uint_overflow_mul_with_overflow);
    test(test_big_uint_bitand);
    test(test_big_uint_bitor);
    test(test_big_uint_bitxor);
    test(test_big_uint_bitnot);
    test(test_big_uint_shl);
    test(test_big_uint_shr);
    test(test_big_uint_divmod_with_rem);
    test(test_big_uint_divmod_without_rem);
    test(test_big_uint_from_string);
    test(test_big_uint_to_string);
    test(test_big_uint_from_u64);
    test(test_big_uint_from_bytes_little_endian);
    test(test_big_uint_get_bytes_little_endian);
    test(test_big_uint_from_bytes_big_endian);
    test(test_big_uint_get_bytes_big_endian);
    END_TEST();

    return 0;
}
