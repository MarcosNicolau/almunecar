#include <short_weierstrass.h>

CurveOperationResult double_affine(EllipticCurve curve, CurvePoint a, CurvePoint *out) {
    if (a.infinity) {
        curve_point_cpy(out, a);
        return Ok(CurveOperationResult, {});
    }
    BigUint lambda = biguint_new_heap(curve.points_size_in_bits / 64);
    BigUint num = biguint_new_heap(curve.points_size_in_bits / 64);
    BigUint denom = biguint_new_heap(curve.points_size_in_bits / 64);

    BigUint scalar = biguint_new(1);

    biguint_from_u64(2, &scalar);
    biguint_pow_mod(a.x, scalar, curve.p, &num);
    biguint_from_u64(3, &scalar);
    biguint_mul(scalar, a.x, &num);
    biguint_add_mod(num, curve.a, curve.p, &num);

    biguint_from_u64(2, &scalar);
    biguint_mul_mod(a.y, scalar, curve.p, &denom);

    biguint_div(num, denom, &lambda);
    biguint_pow_mod(lambda, scalar, curve.p, &out->x);
    biguint_sub_mod(out->x, a.x, curve.p, &out->x);
    biguint_sub_mod(out->x, a.x, curve.p, &out->x);

    biguint_sub_mod(a.x, out->x, curve.p, &out->y);
    biguint_mul_mod(lambda, out->y, curve.p, &out->y);
    biguint_sub_mod(out->y, a.y, curve.p, &out->y);

    biguint_free(&lambda, &num, &denom);
}

CurveOperationResult sum_affine(EllipticCurve curve, CurvePoint a, CurvePoint b, CurvePoint *out) {
    if (a.infinity) {
        curve_point_cpy(out, b);
        return Ok(CurveOperationResult, {});
    }

    if (b.infinity) {
        curve_point_cpy(out, a);
        return Ok(CurveOperationResult, {});
    }

    if (curve_point_eq(a, b)) {
        return double_affine(curve, a, out);
    }

    // validations went through, now sum the points using the chord rule
    BigUint lambda = biguint_new_heap(curve.points_size_in_bits / 64);
    BigUint num = biguint_new_heap(curve.points_size_in_bits / 64);
    BigUint denom = biguint_new_heap(curve.points_size_in_bits / 64);

    biguint_sub_mod(b.y, a.y, curve.p, &num);
    biguint_sub_mod(b.x, a.x, curve.p, &denom);
    biguint_div(num, denom, &lambda);

    BigUint two = biguint_new(1);
    // TODO add comment explaining formula
    biguint_from_u64(2, &two);
    biguint_pow_mod(lambda, two, curve.p, &out->x);
    biguint_sub_mod(out->x, a.x, curve.p, &out->x);
    biguint_sub_mod(out->x, b.x, curve.p, &out->x);

    biguint_sub_mod(a.x, out->x, curve.p, &out->y);
    biguint_mul_mod(lambda, out->y, curve.p, &out->y);
    biguint_sub_mod(out->y, a.y, curve.p, &out->y);

    biguint_free(&lambda, &num, &denom);
}

CurveOperationResult double_projective(EllipticCurve curve, CurvePoint a, CurvePoint *out) {}

CurveOperationResult sum_projective(EllipticCurve curve, CurvePoint a, CurvePoint b, CurvePoint *out) {}

CurveOperationResult double_compressed(EllipticCurve curve, CurvePoint a, CurvePoint *out) {}

CurveOperationResult sum_compressed(EllipticCurve curve, CurvePoint a, CurvePoint b, CurvePoint *out) {}

CurveOperationResult short_weierstrass_point_sum(EllipticCurve curve, CurvePoint a, CurvePoint b, CurvePoint *out) {
    switch (a.coord) {
    case Affine:
        return sum_affine(curve, a, b, out);
    case Projective:
        return sum_projective(curve, a, b, out);
    case Compressed:
        return sum_compressed(curve, a, b, out);
    }
}

CurveOperationResult short_weierstrass_point_double(EllipticCurve curve, CurvePoint a, CurvePoint *out) {
    switch (a.coord) {
    case Affine:
        return double_affine(curve, a, out);
    case Projective:
        return double_projective(curve, a, out);
    case Compressed:
        return double_compressed(curve, a, out);
    }
}

CurveOperationResult short_weierstrass_point_inverse(EllipticCurve, CurvePoint, CurvePoint *out) {}

CurveOperationResult short_weierstrass_point_mul(EllipticCurve curve, CurvePoint a, BigUint n, CurvePoint *out) {
    // for now we are naively applying a sum n times
    // for a start, we could apply exponentiation by square to reduce it by quite a lot
    CurveOperationResult (*operation)(EllipticCurve, CurvePoint, CurvePoint, CurvePoint *out) = NULL;
    switch (a.coord) {
    case Affine:
        operation = sum_affine;
        break;
    case Projective:
        operation = sum_projective;
        break;
    case Compressed:
        operation = sum_compressed;
    };

    CurvePoint base = curve_point_new_heap(curve.points_size_in_bits, a.coord, a.expression);
    curve_point_cpy(&base, a);
    BigUint i = biguint_new_heap(curve.points_size_in_bits / 64);
    BigUint one = biguint_new(1);
    biguint_one(&one);

    for (; biguint_cmp(i, n) < 0; biguint_add(i, one, &i)) {
        CurveOperationResult result = operation(curve, a, base, &a);
        if (!result.success) {
            biguint_free(&i);
            return result;
        }
    }

    biguint_free(&i);
    curve_point_free(&base);
}
