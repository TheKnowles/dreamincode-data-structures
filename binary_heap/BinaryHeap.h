#ifndef BINARYHEAP_H
#define BINARYHEAP_H
#include "HeapNode.h"

//MAX HEAP
template<class T>
class BinaryHeap{
private:
	HeapNode<T>** storage; //array backed heap
	int arrSize;
	int heapSize;

	//helper functions
	void percolateUp(int);
	void percolateDown(int);
	void inOrder(int);
	void postOrder(int);
	void preOrder(int);
	int numChildren(int);

	bool isEmpty();
public:
	BinaryHeap(int);
	~BinaryHeap();

	//heap operations
	void insertNode(T);
	T removeMax(); //or pop()?


	//traversal
	void inOrderTraversal();
	void postOrderTraversal();
	void preOrderTraversal();
	void linearDisplay();
};

//******IMPLEMENTATION********

template<class T>
BinaryHeap<T>::BinaryHeap(int size){
	storage = new HeapNode<T>*[size];
	for(int i = 0; i < size; i++) storage[i] = NULL;
	arrSize = size;
	heapSize = 0;
}

template<class T>
BinaryHeap<T>::~BinaryHeap(){
	//used for debugging, could just use heapSize as the conditional
	int usedCount = heapSize;
	for(int i = 0; i < usedCount; i++){
		if(storage[i] != NULL) delete storage[i]; //free each node, if applicable
		heapSize--;
	}
	//for debug purposes
	cout << "heap size is (should be zero): " << heapSize << endl;
	delete[] storage;
}

template<class T>
void BinaryHeap<T>::insertNode(T data){
	if(heapSize == arrSize){
		cerr << "No more room in this heap!" << endl;
		return;
	}

	HeapNode<T>* temp = new HeapNode<T>(data);
	//insert at end
	heapSize++;
	storage[heapSize-1] = temp;
	percolateUp(heapSize-1);
}

template<class T>
void BinaryHeap<T>::percolateUp(int curIndex){
	int parentIndex;
	HeapNode<T>* temp;
	if(curIndex != 0) { //are we at the "top"?
		parentIndex = (curIndex-1)/2;
		//max heap
		//if the parent is "less then" the child, it violates our heap property
		//perk up!
		if(storage[parentIndex]->getData() < storage[curIndex]->getData()){
			temp = storage[parentIndex];
			storage[parentIndex] = storage[curIndex];
			storage[curIndex] = temp;
			percolateUp(parentIndex);
		}
	}
}


template<class T>
T BinaryHeap<T>::removeMax(){
	if(isEmpty()) {
		cout << "Heap is empty!" << endl;
		return NULL;
	}
	T value = storage[0]->getData();
	HeapNode<T>* temp = storage[0];
	//replace max with last filled node slot (heapSize-1)
	//perc down
	storage[0] = storage[heapSize-1];
	storage[heapSize-1] = temp;
	delete storage[heapSize-1]; //free
	storage[heapSize-1] = NULL; //avoid dangling pointers
	heapSize--;
	percolateDown(0);
	return value;
}

template<class T>
void BinaryHeap<T>::percolateDown(int curIndex){
	int children = 0;
	if(children = numChildren(curIndex)){ //are we at the "bottom"?
		HeapNode<T>* temp;
		int left = 2*curIndex+1, right = 2*curIndex+2;
		switch(children){ //1 child or 2 children
			case 1:
				//if the child is larger, perk the current node down
				if(storage[curIndex]->getData() < storage[left]->getData()){
					temp = storage[curIndex];
					storage[curIndex] = storage[left];
					storage[left] = temp;
					percolateDown(left);
				}
				break;
			case 2:
				//2 children, determine which is larger, perk down that direction
				if((storage[curIndex]->getData() < storage[left]->getData()) || 
							(storage[curIndex]->getData() < storage[right]->getData())){
					//left child is greater
					if((storage[left]->getData() > storage[right]->getData())){
						temp = storage[curIndex];
						storage[curIndex] = storage[left];
						storage[left] = temp;
						percolateDown(left);
					}
					else{ //right child is greater
						temp = storage[curIndex];
						storage[curIndex] = storage[right];
						storage[right] = temp;
						percolateDown(right);
					}
				}
				break;
			default:
				cout << "wtf? this shouldn't happen" << endl;
				break;
		}
	}
	return;
}

template<class T>
void BinaryHeap<T>::inOrderTraversal(){
	if(isEmpty()){
		cout << "Heap is empty!" << endl;
		return;
	}
	inOrder(0);
	cout << endl;
}

template<class T>
void BinaryHeap<T>::inOrder(int curIndex){
	//left
	//cur
	//right
	if(((2*curIndex+1) < arrSize) && (storage[2*curIndex+1] != NULL)) inOrder(2*curIndex+1);
	cout << storage[curIndex]->getData() << " ";
	if(((2*curIndex+2) < arrSize) && (storage[2*curIndex+2] != NULL)) inOrder(2*curIndex+2);
}

template<class T>
void BinaryHeap<T>::postOrderTraversal(){
	if(isEmpty()){
		cout << "Heap is empty!" << endl;
		return;
	}
	postOrder(0);
	cout << endl;
}

template<class T>
void BinaryHeap<T>::postOrder(int curIndex){
	//left
	//right
	//cur
	if(((2*curIndex+1) < arrSize) && (storage[2*curIndex+1] != NULL)) inOrder(2*curIndex+1);
	if(((2*curIndex+2) < arrSize) && (storage[2*curIndex+2] != NULL)) inOrder(2*curIndex+2);
	cout << storage[curIndex]->getData() << " ";
}

template<class T>
void BinaryHeap<T>::preOrderTraversal(){
	if(isEmpty()){
		cout << "Heap is empty!" << endl;
		return;
	}
	preOrder(0);
	cout << endl;
}

template<class T>
void BinaryHeap<T>::preOrder(int curIndex){
	//cur
	//left
	//right
	cout << storage[curIndex]->getData() << " ";
	if(((2*curIndex+1) < arrSize) && (storage[2*curIndex+1] != NULL)) inOrder(2*curIndex+1);
	if(((2*curIndex+2) < arrSize) && (storage[2*curIndex+2] != NULL)) inOrder(2*curIndex+2);
}


template<class T>
void BinaryHeap<T>::linearDisplay(){
	for(int i = 0; i < heapSize; i++){
		cout << storage[i]->getData() << " ";
	}
	cout << endl;
}

//handy for percolating, saves us some long conditional checks
template<class T>
int BinaryHeap<T>::numChildren(int index){
	int children = 0, left = 2*index+1, right = 2*index+2;
	if(left >= heapSize) return 0;
	if(storage[left] != NULL) children++;
	if (storage[right] != NULL) children++;
	return children;
}


template<class T>
bool BinaryHeap<T>::isEmpty(){
	return (heapSize == 0);
}
#endif


