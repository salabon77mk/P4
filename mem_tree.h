// AUTHORS
// MYKOLA KUSYY
// GARRETT MCLAUGHLIN
#ifndef MEM_TREE_H
#define MEM_TREE_H


#include <stdlib.h>


enum State {FREE, ALLOCATED};

void deleteNode(struct Node* node);
void insert(void* ptr, size_t size);

struct Range* rangeSearch(void* ptr, size_t size);
struct Node* search(void* key);
enum State getState(void* key);
void setState(size_t addr, enum State newState);

void printTree(struct Node* root);
//split??
#endif
