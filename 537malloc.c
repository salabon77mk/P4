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

	// ensure the memory region consists of all free nodes
	checkAllFree(range);
	// delete all nodes except for the first one
	for(size_t i = 1; i < range->num_nodes; i++){ //delete all but the first one
		deleteNode(range->nodes[i]->start_addr);
	}

	//then we'll either restructure our node or split it
	singleNodeCase(range->nodes[0], mem, max, size);
	destroyRange(&range);
	return mem;
}

void free537(void* ptr){
	if(ptr == NULL){	
		fprintf(stderr, "Cannot free a NULL pointer\n");
		exit(-1);
	}

	struct Node* node = search(ptr);

	// search returned nothing, which means ptr was not allocated by malloc537
	if(node == NULL){
		fprintf(stderr, "Memory at %p has not been allocated with malloc537\n", ptr);
		exit(-1);
	}	

	if(getState(node) == FREE){
		fprintf(stderr, "Double free attempt detected at %p\n", ptr);
		exit(-1);
	}

	// pass all checks, free the node, change the state
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
	
	// node exists in tree, must be deleted
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
		//edge cases for first and last node
		if(i == 0){
			if(ptr < range->nodes[i]->start_addr){
				fprintf(stderr, "ptr: %p is out of bounds for address the current node at addr: %p\n"
						, ptr, range->nodes[i]->start_addr);
				exit(-1);
			}
		}
		else if(i == range->num_nodes - 1){
			void* max = ptr + size;
			if(max > range->nodes[i]->end_addr){
				fprintf(stderr, "ptr: %p is out of bounds for address the current node at addr: %p\n"
						, ptr, range->nodes[i]->start_addr);
				exit(-1);
			}
		}
	}
	destroyRange(&range);
}

/* Deals only with a single node from tree
 * Error if the node is allocated (trying to overwrite memory)
 * If the new memory region does not go out of bounds for the node, just restructure the current node
 * Otherwise split the node
 */
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

// Checks if all nodes in the range are free
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
