// AUTHORS
// MYKOLA KUSYY
// GARRETT MCLAUGHLIN
#ifndef _537MALLOC_H
#define _537MALLOC_H

#include <stdlib.h>

void* malloc537(size_t size);
void free537(void* ptr);
void* realloc537(void* ptr, size_t size);
void memcheck537(void* ptr, size_t size);


#endif
