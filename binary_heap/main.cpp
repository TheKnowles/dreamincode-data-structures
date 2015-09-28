#include "BinaryHeap.h"

int main(){
	{
		BinaryHeap<int>* test = new BinaryHeap<int>(5);
		test->insertNode(4);
		test->insertNode(10);
		test->insertNode(43);
		test->insertNode(100);
		test->insertNode(15);
		cout << "Before removal [in order traversal]: " << endl;
		test->inOrderTraversal();
		cout << endl << endl;
		cout << "Popped: " << test->removeMax() << endl << endl;
		cout << "After removal [in order traversal]: " << endl;
		test->inOrderTraversal();
		cout << endl << endl;
		delete test;
	}
	return 0;
}


