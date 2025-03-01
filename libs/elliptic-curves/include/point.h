#ifndef ELLIPTIC_CURVES_POINT
#define ELLIPTIC_CURVES_POINT

#include "curve.h"
#include <primitive-types/biguint.h>
#include <utils/types.h>

typedef enum {
    Affine,
    Projective,
    Compressed,
} CurvePointCoordSystem;

typedef enum { LowerHalf, UpperHalf } PointSign;

typedef struct {
    BigUint x;
    BigUint y;
    BigUint z;
    int infinity; // whether it represents the point at infinity (1 if it does, 0 otherwise)
    PointSign sign;
    CurvePointCoordSystem coord;
    CurveExpression expression;
} CurvePoint;

/**
 * Create a new point on the coord system and expresion
 */
#define coord_point_new(BIT_SIZE, COORD, EXP)                                                                          \
    (CurvePoint){                                                                                                      \
        .x = biguint_new(BIT_SIZE / 64),                                                                               \
        .y = biguint_new(BIT_SIZE / 64),                                                                               \
        .z = biguint_new(BIT_SIZE / 64),                                                                               \
        .infinity = 1,                                                                                                 \
        .sign = LowerHalf,                                                                                             \
        .coord = COORD,                                                                                                \
        .expression = EXP,                                                                                             \
    };

typedef enum {
    CurvePointsCoordMismatch,
    CurvePointsCurveExpressionMismatch,
    CurveDoesSupportExpression,
    CurveInvalidPoint
} CurveOperationError;

DEFINE_RESULT(struct {}, CurveOperationError, CurveOperationResult);

/**
 * Creates a new point on the given coordinate system.
 * The points will belong to the curve default expression
 */
void curve_point_from_affine(EllipticCurve curve, BigUint x, BigUint y, CurvePoint *);
void curve_point_from_projective(EllipticCurve curve, BigUint x, BigUint y, BigUint z, CurvePoint *);
void curve_point_from_compressed(EllipticCurve curve, BigUint x, CurvePoint *);

CurveOperationResult curve_point_sum(EllipticCurve, CurvePoint, CurvePoint, CurvePoint *out);
CurveOperationResult curve_point_sub(EllipticCurve, CurvePoint, CurvePoint, CurvePoint *out);
CurveOperationResult curve_point_mul(EllipticCurve, CurvePoint, CurvePoint, CurvePoint *out);
CurveOperationResult curve_point_double(EllipticCurve, CurvePoint, CurvePoint *out);
CurveOperationResult curve_point_inverse(EllipticCurve, CurvePoint, CurvePoint *out);

void curve_point_to_affine(CurvePoint *);
void curve_point_to_compressed(CurvePoint *);
void curve_point_to_projective(CurvePoint *);

void curve_point_to_short_weierstrass(CurvePoint *);
void curve_point_to_montgomery(CurvePoint *);
void curve_point_to_edwards(CurvePoint *);

#endif
