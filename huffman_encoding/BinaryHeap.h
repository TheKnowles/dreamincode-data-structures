#ifndef BINARYHEAP_H
#define BINARYHEAP_H
#include "HuffmanNode.h"

class BinaryHeap{
private:
	HuffmanNode** storage;
	int heapSize;
	int capacity;

	void percUp(int);
	void percDown(int);
	int numChildren(int);
	
	void inOrder(int);
	void postOrder(int);
	void preOrder(int);
public:
	BinaryHeap(int);
	~BinaryHeap();
	void insert(HuffmanNode*);
	HuffmanNode* removeMin();
	HuffmanNode peekMin();

	bool isEmpty();
	int getHeapSize()							{return heapSize;};
	HuffmanNode* getRoot()						{return storage[0];};
	//traversal
	//not really necessary for Huffman, left for debug purposes
	void inOrderTraversal();
	void postOrderTraversal();
	void preOrderTraversal();
	void linearDisplay();
	//debug
	void emptyHeap();
};
#endif


