#ifndef NODE_H
#define NODE_H

#include <memory>
#include <ostream>

template<class T>
class Node {
private:
	T data;
	int balanceFactor;
	int height; //heiht of the subtree this node is a root of
	std::shared_ptr<Node<T>> left;
	std::shared_ptr<Node<T>> right;
	std::shared_ptr<Node<T>> parent;
	Node& operator=(Node&);
public:
	Node();
	Node(T);
	Node(const Node&);

	int getBalanceFactor()						{return balanceFactor;}
	int getHeight()								{return height;}
	T getData()									{return data;};
	std::shared_ptr<Node<T>> getLeftChild()		{return left;};
	std::shared_ptr<Node<T>> getRightChild()		{return right;};
	std::shared_ptr<Node<T>> getParent()			{return parent;};
	bool isLeafNode(){
		return (left == nullptr && right == nullptr);
	}
	bool hasOneChild(){
		return (
				(left != nullptr and right == nullptr) ||
				(left == nullptr and right != nullptr)
		);
	}
	//used to clean up code in AVL
	//assumes the caller is not a retard and checked hasOneChild() above
	//before calling this, no error checking on my part
	std::shared_ptr<Node<T>> getSoloChild(){
		if(left != nullptr) return left;
		else return right;
	}


	void setBalanceFactor(int);
	void setHeight(int);
	void setLeftChild(std::shared_ptr<Node<T>>);
	void setRightChild(std::shared_ptr<Node<T>>);
	void setParent(std::shared_ptr<Node<T>>);
	void setData(T);

	friend std::ostream& operator <<(std::ostream& out, const Node<T>& node){
		out << node.data << " bf:" << node.balanceFactor << " h:" << node.height << std::endl;
		return out;
	}
};

template<class T>
Node<T>::Node(){
	data = 0;
	parent = left = right = nullptr;
	balanceFactor = height = 0;
}

template<class T>
Node<T>::Node(T element){
	data = element;
	parent = left = right = nullptr;
	balanceFactor = height = 0;
}

template<class T>
Node<T>::Node(const Node<T>& cp){
	data = cp.data;
	parent = cp.parent;
	left = cp.left;
	right = cp.right;
	balanceFactor = cp.balanceFactor;
	height = cp.height;
}

template<class T>
void Node<T>::setBalanceFactor(int val){
	balanceFactor = val;
}

template<class T>
void Node<T>::setHeight(int val){
	height = val;
}

template<class T>
void Node<T>::setLeftChild(std::shared_ptr<Node<T>> node){
	left = node;
}

template<class T>
void Node<T>::setRightChild(std::shared_ptr<Node<T>> node){
	right = node;
}

template<class T>
void Node<T>::setParent(std::shared_ptr<Node<T>> node){
	parent = node;
}

template<class T>
void Node<T>::setData(T element){
	data = element;
}

#endif
