#ifndef HUFFMANCODE_H
#define HUFFMANCODE_H
#include <string>
#include <map>
#include"BinaryHeap.h"
using std::string;
using std::map;

class HuffmanCode{
private:
	string data;
	string encodedData;
	BinaryHeap* heap;
	map<char, int> frequencyTable;
	map<char, string> huffmanTable;
	void buildTable();
	void buildHeap();
	void getHuffmanEncoding(HuffmanNode*, string);
	void encode();
public:
	HuffmanCode(string);
	~HuffmanCode();
	string getSourceString()						{return data;};
	string getEncodedString()						{return encodedData;};
	void displayTable();
	void displayHeap();
	void displayHuffmanTable();
	string decodeString(string);

	//debug
	void emptyHeap()								{heap->emptyHeap();};
};
#endif


