#ifndef ELLIPTIC_CURVES_SHORT_WEIERSTRASS
#define ELLIPTIC_CURVES_SHORT_WEIERSTRASS

#include "point.h"

CurveOperationResult short_weierstrass_point_sum(EllipticCurve, CurvePoint, CurvePoint, CurvePoint *out);
CurveOperationResult short_weierstrass_point_double(EllipticCurve, CurvePoint, CurvePoint *out);
CurveOperationResult short_weierstrass_point_mul(EllipticCurve, CurvePoint, BigUint n, CurvePoint *out);
CurveOperationResult short_weierstrass_point_inverse(EllipticCurve, CurvePoint, CurvePoint *out);

#endif
