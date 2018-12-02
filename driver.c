// AUTHORS
// MYKOLA KUSYY
// GARRETT MCLAUGHLIN
#include "mem_tree.h"
#include <stdio.h>
#include <stdlib.h>

int main(void){
	int nodes = 10;
	void** arr = malloc(sizeof(void*) * nodes);
	size_t size = 1;
	for(int i = 0; i < nodes; i++){
		void* ptr = malloc(sizeof(size));
		arr[i] = ptr;
		printf("CURR VAL AT %d:  %p\n", i, ptr);
		insert(ptr, size);
		size++;	
	}
	printf("\n");
	/*
	for(int i = 0; i < nodes; i++){
		void* ptr = search(arr[i]);
		printf("SEARCH VALUE AT %d: %p\n %p\n", i, ptr, arr[i]);
	}
	*/

	struct Range* range = rangeSearch(arr[0], arr[9]);
	for(size_t i = 0; i < range->num_nodes; i++){
		printf("NODE %zu's VALUE: %p\n", i, range->nodes[i]->start_addr);
	}

//	preorderPrint();
}
