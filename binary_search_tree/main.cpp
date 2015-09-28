#include <iostream>
#include <string>
#include "BinarySearchTree.h"
using namespace std;

int main(){
	BinarySearchTree<int>* tree = new BinarySearchTree<int>(6);
	tree->addNode(10);
	tree->addNode(3);

	BinarySearchTree<int>* tree2 = new BinarySearchTree<int>(6);
	tree2->addNode(10);
	tree2->addNode(3);
	tree2->addNode(4);

	cout << BinarySearchTree<int>::isIsomorphic(tree, tree2) << endl;
	delete tree;
	delete tree2;
	return 0;
}


