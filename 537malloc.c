// AUTHORS
// MYKOLA KUSYY
// GARRETT MCLAUGHLIN
#include "mem_tree.h"
#include <stdlib.h>
#include <stdio.h>


// TODO: Add node to tree

static void singleNodeCase(struct Node* node, void* start, void* end, size_t size);
static void checkAllFree(struct Range* range);
static void* reallocWrapper(void* ptr, size_t size);

void* malloc537(size_t size){
	if (size == 0) {
		fprintf(stderr, "Attempting to call malloc on size of 0 bytes\n");
		return NULL;
	} 

	void* mem = malloc(size);
        void* max = mem + size;	
	if (mem == NULL) {
		fprintf(stderr, "Malloc failed in 537malloc.c:malloc537 method\n");
		exit(-1);		
	}
 	struct Range* range = rangeSearch(mem, max);
	if(range == NULL){ //special case, no nodes so we just insert
		insert(mem, size);
		return mem;
	}
	checkAllFree(range);
	//now we have to handle the nodes
	for(size_t i = 1; i < range->num_nodes; i++){ //delete all but the first one
		deleteNode(range->nodes[i]->start_addr);
	}
	singleNodeCase(range->nodes[0], mem, max, size);
	destroyRange(&range);
	return mem;
}

void free537(void* ptr){
	if(ptr == NULL){	
		fprintf(stderr, "Cannot free a NULL pointer");
		exit(-1);
	}

	struct Node* node = search(ptr);
	if(node == NULL){
		fprintf(stderr, "Memory at %p has not been allocated with malloc537\n", ptr);
		exit(-1);
	}	

	// do range search then do a search again
	/* struct Range* range = rangeSearch(node->start_addr, node->end_addr);
	 * if(range->nodes[0]->start_addr != ptr){
	 *	fprintf(stderr, "Attempting to not free the first byte allocated by malloc537\n");
	 * 	exit(-1);
	 * }
	 * destroyRange(range);
	 */


	if(getState(node) == FREE){
		fprintf(stderr, "Double free attempt detected at %p\n", ptr);
		exit(-1);
	}

	free(node->start_addr);
	setState(node, FREE);
}

void* realloc537(void* ptr, size_t size){
	if(ptr == NULL){
		void* mem = malloc537(size);
		return mem;
	}
	else if(size == 0){
		free537(ptr);
		return NULL;
	}

	//otherwise do a reallocation
	struct Node* node = search(ptr);
	if(node != NULL){
		deleteNode(ptr);
	}
	ptr = reallocWrapper(ptr, size);
	insert(ptr, size);
	
	return ptr;
}

void memcheck537(void* ptr, size_t size){
	void* max = ptr + size;
	struct Range* range = rangeSearch(ptr, max);
	if(range == NULL){
		fprintf(stderr, "Memory range from %p to %p not allocated with malloc537\n", ptr, max);
		exit(-1);
	}

	for(size_t i = 0; i < range->num_nodes; i++){
		if(getState(range->nodes[i]) == FREE){
			fprintf(stderr, "Memory range from %p to %p lies within freed memory\n", ptr, max);
			exit(-1);
		}
	}
	destroyRange(&range);
}

static void singleNodeCase(struct Node* node, void* start_mem, void* end_mem, size_t size){
	if(getState(node) == ALLOCATED){
		fprintf(stderr, "Attempting to overwrite allocated memory at %p\n", start_mem);
		exit(-1);
	}

	//just restructure the current node since it's within the range	
	if(end_mem < node->end_addr){
		node->start_addr = start_mem;
	        node->end_addr = end_mem;
		node->length = (size_t)(end_mem - start_mem);	
		setState(node, ALLOCATED);
	}
	//otherwise there's overlap, need to split the nodes
	else{
		node->end_addr = start_mem - 1;
		node->length = (size_t)(node->end_addr - node->start_addr);
		insert(start_mem, size);
	}

}

static void checkAllFree(struct Range* range){	
	for(size_t i = 0; i < range->num_nodes; i++){ //make sure all nodes in range are free
		if(getState(range->nodes[i]) == ALLOCATED){
			fprintf(stderr, "Attempting to overwrite allocated node(s) starting at %p\n", range->nodes[i]->start_addr);
			exit(-1);
		}
	}	
}
static void* reallocWrapper(void* ptr, size_t size){
	void* newData = realloc(ptr, size);	
	if( newData == NULL){
		fprintf(stderr, "Failed to realloc in 537malloc.c:reallocWrapper");
		exit(-1);
	}
	ptr = newData;
	return ptr;
}




	/*
	if(range->num_nodes == 1){ //special case
		singleNodeCase(range->nodes[0], mem, max, size);
	}
	else{ //multiple nodes to handle
		checkAllFree(range);
		//now we have to handle the nodes
		for(size_t i = 1; i < range->num_nodes; i++){ //delete all but the first one
			delete(range->nodes[i]);
		}
		singleNodeCase(range->nodes[0], mem, max, size);
	}
	destroyRange(&range);
	return mem;
	*/
