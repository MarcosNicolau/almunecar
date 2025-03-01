#ifndef ELLIPTIC_CURVES_SECP256K1
#define ELLIPTIC_CURVES_SECP256K1

#include <primitive-types/u256.h>
#include <utils/types.h>

// Domain parameters
#define p u256_from_dec_string("115792089237316195423570985008687907853269984665640564039457584007908834671663")
#define a u256_from_dec_string("0")
#define b u256_from_dec_string("7")
#define G_x u256_from_dec_string("55066263022277343669578718895168534326250603453777594175500187360389116729240")
#define G_y u256_from_dec_string("32670510020758816978083085130507043184471273380659243275938904335757337482424")
#define n u256_from_dec_string("115792089237316195423570985008687907852837564279074904382605163141518161494337")
#define h u256_from_dec_string("1")

typedef enum {
    Affine,
    Projective,
    Compressed,
} CoordSystem;

typedef struct {
    u256 x;
    u256 y;
    u256 z;
    int infinity; // whether it represents the point at infinity (1 if it does, 0 otherwise)
    CoordSystem coord;
} secp256k1_Point;

typedef enum { CoordMismatch } CurveOperationError;

DEFINE_RESULT(secp256k1_Point, CurveOperationError, secp256k1_CurveOperationResult);

// Returns a new secp256k1_Point in the given coordinate system
// Both points must be in the same coordinates
secp256k1_CurveOperationResult secp256k1_sum(secp256k1_Point, secp256k1_Point);
secp256k1_CurveOperationResult secp256k1_double(secp256k1_Point);
secp256k1_CurveOperationResult secp256k1_sub(secp256k1_Point, secp256k1_Point);
secp256k1_CurveOperationResult secp256k1_mul(secp256k1_Point, secp256k1_Point);

void secp256k_to_affine(secp256k1_Point *);
void secp256k_to_compressed(secp256k1_Point *);
void secp256k_to_projective(secp256k1_Point *);

#endif
