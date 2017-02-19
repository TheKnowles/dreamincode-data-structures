#include <iostream>
#include <string>
#include "AVLTree.h"
using namespace std;

int main(){
//
//	//right right default case
//	AVLTree<int> tree1(5);
//	tree1.addNode(10);
//	tree1.addNode(15);
//	tree1.displayFullNodeInfo();
//	tree1.addNode(12);
//	tree1.displayFullNodeInfo();
//	tree1.addNode(16);
//	tree1.displayFullNodeInfo();
//	tree1.addNode(17);
//	tree1.displayFullNodeInfo();
//
//	//left left default case
//	AVLTree<int> tree2(10);
//	tree2.addNode(8);
//	tree2.addNode(5);
//	tree2.displayFullNodeInfo();
//	tree2.addNode(4);
//	tree2.addNode(3);
//	tree2.addNode(2);
//	tree2.displayFullNodeInfo();
//
//	//default right left case
//	AVLTree<int> tree3(3);
//	tree3.addNode(5);
//	tree3.addNode(4);
//	tree3.displayFullNodeInfo();
//
//	//default left right case
//	AVLTree<int> tree4(5);
//	tree4.addNode(3);
//	tree4.addNode(4);
//	tree4.displayFullNodeInfo();

	//DELETION
	//leaf node
//	AVLTree<int> delete_tree_1(5);
//	delete_tree_1.addNode(3);
//	delete_tree_1.addNode(7);
//	delete_tree_1.displayFullNodeInfo();
//	delete_tree_1.removeNode(3);
//	delete_tree_1.displayFullNodeInfo();

//	//node with one child, no rotation
//	AVLTree<int> delete_tree_2(5);
//	delete_tree_2.addNode(3);
//	delete_tree_2.addNode(7);
//	delete_tree_2.addNode(2);
//	delete_tree_2.displayFullNodeInfo();
//	delete_tree_2.removeNode(3);
//	delete_tree_2.displayFullNodeInfo();
//
//	//node with one child that rotates
//	AVLTree<int> delete_tree_3(10);
//	delete_tree_3.addNode(8);
//	delete_tree_3.addNode(12);
//	delete_tree_3.addNode(7);
//	delete_tree_3.addNode(11);
//	delete_tree_3.addNode(13);
//	delete_tree_3.addNode(14);
//	delete_tree_3.displayFullNodeInfo();
//	delete_tree_3.removeNode(8);
//	delete_tree_3.displayFullNodeInfo();
//
//	//deleting root node
//	AVLTree<int> delete_tree_4(10);
//	delete_tree_4.addNode(8);
//	delete_tree_4.removeNode(10);
//	delete_tree_4.displayFullNodeInfo();
//
	//deleting a node with two children, requiring swap node, no rotation needed
	//deletes root node
//	AVLTree<int> delete_tree_6(10);
//	delete_tree_6.addNode(8);
//	delete_tree_6.addNode(11);
//	delete_tree_6.removeNode(10);
//	delete_tree_6.displayFullNodeInfo();

//	//delete node with two children, requires rotation on insertion
	AVLTree<int> delete_tree_6(10);
	delete_tree_6.addNode(9);
	delete_tree_6.addNode(15);
	delete_tree_6.addNode(7);
	delete_tree_6.addNode(11);
	delete_tree_6.addNode(17);
	delete_tree_6.addNode(12);
	delete_tree_6.displayFullNodeInfo();
	delete_tree_6.removeNode(15);
	delete_tree_6.displayFullNodeInfo();

	return 0;
}


