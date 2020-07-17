// uint128 implemented by 2 unsigned long long
typedef struct uint128 {
	unsigned long long lo, hi;
} uint128_XD;
const uint128_XD zero128 = {
	.hi = 0,
	.lo = 0
};
// static makes functions restricted to the file where they are declared
static inline void add ( uint128_XD *ret, uint128_XD x, uint128_XD y ) {
	ret->hi = x.hi + y.hi;
	if ( y.lo > ~x.lo ) {
		ret->hi++;
	}
	ret->lo = x.lo + y.lo;
}

// since they are unsigned...
static inline void sub ( uint128_XD *ret, uint128_XD x, uint128_XD y ) {
	ret->hi = x.hi - y.hi;
	if ( x.lo < y.lo ) {
		ret->hi--;
	} 
	ret->lo = x.lo - y.lo;		
}

static inline void lsft ( uint128_XD *ret, uint128_XD x, int shift ) {
	if ( shift == 0 ) {
		*ret = x;
		return ;
	}
	if ( shift >= 64 ) {
		ret->hi = x.lo << (shift - 64);
		ret->lo = 0ull;
		return ;
	}
	ret->hi = x.hi << shift;
	ret->hi |= x.lo >> (64-shift);
	ret->lo = x.lo << shift;
}

static inline void rsft ( uint128_XD *ret, uint128_XD x, int shift ) {
	if ( shift == 0 ) {
		*ret = x;
		return ;
	}
	if ( shift >= 64 ) {
		ret->lo = x.hi >> (shift - 64);
		ret->hi = 0ull;
		return ;
	}
	ret->lo = x.lo >> shift;
	ret->lo |= x.hi << (64-shift);
	ret->hi = x.hi >> shift;
}
// check if the i-th bit of x is 1 (0-th bit is LSB)
static inline int is_one ( uint128_XD x, int i ) {
	if ( i >= 128 ) {
		exit(1);
	}
	if ( i >=64 ) {
		return (x.hi >> (i-64)) & 1;
	}
	else {
		return (x.lo >> i) & 1;
	}
}
static inline void mul ( uint128_XD *ret, uint128_XD u, uint128_XD l ) {
	*ret = zero128;
	/*
	int a = __builtin_popcountll(x.lo) + __builtin_popcountll(x.hi);
	int b = __builtin_popcountll(y.lo) + __builtin_popcountll(y.hi);
	uint128_XD l, u; // u = Multiplicand, l = Multiplier
	if ( a >= b ) {
		u = x;
		l = y;
	} else {
		u = y;
		l = x;
	}*/

	for ( int i=0; i<128; ++i ) {
		if ( is_one(l, i) ) {
			uint128_XD tmp;
			lsft(&tmp, u, i);
			add(ret, *ret, tmp);
		}
	}
}
void display_bs ( unsigned long long x ) {
	int ar[70] = {};
	int tmp = 0;
	printf("%llu\n", x);
	while ( x > 0 ) {
		ar[tmp++] = x%2;
		x >>= 1;
	}
	for ( int i=63; i>=0; i-- ) {
		printf("%d", ar[i]);
	}
	puts("");
}
static inline void mul_clz ( uint128_XD *ret, uint128_XD u, uint128_XD l ) {
	*ret = zero128;
	/*
	int a = __builtin_popcountll(x.lo) + __builtin_popcountll(x.hi);
	int b = __builtin_popcountll(y.lo) + __builtin_popcountll(y.hi);
	uint128_XD l, u; // u = Multiplicand, l = Multiplier
	uint128_XD tmp;
	if ( a >= b ) {
		u = x;
		l = y;
	} else {
		u = y;
		l = x;
	}*/
	uint128_XD tmp;
	unsigned long long hi = l.hi;
	while ( hi > 0 ) {
		int pos = __builtin_clzll(hi);
		lsft(&tmp, u, 127 - pos);
		add(ret, *ret, tmp);
		hi ^= 1ull << (63 - pos);
	}
	unsigned long long lo = l.lo;
	while ( lo > 0 ) {
		int pos = __builtin_clzll(lo);
		lsft(&tmp, u, 63 - pos);
		add(ret, *ret, tmp);
		lo ^= 1ull << (63-pos);
	}
}