#include "HuffmanCode.h"
#include <iostream>
#include <utility>
using namespace std;

HuffmanCode::HuffmanCode(string src){
	data = src;
	encodedData = "";
	//worst case scenario, all unique characters
	//must accomodate
	heap = new BinaryHeap(data.length());
	buildTable();
	buildHeap();
}

HuffmanCode::~HuffmanCode(){
	if(heap != NULL) delete heap;
}

void HuffmanCode::buildTable(){
	for(size_t i = 0; i < data.length(); i++){
		char c = data.at(i);
		if (frequencyTable.find(data.at(i)) == frequencyTable.end()){
			frequencyTable.insert(pair<char, int>(c, 1));
		}
		else{
			frequencyTable[c]++;
		}
	}
}

void HuffmanCode::buildHeap(){
	//fill heap
	for(map<char,int>::iterator it = frequencyTable.begin(); 
		it != frequencyTable.end(); ++it){
			heap->insert(new HuffmanNode(it->first, it->second));
	}
	//pop off two at a time until you get one left
	while(!heap->isEmpty()){
		if(heap->getHeapSize() == 1) break;
		HuffmanNode* left = heap->removeMin();
		HuffmanNode* right = heap->removeMin();
		heap->insert(new HuffmanNode(left, right));
	}
	//final one is the root of your encoding tree
	string code = "";
	getHuffmanEncoding(heap->getRoot(), code);
	encode();
}

void HuffmanCode::getHuffmanEncoding(HuffmanNode* root, string code){
	if(root->getLeft() == NULL){
		root->setHuffmanCode(code);
		huffmanTable.insert(pair<char, string>(root->getLetter(), code));
		return;
	}
	else{
		getHuffmanEncoding(root->getLeft(), code+"0");
		getHuffmanEncoding(root->getRight(), code+"1");
	}
}

void HuffmanCode::displayTable(){
	cout << "Frequency Table:" << endl;
	for(map<char,int>::iterator it = frequencyTable.begin(); 
		it != frequencyTable.end(); ++it){
			cout << it->first << "\t" << it->second << endl;
	}
}

void HuffmanCode::displayHuffmanTable(){
	cout << "Huffman Table:" << endl;
	for(map<char,string>::iterator it = huffmanTable.begin(); 
		it != huffmanTable.end(); ++it){
			cout << it->first << "\t" << it->second << endl;
	}
}

void HuffmanCode::encode(){
	for(size_t i = 0; i < data.length(); i++){
		encodedData.append(huffmanTable[data.at(i)]);
		encodedData.append(" ");
	}
}

string HuffmanCode::decodeString(string src){
	string info = "";
	size_t lastBlock = 0;
	for(size_t i = 0; i < src.length(); i++){
		if (src.at(i) == ' '){
			string temp = src.substr(lastBlock, i-lastBlock);
			lastBlock = i+1;
			for(map<char,string>::iterator it = huffmanTable.begin();
				it != huffmanTable.end(); ++it){
					if (it->second == temp){
						info += (it->first);
						break;
					}
			}
		}
	}
	return info;
}

void HuffmanCode::displayHeap(){
	heap->inOrderTraversal();
}


