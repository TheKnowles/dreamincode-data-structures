#include "radix.h"

#include <fstream>

void load_dictionary(const std::string& file, RadixTree& radixTree){
  std::cout << "Loading test dictionary..." << std::endl;
  std::ifstream in(file);
  std::string item;
  while(std::getline(in, item)){
    radixTree.insert(item);
    std::cout << radixTree << std::endl;
  }
}

int main(){
  RadixTree radixTree;
  load_dictionary("conf/sample_dict.txt", radixTree);
  std::cout << radixTree << std::endl;
  radixTree.lookup("water");
  radixTree.lookup("waterfall");
  radixTree.lookup("follerskate");
  radixTree.lookup("pea");
  radixTree.lookup("peas");
  radixTree.lookup("peanut");
  radixTree.lookup("waterfalls");
  radixTree.lookup("tes");
  radixTree.lookup("failed");
  std::vector<std::string> words = radixTree.word_list();
  std::cout << words.size() << " words in RadixTree: \n";
  for(auto&& word : words) std::cout << "\t" << word << std::endl;
  radixTree.remove("water");
  radixTree.remove("player");
  std::cout << radixTree << std::endl;
  words = radixTree.word_list();
  std::cout << words.size() << " words in RadixTree: \n";
  for(auto&& word : words) std::cout << "\t" << word << std::endl;
  radixTree.lookup("players");
  radixTree.remove("fail");
  radixTree.remove("failure");
  radixTree.insert("failure");
  radixTree.insert("fail");
  std::cout << radixTree << std::endl;
  words = radixTree.word_list();
  std::cout << words.size() << " words in RadixTree: \n";
  for(auto&& word : words) std::cout << "\t" << word << std::endl;
  return 0;
}
