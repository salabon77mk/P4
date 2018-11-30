// AUTHORS
// MYKOLA KUSYY
// GARRETT MCLAUGHLIN
#include "mem_tree.h"
#include <stdlib.h>
#include <stdio.h>


// TODO: Add node to tree
void* malloc537(size_t size){
	if (size == 0) {
		fprintf(stderr, "Attempting to call malloc on size of 0 bytes\n");
	} else {
		test = malloc(size);
		if (test == NULL) {
			fprintf(stderr, "Malloc failed in 537malloc.c:malloc537 method\n");
			exit(-1);		
		} else {
			// It worked!
		}
	}
}

void free537(void* ptr){
	
}

void* realloc537(void* ptr, size_t size){

}

void memcheck537(void* ptr, size_t size){

}
