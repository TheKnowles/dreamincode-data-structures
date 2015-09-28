#include <iostream>
using namespace std;

template<class T>
class Node {
private:
	T data;
	Node<T>* left;
	Node<T>* right;
public:
	Node();
	Node(T);
	T getData()						{return data;};
	Node<T>* getLeftChild()			{return left;};
	Node<T>* getRightChild()		{return right;};
	void setLeftChild(Node<T>*);
	void setRightChild(Node<T>*);
	void setData(T);
};

template<class T>
Node<T>::Node(){
	data = NULL;
	left = right = NULL;
}

template<class T>
Node<T>::Node(T element){
	data = element;
	left = right = NULL;
}

template<class T>
void Node<T>::setLeftChild(Node<T>* node){
	left = node;
}

template<class T>
void Node<T>::setRightChild(Node<T>* node){
	right = node;
}

template<class T>
void Node<T>::setData(T element){
	data = element;
}


