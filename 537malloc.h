#ifndef 537MALLOC_H
#define 537MALLOC_H

#include <stdlib.h>

void* malloc537(void* ptr);
void free537(void* ptr);
void* realloc537(void* ptr, size_t size);
void memcheck537(void* ptr, size_t size);


#endif
