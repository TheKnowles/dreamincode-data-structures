#ifndef TRIE_H
#define TRIE_H

#include<iostream>
#include <memory>
#include <string>
#include <vector>
#include <algorithm>
#include <sstream>

class Trie {
private:
  struct Node {
    Node() = default;
    Node(char letter, Node* parent) noexcept
      : rep{(parent ? parent->rep + letter : std::string{letter, 1})},
        parent{parent} {}
    bool print{false};
    std::string rep{""};
    Node* parent{nullptr};
    std::array<std::unique_ptr<Node>, 26> children{{}};
    void print_helper(std::ostream& os) const noexcept;
    void lookup_helper(std::vector<std::string>& list, const size_t& depth) const noexcept;
    void first_letter_diff_look(std::vector<std::string>& list, const int index, const std::string& word) noexcept;
  };


public:
  Trie() : _root{new Node}, _size{0} {};
  size_t size() const noexcept { return _size;}

  bool exists(const std::string& item) noexcept;
  void insert(const std::string& item);
  void remove(const std::string& item);
  void lookup(const std::string& item) noexcept;
  friend std::ostream& operator<<(std::ostream& os, const Trie& rhs);

private:
  std::unique_ptr<Node> _root;
  size_t _size;
};


bool Trie::exists(const std::string& item) noexcept {
  Node* cur_node = _root.get();
  for(auto&& letter : item){
    cur_node = cur_node->children[letter-97].get();
    if (cur_node == nullptr) return false;
  }
  return cur_node->print;
}

void Trie::remove(const std::string& item){
  if (!exists(item)) return;

  std::unique_ptr<Node>* cur_node = &_root;
  for(auto&& letter : item){ //get to the end of the word
    cur_node = &((*cur_node)->children[letter-97]);
  } 

  for(auto&& node : (*cur_node)->children){
    if (node != nullptr){ //there exists some longer word, unmark print
      (*cur_node)->print = false;
      break;
    }
  }
  //does not remove the entire word, either marks it as not printable
  //(in the case of a smaller word: bat/bats with bat removed)
  //or it sets the pointer to null at the end, the other letters still exist
  if((*cur_node)->print) cur_node->reset(nullptr);
  _size--;
}

void Trie::insert(const std::string& item) {
  if (exists(item)){
    std::cout << item << " is already in trie." << std::endl;
    return;
  }

  std::unique_ptr<Node>* cur_node = &_root;
  Node* parent = cur_node->get();
  for(auto&& letter : item){
    cur_node = &((*cur_node)->children[letter-97]);
    if (*cur_node == nullptr) cur_node->reset(new Node(letter, parent));
    parent = cur_node->get();
  } 
  parent->print = true;
  _size++;
}

void Trie::Node::lookup_helper(std::vector<std::string>& list, const size_t& depth) const noexcept {
  for(auto&& node : children){
    if(node != nullptr){
      node->lookup_helper(list, depth);
    } 
  }

  if (print && ((rep.length() >= depth-1) && (rep.length() <= depth+1))){
    list.push_back(rep);
  }
}

void Trie::Node::first_letter_diff_look(std::vector<std::string>& list, const int index, const std::string& word) noexcept{
  if (rep.substr(1) == word) {
    list.push_back(rep);
    return;
  }
  if (children[word[index]-97] != nullptr) children[word[index]-97]->first_letter_diff_look(list, (index+1), word);
}

void Trie::lookup(const std::string& item) noexcept {
  std::cout << "Searching for " << item << std::endl;
  if(exists(item)) {
    std::cout << "Trie contains: " << item << std::endl;
    return;
  }

  //build list of possible suggestions from existing words
  const size_t str_len = item.length();

  std::vector<std::string> hits{};
  for(auto&& node : _root->children){
    if(node != nullptr && node->rep[0] == item[0]){
      node->lookup_helper(hits, str_len);
    }
  }

  //check for words that only different by the first letter
  //use _root rather than the first letter node
  const std::string& suffix_array = item.substr(1);
  for(auto&& node : _root->children){
    if(node != nullptr){
      if (node->children[suffix_array[0]-97] != nullptr){
        node->first_letter_diff_look(hits, 0, suffix_array); 
      }
    }
  }
    
  std::cout << item << " not found. Did you mean?\n";
  if (hits.size() == 0){
    std::cout << "\tNo suggestions found.\n";
    return;
  }

  std::sort(hits.begin(), hits.end());

  for(auto&& hit : hits){
    std::cout << "\t" << hit << " \n";
  }
  std::cout << std::endl;
}

void Trie::Node::print_helper(std::ostream& os) const noexcept {
  if (print) os << rep << "\n";
  for(auto&& node : children){
    if(node != nullptr){
      node->print_helper(os);
    } 
  }
}

std::ostream& operator<<(std::ostream& os, const Trie& rhs){
  std::cout << rhs._size << " words in trie:\n";
  for(auto&& node : rhs._root->children){
    if(node) node->print_helper(os);
  }
  return os;
}

#endif
