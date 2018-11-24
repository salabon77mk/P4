#include <stdlib.h>
#include <stdio.h>
#include "mem_tree.h"

struct Node* createNode(size_t addr, size_t size){
	struct Node* node = (struct Node*) malloc(sizeof(struct Node));

	if(node == NULL){
		fprintf(stderr, "malloc failed in mem_tree.c:createNode");
		exit(-1);
	}

	node->start_addr = addr;
	node->length = size;
	node->end_addr = addr + size;
	node->left = NULL;
	node->right = NULL;
	node->color = RED; //defaults
	node->status = FREE; //defaults

	return node;
}
