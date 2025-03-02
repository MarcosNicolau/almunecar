#ifndef ELLIPTIC_CURVES
#define ELLIPTIC_CURVES

#include <primitive-types/biguint.h>
#include <stdint.h>

typedef enum { ShortWeierstrass, Montgomery, Edwards } CurveExpression;

// we are only accounting for the prime case when defining the elliptic curve
// NOTE: when defining a curve, it is expected that all parameters are of the same size
typedef struct {
    BigUint p;
    BigUint a;
    BigUint b;
    BigUint g_x;
    BigUint g_y;
    BigUint n;
    BigUint h;
    int supports_montgomery_form; // whether the curve can be written in montgomery form
    int supports_edward_form;     // whether the curve can be written in edward form
    CurveExpression default_expression;
} EllipticCurve;

#endif
