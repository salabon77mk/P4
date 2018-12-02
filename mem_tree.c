// AUTHORS
// MYKOLA KUSYY
// GARRETT MCLAUGHLIN
#include <stdlib.h>
#include <stdio.h>
#include "mem_tree.h"

// TODO 

enum Comparator{
	LESS,
	EQUAL,
	GREATER
};

static struct Node* root = NULL;

static void flipColors(struct Node* parent);
static int isRed(struct Node* node);
static struct Node* createNode(void* ptr, size_t size);
static struct Node* rotateLeft(struct Node* parent);
static struct Node* rotateRight(struct Node* parent);
static struct Node* moveRedLeft(struct Node* node);
static struct Node* moveRedRight(struct Node* node);

static struct Node* insertNode(struct Node* node, void* ptr, size_t size );

static void* searchHelp(struct Node* node, void* key);
static int contains(void* ptr);
static struct Node* balance(struct Node* node);


static struct Node* delete(struct Node* parent, void* ptr);
static struct Node* deleteMin();
static struct Node* min(struct Node* parent);
static enum Comparator compare(void* val1, void* val2);

static void nullCheck(void* ptr);
static void preorderPrintHelp(struct Node* node);
static void inorderPrintHelp(struct Node* node);
static void printCont(struct Node* node);

static struct Range* createRange(struct Node** nodes, size_t size);
static void 
fillOut(struct Node** nodeArr, size_t* size, size_t* index, struct Node* node,  void* low, void* high);
static void* doubleAllocatedMem(void* data, size_t* currSize, size_t typeSize);

struct Node* createNode(void* key, size_t size){
	struct Node* node = (struct Node*) malloc(sizeof(struct Node));
	nullCheck(node);

	node->state = ALLOCATED; //defaults
	node->color = RED; //defaults
	node->start_addr = key;
	node->length = size;
	node->end_addr = node->start_addr + size;
	node->left = NULL;
	node->right = NULL;

	return node;
}

enum State getState(struct Node* node){
	return node->state;
}

void setState(struct Node* node, enum State newState){
	node->state = newState;
}

void deleteNode(void* key){
	if(!contains(key)){
		return;
	}

	if(!isRed(root->left) && !isRed(root->right)){
		root->color = RED;
	}
	
	root = delete(root, key);

	if(root != NULL){
		root->color = BLACK;
	}
}

// TODO MEMORY DESTRUCTION
static struct Node* delete(struct Node* parent, void* key){
	if(compare(key, parent->start_addr) == LESS){
		if(parent->left != NULL){
			if(!isRed(parent->left) && !isRed(parent->left->left)){
				parent = moveRedLeft(parent);
			}
			parent->left = delete(parent->left, key);
		}
	}
	else{
		if(isRed(parent->left)){
			parent = rotateRight(parent);
		}
		if(compare(key, parent->start_addr) == EQUAL && (parent->right == NULL)){
			return NULL;
		}
		if(parent->right != NULL){
			if(!isRed(parent->right) && !isRed(parent->right->left)){
				parent = moveRedRight(parent);
			}
		}
		if(compare(key, parent->start_addr) == EQUAL){
		
			struct Node* node = min(parent->right);
			parent->start_addr = node->start_addr;		
			parent->length = node->length;
			parent->state = node->state; // ????
			parent->end_addr = node->end_addr;

			parent->right = deleteMin(parent->right);
		
		}
		else parent->right = delete(parent->right, key);
	}

	return balance(parent);
}

/* Uncomment later if needed
void deleteMin() {
	if (!isRed(root->left) && !isRed(root->right))
		root->color = RED;

	root = deleteMin(root);
	if (root != NULL) root->color = BLACK;
}
*/

static struct Node* deleteMin(struct Node* parent){
	if (parent->left == NULL){
		free(parent->start_addr);
		parent->start_addr = NULL;
		parent->end_addr = NULL;
		free(parent);
		parent = NULL;
		return NULL;
	}
	
	if (!isRed(parent->left) && !isRed(parent->left->left))
		parent = moveRedLeft(parent);

	parent->left = deleteMin(parent->left);
	return balance(parent);
}

static struct Node* min(struct Node* parent){
	if (parent->left == NULL) return parent;
	else return min(parent->left);
}

static struct Node* balance(struct Node* node){
	if(isRed(node->right)){
		node = rotateLeft(node);
	}
	if(isRed(node->left) && isRed(node->left->left)){
		node = rotateRight(node);
	}
	if(isRed(node->left) && isRed(node->right)){
		flipColors(node);
	}
	return node;
}

void insert(void* ptr, size_t size){
	root = insertNode(root, ptr, size);
	root->color = BLACK;
}

static struct Node* insertNode(struct Node* node, void* ptr, size_t size){
	if(node == NULL){
		return createNode(ptr, size);
	}
	
	enum Comparator cmp = compare(ptr, node->start_addr);
	if (cmp == LESS) node->left = insertNode(node->left, ptr, size);
	else if (cmp == GREATER) node->right = insertNode(node->right, ptr, size);
//	else node->length = size;


	if(isRed(node->right) && !isRed(node->left)){
		node = rotateLeft(node);
	}
	if(isRed(node->left) && isRed(node->left->left)){
		node = rotateRight(node);
	}
	if(isRed(node->left) && isRed(node->right)){
		flipColors(node);			
	}

	return node;
}

static enum Comparator compare(void* val1, void* val2){
	if(val1 < val2){
		return LESS;
	}
	else if(val1 > val2){
		return GREATER;
	}
	
	return EQUAL;
	
}

struct Node* search(void* key){
	return searchHelp(root, key);
}

static void* searchHelp(struct Node* node, void* key){
	while (node != NULL) {
		enum Comparator cmp = compare(key, node->start_addr);

		if (cmp == LESS){       
			node = node->left;
		}
		else if (cmp == GREATER){ 
			node = node->right;
		}
		else{
		       	return node;
		}
	}
	return NULL;
}

static int contains(void* key){
	struct Node* node = search(key);
	if(node != NULL){
		return 1;
	}
	return 0;
}

static int isRed(struct Node* node){
	if(node == NULL){
		return 0;
	}

	if(node->color == RED){
		return 1;
	}
	return 0;
}

static struct Node* rotateLeft(struct Node* parent){
	struct Node* swap = parent->right;
	parent->right = swap->left;
	swap->left = parent;
	swap->color = parent->color;
	parent->color = RED;
	return swap;
}

static struct Node* rotateRight(struct Node* parent){
	struct Node* swap = parent->left;
	parent->left = swap->right;
	swap->right = parent;
	swap->color = parent->color;
	parent->color = RED;
	return swap;
}

static struct Node* moveRedLeft(struct Node* node){
	flipColors(node);
	if(node->right != NULL){
		if(isRed(node->right->left)){
			node->right = rotateRight(node->right);
			node = rotateLeft(node);
			flipColors(node);
		}
	}
	return node;
}

static struct Node* moveRedRight(struct Node* node){
	flipColors(node);
	if(node->left != NULL){
		if(isRed(node->left->left)){
			node = rotateRight(node);
			flipColors(node);
		}
	}
	return node;
}

static void flipColors(struct Node* parent){
	parent->color = RED;
	if(parent->left != NULL){
		parent->left->color = BLACK;
	}

	if(parent->right != NULL){
		parent->right->color = BLACK;
	}
}

static void nullCheck(void* ptr){
	if(ptr == NULL){
		fprintf(stderr, "allocation failed in mem_tree.c");
		exit(-1);	
	}
}


void preorderPrint(){
	preorderPrintHelp(root);	
}

static void preorderPrintHelp(struct Node* node){
	if(node == NULL){
		return;
	}

	printCont(node);
	preorderPrintHelp(node->left);
	preorderPrintHelp(node->right);
}

void inorderPrint(){
	inorderPrintHelp(root);
}


static void inorderPrintHelp(struct Node* node){
	if(node == NULL){
		return;
	}

	inorderPrintHelp(node->left);
	printCont(node);
	inorderPrintHelp(node->right);
}

static void printCont(struct Node* node){
 	printf("START ADDRESS %p\n", node->start_addr);
	printf("END ADDDRESS %p\n", node->end_addr);
	printf("SIZE %zu\n", node->length);
	printf("STATE %d WHERE FREE = 0, ALLOC = 1\n", node->state);
	printf("\n");
}


static struct Range* createRange(struct Node** nodes, size_t size){
	struct Range* rangeNodes = (struct Range*) malloc(sizeof(struct Range) * size);
	nullCheck(rangeNodes);
	rangeNodes->nodes = nodes;
	rangeNodes->num_nodes = size;
	return rangeNodes;
}

void destroyRange(struct Range** range){
	free((*range)->nodes);
	(*range)->nodes = NULL;
	free(*range);
	*range = NULL;
}

struct Range* rangeSearch(void* low, void* high){
	size_t size = 1024;
	size_t index = 0;

	struct Node** nodes = (struct Node**) malloc(sizeof(struct Node*) * size);
	nullCheck(nodes);

	fillOut(nodes, &size, &index, root, low, high);
	if(index == 0){
		free(nodes);
		nodes = NULL;
		return NULL;
	}

	struct Range* result = createRange(nodes, index);
	return result;
}

static void 
fillOut(struct Node** nodeArr, size_t* size, size_t* index, struct Node* node, void* low, void* high){
	if(node == NULL){
		return;
	}
	if(*index >= *size){
		nodeArr = (struct Node**) doubleAllocatedMem(nodeArr, size, sizeof(struct Node*));
	}
	
	if(low < node->start_addr && node->left != NULL){
		fillOut(nodeArr, size, index, node->left, low, high);
	}
	if(!(high < node->start_addr) && !(low > node->end_addr)){
		nodeArr[(*index)] = node;
		(*index)++;
		(*size)++;
	}
	if(high > node->end_addr && node->right != NULL){
		fillOut(nodeArr, size, index, node->right, low, high);
	}

}

static void* doubleAllocatedMem(void* data, size_t* currSize, size_t typeSize){
	void* newData =  realloc(data, ((*currSize) * 2) * typeSize);
	if( newData == NULL){
		fprintf(stderr, "Failed to realloc in mem_manage.c:doubleAllocatedMem");
		exit(-1);
	}
	*currSize = (*currSize) * 2;
	data = newData;
	return data;
}
