#include "Node.h"
#include <iostream>
using namespace std;

template<class T>
class LinkedList {
private:
	Node<T>* head;
	Node<T>* curNode;
	Node<T>* tailNode; //used to make end insertion O(1) operation rather then O(n)
	int size;
public:
	LinkedList();
	LinkedList(T);
	~LinkedList();

	//functionality
	int getSize()				{return size;};
	void append(T);
	void displayAll();
	bool contains(T);
	void removeNode(T);
	T front()					{return head->getData();};
	T back()					{return tailNode->getData();};
};



template<class T>
LinkedList<T>::LinkedList(){
	head = curNode = tailNode = NULL;
	size = 0;
}

template<class T>
LinkedList<T>::LinkedList(T element){
	Node<T>* temp = new Node<T>(element);
	head = temp;
	curNode = head;
	tailNode = head;
	head->setNext(NULL);
	size = 1;
}

template<class T>
LinkedList<T>::~LinkedList(){
	//iterate and free all the memory
	Node<T>* temp; //used for deletion
	curNode = head; //start at the beginning
	int counter = 0; //index tracker, for memory allocation checking
	if(head != NULL){
		do{
			//logging purposes
			cout << counter << " index freed" << endl;
			temp = curNode->getNext(); //get a handle to the next node
			delete curNode; //free current
			curNode = temp; //obtain the handle
			counter++;
			size--;
		}while (curNode != NULL);
	}
	else {
		cout << "List is empty, nothing to free" << endl;
	}

	cout << "Size (should be zero): " << size << endl;
}

template<class T>
void LinkedList<T>::append(T element){
	curNode = head; //start at the beginning
	Node<T>* temp;
	if(head != NULL){
		/* 
		******* O(n) method ************
		while(curNode->getNext() != NULL){
			//get to the end
			curNode = curNode->getNext();
		}
		********************************/
		//O(1) method
		temp = new Node<T>(element);
		tailNode->setNext(temp);
		tailNode = temp;
		cout << element << " successfully appended to the list!" << endl;
	}
	else {
		temp = new Node<T>(element);
		head = temp;
		head->setNext(NULL);
		tailNode = head;
		cout << "Default constructor used, " << element << " head successfully created!" << endl;
	}
	size++;
	return;
}

template<class T>
void LinkedList<T>::displayAll(){
	//O(n)
	curNode = head;
	cout << "Displaying list: " << endl;
	while(curNode != NULL){
		cout << curNode->getData() << " ";
		curNode = curNode->getNext();
	}
	cout << endl;
	return;
}

template<class T>
bool LinkedList<T>::contains(T element){
	//O(n) search
	curNode = head;
	while(curNode != NULL){
		if(curNode->getData() == element){
			return true;
		}
		curNode = curNode->getNext();
	}
	return false;
}

template<class T>
void LinkedList<T>::removeNode(T element){
	Node<T>* temp, *previous; //we'll need these later
	curNode = head; //default assignment
	//O(1) check (head node)
	if (head->getData() == element) { //remove head
		head = head->getNext();
		delete curNode;
		size--;
		cout << element << " was the head node, it was successfully removed" << endl;
	}
	else {
		while(curNode != NULL){
			//check data
			if(curNode->getData() == element){
				temp = curNode->getNext();
				if (temp == NULL){
					previous->setNext(NULL);
				}
				else{
					previous->setNext(temp);
				}
				delete curNode;
				curNode = temp;
				size--;
				cout << element << " a non head node, was successfully deleted!" << endl;
				break;
			}
			else {
				previous = curNode;
				curNode = curNode->getNext();
			}
		}
	}
}


