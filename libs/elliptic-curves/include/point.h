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
    int infinity;   // whether it represents the point at infinity (1 if it does, 0 otherwise)
    PointSign sign; // in compressed form, the point sign to reconstruct the y coordinate
    CurvePointCoordSystem coord;
    CurveExpression expression;
} CurvePoint;

/**
 * Create a new point on the coord system and expresion
 */
#define curve_point_new(BIT_SIZE, COORD, EXP)                                                                          \
    (CurvePoint){                                                                                                      \
        .x = biguint_new(BIT_SIZE / 64),                                                                               \
        .y = biguint_new(BIT_SIZE / 64),                                                                               \
        .z = biguint_new(BIT_SIZE / 64),                                                                               \
        .infinity = 1,                                                                                                 \
        .sign = LowerHalf,                                                                                             \
        .coord = COORD,                                                                                                \
        .expression = EXP,                                                                                             \
    };

#define curve_point_new_heap(BIT_SIZE, COORD, EXP)                                                                     \
    (CurvePoint){                                                                                                      \
        .x = biguint_new_heap(BIT_SIZE / 64),                                                                          \
        .y = biguint_new_heap(BIT_SIZE / 64),                                                                          \
        .z = biguint_new_heap(BIT_SIZE / 64),                                                                          \
        .infinity = 1,                                                                                                 \
        .sign = LowerHalf,                                                                                             \
        .coord = COORD,                                                                                                \
        .expression = EXP,                                                                                             \
    };

#define curve_point_free(...)                                                                                          \
    CurvePoint *ANONYMOUS_VARIABLE(points)[] = {__VA_ARGS__};                                                          \
    for (size_t i = 0; i < sizeof(ANONYMOUS_VARIABLE(points)) / sizeof(ANONYMOUS_VARIABLE(points)[0]); i++) {          \
        biguint_free(&ANONYMOUS_VARIABLE(points)[i]->x, &ANONYMOUS_VARIABLE(points)[i]->y,                             \
                     &ANONYMOUS_VARIABLE(points)[i]->z)                                                                \
    }

typedef enum {
    CurvePointsCoordMismatch,
    CurvePointsExpressionMismatch,
    CurveDoesNotSupportExpression,
    CurveInvalidPoint
} CurveOperationError;

DEFINE_RESULT(struct {}, CurveOperationError, CurveOperationResult);

/**
 * Creates a new point on the given coordinate system.
 * The points will belong to the curve default expression
 */
void curve_point_from_affine(EllipticCurve curve, CurveExpression exp, BigUint x, BigUint y, CurvePoint *);
void curve_point_from_projective(EllipticCurve curve, CurveExpression exp, BigUint x, BigUint y, BigUint z,
                                 CurvePoint *);
void curve_point_from_compressed(EllipticCurve curve, CurveExpression exp, BigUint x, CurvePoint *);
void curve_point_from_generator(EllipticCurve curve, BigUint n, CurvePoint *);

CurveOperationResult curve_point_sum(EllipticCurve, CurvePoint, CurvePoint, CurvePoint *out);
CurveOperationResult curve_point_mul(EllipticCurve, CurvePoint, BigUint n, CurvePoint *out);
CurveOperationResult curve_point_inverse(EllipticCurve, CurvePoint, CurvePoint *out);
CurveOperationResult curve_point_double(EllipticCurve, CurvePoint, CurvePoint *out);

void curve_point_to_affine(CurvePoint *);
void curve_point_to_compressed(CurvePoint *);
void curve_point_to_projective(CurvePoint *);

void curve_point_to_short_weierstrass(CurvePoint *);
void curve_point_to_montgomery(CurvePoint *);
void curve_point_to_edwards(CurvePoint *);
void curve_point_cpy(CurvePoint *dst, CurvePoint src);
/**
 * Checks for equality
 * 1: if equal
 * 0: if not
 */
int curve_point_eq(CurvePoint a, CurvePoint b);

/**
 * Verifies the point belongs to the curve by computing the expression definition
 */
int curve_point_verify(EllipticCurve curve, CurvePoint a);

// prints the curve point
void curve_point_debug(CurvePoint a);

void curve_point_generator(EllipticCurve curve, CurvePoint *out);

#endif
