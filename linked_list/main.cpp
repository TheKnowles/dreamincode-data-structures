#include <iostream>
#include <string>
#include "LinkedList.h"
using namespace std;

int main() {
	
	LinkedList<string>* anotherList = new LinkedList<string>();

	anotherList->append("Bob");
	anotherList->append("Knowles");
	anotherList->append("Mary");
	anotherList->removeNode("Knowles");
	anotherList->displayAll();

	delete anotherList;
	

	return 0;
}


