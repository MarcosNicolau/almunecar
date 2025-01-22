#include <assert.h>
#include <biguint.h>
#include <string.h>

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
    int limit = dst->size;
    if (dst->size > src.size)
        limit = src.size;

    for (int i = 0; i < dst->size; i++) {
        if (i < limit)
            dst->limbs[i] = src.limbs[i];
        else
            dst->limbs[i] = 0;
    }
}

void biguint_from_dec_string(char *str, BigUint *out) {
    biguint_zero(out);

    BigUint ten = biguint_new_heap(out->size);
    biguint_from_u64(10, &ten);

    int len = strlen(str);
    for (int i = 0; i < len; i++) {
        uint64_t digit = str[i] - '0';
        BigUint digit_big = biguint_new_heap(out->size);
        biguint_from_u64(digit, &digit_big);
        biguint_mul(out, ten);
        biguint_add(out, digit_big);
        biguint_free_limbs(&digit_big);
    }

    biguint_free_limbs(&ten);
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
    biguint_free_limbs(&ten);
    biguint_free_limbs(&dividend);
    biguint_free_limbs(&quot);
    biguint_free_limbs(&rem);
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
    for (int i = a.size - 1; i >= 0; i--) {
        uint64_t a_i = (uint64_t)a.limbs[i];
        uint64_t b_i = (uint64_t)b.limbs[i];
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
int biguint_overflow_add(BigUint *a, BigUint b) {
    uint64_t carry = 0;
    for (int i = 0; i < a->size; i++) {
        u64_overflow_op addition = u64_overflow_add(a->limbs[i], b.limbs[i]);
        u64_overflow_op carry_addition = u64_overflow_add(addition.res, carry);
        a->limbs[i] = carry_addition.res;
        carry = addition.overflow + carry_addition.overflow;
    }
    return carry > 0;
};

int biguint_overflow_sub(BigUint *a, BigUint b) {
    uint64_t carry = 0;
    for (int i = 0; i < a->size; i++) {
        u64_overflow_op sub = u64_overflow_sub(a->limbs[i], b.limbs[i]);
        u64_overflow_op carry_sub = u64_overflow_sub(sub.res, carry);
        a->limbs[i] = carry_sub.res;
        carry = sub.overflow + carry_sub.overflow;
    }
    return carry > 0;
};

int biguint_overflow_mul(BigUint *a, BigUint b) {
    uint64_t result[a->size * 2];
    for (int i = 0; i < a->size * 2; i++)
        result[i] = 0;
    for (int i = 0; i < a->size; i++) {
        uint64_t carry = 0;
        for (int j = 0; j < a->size; j++) { /* calculate result */
            u64_mul_op mul = u64_mul(a->limbs[j], b.limbs[i]);
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
    for (int i = 4; i < a->size * 2; i++) {
        if (result[i] != 0) {
            overflow = 1;
            break;
        }
    }
    for (int i = 0; i < a->size; i++)
        a->limbs[i] = result[i];

    return overflow;
};

// https://en.wikipedia.org/wiki/Exponentiation_by_squaring
int biguint_overflow_pow(BigUint *a, BigUint exponent) {
    if (biguint_is_zero(exponent)) {
        biguint_one(a);
        return 0;
    }

    int overflow = 0;
    BigUint one = biguint_new_heap(a->size);
    BigUint y = biguint_new_heap(a->size);
    biguint_one(&y);
    biguint_one(&one);

    while (biguint_cmp(exponent, one) > 0) {
        if (biguint_is_even(*a)) {
            overflow = biguint_overflow_mul(a, *a);
            biguint_shr(&exponent, 1);
        } else {
            overflow = biguint_overflow_mul(&y, *a);
            overflow = biguint_overflow_mul(a, *a);
            biguint_sub(&exponent, one);
            biguint_shr(&exponent, 1);
        }
    }

    biguint_free(&one, &y);
    return overflow;
}

void biguint_add(BigUint *a, BigUint b) { biguint_overflow_add(a, b); }

void biguint_sub(BigUint *a, BigUint b) { biguint_overflow_sub(a, b); }

void biguint_mul(BigUint *a, BigUint b) { biguint_overflow_mul(a, b); }

void biguint_pow(BigUint *a, BigUint exponent) { biguint_overflow_pow(a, exponent); }

void biguint_bitand(BigUint *a, BigUint b) {
    for (int i = 0; i < a->size; i++)
        a->limbs[i] = a->limbs[i] & b.limbs[i];
}

void biguint_bitor(BigUint *a, BigUint b) {
    for (int i = 0; i < a->size; i++)
        a->limbs[i] = a->limbs[i] | b.limbs[i];
}

void biguint_bitxor(BigUint *a, BigUint b) {
    for (int i = 0; i < a->size; i++)
        a->limbs[i] = a->limbs[i] ^ b.limbs[i];
}

void biguint_bitnot(BigUint *a) {
    for (int i = 0; i < a->size; i++)
        a->limbs[i] = ~a->limbs[i];
}

void biguint_shl(BigUint *a, int shift) {
    int shift_start = shift / 64;
    int shift_mod = shift % 64;
    BigUint cpy = biguint_new_heap(a->size);
    biguint_cpy(&cpy, *a);
    biguint_zero(a);

    for (int i = shift_start; i < a->size; i++) {
        a->limbs[i] = cpy.limbs[i - shift_start] << shift_mod;
    }

    /* calculate carry */
    if (shift_mod > 0) {
        for (int i = shift_start + 1; i < a->size; i++) {
            a->limbs[i] += cpy.limbs[i - shift_start - 1] >> (64 - shift_mod);
        }
    }

    biguint_free_limbs(&cpy);
}

void biguint_shr(BigUint *a, int shift) {
    int shift_start = shift / 64;
    int shift_mod = shift % 64;
    BigUint cpy = biguint_new_heap(a->size);
    biguint_cpy(&cpy, *a);
    biguint_zero(a);

    for (int i = shift_start; i < a->size; i++) {
        a->limbs[i - shift_start] = cpy.limbs[i] >> shift_mod;
    }

    /* calculate carry */
    if (shift_mod > 0) {
        for (int i = shift_start + 1; i < a->size; i++) {
            a->limbs[i - shift_start - 1] += cpy.limbs[i] << (64 - shift_mod);
        }
    }

    biguint_free_limbs(&cpy);
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
    biguint_shl(&shift_copy, shift);
    while (1) {
        /* rem >= shift_copy */
        if (biguint_cmp(*rem, shift_copy) >= 0) {
            quot->limbs[shift / 64] |= ((uint64_t)1 << (uint64_t)(shift % 64));
            biguint_sub(rem, shift_copy);
        }
        if (shift == 0)
            break;
        shift -= 1;
        biguint_shr(&shift_copy, 1);
    }

    biguint_free_limbs(&shift_copy);
}

void biguint_div(BigUint a, BigUint b, BigUint *out) {
    BigUint rem = biguint_new_heap(out->size);
    biguint_divmod(a, b, out, &rem);
    biguint_free_limbs(&rem);
}

void biguint_mod(BigUint a, BigUint b, BigUint *out) {
    BigUint quot = biguint_new_heap(out->size);
    biguint_divmod(a, b, &quot, out);
    biguint_free_limbs(&quot);
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
