#include <iostream>
#include <cstring>
#include "LinkedList.h"
using namespace std;

//LinkedList Chained Table
template <typename T>
class ChainedHashTable{
private:
	LinkedList<T>* table[31]; 
	int hash(T); //only the table needs it
public:
	ChainedHashTable();
	~ChainedHashTable();
	void addValue(T);
	T retrieveValue(int index);
	bool contains(T);
	void displayTableContents();
};

template<typename T>
ChainedHashTable<T>::ChainedHashTable(){
	memset(table, NULL, sizeof(table)); //same deal, but NULL for ptrs
}

template<typename T>
ChainedHashTable<T>::~ChainedHashTable(){
	//free the memory of each list
	for(int i = 0; i < 31; i++){
		if(table[i]) delete table[i]; //if the slot was never used, leave it alone
	}
}

template <typename T>
int ChainedHashTable<T>::hash(T value){
	return value % 31;
}

template <typename T>
void ChainedHashTable<T>::addValue(T value){
	int index = hash(value);
	if(table[index] == NULL){
		table[index] = new LinkedList<T>(value);
	}
	else{
		table[index]->append(value);
	}
}

template <typename T>
bool ChainedHashTable<T>::contains(T value){
	int index = hash(value);
	if (table[index] == NULL) return false;
	else{
		return table[index]->contains(value);
	}
}

template <typename T>
void ChainedHashTable<T>::displayTableContents(){
	for(int i = 0; i < 31; i++){
		if(table[i] != NULL){
			cout << "List at index " << i << ": ";
			table[i]->displayAll();
			cout << endl;
		}
	}
}

int main(){
  ChainedHashTable<int>* test = new ChainedHashTable<int>();
  test->addValue(62); //both of these hash to zero (mod 31)
  test->addValue(31);
  test->addValue(107); //to show a value away from the initial probing
  test->addValue(138); //linear probe index 15
  test->addValue(61); //index 30
  test->addValue(123); //wrap around, must go past indexes 0 and 1, from above 
  test->displayTableContents();
  delete test;
  return 0;
}

