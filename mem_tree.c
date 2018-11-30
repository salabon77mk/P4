// AUTHORS
// MYKOLA KUSYY
// GARRETT MCLAUGHLIN
# include <stdlib.h>
#include <stdio.h>
#include "mem_tree.h"

// TODO: search, min, getMin, delete, part of insert
// TODO 

static enum Color {RED, BLACK};
static enum Comparator{
	LESS,
	EQUAL,
	GREATER
};

struct Node{
	enum State state;
	enum Color color;
	unsigned int height; //debug purps
	void* start_addr;
	size_t length;
	void* end_addr;
	struct Node* left;
	struct Node* right;
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
static struct Node* searchHelp(struct Node* node, size_t addr); //convert to size_t in 537malloc.c
static struct Node* balance(struct Node* node);


static struct Node* delete(struct Node* parent, void* ptr);
static struct Node* deleteMin();
static struct Node* min(struct Node* parent);

static void nullCheck(void* ptr);
static struct Range_Nodes* createRange(struct Node** nodes, size_t size);
static int compare(size_t val1, size_t val2);

// pointer indirection
// low = key value
static struct void fillOut
(struct Nodes*** nodes, size_t* size, size_t* index, struct Node* currNode, size_t low, size_t max); 

struct Node* createNode(void* ptr, size_t size){
	struct Node* node = (struct Node*) malloc(sizeof(struct Node));
	nullCheck(nodes);

	node->status = ALLOCATED; //defaults
	node->color = RED; //defaults
	node->height = 0;
	node->start_addr = ptr
	node->length = size;
	node->end_addr = node->start_addr + size;
	node->left = NULL;
	node->right = NULL;

	return node;
}


void deleteNode(void* key){
	if(!isRed(root->left) && !isRed(root->right)){
		root->color = RED;
	}
	
	root = delete(root, key);

	if(root != NULL){
		root->color = BLACK;
	}
}

// TODO
static struct Node* delete(struct Node* parent, void* key){
	if(compare(key, parent->start_addr) == LESS){
		if(!isRed(parent->left) && !isRed(parent->left->left)){
			parent = moveRedLeft(parent);
		}
		parent->left = delete(parent->left, key);
	}
	else{
		if(){
		}
		if(){
		}
		if(){
		}
		if(){
		/*
		struct Node* node = min(parent->right);
		parent->start_addr = node->start_addr;		
		parent->length = node->length;
		parent->status = node->status;?????

		parent->right = deleteMin(parent->right);
		*/

		}
		else parent->right = delete(parent->right, key);
	}

	return balance(parent);
}
static struct Node* deleteMin(){

}

static struct Node* min(struct Node* parent){

}

static struct Node* balance(struct Node* node){
	if(isRed(node->right)){
		node = rotateLeft(node);
	}
	if(isRed(node->left) && isRed(node->left->left)){
		node = rotateRight(node);
	}
	if(isRed(node->left) && isRed(node->right)){
		node = flipColors(node);
	}
	return node;
}

void insert(void* ptr, size_t size){
	root = insertNode(root, ptr, size);
	root->color = BLACK;
}

static struct Node* insertNode(struct Node* node, void* ptr, size_t size ){
	if(node == NULL){
		return createNode(void* ptr, size_t size);
	}
	//enum Comparator comp = compare();
	if(isRed(node->right) && !isRed(node->left)){
		node = rotateLeft(node);
	}
	if(isRed(node->left) && isRed(node->left->left)){
		node = rotateRight(node);
	}
	if(isRed(node->left) && isRed(node->right)){
		node = flipColors(node);			
	}

	return node;
}

static enum Comparator compare(void* ptr1, void* ptr2){
	if(val1 < val2){
		return LESS;
	}
	else if(val1 > val2){
		return GREATER;
	}
	else{
		return EQUAL;
	}
}

struct Node* search(size_t addr){

}

static struct Node* searchHelp(struct Node* node, size_t addr){
	
}

static int isRed(struct Node* node){
	if(node->color == RED){
		return 1;
	}
	return 0;
}

static struct Node* rotateLeft(struct Node* parent){
	struct Node* swap = parent.right;
	parent.right = swap.left;
	swap.left = parent;
	swap.color = parent.color;
	parent.color = RED;
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
	if(isRed(node->right->left)){
		node->right = rotateRight(node->right);
		node = rotateLeft(node);
		flipColors(node);
	}
	return node;
}

static struct Node* moveRedRight(struct Node* node){
	flipColors(node);
	if(isRed(node->left->left)){
		node = rotateRight(node);
		flipColors(node);
	}
	return node;
}

static void flipColors(struct Node* parent){
	parent->color = RED;
	parent->left->color = BLACK;
	parent->right->color = BLACK;

}

void printTree(struct Node* root){

}

static void nullCheck(void* ptr){
	if(ptr == NULL){
		fprintf(stderr, "allocation failed in mem_tree.c");
		exit(-1);	
	}
}

enum State getState(void* key){
	struct Node* node = search(key);
	return node->state;
}

void setState(void* key, enum State newState){
	struct Node* node = search(key);
	node->state = newState;
}
/*
static struct Range_Nodes* createRange(struct Node** nodes, size_t size){
	struct Range_Nodes* rangeNodes = (struct Range_Nodes*) malloc(sizeof(struct Range_Nodes) * size);
	nullCheck(rangeNodes);
	rangeNodes->nodes = nodes;
	rangeNodes->size = size;
	return rangeNodes;
}
struct Range_Nodes* rangeSearch(struct Node* root, void* min, void* max){
	size_t size = 1024;
	size_t index = 0;

	struct Node** nodes = (struct Node**) malloc(sizeof(struct Node*) * size);
	nullCheck(nodes);

	fillOut(&nodes, &size, &index, root, min, max);

	// implement size checker

	struct Range_Nodes* result = createRange(nodes, currSize);
	return result;
}

static struct Node** 
fillOut(struct Node** nodeArr, size_t* size, size_t* index, struct Node* node,  size_t min, size_t max){
	if(node == NULL){
		return;
	}

	if(*currSize >= *size){
		*size *= 2;
		*nodes = (struct Node**) realloc(sizeof(struct Node*) * (*size));
		nullCheck(*nodes);
	}



	//high low comparisons
}
*/
