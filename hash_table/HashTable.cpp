#include <iostream>
#include <cstring>
using namespace std;

template <typename T>
class HashTable{
private:
	T table[31]; //set amount of storage, small for example
	int hash(T); //only the table needs it
public:
	HashTable();
	~HashTable()							{};
	void addValue(T);
	T retrieveValue(int index)				{return table[index];};
	bool contains(T);
	void displayTableContents();
};

template <typename T>
HashTable<T>::HashTable(){
	memset(table, 0, sizeof(table)); //zero it out, for illustrative purposes
}

template <typename T>
int HashTable<T>::hash(T value){
	return value % 31; //simple just for the framework
}

template <typename T>
bool HashTable<T>::contains(T value){
	if(table[hash(value)]) return true;
	else return false; 
}

template <typename T>
void HashTable<T>::addValue(T value){
	int index = hash(value);
	if(table[index] == 0){ //look it's free!
		table[index] = value; //go ahead and add it!
	}
	else{
		bool notDone = true;
		while(notDone){
			index++;
			if(!table[index]){ //if !0, i.e. empty
				table[index] = value;
				notDone = false;
			}
		}
	}
	return;
}

template <typename T>
void HashTable<T>::displayTableContents(){
	for(int i = 0; i < 31; i++){
		cout << table[i] << endl;
	}
	return;
}

int main(){
	HashTable<int>* test = new HashTable<int>();
	test->addValue(62); //both of these hash to zero (mod 31)
	test->addValue(31);
	test->addValue(107); //to show a value away from the initial probing
	test->displayTableContents();
	delete test;
	return 0;
}

