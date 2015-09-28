#ifndef HEAPNODE_H
#define HEAPNODE_H

#include <iostream>
using namespace std;

template<class T>
class HeapNode{
private:
	T data;
public:
	HeapNode();
	HeapNode(T);
	~HeapNode();
	T getData()						{return data;};
};

//*****IMPLEMENTATION******
template<class T>
HeapNode<T>::HeapNode(){
	data = NULL;
}

template<class T>
HeapNode<T>::HeapNode(T data){
	this->data = data;
}

template<class T>
HeapNode<T>::~HeapNode(){
}
#endif


