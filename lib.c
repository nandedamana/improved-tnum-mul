#include <stdio.h>
#include <stdlib.h>
#include "lib.h"

struct tnum tnum_add(struct tnum a, struct tnum b)
{
	u64 sm, sv, sigma, chi, mu;

	sm = a.mask + b.mask;
	sv = a.value + b.value;
	sigma = sm + sv;
	chi = sigma ^ sv;
	mu = chi | a.mask | b.mask;
	return TNUM(sv & ~mu, mu);
}

struct tnum tnum_lshift(struct tnum a, u8 shift)
{
	return TNUM(a.value << shift, a.mask << shift);
}

struct tnum tnum_rshift(struct tnum a, u8 shift)
{
	return TNUM(a.value >> shift, a.mask >> shift);
}

struct tnum tnum_mul(struct tnum a, struct tnum b)
{
	u64 acc_v = a.value * b.value;
	struct tnum acc_m = TNUM(0, 0);

	while (a.value || a.mask) {
		/* LSB of tnum a is a certain 1 */
		if (a.value & 1)
			acc_m = tnum_add(acc_m, TNUM(0, b.mask));
		/* LSB of tnum a is uncertain */
		else if (a.mask & 1)
			acc_m = tnum_add(acc_m, TNUM(0, b.value | b.mask));
		/* Note: no case for LSB is certain 0 */
		a = tnum_rshift(a, 1);
		b = tnum_lshift(b, 1);
	}
	return tnum_add(TNUM(acc_v, 0), acc_m);
}

// END from linux

_Bool wellformed(struct tnum x)
{
	return (x.mask & x.value) == 0;
}

_Bool ingamma(u64 x, struct tnum T)
{
	return (x & ~T.mask) == T.value;
}

void print_tnum(char *lbl, struct tnum x)
{
	printf("%s = { mask: %u, value: %u }\n", lbl, x.mask, x.value);
}

_Bool tnums_differ(struct tnum x, struct tnum y)
{
	return (x.mask != y.mask) || (x.value != y.value);
}
