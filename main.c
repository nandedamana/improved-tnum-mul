#include <assert.h>
#include <stdarg.h>
#include <stdbool.h>

#include "main.h"


uint64_t MAX_U64 = 16;

void u64vector__resize(u64vector *this, int newcount)
{
	if(newcount > this->alloccount) {
		int alloccountbak = this->alloccount;

		this->alloccount = 2 * this->alloccount;

		assert(this->alloccount > alloccountbak); /* vector.ngg:17 */

		assert(newcount <= this->alloccount); /* vector.ngg:18 */

		this->arr = realloc(this->arr, this->alloccount * sizeof(this->arr[0]));
		if(this->arr == NULL) { perror(NULL); exit(EXIT_FAILURE); }

	}

	this->count = newcount;
}

void u64vector_append(u64vector *this, uint64_t newitem)
{
	int newcount = 1 + this->count;

	u64vector__resize(this, newcount);

	this->arr[newcount - 1] = newitem;
}

void u64vector_clear(u64vector *this)
{
	this->count = 0;
	this->alloccount = 8;
	this->arr = realloc(this->arr, this->alloccount * sizeof(this->arr[0]));
	if(this->arr == NULL) { perror(NULL); exit(EXIT_FAILURE); }

}

uint64_t u64vector_get_item(u64vector *this, int index)
{
	assert(index < this->count); /* vector.ngg:43 */

	return this->arr[index];
}

uint64_t u64vector_pop(u64vector *this)
{
	assert(this->count > 0); /* vector.ngg:49 */
	uint64_t r = u64vector_get_item(this, this->count - 1);
	this->count = this->count - 1;

	return r;
}

void u64vector_set_item(u64vector *this, int index, uint64_t itm)
{
	assert(index < this->count); /* vector.ngg:62 */

	this->arr[index] = itm;
}

int u64vector_get_count(u64vector *this)
{
	return this->count;
}

_Bool u64vector_is_empty(u64vector *this)
{
	return 0 == this->count;
}

void u64vector_destruct(u64vector *this)
{
	if(this->arr) {
		free(this->arr);
	}
}

void u64vector_construct(u64vector *this)
{
	this->alloccount = 8;
	uint64_t *_tmp_1 = (uint64_t *) calloc((size_t) 8, sizeof(uint64_t));
	if(_tmp_1 == NULL) {
		perror(NULL);
		exit(EXIT_FAILURE);
	}

	this->arr = _tmp_1;
	this->count = 0;
}

tnum tnum_union(tnum t1, tnum t2)
{
	uint64_t value = t1.value & t2.value;
	uint64_t mask = ((t1.value ^ t2.value) | t1.mask) | t2.mask;

	return TNUM(value & (~mask), mask);
}

tnum getalpha(u64vector *xs)
{
	int _ngg_tmp_1;
	uint64_t x;
	assert(u64vector_get_count(xs) > 0); /* main.ngg:35 */

	tnum T = TNUM(u64vector_get_item(xs, 0), 0);

	for(_ngg_tmp_1 = 0; _ngg_tmp_1 < u64vector_get_count(xs); _ngg_tmp_1 += 1) {
		x = u64vector_get_item(xs, _ngg_tmp_1);
		T = tnum_union(T, TNUM(x, 0));
	}

	return T;
}

u64vector * getgamma(tnum T)
{
	size_t x;
	u64vector *vec = (u64vector *) malloc(sizeof(u64vector));
	if(vec == NULL) {
		perror(NULL);
		exit(EXIT_FAILURE);
	}

	u64vector_construct(vec);

	for(x = 0u; x <= (T.mask | T.value); x += 1) {
		if(ingamma(x, T)) {
			u64vector_append(vec, x);
		}
	}

	u64vector *_tmp_1;
	_tmp_1 = vec;
	vec = NULL;
	return _tmp_1;
}

void append_nondup(u64vector *vec, uint64_t x)
{
	int _ngg_tmp_2;
	uint64_t y;
	for(_ngg_tmp_2 = 0; _ngg_tmp_2 < u64vector_get_count(vec); _ngg_tmp_2 += 1) {
		y = u64vector_get_item(vec, _ngg_tmp_2);
		if(y == x) {
			return;
		}
	}

	u64vector_append(vec, x);
}

u64vector * mulvec(u64vector *avec, u64vector *bvec)
{
	int _ngg_tmp_3;
	uint64_t a;
	u64vector *vec = (u64vector *) malloc(sizeof(u64vector));
	if(vec == NULL) {
		perror(NULL);
		exit(EXIT_FAILURE);
	}

	u64vector_construct(vec);

	for(_ngg_tmp_3 = 0; _ngg_tmp_3 < u64vector_get_count(avec); _ngg_tmp_3 += 1) {
		int _ngg_tmp_4;
		uint64_t b;
		a = u64vector_get_item(avec, _ngg_tmp_3);
		for(_ngg_tmp_4 = 0; _ngg_tmp_4 < u64vector_get_count(bvec); _ngg_tmp_4 += 1) {
			b = u64vector_get_item(bvec, _ngg_tmp_4);
			append_nondup(vec, a * b);
		}
	}

	u64vector *_tmp_1;
	_tmp_1 = vec;
	vec = NULL;
	return _tmp_1;
}

_Bool left_subset_of_right(u64vector *l, u64vector *r)
{
	int _ngg_tmp_5;
	uint64_t li;
	for(_ngg_tmp_5 = 0; _ngg_tmp_5 < u64vector_get_count(l); _ngg_tmp_5 += 1) {
		int _ngg_tmp_6;
		uint64_t ri;
		li = u64vector_get_item(l, _ngg_tmp_5);
		_Bool found = false;

		for(_ngg_tmp_6 = 0; _ngg_tmp_6 < u64vector_get_count(r); _ngg_tmp_6 += 1) {
			ri = u64vector_get_item(r, _ngg_tmp_6);
			if(li == ri) {
				found = true;
				break;
			}
		}

		if(!found) {
			return false;
		}
	}

	return true;
}

void printvec(const char * lbl, u64vector *vec)
{
	int _ngg_tmp_7;
	uint64_t x;
	printf("%s = { ", lbl);

	for(_ngg_tmp_7 = 0; _ngg_tmp_7 < u64vector_get_count(vec); _ngg_tmp_7 += 1) {
		x = u64vector_get_item(vec, _ngg_tmp_7);
		printf("%zu, ", x);
	}

	puts(" }");
}

_ngg_tuple_isoptimal isoptimal(tnum P, tnum Q)
{
	tnum linprod = tnum_mul(P, Q);
	tnum myprod = my_tnum_mul(P, Q);

	u64vector *gamma_linprod = getgamma(linprod);
	u64vector *gamma_myprod = getgamma(myprod);
	u64vector *gamma_P = getgamma(P);
	u64vector *gamma_Q = getgamma(Q);
	u64vector *exactprods = mulvec(gamma_P, gamma_Q);
	tnum optprod = getalpha(exactprods);

	u64vector *gamma_optprod = getgamma(optprod);

	print_tnum("P", P);
	print_tnum("Q", Q);
	print_tnum("linprod", linprod);
	print_tnum("myprod", myprod);
	print_tnum("optprod", optprod);

	printf("count(gamma_linprod) = %d\n", u64vector_get_count(gamma_linprod));
	printf("count(gamma_myprod) = %d\n", u64vector_get_count(gamma_myprod));
	printf("count(gamma_P) = %d\n", u64vector_get_count(gamma_P));
	printf("count(gamma_Q) = %d\n", u64vector_get_count(gamma_Q));
	printf("count(exactprods) = %d\n", u64vector_get_count(exactprods));
	printf("count(gamma_optprod) = %d\n", u64vector_get_count(gamma_optprod));

	printvec("gamma_linprod", gamma_linprod);
	printvec("gamma_myprod", gamma_myprod);
	printvec("gamma_P", gamma_P);
	printvec("gamma_Q", gamma_Q);
	printvec("exactprods", exactprods);
	printvec("gamma_optprod", gamma_optprod);

	assert(!(u64vector_get_count(gamma_myprod) < u64vector_get_count(exactprods))); /* main.ngg:151 */

	assert(u64vector_get_count(gamma_optprod) <= u64vector_get_count(gamma_myprod)); /* main.ngg:154 */

	assert(left_subset_of_right(exactprods, gamma_myprod)); /* main.ngg:157 */

	_Bool optimal = true;

	if(tnums_differ(myprod, optprod)) {
		optimal = false;
		puts("^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^ Suboptimal!");
	}

	puts("");

	MineVsKernel mine_vs_kernel = MINE_VS_KERNEL_WORSE;

	if(u64vector_get_count(gamma_myprod) < u64vector_get_count(gamma_linprod)) {
		mine_vs_kernel = MINE_VS_KERNEL_BETTER;
	} else if(u64vector_get_count(gamma_myprod) == u64vector_get_count(gamma_linprod)) {
		mine_vs_kernel = MINE_VS_KERNEL_SAME;
	}

	if(gamma_linprod) {
		u64vector_destruct(gamma_linprod);
		free(gamma_linprod);
	}

	if(gamma_myprod) {
		u64vector_destruct(gamma_myprod);
		free(gamma_myprod);
	}

	if(gamma_P) {
		u64vector_destruct(gamma_P);
		free(gamma_P);
	}

	if(gamma_Q) {
		u64vector_destruct(gamma_Q);
		free(gamma_Q);
	}

	if(exactprods) {
		u64vector_destruct(exactprods);
		free(exactprods);
	}

	if(gamma_optprod) {
		u64vector_destruct(gamma_optprod);
		free(gamma_optprod);
	}

	return (_ngg_tuple_isoptimal){optimal, mine_vs_kernel};
}

int main(int argc, char * *argv)
{
	size_t _ngg_tmp_8;
	int bits;
	int BITS[1] = {4};

	int bettercount = 0;
	int samecount = bettercount;
	int worsecount = bettercount;

	for(_ngg_tmp_8 = 0u; _ngg_tmp_8 < (sizeof BITS / sizeof BITS[0]); _ngg_tmp_8 += (1u)) {
		size_t xm;
		bits = BITS[_ngg_tmp_8];
		unsigned int maxnum = (unsigned int) (pow(2, bits) - 1);
		_Bool optimal_for_bits = true;

		#pragma omp parallel for
		for(xm = 0u; xm <= maxnum; xm += 1) {
			size_t xv;
			for(xv = 0u; xv <= maxnum; xv += 1) {
				size_t ym;
				tnum x = TNUM(xv, xm);
				if(!wellformed(x)) {
					continue;
				}

				for(ym = 0u; ym <= maxnum; ym += 1) {
					size_t yv;
					for(yv = 0u; yv <= maxnum; yv += 1) {
						tnum y = TNUM(yv, ym);
						if(!wellformed(y)) {
							continue;
						}

						_ngg_tuple_isoptimal _ngg_tmp_0 = isoptimal(x, y);
						MineVsKernel mine_vs_kernel = _ngg_tmp_0.m1;
						_Bool optimal = _ngg_tmp_0.m0;
						if(!optimal) {
							optimal_for_bits = false;
						}

						switch(mine_vs_kernel) {
						case MINE_VS_KERNEL_BETTER:
						{
							bettercount += 1;
							break;
						}
						case MINE_VS_KERNEL_SAME:
						{
							samecount += 1;
							break;
						}
						case MINE_VS_KERNEL_WORSE:
						{
							worsecount += 1;
							break;
						}
						}
					}
				}
			}
		}

		printf("optimal for %d bit(s) = %d\n", bits, optimal_for_bits);
		puts("Mine vs kernel: ");
		printf("  better = %d\n", bettercount);
		printf("  same   = %d\n", samecount);
		printf("  worse  = %d\n", worsecount);
	}

	return 0;
}

_ngg_tuple_isoptimal _ngg_tuple_isoptimal_default()
{
	_ngg_tuple_isoptimal s;
	s.m0 = false;
	s.m1 = MINE_VS_KERNEL_BETTER;
	return s;
}
