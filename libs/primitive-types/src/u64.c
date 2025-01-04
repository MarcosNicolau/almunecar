#include "u64.h"

u64_overflow_op u64_overflow_add(uint64_t a, uint64_t b)
{
    u64_overflow_op op;
    op.res = a + b;
    op.overflow = op.res < a;
    return op;
}

u64_overflow_op u64_overflow_sub(uint64_t a, uint64_t b)
{
    u64_overflow_op op;
    op.res = a - b;
    op.overflow = op.res > a;
    return op;
}

u64_mul_op u64_mul(uint64_t a, uint64_t b)
{
    u64_mul_op op;
    __uint128_t res = (__uint128_t)a * (__uint128_t)b;
    op.res = (uint64_t)res;
    op.carry = (uint64_t)(res >> 64);

    return op;
}

u64_overflow_op u64_overflow_mul(uint64_t a, uint64_t b)
{
    u64_overflow_op op;
    u64_mul_op mul = u64_mul(a, b);
    op.res = mul.res;
    op.overflow = mul.carry > 0;
    return op;
}

int u64_leading_zeros(uint64_t a)
{
    int count = 0;

    for (int i = 0; i < 64; i++)
    {
        if (a >> i == 0)
            break;
        count++;
    }

    return 64 - count;
}
