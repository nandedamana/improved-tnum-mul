#include <assert.h>
#include <stdarg.h>
#include <stdbool.h>

#include "nggvector.h"


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
