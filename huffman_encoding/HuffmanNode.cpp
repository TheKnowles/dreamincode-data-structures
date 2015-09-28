#include "HuffmanNode.h"
using namespace std;

HuffmanNode::HuffmanNode(){
	letter = ' ';
	frequency = 0;
	left = right = NULL;
}

HuffmanNode::HuffmanNode(char c, int i){
	letter = c;
	frequency = i;
	left = right = NULL;
}

HuffmanNode::HuffmanNode(HuffmanNode* left, HuffmanNode* right){
	this->left = left;
	this->right = right;
	frequency = left->getFrequency() + right->getFrequency();
	letter = NULL;
}

HuffmanNode::~HuffmanNode(){
	if(left != NULL) delete left;
	if(right != NULL) delete right;
}

HuffmanNode::HuffmanNode(const HuffmanNode& rhs){
	//cout << "in copy constructor copying: " << rhs.letter << endl;
	
	if(rhs.left != NULL){
		left = new HuffmanNode();
		*left = *(rhs.left);
	}
	if(rhs.right != NULL){
		right = new HuffmanNode();
		*right = *(rhs.right);
	}
	code = rhs.code;
	letter = rhs.letter;
	frequency = rhs.frequency;
}


ostream& operator<<(ostream& os, const HuffmanNode& rhs){
	os <<  "\'" << rhs.letter << " " << rhs.frequency << "\' ";
	return os;
}

HuffmanNode& HuffmanNode::operator=(const HuffmanNode& rhs){
	//cout << "In assigbnment operator overload." << endl;
	if(!(this == &rhs)){
		//copy
		code = rhs.code;
		letter = rhs.letter;
		frequency = rhs.frequency;
		if(!(left == NULL)) delete left;
		if(!(right == NULL)) delete right;

		if(rhs.left != NULL){
			left = new HuffmanNode();
			*left = *(rhs.left);
			if(rhs.right != NULL){
				right = new HuffmanNode();
				*right = *(rhs.right);
			}
		}
		else left = right = NULL;
	}
	return *this;
}


