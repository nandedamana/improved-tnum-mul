typedef struct u64vector u64vector;
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
struct u64vector {
	uint64_t *arr;
	int alloccount;
	int count;
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
