#include<iostream>
#include <fstream>

#include "trie.h"

void load_dictionary(const std::string& file, Trie& trie){
  std::cout << "Loading test dictionary..." << std::endl;
  std::ifstream in(file);
  std::string item;
  while(std::getline(in, item)){
    trie.insert(item);
  }
}

int main(){
  Trie trie;
  load_dictionary("sample_dict.txt", trie);
  std::cout << trie << std::endl;
  trie.lookup("follerskate");
  trie.lookup("cart");
  trie.lookup("bart");
}
