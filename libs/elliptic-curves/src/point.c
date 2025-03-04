#include <point.h>
#include <short_weierstrass.h>

void curve_point_debug(CurvePoint a) {
    printf("X: ");
    biguint_println(a.x);
    printf("Y: ");
    biguint_println(a.y);
    printf("Z: ");
    biguint_println(a.z);
    printf("infinity: %d\n", a.infinity);
    printf("coord system: %d\n", a.coord);
    printf("expression: %d\n", a.expression);
    printf("sign: %d\n", a.sign);
};

void curve_point_generator(EllipticCurve curve, CurvePoint *out) {
    biguint_cpy(&out->x, curve.g_x);
    biguint_cpy(&out->y, curve.g_y);
    biguint_one(&out->z);
    out->coord = Affine;
    out->infinity = 0;
    out->expression = ShortWeierstrass;
}

void curve_point_from_generator(EllipticCurve curve, BigUint n, CurvePoint *out) {
    CurvePoint generator = curve_point_new_heap(curve.points_size_in_bits, Affine, ShortWeierstrass);
    curve_point_generator(curve, &generator);
    curve_point_mul(curve, generator, n, out);

    curve_point_free(&generator);
};

CurveOperationResult curve_point_sum(EllipticCurve curve, CurvePoint a, CurvePoint b, CurvePoint *out) {
    if (a.expression != b.expression)
        return Err(CurveOperationResult, CurvePointsExpressionMismatch);

    if (a.coord != b.coord)
        return Err(CurveOperationResult, CurvePointsCoordMismatch);

    // if (!curve_point_verify(curve, a) || !curve_point_verify(curve, b))
    //     return Err(CurveOperationResult, CurveInvalidPoint);

    switch (a.expression) {
    case ShortWeierstrass:
        return short_weierstrass_point_sum(curve, a, b, out);
    default:
        return Err(CurveOperationResult, CurveDoesNotSupportExpression);
    }
};

CurveOperationResult curve_point_mul(EllipticCurve curve, CurvePoint a, BigUint n, CurvePoint *out) {
    // if (!curve_point_verify(curve, a) || !curve_point_verify(curve, b))
    //     return Err(CurveOperationResult, CurveInvalidPoint);

    switch (a.expression) {
    case ShortWeierstrass:
        return short_weierstrass_point_mul(curve, a, n, out);
    default:
        return Err(CurveOperationResult, CurveDoesNotSupportExpression);
    }
};

void curve_point_cpy(CurvePoint *dst, CurvePoint src) {
    biguint_cpy(&dst->x, src.x);
    biguint_cpy(&dst->y, src.y);
    biguint_cpy(&dst->z, src.z);

    dst->coord = src.coord;
    dst->expression = src.expression;
    dst->sign = src.sign;
    dst->infinity = src.infinity;
}

int curve_point_eq(CurvePoint a, CurvePoint b) {
    if (a.coord != b.coord || a.expression != b.expression || a.infinity != b.infinity) {
        return 0;
    }

    if (biguint_cmp(a.x, b.x) != 0 || biguint_cmp(a.y, b.y) != 0 || biguint_cmp(a.z, b.z) != 0) {
        return 0;
    }

    return 1;
}
