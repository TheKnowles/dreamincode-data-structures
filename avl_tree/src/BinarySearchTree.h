#include "Node.h"
#include <iostream>
using std::cout;
using std::endl;

template<class T>
class BinarySearchTree{
protected:
	Node<T>* root;
	Node<T>* curNode;
	//private recursive helper functions
	void destroyTree(Node<T>*);
	void add(Node<T>*, T);
	void preOrder(Node<T>*);
	void postOrder(Node<T>*);
	void inOrder(Node<T>*);
	bool containsElement(T);
	int size;
public:
	BinarySearchTree();
	BinarySearchTree(T);
	~BinarySearchTree();

	//functionality
	void addNode(T);
	void removeNode(T);
	void preOrderTraversal();
	void postOrderTraversal();
	void inOrderTraversal();
	int totalNodes()					{return size;};
};


template<class T>
BinarySearchTree<T>::BinarySearchTree(){
	root = NULL;
	size = 0;
}

template<class T>
BinarySearchTree<T>::BinarySearchTree(T element){
	Node<T>* temp = new Node<T>(element);
	root = temp;
	size = 1;
}

template<class T>
BinarySearchTree<T>::~BinarySearchTree(){
	std::cout << std::endl << std::endl;
	//use helper function to deallocate the tree
	destroyTree(root);

	std::cout << "Size is (zero indicates all memory was freed): " << size << std::endl;
}

template<class T>
void BinarySearchTree<T>::addNode(T element){
	if (root == NULL){
		root = new Node<T>(element);
		size++;
		return;
	}
	else if (containsElement(element)){
		std::cout << "Tree already has value [" << element << "], insertion aborted." << std::endl;
		return;
	}
	else{
		add(root, element);
	}
}

template<class T>
bool BinarySearchTree<T>::containsElement(T element){
	curNode = root;
	while(curNode != NULL){
		if(curNode->getData() == element){
			return true;
		}
		else {
			if(element > curNode->getData())	curNode = curNode->getRightChild();
			else curNode = curNode->getLeftChild();
		}
	}
	return false;
}

template<class T>
void BinarySearchTree<T>::removeNode(T element){
	//some basic checks 
	if(root == NULL){
		std::cout << "No tree or it's empty!" << std::endl;
		return;
	}
		else if (root->getData() == element){
		std::cout << "Don't delete the root! (I'll work this in later.)" << std::endl;
		return;
	}
	bool found = false;
	Node<T>* parent; //temporary handle for the cur node's parent
	curNode = root; //get a temporary handle current node

	//search the tree for the element
	while(curNode != NULL){
		if(curNode->getData() == element){
			found = true;
			break;
		}
		else {
			parent = curNode; //this is important
			if(element > curNode->getData())	curNode = curNode->getRightChild();
			else curNode = curNode->getLeftChild();
		}
	}
	if(!found){
		std::cout << "Data does not exist in this tree!" << std::endl;
		return;
	}

	//ok we now know the node we want to rmeove is in here
	/*
		Three cases:
		1. leaf node -- very easy
		2. node w/ one child, somewhat challenging
		3. node w/e two children - annoying
	*/

	//leaf
	if((curNode->getLeftChild() == NULL) && (curNode->getRightChild() == NULL)){
		//ensure there are no dangling links
		if(parent->getLeftChild() == curNode) parent->setLeftChild(NULL);
		else parent->setRightChild(NULL);

		//remove
		delete curNode;
		size--; //our rudimentary memory allocator handle
		return;
	}

	//one child
	if((curNode->getLeftChild() == NULL && curNode->getRightChild() != NULL) ||
		(curNode->getLeftChild() != NULL && curNode->getRightChild() == NULL)){
			//handle the first instance
			if(curNode->getLeftChild() == NULL && curNode->getRightChild() != NULL) { //yes I did the same check twice
				/*
					This part in plain English:
					the node we want to renove only has one child, its right child
					we check the parent, if the "to be deleted" node is its left child
					assign linkage and delete
				*/
				if(parent->getLeftChild() == curNode){
					parent->setLeftChild(curNode->getRightChild());
				}
				else{ //same deal, but if the deleted node is on the right 
					parent->setRightChild(curNode->getRightChild());
				}
			}
			else { //left child present, no right child 
				/*
					This part in plain English:
					same deal as above, excpet now the left child is "alive"
					and the right child is nonexistent
				*/
				if(parent->getLeftChild() == curNode){
					parent->setLeftChild(curNode->getLeftChild());
				}
				else{
					parent->setRightChild(curNode->getLeftChild());
				}
			}
			//cut down on duplicate code
			//the delete/size-- could go inside each if/else portion
			delete curNode;
			size--;
			return;
	}

	//complex case, two children
	/*
		in short:
		We want to replace the node to be deleted with the smallest value
		in the right subtree. We then delete the leaf
	*/
	if((curNode->getLeftChild() != NULL)&&(curNode->getRightChild() != NULL)){
		Node<T>* treeChecker;
		treeChecker = curNode->getRightChild(); //assign handle and begin searching

		if((treeChecker->getLeftChild() == NULL) &&(treeChecker->getRightChild() == NULL)){
			//praise the lord its a leaf node, how easy
			curNode->setData(treeChecker->getData());
			delete treeChecker;
			size--;
			curNode->setRightChild(NULL);
		}
		else{ //right child has children
			//if it has a left child, move all the way down to find the smallest element
			if(curNode->getRightChild()->getLeftChild() != NULL){ //go to the curNode's right left child
				//long annoying names, but you'll thank me later
				Node<T>* leftCurrent;
				Node<T>* leftCurrentParent; 
				//inital assignment
				leftCurrentParent = curNode->getRightChild();
				leftCurrent = leftCurrentParent->getLeftChild();
				while(leftCurrent->getLeftChild() != NULL){ //let's go down the left child rabbit hole
					leftCurrentParent = leftCurrent;
					leftCurrent = leftCurrent->getLeftChild();
				}
				//assign the leftest most node's info to the node that was flagged for deletion
				curNode->setData(leftCurrent->getData());

				delete leftCurrent;
				size--;
				leftCurrentParent->setLeftChild(NULL); //take care of any danglers
			}
			else{ //left child is non existent, use the right instead
				Node<T>* rightTemp;
				rightTemp = curNode->getRightChild();
				curNode->setData(rightTemp->getData());
				delete rightTemp;
				size--;
			}
		}
		return;
	}

}

template<class T>
void BinarySearchTree<T>::add(Node<T>* root, T element){
	//we want to maintain a somewhat balanced tree
	//so we'll do a binary search tree check
	
	//lesser values to the left, greater to the right
	if (element < root->getData()){
		if(root->getLeftChild() != NULL){
			add(root->getLeftChild(), element);
		}
		else{
			root->setLeftChild(new Node<T>(element));
			size++;
		}
	}
	else{
		if(root->getRightChild() != NULL){
			add(root->getRightChild(), element);
		}
		else{
			root->setRightChild(new Node<T>(element));
			size++;
		}
	}
}

template<class T>
void BinarySearchTree<T>::destroyTree(Node<T>* root){
	//recurse down the tree
	if(root != NULL){
		destroyTree(root->getLeftChild());
		destroyTree(root->getRightChild());
		delete root;
		size--;
	}
}

template<class T>
void BinarySearchTree<T>::preOrderTraversal(){
	std::cout << std::endl <<  "Pre Order:" << std::endl;
	preOrder(root);
}

template<class T>
void BinarySearchTree<T>::postOrderTraversal(){
	std::cout << std::endl << "Post Order:" << std::endl;
	postOrder(root);
}

template<class T>
void BinarySearchTree<T>::inOrderTraversal(){
	std::cout << std::endl << "In Order:" << std::endl;
	inOrder(root);
}

template<class T>
void BinarySearchTree<T>::preOrder(Node<T>* root){
	std::cout << root->getData() << " ";
	if (root->getLeftChild() != NULL) {preOrder(root->getLeftChild());}
	if (root->getRightChild() != NULL) {preOrder(root->getRightChild());}
}

template<class T>
void BinarySearchTree<T>::postOrder(Node<T>* root){
	if (root->getLeftChild() != NULL) {postOrder(root->getLeftChild());}
	if (root->getRightChild() != NULL) {postOrder(root->getRightChild());}
	std::cout << root->getData() << " ";
}

template<class T>
void BinarySearchTree<T>::inOrder(Node<T>* root){
	if (root->getLeftChild() != NULL) {inOrder(root->getLeftChild());}
	std::cout << root->getData() << " ";
	if (root->getRightChild() != NULL) {inOrder(root->getRightChild());}
}


