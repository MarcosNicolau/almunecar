#include <assert.h>
#include <biguint.h>
#include <string.h>

void big_uint_free_limbs(BigUint *a) {
    if (!a)
        return;

    free(a->limbs);
}

/**
 * Conversion and initializers
 */
void big_uint_zero(BigUint *out) {
    for (int i = 0; i < out->size; i++)
        out->limbs[i] = 0;
}

void big_uint_one(BigUint *out) {
    big_uint_zero(out);
    out->limbs[0] = 1;
}

void big_uint_from_u64(uint64_t a, BigUint *out) {
    big_uint_zero(out);
    out->limbs[0] = a;
}

void big_uint_cpy(BigUint *dst, BigUint src) {
    for (int i = 0; i < dst->size; i++) {
        dst->limbs[i] = src.limbs[i];
    }
}

void big_uint_from_dec_string(char *str, BigUint *out) {
    big_uint_zero(out);

    BigUint ten = big_uint_new_heap(out->size);
    big_uint_from_u64(10, &ten);

    int len = strlen(str);
    for (int i = 0; i < len; i++) {
        uint64_t digit = str[i] - '0';
        BigUint digit_big = big_uint_new_heap(out->size);
        big_uint_from_u64(digit, &digit_big);
        big_uint_mul(out, ten);
        big_uint_add(out, digit_big);
        big_uint_free_limbs(&digit_big);
    }

    big_uint_free_limbs(&ten);
};

void big_uint_from_bytes_big_endian(uint8_t *bytes, BigUint *out) {
    big_uint_zero(out);
    for (int i = out->size - 1, j = 0; i >= 0; i--, j++) {
        out->limbs[i] = ((uint64_t)bytes[j * 8] << 56) | ((uint64_t)bytes[j * 8 + 1] << 48) |
                        ((uint64_t)bytes[j * 8 + 2] << 40) | ((uint64_t)bytes[j * 8 + 3] << 32) |
                        ((uint64_t)bytes[j * 8 + 4] << 24) | ((uint64_t)bytes[j * 8 + 5] << 16) |
                        ((uint64_t)bytes[j * 8 + 6] << 8) | ((uint64_t)bytes[j * 8 + 7]);
    }
};

void big_uint_get_bytes_big_endian(BigUint value, uint8_t *buffer) {
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

void big_uint_from_bytes_little_endian(uint8_t *bytes, BigUint *out) {
    big_uint_zero(out);
    for (int i = 0; i < out->size; i++) {
        out->limbs[i] = ((uint64_t)bytes[i * 8]) | ((uint64_t)bytes[i * 8 + 1] << 8) |
                        ((uint64_t)bytes[i * 8 + 2] << 16) | ((uint64_t)bytes[i * 8 + 3] << 24) |
                        ((uint64_t)bytes[i * 8 + 4] << 32) | ((uint64_t)bytes[i * 8 + 5] << 40) |
                        ((uint64_t)bytes[i * 8 + 6] << 48) | ((uint64_t)bytes[i * 8 + 7] << 56);
    }
};

void big_uint_get_bytes_little_endian(BigUint value, uint8_t *buffer) {
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

char *big_uint_to_dec_string(BigUint a) {
    char *result = malloc(a.size * 20 + 1); /* multiply by 20, since each part can
                                                 take as much as 20 bytes*/
    int i = a.size * 20 - 1;
    BigUint ten = big_uint_new_heap(a.size);
    big_uint_from_u64(10, &ten);

    BigUint dividend = big_uint_new_heap(a.size);
    big_uint_cpy(&dividend, a);
    BigUint quot = big_uint_new_heap(dividend.size);
    BigUint rem = big_uint_new_heap(dividend.size);

    while (1) {
        big_uint_divmod(dividend, ten, &quot, &rem);
        int digit = rem.limbs[0] + '0';
        result[i] = digit;
        big_uint_cpy(&dividend, quot);
        if (big_uint_is_zero(dividend))
            break;
        i -= 1;
    }

    char *dst = malloc(a.size * 20 + 1 - i);
    int k = 0;
    for (int j = i; j < a.size * 20 + 1; j++) {
        dst[k] = result[i + k];
        k++;
    }
    big_uint_free_limbs(&ten);
    big_uint_free_limbs(&dividend);
    big_uint_free_limbs(&quot);
    big_uint_free_limbs(&rem);
    free(result);
    return dst;
};

/**
 * Utils
 */
int big_uint_bits(BigUint a) {
    for (int i = 1; i < a.size; i++) {
        if (a.limbs[a.size - i] > 0) {
            return 64 * (a.size - i + 1) - u64_leading_zeros(a.limbs[a.size - i]);
        }
    }
    return 64 - u64_leading_zeros(a.limbs[0]);
}

int big_uint_cmp(BigUint a, BigUint b) {
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

int big_uint_is_zero(BigUint a) {
    for (int i = 0; i < a.size; i++) {
        if (a.limbs[i] != 0)
            return 0;
    }
    return 1;
};

/**
 * Operations
 */
int big_uint_overflow_add(BigUint *a, BigUint b) {
    uint64_t carry = 0;
    for (int i = 0; i < a->size; i++) {
        u64_overflow_op addition = u64_overflow_add(a->limbs[i], b.limbs[i]);
        u64_overflow_op carry_addition = u64_overflow_add(addition.res, carry);
        a->limbs[i] = carry_addition.res;
        carry = addition.overflow + carry_addition.overflow;
    }
    return carry > 0;
};

int big_uint_overflow_sub(BigUint *a, BigUint b) {
    uint64_t carry = 0;
    for (int i = 0; i < a->size; i++) {
        u64_overflow_op sub = u64_overflow_sub(a->limbs[i], b.limbs[i]);
        u64_overflow_op carry_sub = u64_overflow_sub(sub.res, carry);
        a->limbs[i] = carry_sub.res;
        carry = sub.overflow + carry_sub.overflow;
    }
    return carry > 0;
};

int big_uint_overflow_mul(BigUint *a, BigUint b) {
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

void big_uint_add(BigUint *a, BigUint b) { big_uint_overflow_add(a, b); }

void big_uint_sub(BigUint *a, BigUint b) { big_uint_overflow_sub(a, b); }

void big_uint_mul(BigUint *a, BigUint b) { big_uint_overflow_mul(a, b); }

void big_uint_bitand(BigUint *a, BigUint b) {
    for (int i = 0; i < a->size; i++)
        a->limbs[i] = a->limbs[i] & b.limbs[i];
}

void big_uint_bitor(BigUint *a, BigUint b) {
    for (int i = 0; i < a->size; i++)
        a->limbs[i] = a->limbs[i] | b.limbs[i];
}

void big_uint_bitxor(BigUint *a, BigUint b) {
    for (int i = 0; i < a->size; i++)
        a->limbs[i] = a->limbs[i] ^ b.limbs[i];
}

void big_uint_bitnot(BigUint *a) {
    for (int i = 0; i < a->size; i++)
        a->limbs[i] = ~a->limbs[i];
}

void big_uint_shl(BigUint *a, int shift) {
    int shift_start = shift / 64;
    int shift_mod = shift % 64;
    BigUint cpy = big_uint_new_heap(a->size);
    big_uint_cpy(&cpy, *a);
    big_uint_zero(a);

    for (int i = shift_start; i < a->size; i++) {
        a->limbs[i] = cpy.limbs[i - shift_start] << shift_mod;
    }

    /* calculate carry */
    if (shift_mod > 0) {
        for (int i = shift_start + 1; i < a->size; i++) {
            a->limbs[i] += cpy.limbs[i - shift_start - 1] >> (64 - shift_mod);
        }
    }

    big_uint_free_limbs(&cpy);
}

void big_uint_shr(BigUint *a, int shift) {
    int shift_start = shift / 64;
    int shift_mod = shift % 64;
    BigUint cpy = big_uint_new_heap(a->size);
    big_uint_cpy(&cpy, *a);
    big_uint_zero(a);

    for (int i = shift_start; i < a->size; i++) {
        a->limbs[i - shift_start] = cpy.limbs[i] >> shift_mod;
    }

    /* calculate carry */
    if (shift_mod > 0) {
        for (int i = shift_start + 1; i < a->size; i++) {
            a->limbs[i - shift_start - 1] += cpy.limbs[i] << (64 - shift_mod);
        }
    }

    big_uint_free_limbs(&cpy);
}

void big_uint_divmod(BigUint a, BigUint b, BigUint *quot, BigUint *rem) {
    int a_bits = big_uint_bits(a);
    int b_bits = big_uint_bits(b);
    big_uint_cpy(rem, a);
    big_uint_zero(quot);

    assert(b_bits != 0);
    if (a_bits < b_bits) {
        return;
    }

    int shift = a_bits - b_bits;
    BigUint shift_copy = big_uint_new_heap(b.size);
    big_uint_cpy(&shift_copy, b);
    big_uint_shl(&shift_copy, shift);
    while (1) {
        /* rem >= shift_copy */
        if (big_uint_cmp(*rem, shift_copy) >= 0) {
            quot->limbs[shift / 64] |= ((uint64_t)1 << (uint64_t)(shift % 64));
            big_uint_sub(rem, shift_copy);
        }
        if (shift == 0)
            break;
        shift -= 1;
        big_uint_shr(&shift_copy, 1);
    }

    big_uint_free_limbs(&shift_copy);
}

/**
 * Debugging
 */
void big_uint_raw_println(BigUint a) {
    printf("[");
    for (int i = 0; i < a.size; i++) {
        printf("%lu", a.limbs[i]);
        if (i != a.size - 1)
            printf(",");
    }
    printf("]\n");
};

void big_uint_raw_print(BigUint a) {
    printf("[");
    for (int i = 0; i < a.size; i++) {
        printf("%lu", a.limbs[i]);
        if (i != a.size - 1)
            printf(",");
    }
    printf("]");
};

void big_uint_println(BigUint a) {
    char *str = big_uint_to_dec_string(a);
    printf("%s\n", str);
    free(str);
};

void big_uint_print(BigUint a) {
    char *str = big_uint_to_dec_string(a);
    printf("%s", str);
    free(str);
};
