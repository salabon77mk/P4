// AUTHORS
// MYKOLA KUSYY
// GARRETT MCLAUGHLIN
#ifndef MEM_TREE_H
#define MEM_TREE_H


#include <stdlib.h>


enum State {FREE, ALLOCATED};
enum Color {RED, BLACK};

struct Range{
	struct Node** nodes;
	size_t num_nodes;
};

struct Node{
	enum State state;
	enum Color color;
	void* start_addr;
	size_t length;
	void* end_addr;
	struct Node* left;
	struct Node* right;
};

void deleteNode(void* ptr);
void insert(void* ptr, size_t size);

struct Range* rangeSearch(void* low, void* high);
struct Node* search(void* key);
enum State getState(struct Node* node);
void setState(struct Node* node, enum State newState);
void destroyRange(struct Range** range);

void preorderPrint();
void inorderPrint();
//split??
#endif
