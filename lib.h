#include <stdint.h>

typedef unsigned char u8;
typedef uint64_t u64;

// BEGIN from linux

#define TNUM(_v, _m)	(struct tnum){.value = _v, .mask = _m}

struct tnum {
	u64 value;
	u64 mask;
};

struct tnum tnum_lshift(struct tnum a, u8 shift);
struct tnum tnum_rshift(struct tnum a, u8 shift);
struct tnum tnum_mul(struct tnum a, struct tnum b);
_Bool wellformed(struct tnum x);
_Bool ingamma(u64 x, struct tnum T);
void print_tnum(char *lbl, struct tnum x);
_Bool tnums_differ(struct tnum x, struct tnum y);
