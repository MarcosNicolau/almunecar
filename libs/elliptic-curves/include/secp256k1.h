#ifndef ELLIPTIC_CURVES_SECP256K1
#define ELLIPTIC_CURVES_SECP256K1

#include "curve.h"

// ignoring unused variables
// as domain params are used in the secp256k1 macro
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"

static uint64_t p[4] = {
    18446744069414583343ULL, 18446744073709551615ULL, 18446744073709551615ULL,
    18446744073709551615ULL};        // 115792089237316195423570985008687907853269984665640564039457584007908834671663
static uint64_t a[4] = {0, 0, 0, 0}; // 0
static uint64_t b[4] = {7, 0, 0, 0}; // 7
static uint64_t g_x[4] = {
    6481385041966929816ULL, 188021827762530521ULL, 6170039885052185351ULL,
    8772561819708210092ULL}; // 55066263022277343669578718895168534326250603453777594175500187360389116729240
static uint64_t g_y[4] = {
    11261198710074299576ULL, 18237243440184513561ULL, 6747795201694173352ULL,
    5204712524664259685ULL};         // 32670510020758816978083085130507043184471273380659243275938904335757337482424
static uint64_t h[4] = {1, 0, 0, 0}; // 1
static uint64_t n[4] = {
    13822214165235122497ULL, 13451932020343611451ULL, 18446744073709551614ULL,
    18446744073709551615ULL}; // 115792089237316195423570985008687907852837564279074904382605163141518161494337

#pragma GCC diagnostic pop

#define secp256k1()                                                                                                    \
    (EllipticCurve){.p = biguint_new_from_limbs(4, p),                                                                 \
                    .a = biguint_new_from_limbs(4, a),                                                                 \
                    .b = biguint_new_from_limbs(4, b),                                                                 \
                    .g_x = biguint_new_from_limbs(4, g_x),                                                             \
                    .g_y = biguint_new_from_limbs(4, g_y),                                                             \
                    .h = biguint_new_from_limbs(4, h),                                                                 \
                    .n = biguint_new_from_limbs(4, n),                                                                 \
                    .supports_edward_form = 0,                                                                         \
                    .supports_montgomery_form = 0,                                                                     \
                    .default_expression = ShortWeierstrass};

#endif
