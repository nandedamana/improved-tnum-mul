typedef struct u64vector u64vector;
typedef enum MineVsKernel MineVsKernel;
typedef struct _ngg_tuple_isoptimal _ngg_tuple_isoptimal;
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include "lib.h"
#include <math.h>
typedef struct tnum tnum;
extern uint64_t MAX_U64;
struct u64vector {
	uint64_t *arr;
	int alloccount;
	int count;
};

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

void u64vector__resize(u64vector *this, int newcount);
void u64vector_append(u64vector *this, uint64_t newitem);
void u64vector_clear(u64vector *this);
uint64_t u64vector_get_item(u64vector *this, int index);
uint64_t u64vector_pop(u64vector *this);
void u64vector_set_item(u64vector *this, int index, uint64_t itm);
int u64vector_get_count(u64vector *this);
_Bool u64vector_is_empty(u64vector *this);
void u64vector_destruct(u64vector *this);
void u64vector_construct(u64vector *this);
tnum getalpha(u64vector *xs);
u64vector * getgamma(tnum T);
void append_nondup(u64vector *vec, uint64_t x);
u64vector * mulvec(u64vector *avec, u64vector *bvec);
_Bool left_subset_of_right(u64vector *l, u64vector *r);
void printvec(const char * lbl, u64vector *vec);
_ngg_tuple_isoptimal isoptimal(tnum P, tnum Q);
int main(int argc, char * *argv);
_ngg_tuple_isoptimal _ngg_tuple_isoptimal_default();
