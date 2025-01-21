#include <primitive-types/biguint.h>
#include <string.h>
#include <utils/test.h>

void test_biguint_overflow_add() {
    BigUint first = biguint_new_with_limbs(4, {18446744073709551615ULL, 18446744073709551615ULL, 1099511627775ULL, 0});
    BigUint second = biguint_new_with_limbs(4, {18446744073709551615ULL, 18446744073709551615ULL, 1099511627775ULL, 0});
    BigUint expected_result =
        biguint_new_with_limbs(4, {18446744073709551614ULL, 18446744073709551615ULL, 2199023255551ULL, 0});

    int overflow = biguint_overflow_add(&first, second);

    assert_that(biguint_cmp(first, expected_result) == 0);
    assert_that(overflow == 0);
}

void test_biguint_overflow_add_with_overflow() {
    BigUint first = biguint_new_with_limbs(4, {UINT64_MAX, UINT64_MAX, UINT64_MAX, UINT64_MAX});
    BigUint second = biguint_new_with_limbs(4, {UINT64_MAX, UINT64_MAX, UINT64_MAX, UINT64_MAX});
    BigUint expected_result = biguint_new_with_limbs(4, {UINT64_MAX - 1, UINT64_MAX, UINT64_MAX, UINT64_MAX});

    int overflow = biguint_overflow_add(&first, second);

    assert_that(biguint_cmp(first, expected_result) == 0);
    assert_that(overflow == 1);
}

void test_biguint_overflow_sub() {
    BigUint first = biguint_new_with_limbs(4, {18446744073709551615ULL, 18446744073709551615ULL, 1099511627775ULL, 0});
    BigUint second = biguint_new_with_limbs(4, {2919980651337220095ULL, 14019525496019259228ULL, 10995116277ULL, 0});
    BigUint expected_result =
        biguint_new_with_limbs(4, {15526763422372331520ULL, 4427218577690292387ULL, 1088516511498ULL, 0});

    int overflow = biguint_overflow_sub(&first, second);

    assert_that(biguint_cmp(first, expected_result) == 0);
    assert_that(overflow == 0);
}

void test_biguint_overflow_sub_with_overflow() {
    BigUint first = biguint_new_with_limbs(4, {0, 0, 0, 0});
    BigUint second = biguint_new_with_limbs(4, {1, 1, 1, 1});
    BigUint expected_result = biguint_new_with_limbs(4, {UINT64_MAX, UINT64_MAX - 1, UINT64_MAX - 1, UINT64_MAX - 1});
    int overflow = biguint_overflow_sub(&first, second);

    assert_that(biguint_cmp(first, expected_result) == 0);
    assert_that(overflow == 1);
}

void test_biguint_overflow_mul() {
    BigUint first = biguint_new_with_limbs(4, {18446744073709551615ULL, 0, 0, 0});
    BigUint second = biguint_new_with_limbs(4, {2919980651337220095ULL, 0, 0, 0});
    BigUint expected_result = biguint_new_with_limbs(4, {15526763422372331521ULL, 2919980651337220094ULL, 0, 0});
    int overflow = biguint_overflow_mul(&first, second);

    assert_that(biguint_cmp(first, expected_result) == 0);
    assert_that(overflow == 0);
}

void test_biguint_overflow_mul_with_overflow() {
    BigUint first = biguint_new_with_limbs(4, {18446744073709551615ULL, 18446744073709551615ULL, 1099511627775ULL, 0});
    BigUint second = biguint_new_with_limbs(4, {2919980651337220095ULL, 14019525496019259228ULL, 10995116277ULL, 0});
    BigUint expected_result = biguint_new_with_limbs(
        4, {15526763422372331521ULL, 4427218577690292387ULL, 17870282210899384074ULL, 14019525494141808257ULL});
    int overflow = biguint_overflow_mul(&first, second);

    assert_that(biguint_cmp(first, expected_result) == 0);
    assert_that(overflow == 1);
}

void test_biguint_bitand() {
    BigUint first =
        biguint_new_with_limbs(4, {18446744073709551615ULL, 18446744073709551615ULL, 1099511627775ULL, 1ULL});
    BigUint second = biguint_new_with_limbs(4, {2919980651337220095ULL, 14019525496019259228ULL, 10995116277ULL, 0});
    BigUint expected_result =
        biguint_new_with_limbs(4, {2919980651337220095ULL, 14019525496019259228ULL, 10995116277ULL, 0});
    biguint_bitand(&first, second);

    assert_that(biguint_cmp(first, expected_result) == 0);
}

void test_biguint_bitor() {
    BigUint first = biguint_new_with_limbs(4, {18446744073709551615ULL, 18446744073709551615ULL, 1099511627775ULL, 0});
    BigUint second = biguint_new_with_limbs(4, {2919980651337220095ULL, 14019525496019259228ULL, 10995116277ULL, 1ULL});
    BigUint expected_result =
        biguint_new_with_limbs(4, {18446744073709551615ULL, 18446744073709551615ULL, 1099511627775ULL, 1});
    biguint_bitor(&first, second);

    assert_that(biguint_cmp(first, expected_result) == 0);
}

void test_biguint_bitxor() {
    BigUint first = biguint_new_with_limbs(4, {18446744073709551615ULL, 18446744073709551615ULL, 1099511627775ULL, 1});
    BigUint second = biguint_new_with_limbs(4, {2919980651337220095ULL, 14019525496019259228ULL, 10995116277ULL, 1});
    BigUint expected_result =
        biguint_new_with_limbs(4, {15526763422372331520ULL, 4427218577690292387ULL, 1088516511498ULL, 0});
    biguint_bitxor(&first, second);

    assert_that(biguint_cmp(first, expected_result) == 0);
}

void test_biguint_bitnot() {
    BigUint first = biguint_new_with_limbs(4, {18446744073709551615ULL, 18446744073709551615ULL, 1099511627775ULL, 0});
    BigUint expected_result = biguint_new_with_limbs(4, {0, 0, 18446742974197923840ULL, 18446744073709551615ULL});
    biguint_bitnot(&first);

    assert_that(biguint_cmp(first, expected_result) == 0);
}

void test_biguint_shl() {
    BigUint first = biguint_new_with_limbs(4, {18446744073709551615ULL, 18446744073709551615ULL, 1099511627775ULL, 0});
    int shift = 130;
    BigUint expected_result = biguint_new_with_limbs(4, {0, 0, 18446744073709551612ULL, 18446744073709551615ULL});
    biguint_shl(&first, shift);

    assert_that(biguint_cmp(first, expected_result) == 0);
}

void test_biguint_shr() {
    BigUint first = biguint_new_with_limbs(4, {18446744073709551615ULL, 18446744073709551615ULL, 1099511627775ULL, 0});
    int shift = 130;
    BigUint expected_result = biguint_new_with_limbs(4, {274877906943, 0, 0, 0});
    biguint_shr(&first, shift);

    assert_that(biguint_cmp(first, expected_result) == 0);
}

void test_biguint_divmod_with_rem() {
    BigUint first = biguint_new_with_limbs(4, {18446744073709551615ULL, 18446744073709551615ULL, 1099511627775ULL, 0});
    BigUint second = biguint_new_with_limbs(4, {2919980651337220095ULL, 14019525496019259228ULL, 10995116277ULL, 0});
    BigUint quot = biguint_new_with_limbs(4, {0});
    BigUint rem = biguint_new_with_limbs(4, {0});
    BigUint expected_quot = biguint_new_with_limbs(4, {100, 0, 0, 0});
    BigUint expected_rem = biguint_new_with_limbs(4, {3149840045630816355, 0, 0, 0});
    biguint_divmod(first, second, &quot, &rem);

    assert_that(biguint_cmp(quot, expected_quot) == 0);
    assert_that(biguint_cmp(rem, expected_rem) == 0);
}

void test_biguint_divmod_without_rem() {
    BigUint first = biguint_new_with_limbs(4, {18446744073709551615ULL, 18446744073709551615ULL, 1099511627775ULL, 0});
    BigUint second = biguint_new_with_limbs(4, {3, 0, 0, 0});
    BigUint quot = biguint_new_with_limbs(4, {0});
    BigUint rem = biguint_new_with_limbs(4, {0});
    BigUint expected_quot =
        biguint_new_with_limbs(4, {6148914691236517205ULL, 6148914691236517205ULL, 366503875925ULL, 0});
    BigUint expected_rem = biguint_new_with_limbs(4, {0, 0, 0, 0});
    biguint_divmod(first, second, &quot, &rem);

    assert_that(biguint_cmp(quot, expected_quot) == 0);
    assert_that(biguint_cmp(rem, expected_rem) == 0);
}

void test_biguint_from_string() {
    BigUint result = biguint_new_with_limbs(4, {0});
    BigUint expected_result =
        biguint_new_with_limbs(4, {18446744073709551615ULL, 18446744073709551615ULL, 1099511627775ULL, 0});
    biguint_from_dec_string("374144419156711147060143317175368453031918731001855", &result);

    assert_that(biguint_cmp(result, expected_result) == 0);
}

void test_biguint_to_string() {
    BigUint number = biguint_new_with_limbs(4, {18446744073709551615ULL, 18446744073709551615ULL, 1099511627775ULL, 0});
    char *result = biguint_to_dec_string(number);
    char *expected_result = "374144419156711147060143317175368453031918731001855";

    assert_that(strcmp(result, expected_result) == 0);
    free(result);
}

void test_biguint_from_u64() {
    BigUint result = biguint_new_with_limbs(4, {0});
    biguint_from_u64(9223372036854775808ULL, &result);
    BigUint expected_result = biguint_new_with_limbs(4, {9223372036854775808ULL, 0, 0, 0});
    assert_that(biguint_cmp(result, expected_result) == 0);
}

void test_biguint_from_bytes_little_endian() {
    uint8_t bytes[32];
    memset(bytes, 0, 32);
    for (int i = 0; i < 21; i++) {
        bytes[i] = 255;
    }
    BigUint result = biguint_new_with_limbs(4, {0});
    BigUint expected_result =
        biguint_new_with_limbs(4, {18446744073709551615ULL, 18446744073709551615ULL, 1099511627775ULL, 0});
    biguint_from_bytes_little_endian(bytes, &result);

    assert_that(biguint_cmp(result, expected_result) == 0);
}

void test_biguint_get_bytes_little_endian() {
    BigUint number = biguint_new_with_limbs(4, {0});
    biguint_from_dec_string("374144419156711147060143317175368453031918731001855", &number);

    uint8_t buffer[32];
    biguint_get_bytes_little_endian(number, buffer);
    uint8_t expected_result[32] = {255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
                                   255, 255, 255, 255, 255, 0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0};

    for (int i = 0; i < 32; i++) {
        assert_that(expected_result[i] == buffer[i]);
    }
}

void test_biguint_from_bytes_big_endian() {
    uint8_t bytes[32];
    memset(bytes, 0, 32);
    for (int i = 0; i < 21; i++) {
        bytes[i] = 255;
    }
    BigUint result = biguint_new_with_limbs(4, {0});
    biguint_from_bytes_big_endian(bytes, &result);
    BigUint expected_result =
        biguint_new_with_limbs(4, {0, 18446744073692774400ULL, 18446744073709551615ULL, 18446744073709551615ULL});

    assert_that(biguint_cmp(result, expected_result) == 0);
}

void test_biguint_get_bytes_big_endian() {
    BigUint number = biguint_new(4);
    biguint_from_dec_string("374144419156711147060143317175368453031918731001855", &number);
    uint8_t buffer[32];
    biguint_get_bytes_big_endian(number, buffer);
    uint8_t expected_result[32] = {0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   255, 255, 255, 255, 255,
                                   255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255};

    for (int i = 0; i < 32; i++) {
        assert_that(expected_result[i] == buffer[i]);
    }
}

int main() {
    BEGIN_TEST();
    test(test_biguint_overflow_add);
    test(test_biguint_overflow_add_with_overflow);
    test(test_biguint_overflow_sub);
    test(test_biguint_overflow_sub_with_overflow);
    test(test_biguint_overflow_mul);
    test(test_biguint_overflow_mul_with_overflow);
    test(test_biguint_bitand);
    test(test_biguint_bitor);
    test(test_biguint_bitxor);
    test(test_biguint_bitnot);
    test(test_biguint_shl);
    test(test_biguint_shr);
    test(test_biguint_divmod_with_rem);
    test(test_biguint_divmod_without_rem);
    test(test_biguint_from_string);
    test(test_biguint_to_string);
    test(test_biguint_from_u64);
    test(test_biguint_from_bytes_little_endian);
    test(test_biguint_get_bytes_little_endian);
    test(test_biguint_from_bytes_big_endian);
    test(test_biguint_get_bytes_big_endian);
    END_TEST();

    return 0;
}
