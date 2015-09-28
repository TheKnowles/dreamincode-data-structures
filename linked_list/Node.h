template<class T>
class Node {
private:
	T data;
	Node<T>* next;
public:
	Node();
	Node(T);
	T getData()					{return data;};
	Node<T>* getNext()			{return next;};
	void setNext(Node<T>*);
};


template<class T>
Node<T>::Node() {
	data = NULL;
	next = NULL;
}

template<class T>
Node<T>::Node(T data){
	this->data = data;
	next = NULL;
}

template<class T>
void Node<T>::setNext(Node<T>* node){
	next = node; 
}


