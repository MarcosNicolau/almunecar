#include <assert.h>
#include <biguint.h>
#include <string.h>

int get_min_size(BigUint a, BigUint b) {
    if (a.size < b.size)
        return a.size;
    else
        return b.size;
}

int get_min_size_three(BigUint a, BigUint b, BigUint c) {
    int limit;
    if (a.size < b.size)

        limit = a.size;
    else
        limit = b.size;

    if (c.size < limit)
        limit = c.size;

    return limit;
}

void biguint_free_limbs(BigUint *a) {
    if (!a)
        return;

    free(a->limbs);
}

/**
 * Conversion and initializers
 */
void biguint_zero(BigUint *out) {
    for (int i = 0; i < out->size; i++)
        out->limbs[i] = 0;
}

void biguint_one(BigUint *out) {
    biguint_zero(out);
    out->limbs[0] = 1;
}

void biguint_from_u64(uint64_t a, BigUint *out) {
    biguint_zero(out);
    out->limbs[0] = a;
}

void biguint_cpy(BigUint *dst, BigUint src) {
    int limit = get_min_size(*dst, src);

    for (int i = 0; i < dst->size; i++) {
        if (i < limit) {
            dst->limbs[i] = src.limbs[i];
        } else {
            dst->limbs[i] = 0;
        }
    }
}

void biguint_from_dec_string(char *str, BigUint *out) {
    biguint_zero(out);

    BigUint ten = biguint_new_heap(out->size);
    biguint_from_u64(10, &ten);
    BigUint digit_big = biguint_new_heap(out->size);

    int len = strlen(str);
    for (int i = 0; i < len; i++) {
        uint64_t digit = str[i] - '0';
        biguint_from_u64(digit, &digit_big);
        biguint_mul(*out, ten, out);
        biguint_add(*out, digit_big, out);
    }

    biguint_free(&ten, &digit_big);
};

void biguint_from_bytes_big_endian(uint8_t *bytes, BigUint *out) {
    biguint_zero(out);
    for (int i = out->size - 1, j = 0; i >= 0; i--, j++) {
        out->limbs[i] = ((uint64_t)bytes[j * 8] << 56) | ((uint64_t)bytes[j * 8 + 1] << 48) |
                        ((uint64_t)bytes[j * 8 + 2] << 40) | ((uint64_t)bytes[j * 8 + 3] << 32) |
                        ((uint64_t)bytes[j * 8 + 4] << 24) | ((uint64_t)bytes[j * 8 + 5] << 16) |
                        ((uint64_t)bytes[j * 8 + 6] << 8) | ((uint64_t)bytes[j * 8 + 7]);
    }
};

void biguint_get_bytes_big_endian(BigUint value, uint8_t *buffer) {
    for (int i = value.size - 1, j = 0; i >= 0; i--, j++) {
        buffer[i * 8] = (value.limbs[j] >> 56) & 0xFF;
        buffer[i * 8 + 1] = (value.limbs[j] >> 48) & 0xFF;
        buffer[i * 8 + 2] = (value.limbs[j] >> 40) & 0xFF;
        buffer[i * 8 + 3] = (value.limbs[j] >> 32) & 0xFF;
        buffer[i * 8 + 4] = (value.limbs[j] >> 24) & 0xFF;
        buffer[i * 8 + 5] = (value.limbs[j] >> 16) & 0xFF;
        buffer[i * 8 + 6] = (value.limbs[j] >> 8) & 0xFF;
        buffer[i * 8 + 7] = value.limbs[j] & 0xFF;
    }
};

void biguint_from_bytes_little_endian(uint8_t *bytes, BigUint *out) {
    biguint_zero(out);
    for (int i = 0; i < out->size; i++) {
        out->limbs[i] = ((uint64_t)bytes[i * 8]) | ((uint64_t)bytes[i * 8 + 1] << 8) |
                        ((uint64_t)bytes[i * 8 + 2] << 16) | ((uint64_t)bytes[i * 8 + 3] << 24) |
                        ((uint64_t)bytes[i * 8 + 4] << 32) | ((uint64_t)bytes[i * 8 + 5] << 40) |
                        ((uint64_t)bytes[i * 8 + 6] << 48) | ((uint64_t)bytes[i * 8 + 7] << 56);
    }
};

void biguint_get_bytes_little_endian(BigUint value, uint8_t *buffer) {
    for (int i = 0; i < value.size; i++) {
        buffer[i * 8] = value.limbs[i] & 0xFF;
        buffer[i * 8 + 1] = (value.limbs[i] >> 8) & 0xFF;
        buffer[i * 8 + 2] = (value.limbs[i] >> 16) & 0xFF;
        buffer[i * 8 + 3] = (value.limbs[i] >> 24) & 0xFF;
        buffer[i * 8 + 4] = (value.limbs[i] >> 32) & 0xFF;
        buffer[i * 8 + 5] = (value.limbs[i] >> 40) & 0xFF;
        buffer[i * 8 + 6] = (value.limbs[i] >> 48) & 0xFF;
        buffer[i * 8 + 7] = (value.limbs[i] >> 56) & 0xFF;
    }
};

char *biguint_to_dec_string(BigUint a) {
    char *result = malloc(a.size * 20 + 1); /* multiply by 20, since each part can
                                                 take as much as 20 bytes*/
    int i = a.size * 20 - 1;
    BigUint ten = biguint_new_heap(a.size);
    biguint_from_u64(10, &ten);

    BigUint dividend = biguint_new_heap(a.size);
    biguint_cpy(&dividend, a);
    BigUint quot = biguint_new_heap(dividend.size);
    BigUint rem = biguint_new_heap(dividend.size);

    while (1) {
        biguint_divmod(dividend, ten, &quot, &rem);
        int digit = rem.limbs[0] + '0';
        result[i] = digit;
        biguint_cpy(&dividend, quot);
        if (biguint_is_zero(dividend))
            break;
        i -= 1;
    }

    char *dst = malloc(a.size * 20 + 1 - i);
    int k = 0;
    for (int j = i; j < a.size * 20 + 1; j++) {
        dst[k] = result[i + k];
        k++;
    }
    dst[k - 1] = '\0';

    biguint_free(&ten, &dividend, &quot, &rem);
    free(result);
    return dst;
};

/**
 * Utils
 */
int biguint_bits(BigUint a) {
    for (int i = 1; i < a.size; i++) {
        if (a.limbs[a.size - i] > 0) {
            return 64 * (a.size - i + 1) - u64_leading_zeros(a.limbs[a.size - i]);
        }
    }
    return 64 - u64_leading_zeros(a.limbs[0]);
}

int biguint_cmp(BigUint a, BigUint b) {
    int limit = get_min_size(a, b);
    for (int i = limit - 1; i >= 0; i--) {
        uint64_t a_i = a.limbs[i];
        uint64_t b_i = b.limbs[i];
        if (a_i < b_i)
            return -1;
        else if (a_i > b_i)
            return 1;
    }
    return 0;
}

int biguint_is_zero(BigUint a) {
    for (int i = 0; i < a.size; i++) {
        if (a.limbs[i] != 0)
            return 0;
    }
    return 1;
};

/**
 * Operations
 */
int biguint_overflow_add(BigUint a, BigUint b, BigUint *out) {
    uint64_t carry = 0;
    int limit = get_min_size_three(a, b, *out);

    for (int i = 0; i < limit; i++) {
        u64_overflow_op addition = u64_overflow_add(a.limbs[i], b.limbs[i]);
        u64_overflow_op carry_addition = u64_overflow_add(addition.res, carry);
        out->limbs[i] = carry_addition.res;
        carry = addition.overflow + carry_addition.overflow;
    }
    return carry > 0;
};

void biguint_add(BigUint a, BigUint b, BigUint *out) { biguint_overflow_add(a, b, out); }

void biguint_add_mod(BigUint a, BigUint b, BigUint m, BigUint *out) {
    biguint_add(a, b, out);
    biguint_mod(*out, m, out);
}

int biguint_overflow_sub(BigUint a, BigUint b, BigUint *out) {
    uint64_t carry = 0;
    int limit = get_min_size_three(a, b, *out);

    for (int i = 0; i < limit; i++) {
        u64_overflow_op sub = u64_overflow_sub(a.limbs[i], b.limbs[i]);
        u64_overflow_op carry_sub = u64_overflow_sub(sub.res, carry);
        out->limbs[i] = carry_sub.res;
        carry = sub.overflow + carry_sub.overflow;
    }
    return carry > 0;
};

void biguint_sub(BigUint a, BigUint b, BigUint *out) { biguint_overflow_sub(a, b, out); }

void biguint_sub_mod(BigUint a, BigUint b, BigUint m, BigUint *out) {
    biguint_sub(a, b, out);
    biguint_mod(*out, m, out);
}

int biguint_overflow_mul(BigUint a, BigUint b, BigUint *out) {
    int limit = get_min_size_three(a, b, *out);
    uint64_t result[limit * 2];
    for (int i = 0; i < limit * 2; i++)
        result[i] = 0;
    for (int i = 0; i < limit; i++) {
        uint64_t carry = 0;
        for (int j = 0; j < limit; j++) { /* calculate result */
            u64_mul_op mul = u64_mul(a.limbs[j], b.limbs[i]);
            uint64_t current = result[i + j];
            u64_overflow_op addition = u64_overflow_add(mul.res, current);
            result[i + j] = addition.res; /* calculate carry */
            uint64_t carry_current = result[i + j + 1];
            u64_overflow_op carry_addition = u64_overflow_add(mul.carry + addition.overflow, carry);
            u64_overflow_op current_carry_addition = u64_overflow_add(carry_addition.res, carry_current);
            result[i + j + 1] = current_carry_addition.res;
            carry = carry_addition.overflow | current_carry_addition.overflow;
        }
    }
    int overflow = 0;
    if (out->size < limit * 2) {
        for (int i = 4; i < limit * 2 && out->size < limit * 2; i++) {
            if (result[i] != 0) {
                overflow = 1;
                break;
            }
        }
    }
    for (int i = 0; i < out->size; i++)
        out->limbs[i] = result[i];

    return overflow;
};

void biguint_mul(BigUint a, BigUint b, BigUint *out) { biguint_overflow_mul(a, b, out); }

void biguint_mul_mod(BigUint a, BigUint b, BigUint m, BigUint *out) {
    int limit = get_min_size_three(a, b, *out);
    // allocate twice the memory to prevent overflow
    BigUint result = biguint_new_heap(limit * 2);
    BigUint mod = biguint_new_heap(limit * 2);
    biguint_cpy(&mod, m);

    biguint_mul(a, b, &result);
    biguint_mod(result, mod, out);

    biguint_free(&result, &mod);
}

// https://en.wikipedia.org/wiki/Exponentiation_by_squaring
int biguint_overflow_pow(BigUint a, BigUint exponent, BigUint *out) {
    if (biguint_is_zero(exponent)) {
        biguint_one(out);
        return 0;
    }

    int overflow = 0;
    BigUint one = biguint_new_heap(a.size);
    BigUint exp = biguint_new_heap(exponent.size);
    BigUint y = biguint_new_heap(a.size);
    biguint_one(&y);
    biguint_one(&one);
    biguint_cpy(&exp, exponent);
    biguint_cpy(out, a);

    while (biguint_cmp(exp, one) > 0) {
        if (biguint_is_even(exp)) {
            overflow |= biguint_overflow_mul(*out, *out, out);
            biguint_shr(exp, 1, &exp);
        } else {
            overflow |= biguint_overflow_mul(y, *out, &y);
            overflow |= biguint_overflow_mul(*out, *out, out);
            biguint_sub(exp, one, &exp);
            biguint_shr(exp, 1, &exp);
        }
    }

    overflow |= biguint_overflow_mul(*out, y, out);

    biguint_free(&one, &exp, &y);
    return overflow;
}

void biguint_pow(BigUint a, BigUint exponent, BigUint *out) { biguint_overflow_pow(a, exponent, out); }

// performs a pow operation keeping the result in bounds over a mod m, using the following identity:
// (a ⋅ b) mod m = [(a mod m) ⋅ (b mod m)] mod m
// https://en.wikipedia.org/wiki/Modular_exponentiation
void biguint_pow_mod(BigUint a, BigUint exponent, BigUint m, BigUint *out) {
    if (biguint_is_zero(exponent)) {
        biguint_one(out);
        return;
    }

    // Since this is the power over a boundary, we want to prevent overflows during multiplication
    // so we have to allocate twice the memory
    // in the end we wrap it around the m and it should fit back into the initial size
    //
    // note that we could use mul_mod but this way we avoid
    // allocating and freeing memory on the heap every iteration as mul_mod does it for use
    BigUint base = biguint_new_heap(a.size * 2);
    BigUint mod = biguint_new_heap(m.size * 2);
    BigUint one = biguint_new(1);
    BigUint exp = biguint_new_heap(exponent.size);
    BigUint y = biguint_new_heap(a.size * 2);
    biguint_one(&y);
    biguint_cpy(&base, a);
    biguint_cpy(&mod, m);
    biguint_one(&one);
    biguint_cpy(&exp, exponent);

    biguint_mod(base, mod, &base);
    while (biguint_cmp(exp, one) > 0) {
        if (biguint_is_even(exp)) {
            biguint_mul(base, base, &base);
            biguint_mod(base, mod, &base);
            biguint_shr(exp, 1, &exp);
        } else {
            biguint_mul(y, base, &y);
            biguint_mod(y, mod, &y);
            biguint_mul(base, base, &base);
            biguint_mod(base, mod, &base);
            biguint_sub(exp, one, &exp);
            biguint_shr(exp, 1, &exp);
        }
    }

    biguint_mul(base, y, &base);
    biguint_mod(base, mod, &base);

    biguint_cpy(out, base);

    biguint_free(&base, &mod, &exp, &y);
}

void biguint_bitand(BigUint a, BigUint b, BigUint *out) {
    int limit = get_min_size_three(a, b, *out);
    for (int i = 0; i < limit; i++)
        out->limbs[i] = a.limbs[i] & b.limbs[i];
}

void biguint_bitor(BigUint a, BigUint b, BigUint *out) {
    int limit = get_min_size_three(a, b, *out);
    for (int i = 0; i < limit; i++)
        out->limbs[i] = a.limbs[i] | b.limbs[i];
}

void biguint_bitxor(BigUint a, BigUint b, BigUint *out) {
    int limit = get_min_size(a, b);
    for (int i = 0; i < limit; i++)
        out->limbs[i] = a.limbs[i] ^ b.limbs[i];
}

void biguint_bitnot(BigUint a, BigUint *out) {
    int limit = get_min_size(a, *out);
    for (int i = 0; i < limit; i++)
        out->limbs[i] = ~a.limbs[i];
}

void biguint_shl(BigUint a, int shift, BigUint *out) {
    int shift_start = shift / 64;
    int shift_mod = shift % 64;
    BigUint cpy = biguint_new_heap(a.size);
    biguint_cpy(&cpy, a);
    biguint_zero(out);
    int limit = get_min_size(a, *out);

    for (int i = shift_start; i < limit; i++) {
        out->limbs[i] = cpy.limbs[i - shift_start] << shift_mod;
    }

    /* calculate carry */
    if (shift_mod > 0) {
        for (int i = shift_start + 1; i < limit; i++) {
            out->limbs[i] += cpy.limbs[i - shift_start - 1] >> (64 - shift_mod);
        }
    }

    biguint_free(&cpy);
}

void biguint_shr(BigUint a, int shift, BigUint *out) {
    int shift_start = shift / 64;
    int shift_mod = shift % 64;
    BigUint cpy = biguint_new_heap(a.size);
    biguint_cpy(&cpy, a);
    biguint_zero(out);
    int limit = get_min_size(a, *out);

    for (int i = shift_start; i < limit; i++) {
        out->limbs[i - shift_start] = cpy.limbs[i] >> shift_mod;
    }

    /* calculate carry */
    if (shift_mod > 0) {
        for (int i = shift_start + 1; i < limit; i++) {
            out->limbs[i - shift_start - 1] += cpy.limbs[i] << (64 - shift_mod);
        }
    }

    biguint_free(&cpy);
}

void biguint_divmod(BigUint a, BigUint b, BigUint *quot, BigUint *rem) {
    int a_bits = biguint_bits(a);
    int b_bits = biguint_bits(b);
    biguint_cpy(rem, a);
    biguint_zero(quot);

    assert(b_bits != 0);
    if (a_bits < b_bits) {
        return;
    }

    int shift = a_bits - b_bits;
    BigUint shift_copy = biguint_new_heap(b.size);
    biguint_cpy(&shift_copy, b);
    biguint_shl(shift_copy, shift, &shift_copy);
    while (1) {
        /* rem >= shift_copy */
        if (biguint_cmp(*rem, shift_copy) >= 0) {
            if (shift / 64 < quot->size) {
                quot->limbs[shift / 64] |= ((uint64_t)1 << (uint64_t)(shift % 64));
            }
            biguint_sub(*rem, shift_copy, rem);
        }
        if (shift == 0)
            break;
        shift -= 1;
        biguint_shr(shift_copy, 1, &shift_copy);
    }

    biguint_free(&shift_copy);
}

void biguint_div(BigUint a, BigUint b, BigUint *out) {
    BigUint rem = biguint_new_heap(out->size);
    biguint_divmod(a, b, out, &rem);
    biguint_free(&rem);
}

void biguint_mod(BigUint a, BigUint b, BigUint *out) {
    BigUint quot = biguint_new_heap(out->size);
    biguint_divmod(a, b, &quot, out);
    biguint_free(&quot);
}

int biguint_is_even(BigUint a) { return (a.limbs[0] & 1) == 0; }

/**
 * Debugging
 */
void biguint_raw_println(BigUint a) {
    printf("[");
    for (int i = 0; i < a.size; i++) {
        printf("%lu", a.limbs[i]);
        if (i != a.size - 1)
            printf(",");
    }
    printf("]\n");
};

void biguint_raw_print(BigUint a) {
    printf("[");
    for (int i = 0; i < a.size; i++) {
        printf("%lu", a.limbs[i]);
        if (i != a.size - 1)
            printf(",");
    }
    printf("]");
};

void biguint_println(BigUint a) {
    char *str = biguint_to_dec_string(a);
    printf("%s\n", str);
    free(str);
};

void biguint_print(BigUint a) {
    char *str = biguint_to_dec_string(a);
    printf("%s", str);
    free(str);
};
