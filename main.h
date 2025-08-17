typedef enum MineVsKernel MineVsKernel;
typedef struct _ngg_tuple_isoptimal _ngg_tuple_isoptimal;
typedef struct CumulativeStat CumulativeStat;
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <omp.h>
#include "lib.h"
#include <math.h>
#include "./nggvector.h"
typedef struct tnum tnum;
extern uint64_t MAX_U64;
enum MineVsKernel {
	MINE_VS_KERNEL_MINE_BETTER,
	MINE_VS_KERNEL_SAME,
	MINE_VS_KERNEL_MINE_WORSE,
};

struct _ngg_tuple_isoptimal {
	_Bool m0;
	_Bool m1;
	MineVsKernel m2;
};

struct CumulativeStat {
	int bettercount;
	int samecount;
	int worsecount;
	int optimalcount;
	int optimalcount_linux;
	int totalcount;
};

tnum getalpha(u64vector *xs);
u64vector * getgamma(tnum T);
void append_nondup(u64vector *vec, uint64_t x);
u64vector * mulvec(u64vector *avec, u64vector *bvec);
_Bool left_subset_of_right(u64vector *l, u64vector *r);
void printvec(const char * lbl, u64vector *vec);
_ngg_tuple_isoptimal isoptimal(tnum P, tnum Q, _Bool commutative, _Bool print_each, _Bool print_sets);
void print_cumustat(const char * lbl, int bits, CumulativeStat cumustat);
int main(int argc, char * *argv);
_ngg_tuple_isoptimal _ngg_tuple_isoptimal_default();
CumulativeStat cumulative_stat_default();
