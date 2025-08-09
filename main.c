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
	int _ngg_tmp_0;
	uint64_t y;
	for(_ngg_tmp_0 = 0; _ngg_tmp_0 < u64vector_get_count(vec); _ngg_tmp_0 += 1) {
		y = u64vector_get_item(vec, _ngg_tmp_0);
		if(y == x) {
			return;
		}
	}

	u64vector_append(vec, x);
}

u64vector * mulvec(u64vector *avec, u64vector *bvec)
{
	int _ngg_tmp_1;
	uint64_t a;
	u64vector *vec = (u64vector *) malloc(sizeof(u64vector));
	if(vec == NULL) {
		perror(NULL);
		exit(EXIT_FAILURE);
	}

	u64vector_construct(vec);

	for(_ngg_tmp_1 = 0; _ngg_tmp_1 < u64vector_get_count(avec); _ngg_tmp_1 += 1) {
		int _ngg_tmp_2;
		uint64_t b;
		a = u64vector_get_item(avec, _ngg_tmp_1);
		for(_ngg_tmp_2 = 0; _ngg_tmp_2 < u64vector_get_count(bvec); _ngg_tmp_2 += 1) {
			b = u64vector_get_item(bvec, _ngg_tmp_2);
			append_nondup(vec, a * b);
		}
	}

	u64vector *_tmp_1;
	_tmp_1 = vec;
	vec = NULL;
	return _tmp_1;
}

void printvec(const char * lbl, u64vector *vec)
{
	int _ngg_tmp_3;
	uint64_t x;
	printf("%s = { ", lbl);

	for(_ngg_tmp_3 = 0; _ngg_tmp_3 < u64vector_get_count(vec); _ngg_tmp_3 += 1) {
		x = u64vector_get_item(vec, _ngg_tmp_3);
		printf("%zu, ", x);
	}

	puts(" }");
}

void test(tnum P, tnum Q)
{
	tnum linprod = tnum_mul(P, Q);

	u64vector *gamma_linprod = getgamma(linprod);
	u64vector *gamma_P = getgamma(P);
	u64vector *gamma_Q = getgamma(Q);
	u64vector *optprodvec = mulvec(gamma_P, gamma_Q);

	print_tnum("P", P);
	print_tnum("Q", Q);
	print_tnum("linprod", linprod);

	printf("count(gamma_linprod) = %d\n", u64vector_get_count(gamma_linprod));
	printf("count(gamma_P) = %d\n", u64vector_get_count(gamma_P));
	printf("count(gamma_Q) = %d\n", u64vector_get_count(gamma_Q));
	printf("count(optprodvec) = %d\n", u64vector_get_count(optprodvec));

	printvec("gamma_linprod", gamma_linprod);
	printvec("gamma_P", gamma_P);
	printvec("gamma_Q", gamma_Q);
	printvec("optprodvec", optprodvec);

	assert(!(u64vector_get_count(gamma_linprod) < u64vector_get_count(optprodvec))); /* main.ngg:93 */

	if(u64vector_get_count(gamma_linprod) > u64vector_get_count(optprodvec)) {
		puts("^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^ Suboptimal!");
	}

	puts("");
	if(gamma_linprod) {
		u64vector_destruct(gamma_linprod);
		free(gamma_linprod);
	}

	if(gamma_P) {
		u64vector_destruct(gamma_P);
		free(gamma_P);
	}

	if(gamma_Q) {
		u64vector_destruct(gamma_Q);
		free(gamma_Q);
	}

	if(optprodvec) {
		u64vector_destruct(optprodvec);
		free(optprodvec);
	}
}

int main(int argc, char * *argv)
{
	size_t xm;
	for(xm = 0u; xm <= MAX_U64; xm += 1) {
		size_t xv;
		for(xv = 0u; xv <= MAX_U64; xv += 1) {
			size_t ym;
			tnum x = TNUM(xv, xm);
			if(!wellformed(x)) {
				continue;
			}

			for(ym = 0u; ym <= MAX_U64; ym += 1) {
				size_t yv;
				for(yv = 0u; yv <= MAX_U64; yv += 1) {
					tnum y = TNUM(yv, ym);
					if(!wellformed(y)) {
						continue;
					}

					test(x, y);
				}
			}
		}
	}

	return 0;
}
