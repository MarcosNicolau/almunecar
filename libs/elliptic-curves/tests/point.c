
#include <elliptic-curves/point.h>
#include <elliptic-curves/secp256k1.h>
#include <math/random.h>
#include <primitive-types/biguint.h>
#include <utils/test.h>

void test_short_weierstrass_sum() {
    EllipticCurve secp256k1 = secp256k1();
    CurvePoint a = curve_point_new(secp256k1_points_size_in_bits, Affine, ShortWeierstrass);
    BigUint n_a = biguint_new(1);
    biguint_from_u64(2, &n_a);
    curve_point_from_generator(secp256k1, n_a, &a);

    CurvePoint b = curve_point_new(secp256k1_points_size_in_bits, Affine, ShortWeierstrass);
    BigUint n_b = biguint_new(1);
    biguint_from_u64(3, &n_b);
    curve_point_from_generator(secp256k1, n_b, &b);

    CurvePoint result = curve_point_new(secp256k1_points_size_in_bits, Affine, ShortWeierstrass);
    curve_point_sum(secp256k1, a, b, &result);

    BigUint expected_x = biguint_new(secp256k1_points_size_in_bits / 64);
    biguint_from_dec_string("", &expected_x);
    BigUint expected_y = biguint_new(secp256k1_points_size_in_bits / 64);
    biguint_from_dec_string("", &expected_y);

    assert_that(biguint_cmp(result.x, expected_x) == 0);
    assert_that(biguint_cmp(result.y, expected_y) == 0);
}

void test_short_weierstrass_sum_should_double_on_equal_points() {
    EllipticCurve secp256k1 = secp256k1();
    CurvePoint a = curve_point_new(secp256k1_points_size_in_bits, Affine, ShortWeierstrass);
    CurvePoint b = curve_point_new(secp256k1_points_size_in_bits, Affine, ShortWeierstrass);
    CurvePoint result = curve_point_new(secp256k1_points_size_in_bits, Affine, ShortWeierstrass);
    curve_point_sum(secp256k1, a, b, &result);

    BigUint expected_x = biguint_new(secp256k1_points_size_in_bits / 64);
    biguint_from_dec_string("", &expected_x);
    BigUint expected_y = biguint_new(secp256k1_points_size_in_bits / 64);
    biguint_from_dec_string("", &expected_y);

    assert_that(biguint_cmp(result.x, expected_x) == 0);
    assert_that(biguint_cmp(result.y, expected_y) == 0);
}

void test_short_weierstrass_sum_should_return_same_on_infinite_point() {
    EllipticCurve secp256k1 = secp256k1();
    CurvePoint a = curve_point_new(secp256k1_points_size_in_bits, Affine, ShortWeierstrass);
    CurvePoint b = curve_point_new(secp256k1_points_size_in_bits, Affine, ShortWeierstrass);
    CurvePoint result = curve_point_new(secp256k1_points_size_in_bits, Affine, ShortWeierstrass);
    curve_point_sum(secp256k1, a, b, &result);

    BigUint expected_x = biguint_new(secp256k1_points_size_in_bits / 64);
    biguint_from_dec_string("", &expected_x);
    BigUint expected_y = biguint_new(secp256k1_points_size_in_bits / 64);
    biguint_from_dec_string("", &expected_y);

    assert_that(biguint_cmp(result.x, expected_x) == 0);
    assert_that(biguint_cmp(result.y, expected_y) == 0);
}

int main() {
    BEGIN_TEST()
    test(test_short_weierstrass_sum);
    // test(test_short_weierstrass_sum_should_double_on_equal_points);
    // test(test_short_weierstrass_sum_should_return_same_on_infinite_point);
    END_TEST()

    return 0;
}
