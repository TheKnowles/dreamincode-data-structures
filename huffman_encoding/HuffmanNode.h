#ifndef HUFFMANNODE_H
#define HUFFMANNODE_H
#include <iostream>
#include <string>
using std::string;
using std::ostream;

class HuffmanNode{
private:
	int frequency;
	char letter;
	string code;
	HuffmanNode* left, *right;
public:
	HuffmanNode();
	HuffmanNode(char, int);
	HuffmanNode(HuffmanNode*, HuffmanNode*);
	HuffmanNode(const HuffmanNode&);
	~HuffmanNode();
	int getFrequency()							{return frequency;};
	char getLetter()							{return letter;};
	string getCode()							{return code;};
	HuffmanNode* getLeft()						{return left;};
	HuffmanNode* getRight()						{return right;};

	void setHuffmanCode(string s)				{code = s;};
	friend ostream& operator<<(ostream&, const HuffmanNode&);

	HuffmanNode& operator=(const HuffmanNode&);
};
#endif


