#include <iostream>
#include <string>
#include "HuffmanCode.h"
using namespace std;

int main(){
	HuffmanCode* test = new HuffmanCode("this is an example of a huffman tree");
	test->displayTable();
	test->displayHuffmanTable();
	string code = test->getEncodedString();
	cout << "Encoded string: " << code << endl;
	cout << "Decoded string: " << test->decodeString(code) << endl;
	delete test;
	return 0;
}


