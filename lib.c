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

// TODO make sure this is correct
struct tnum my_tnum_mul_proto(struct tnum a, struct tnum b)
{
	struct tnum acc = TNUM(0, 0);

	while (a.value || a.mask) {
		/* LSB of tnum a is a certain 1 */
		if (a.value & 1)
			acc = tnum_add(acc, b);
		/* LSB of tnum a is uncertain */
		else if (a.mask & 1) {
			u64 msk = b.value | b.mask;
			struct tnum partprod = TNUM(b.value & ~msk, msk);
			acc = tnum_add(acc, partprod);
		}
		/* Note: no case for LSB is certain 0 */
		a = tnum_rshift(a, 1);
		b = tnum_lshift(b, 1);
	}
	return acc;
}

// No value-mask decomposition unlike Harishankar et al.
struct tnum my_tnum_mul(struct tnum a, struct tnum b)
{
	struct tnum acc = TNUM(0, 0);

	while (a.value || a.mask) {
		/* LSB of tnum a is a certain 1 */
		if (a.value & 1)
			acc = tnum_add(acc, b);
		/* LSB of tnum a is uncertain */
		else if (a.mask & 1) {
			// a.value[i] could be 0 or 1
			//struct tnum acc_0, acc_1; // accumulator for 0 and 1
			struct tnum acc_1; // accumulator when a.value[i] = 1

			// This approach considers mu's to be either zero or one altogether, solving the challenge pointed out by Harishankar et al. (see the last example in their paper -- 11 * x1)

			// Assume a.value[i] is 0
			//acc_0 = tnum_add(acc, TNUM(0, 0)); // Unoptimized
			//acc_0 = acc; // optimized (but this can be avoided altogether)

			// Assume a.value[i] is 1
			u64 msk = b.value | b.mask;
			struct tnum partprod = TNUM(b.value & ~msk, msk);
			acc_1 = tnum_add(acc, partprod);

			//acc = tnum_union(acc_0, acc_1);
			acc = tnum_union(acc, acc_1);
		}
		/* Note: no case for LSB is certain 0 */
		a = tnum_rshift(a, 1);
		b = tnum_lshift(b, 1);
	}
	return acc;
}

// Use value-mask decomposition like Harishankar et al.
// This seems to have exactly the same precision as of the kernel version.
struct tnum my_tnum_mul_decompose(struct tnum a, struct tnum b)
{
	u64 acc_v = a.value * b.value;
	struct tnum acc_m = TNUM(0, 0);

	while (a.value || a.mask) {
		/* LSB of tnum a is a certain 1 */
		if (a.value & 1)
			acc_m = tnum_add(acc_m, TNUM(0, b.mask));
		/* LSB of tnum a is uncertain */
		else if (a.mask & 1) {
			// a.value[i] could be 0 or 1
			struct tnum acc_m_0, acc_m_1;

			// This approach considers mu's to be either zero or one altogether, solving the challenge pointed out by Harishankar et al. (see the last example in their paper -- 11 * x1)

			// Assume a.value[i] is 0
			acc_m_0 = tnum_add(acc_m, TNUM(0, 0)); // TODO simplify?

			// Assume a.value[i] is 1
			u64 msk = b.value | b.mask;
			struct tnum partprod = TNUM(0, msk);
			acc_m_1 = tnum_add(acc_m, partprod);

			acc_m = tnum_union(acc_m_0, acc_m_1);
		}
		/* Note: no case for LSB is certain 0 */
		a = tnum_rshift(a, 1);
		b = tnum_lshift(b, 1);
	}
	return tnum_add(TNUM(acc_v, 0), acc_m);
}

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
