// tests uint128_XD with <stdint.h>'s __int128
#include <limits.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "uint128_XD.h"

#define NINETEEN 10000000000000000000ull
typedef unsigned long long ull;
typedef unsigned __int128 uint128_t;
ull randull()
{
    return (ull) rand() * (ull) rand();
}
void print128_t(uint128_t q)
{
    ull hi, lo;
    hi = q / NINETEEN;
    lo = q % NINETEEN;
    if (hi == 0ull) {
        printf("%llu\n", lo);
    } else {
        printf("%llu%019llu\n", hi, lo);
    }
}
uint128_t XD_to_std(uint128_XD y)
{
    uint128_t x = y.hi;
    x = (x << 64) + y.lo;
    return x;
}
void print128_XD(uint128_XD q)
{
    print128_t(XD_to_std(q));
}
int main()
{
    // srand(time(NULL));
    int t = 1;
    for (int i = 0; i < t; ++i) {
        uint128_t a = randull(), b = randull();
        uint128_XD x = zero128, y = zero128, xy, xy_clz;
        x.lo = a, y.lo = b;
        mul(&xy, x, y);
        mul_clz(&xy_clz, x, y);
        print128_t(a * b);
        print128_XD(xy);
        print128_XD(xy_clz);
    }
    return 0;
}