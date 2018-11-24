#ifndef MEM_TREE_H
#define MEM_TREE_H


#include <stdlib.h>

enum Color {RED, BLACK};
enum Status {FREE, ALLOCATED};

struct Node{
	enum Status status;
	enum Color color;
	size_t start_addr;
	size_t end_addr; //for ease of use
	size_t length;
	struct Node* left;
	struct Node* right;
};

struct Node* createNode(size_t start_addr, size_t size);
void deleteNode(struct Node* node);
void insert(struct Node* node);
struct Node* search(struct Node* node);

//split??
#endif
