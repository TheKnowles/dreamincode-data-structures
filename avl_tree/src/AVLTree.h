#ifndef AVL_TREE_H
#define AVL_TREE_H

#include "Node.h"
#include <iostream>
#include <set>
#define DEBUG

//nifty trick to enforce a semicolon on the end of lines using this macro
//DEBUG_OUT(whatever); in code
#ifdef DEBUG
#define DEBUG_OUT(x) \
	do { std::cout << x << std::endl; } while(false)
#endif

#define LOG_LINE(x) \
	do { std::cout << x << std::endl;} while(false)

#define LOG(x) \
	do { std::cout << x;} while(false)

template <class T>
class AVLTree {
private:
	std::shared_ptr<Node<T>> root;
	std::shared_ptr<Node<T>> curNode;
	std::shared_ptr<Node<T>> earliestUnbalancedNode;
	int height;
	std::set<T> elements;
	bool unbalanced;

	//private helpers for cleaner recursion
	void add(std::shared_ptr<Node<T>>, std::shared_ptr<Node<T>>);
	void remove(std::shared_ptr<Node<T>>, std::shared_ptr<Node<T>>);
	void preOrder(std::shared_ptr<Node<T>>);
	void postOrder(std::shared_ptr<Node<T>>);
	void inOrder(std::shared_ptr<Node<T>>);
	void inOrderFullNode(std::shared_ptr<Node<T>>);

	void recursiveHeightCalc(std::shared_ptr<Node<T>>);
	int calculateSubtreeHeight(std::shared_ptr<Node<T>>);
	void balanceTree(std::shared_ptr<Node<T>>);
	void rotateLeft(std::shared_ptr<Node<T>>, bool firstOp = false);
	void rotateRight(std::shared_ptr<Node<T>>, bool firstOp = false);
	std::shared_ptr<Node<T>> getNode(T);
	std::shared_ptr<Node<T>> findSwapNodeInSubtree(std::shared_ptr<Node<T>>, bool&);
public:
	AVLTree(T);
	~AVLTree();

	void addNode(T);
	void removeNode(T);
	void preOrderTraversal();
	void postOrderTraversal();
	void inOrderTraversal();
	void displayFullNodeInfo();

};



template <class T>
AVLTree<T>::AVLTree(T rootVal){
	root = std::make_shared<Node<T>>(rootVal);
	elements.insert(rootVal);
	curNode = root;
	height = 1;
	unbalanced = false;
}

template <class T>
AVLTree<T>::~AVLTree(){

}
template <class T>
std::shared_ptr<Node<T>> AVLTree<T>::getNode(T val){
	std::shared_ptr<Node<T>> temp = root;
	while(temp != nullptr){
		if(temp->getData() == val){
			return temp;
		}
		else {
			if(val > temp->getData())	temp = temp->getRightChild();
			else temp = temp->getLeftChild();
		}
	}
	//guaranteed to find it since we check for exitance before going in to this function
}

template <class T>
void AVLTree<T>::addNode(T value){
	DEBUG_OUT("Adding node " << value);
	std::shared_ptr<Node<T>> newNode = std::make_shared<Node<T>>(value);
	//fucking dumb, I hate this crap, why does the compiler need the typename here?
	//worst random issue I run into frequently, "dependent name"
	//fine it could be a type, whatever. get your life together compiler
	std::pair<typename std::set<T>::iterator, bool> result;
	result = elements.insert(value);
	if (result.second == false){
		DEBUG_OUT("Element " << value << " already in tree");
		return;
	}
	curNode = root;
	add(curNode, newNode);

	//calculate subtree heights to see if the tree became unbalanced after last insertion
	//balanceFactor = height (left subtree) - height (right subtree)
	//this function will assign BF to each node as it goes down

	recursiveHeightCalc(root);

	//at most one subtree can be -2/2/unbalanced
	if(unbalanced){
		DEBUG_OUT("Tree is unbalanced at node: " << earliestUnbalancedNode->getData());
		balanceTree(earliestUnbalancedNode);
		unbalanced = false;
		earliestUnbalancedNode = nullptr;
	}
}

template <class T>
void AVLTree<T>::removeNode(T value){
	if(elements.find(value) == elements.end()){
		LOG_LINE("Element " << value << " not in tree");
		return;
	}

	DEBUG_OUT("Deleting node:" << value);
	std::shared_ptr<Node<T>> nodeToDel = getNode(value);
	/*
	 node X
	 if X is a leaf, delete it, rebalance factor up the subtree
	 if X has one child, attach that child to X's parent
	 else
	 	 find node Y => largest node in node X left subtree or smallest in right subtree
	 	 swap links
	 	 delete swapNode
	 */
	if(nodeToDel->isLeafNode()){
		DEBUG_OUT("node: " << value << " is leaf node");
		std::shared_ptr<Node<T>> parent = nodeToDel->getParent();
		if(parent->getLeftChild() == nodeToDel) parent->setLeftChild(nullptr);
		else if (parent->getRightChild() == nodeToDel) parent->setRightChild(nullptr);
		else LOG_LINE("ERROR, node to delete does not match parent link");
	}
	else if (nodeToDel->hasOneChild()){
		DEBUG_OUT("node: " << value << " has one child");
		std::shared_ptr<Node<T>> parent = nodeToDel->getParent();
		std::shared_ptr<Node<T>> singleChild = nodeToDel->getSoloChild();
		if(parent != nullptr){
			//swap handles for child and deleted node's parent
			singleChild->setParent(parent);
			if(parent->getLeftChild() == nodeToDel) parent->setLeftChild(singleChild);
			else if (parent->getRightChild() == nodeToDel) parent->setRightChild(singleChild);
			else DEBUG_OUT("ERROR, node to delete does not match parent link");
		}
		else { //parent == nullptr == root node
			DEBUG_OUT("Deleting root node, handing off ptrs");
			root = singleChild;
			singleChild->setParent(nullptr);
		}
	}
	else {
		bool leftSubtree = false;
		std::shared_ptr<Node<T>> swapNode = findSwapNodeInSubtree(nodeToDel, leftSubtree);
		DEBUG_OUT("Found swapNode: " << swapNode->getData());
		//swap node data, keep in tact all pointers
		//based on what subtree we went down, assign
		//if the opposite side child is null, bring the swap node's child up
		//otherwise assign any subtree of the swap node to the left/right child
		//of the node originally marked for deletion
		nodeToDel->setData(swapNode->getData());
		if(leftSubtree) {
			swapNode->getParent()->setLeftChild(swapNode->getLeftChild());
			swapNode->getParent()->setRightChild(nullptr);
		}
		else {
			swapNode->getParent()->setRightChild(swapNode->getRightChild());
			swapNode->getParent()->setLeftChild(nullptr);
		}
	}
	//check the tree and balance as necessary
	recursiveHeightCalc(root);
	if(unbalanced){
		DEBUG_OUT("Tree is unbalanced at node: " << earliestUnbalancedNode->getData());
		balanceTree(earliestUnbalancedNode);
		unbalanced = false;
		earliestUnbalancedNode = nullptr;
	}
}

//find the largest node without a right child in the left subtree
//otherwise grab the smallest node in the right subtree that has no left child
//mark which subtree we went down for later
template <class T>
std::shared_ptr<Node<T>> AVLTree<T>::findSwapNodeInSubtree(std::shared_ptr<Node<T>> treeRoot, bool& leftSubtree){
	if (treeRoot->getLeftChild() != nullptr){
		std::shared_ptr<Node<T>> left = treeRoot->getLeftChild();
		while(left != nullptr){
			if(left->getRightChild() == nullptr) {
				leftSubtree = true;
				return left;
			}
			else left = left->getRightChild();
		}
	}
	else if (treeRoot->getRightChild() != nullptr){
		std::shared_ptr<Node<T>> right = treeRoot->getRightChild();
		while(right != nullptr){
			if(right->getLeftChild() == nullptr) return right;
			else right = right->getLeftChild();
		}
	}
	DEBUG_OUT("ERROR, did not find appropriate swap node, clearly I'm an idiot");
	return nullptr;
}



template <class T>
void AVLTree<T>::rotateLeft(std::shared_ptr<Node<T>> node, bool firstOp){
	//node is the root of the unbalanced subtree
	//node becomes left child of its right child
	Node<T> temp(*node->getRightChild());
	std::shared_ptr<Node<T>> newRoot  = node->getRightChild(); //no new allocation
	if(newRoot->getLeftChild() != nullptr) node->setRightChild(newRoot->getLeftChild());
	else node->setRightChild(nullptr);

	newRoot->setLeftChild(node);

	//in the event we are rotating the root of the tree, reassign handle and make parent nil
	if(root == node){
		DEBUG_OUT("node rotated left was tree root, reassigning");
		root = newRoot;
		newRoot->setParent(nullptr);
		//if we rotated the root, make its right child the left child of node->getRightChild
		if(temp.getLeftChild() != nullptr) newRoot->getLeftChild()->setRightChild(temp.getLeftChild());
	}
	else {
		DEBUG_OUT("Unbalanced root was not tree root");
		newRoot->setParent(node->getParent()); //non tree root case, grab next ancestor
		if(firstOp){
			newRoot->getParent()->setLeftChild(newRoot);
		}
		else {
			newRoot->getParent()->setRightChild(newRoot);
		}
	}
	node->setParent(newRoot);

	recursiveHeightCalc(root);
}

template <class T>
void AVLTree<T>::rotateRight(std::shared_ptr<Node<T>> node, bool firstOp){
	//node is the root of the unbalanced tree
	Node<T> temp(*node->getLeftChild());
	std::shared_ptr<Node<T>> newRoot  = node->getLeftChild(); //no new allocation
	if(newRoot->getRightChild() != nullptr) node->setLeftChild(newRoot->getRightChild());
	else node->setLeftChild(nullptr);

	newRoot->setRightChild(node);

	if(root == node){
		DEBUG_OUT("node rotated right was tree root, reassigning");
		root = newRoot;
		newRoot->setParent(nullptr);
		//if we rotated the root, make its left child the right child of node->getLeftChild
		if(temp.getRightChild() != nullptr) newRoot->getRightChild()->setLeftChild(temp.getRightChild());
	}
	else {
		DEBUG_OUT("Unbalanced root was not tree root");
		newRoot->setParent(node->getParent()); //non tree root case, grab next ancestor
		if(firstOp){
			newRoot->getParent()->setRightChild(newRoot);
		}
		else {
			newRoot->getParent()->setLeftChild(newRoot);
		}
	}
	node->setParent(newRoot);

	recursiveHeightCalc(root);
}

template <class T>
void AVLTree<T>::balanceTree(std::shared_ptr<Node<T>> node){
	//get rid of these checks by flagging which side of the tree we go down
	if(node->getBalanceFactor() == -2){
		DEBUG_OUT("Right subtree of this node is unbalanced");
		if(node->getRightChild()->getBalanceFactor() == -1){
			DEBUG_OUT("Right Right case");
			//one rotation to the left will fix the tree
			rotateLeft(node);
		}
		else {
			DEBUG_OUT("Right Left case");
			//rotate the child of the unbalanced subtree to the right then rotate left, bam done
			rotateRight(node->getRightChild(), true);
			rotateLeft(node);
		}
	}
	else{ //2
		DEBUG_OUT("Left subtree of this node is unbalanced");
		if(node->getLeftChild()->getBalanceFactor() == 1){
			DEBUG_OUT("Left Left case");
			//one rotation to the right will fix the tree
			rotateRight(node);
		}
		else {
			DEBUG_OUT("Left Right case");
			rotateLeft(node->getLeftChild(), true);
			rotateRight(node);
		}
	}
}

//this function calculates subtree heights and does balance factors
//originally this was going to be recursive, maybe do that in the future
template <class T>
int AVLTree<T>::calculateSubtreeHeight(std::shared_ptr<Node<T>> node){
	if(node->getLeftChild() == nullptr and node->getRightChild() == nullptr){
		node->setHeight(0);
		node->setBalanceFactor(0);
		return 1; //this isn't really necessary
	}
	else if(node->getLeftChild() != nullptr and node->getRightChild() == nullptr){
		node->setHeight(node->getLeftChild()->getHeight() + 1);
		node->setBalanceFactor((node->getLeftChild()->getHeight()+1)-0);//pointless math, but here for illustration
	}
	else if(node->getLeftChild() == nullptr and node->getRightChild() != nullptr){
		node->setHeight(node->getRightChild()->getHeight() + 1);
		node->setBalanceFactor(0-(node->getRightChild()->getHeight()+1));
	}
	else if(node->getLeftChild() != nullptr and node->getRightChild() != nullptr){
		node->setHeight(
				(node->getLeftChild()->getHeight() < node->getRightChild()->getHeight()) ?
						node->getRightChild()->getHeight() + 1 :
						node->getLeftChild()->getHeight() + 1
				);
		node->setBalanceFactor(node->getLeftChild()->getHeight() - node->getRightChild()->getHeight());
	}
	int bf = node->getBalanceFactor();
	DEBUG_OUT("node in calcsubtree: " << *node);
	if((bf == -2) or (bf == 2)){
		unbalanced = true;
		if(earliestUnbalancedNode == nullptr) earliestUnbalancedNode = node;
	}
}

template <class T>
void AVLTree<T>::add(std::shared_ptr<Node<T>> itrNode, std::shared_ptr<Node<T>> node){
	if(node->getData() < itrNode->getData()){
		DEBUG_OUT("Entering left subtree");
		if(itrNode->getLeftChild() == nullptr){
			itrNode->setLeftChild(node);
			node->setParent(itrNode);
		}
		else {
			add(itrNode->getLeftChild(), node);
		}
	}
	else {
		DEBUG_OUT("Entering right subtree");
		if(itrNode->getRightChild() == nullptr){
			itrNode->setRightChild(node);
			node->setParent(itrNode);
		}
		else {
			add(itrNode->getRightChild(), node);
		}
	}
}

template <class T>
void AVLTree<T>::recursiveHeightCalc(std::shared_ptr<Node<T>> node){
	if(node->getLeftChild() != nullptr) recursiveHeightCalc(node->getLeftChild());
	if(node->getRightChild() != nullptr) recursiveHeightCalc(node->getRightChild());
	calculateSubtreeHeight(node);
}

//****************************************************************
// TRAVERSAL
//****************************************************************
template <class T>
void AVLTree<T>::preOrder(std::shared_ptr<Node<T>> node){
	LOG(node->getData() << " ");
	if(node->getLeftChild() != nullptr) preOrder(node->getLeftChild());
	if(node->getRightChild() != nullptr) preOrder(node->getRightChild());
}

template <class T>
void AVLTree<T>::postOrder(std::shared_ptr<Node<T>> node){
	if(node->getLeftChild() != nullptr) preOrder(node->getLeftChild());
	if(node->getRightChild() != nullptr) preOrder(node->getRightChild());
	LOG(node->getData() << " ");
}

template <class T>
void AVLTree<T>::inOrder(std::shared_ptr<Node<T>> node){
	if(node->getLeftChild() != nullptr) preOrder(node->getLeftChild());
	LOG(node->getData() << " ");
	if(node->getRightChild() != nullptr) preOrder(node->getRightChild());
}

template <class T>
void AVLTree<T>::inOrderFullNode(std::shared_ptr<Node<T>> node){
	if(node->getLeftChild() != nullptr) inOrderFullNode(node->getLeftChild());
	LOG(*node);
	if(node->getRightChild() != nullptr) inOrderFullNode(node->getRightChild());
}

template <class T>
void AVLTree<T>::preOrderTraversal(){
	LOG_LINE("pre order traversal: ");
	preOrder(root);
	LOG_LINE("");
}

template <class T>
void AVLTree<T>::postOrderTraversal(){
	LOG_LINE("post order traversal: ");
	postOrder(root);
	LOG_LINE("");
}

template <class T>
void AVLTree<T>::inOrderTraversal(){
	LOG_LINE("in order traversal: ");
	inOrder(root);
	LOG_LINE("");
}

template <class T>
void AVLTree<T>::displayFullNodeInfo(){
	LOG_LINE("in order traversal full node info: ");
	inOrderFullNode(root);
}

#endif
