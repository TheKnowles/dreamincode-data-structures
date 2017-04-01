#ifndef __RADIX_H_
#define __RADIX_H_

#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include <algorithm>
#include <sstream>
#include <regex>

class RadixTree {
private:
  struct Node {
    Node() = default;
    Node(std::string str, Node* parent) noexcept
      : rep{str}, parent{parent} {}
    Node(const Node&) = delete;
    Node& operator=(const Node&) = delete;
    bool print{false};
    std::string rep{""};
    Node* parent{nullptr};
    std::vector<std::unique_ptr<Node>> children{};
    void print_helper(std::ostream& os) const noexcept;
    void lookup_helper(std::vector<std::string>& list, const size_t index, const std::string& word) const noexcept;
    void word_list_helper(std::vector<std::string>& list) noexcept;
  };
  
public:
  RadixTree() : _root{new Node}, _size{0} {};
  size_t size() const noexcept { return _size;}

  void insert(const std::string& item);
  void remove(const std::string& item);
  void lookup(const std::string& item) noexcept;
  std::vector<std::string> word_list() noexcept;
  friend std::ostream& operator<<(std::ostream& os, const RadixTree& rhs);

private:
  bool exists(const std::string& item, std::unique_ptr<Node>** node) noexcept;
  void word_list_helper(std::vector<std::string>& list) noexcept;
  std::unique_ptr<Node> _root;
  size_t _size;
};

bool RadixTree::exists(const std::string& item, std::unique_ptr<Node>** node = nullptr) noexcept {
  std::unique_ptr<Node>* cur_node = &_root;
  size_t total_matched_len = 0;
  size_t match_index = 0;
  bool matched_child = false;
  std::string s_item{item.substr(total_matched_len)};
  while(*cur_node != nullptr && total_matched_len < item.length()){
    s_item = item.substr(total_matched_len);
    for(auto&& letter : (*cur_node)->rep){
      if(letter == s_item[match_index]){
        total_matched_len++;
        match_index++;
      }
      else break;
    }
    for(auto&& child : (*cur_node)->children){
      if(child->rep[0] == s_item[match_index]){
        cur_node = &child;
        matched_child = true;
        break;
      } 
    }
    if(!matched_child) break;
    match_index = 0; //reset for child node
    matched_child = false;
  }
  //for remove grab the handle to the node we found
  if (node != nullptr) *node = &(*cur_node);
  //did we match all the letter of the search string and does the final substr match the rep (aka suffix match)
  //the latter is important in the case of a longer word being inserted before a shorter word: waterfall, water
  //it is also possible that a word is in the tree, but was not inserted, example insert peas, search for pea 
  return ((*cur_node)->print && total_matched_len == item.length() && s_item.length() == (*cur_node)->rep.length());
}

void RadixTree::remove(const std::string& item){
  std::unique_ptr<Node>* cur_node;
  if (!exists(item, &cur_node)){
    std::cout << item << " not in RadixTree\n";
    return;
  }
  std::cout << "Removing " << item << "/" << (*cur_node)->rep << " [str/node_rep] from RadixTree\n";
  //note: this will not attempt to clean up two levels of nodes depending on order of delete
  //3 cases:
  //  1. no children, kill node
  //  2. one child, merge, kill child
  //  3. 2+ children, this means  we are "deleting" a subtree root, but there exists longer words, mark print false
  if ((*cur_node)->children.size() == 0){
    (*cur_node)->print = false; 
     Node* parent = (*cur_node)->parent;
     parent->children.erase(std::remove_if(parent->children.begin(), 
                                           parent->children.end(), 
                                           [cur_node](std::unique_ptr<Node>& node) {return node->rep == (*cur_node)->rep;} ));
  } 
  else if ((*cur_node)->children.size() == 1){
    (*cur_node)->rep.append((*cur_node)->children.back()->rep);
    (*cur_node)->children.clear();
  }
  else {
    (*cur_node)->print = false;
  }
  _size--;
}

void RadixTree::insert(const std::string& item) {
  if (exists(item)){
    std::cout << item << " is already in RadixTree." << std::endl;
    return;
  }
  std::unique_ptr<Node>* cur_node = &_root;
  Node* parent = cur_node->get();
  size_t matched_len = 0, total_matched_len = 0;
  do {
    std::string s_item{item.substr(total_matched_len)};
    for(size_t i = 0; (i < s_item.length() && i < (*cur_node)->rep.length()); i++){
      if((*cur_node)->rep[i] == s_item[i]){
        matched_len++;
        total_matched_len++;
      }
      else break; //if we hit a non matching letter, immediately start looking at children
    }
    bool matched_child = false;
    for(auto&& child : (*cur_node)->children){
      //std::cout << "searching for " << item << " :: cnm: " << child->rep[0] << ":" << s_item[matched_len] << std::endl;
      if(child->rep[0] == s_item[matched_len]){//only one child, if any, will match
        //std::cout << "cnm matched, moving to: " << child->rep << std::endl;
        matched_child = true;
        parent = cur_node->get();
        cur_node = &child;
        break;
      }
    }
    //two exit possibilities, we either find the entire word across nodes OR we hit the end of our search
    if(total_matched_len == item.length()) break;
    if(!matched_child) break;
    matched_len = 0; //reset for next layer
  } while(true);

  //std::cout << "searching for " << item << " :: total matches: " << total_matched_len << " ml=" << matched_len << std::endl;
  if (total_matched_len == 0){ //base case: insert the entire word as a new node
    //std::cout << "cn=" << (*cur_node)->rep << " child=" << item << std::endl;
    (*cur_node)->children.push_back(std::unique_ptr<Node>(new Node(item, parent)));
    (*cur_node)->children.back()->print = true;
  }
  else {
    //in this case we either matched a whole word on one or more nodes OR we ran out of things to look at and must insert
    std::unique_ptr<Node>* back = &(*cur_node);
    std::string prefix{(*back)->rep.substr(0, matched_len)}; //root to split on
    std::string suffix{(*back)->rep.substr(matched_len)}; //existing suffix, if any
    std::string new_suffix{item.substr(total_matched_len)}; //new suffix, if any
    //std::cout << "back=" << (*back)->rep << " p=" << prefix << " s=" << suffix << " ns=" << new_suffix << " ml=" << matched_len << std::endl;
    (*back)->rep = prefix;
  
    if(suffix.length() > 0){
      std::unique_ptr<Node> tmp(new Node(suffix, (*back).get()));
      for(auto&& node : (*back)->children){
        node->parent = tmp.get();
        tmp->children.push_back(std::move(node)); 
      }
      (*back)->children.clear();
      (*back)->children.push_back(std::move(tmp));
      (*back)->children.back()->print = (*back)->print;
    }
    //edge cases where the original item was printable or not...
    if(new_suffix.length() > 0){
      (*back)->children.push_back(std::unique_ptr<Node>(new Node(new_suffix, (*back).get())));
      (*back)->children.back()->print = true;
      if (suffix.length() > 0) (*back)->print = false;
    }
    //after fixing up the children nodes, check to see if we matched a whole word on a subset of an existing node and mark accordingly
    if(total_matched_len == item.length()) (*back)->print = true;
  }
  _size++;
}

void RadixTree::Node::lookup_helper(std::vector<std::string>& list, const size_t index, const std::string& word) const noexcept{
  int tmp_index = index;
  for(auto&& letter : rep){
    if (letter == word[tmp_index]){
      //std::cout << "t_i: " << tmp_index << std::endl;
      tmp_index++;
    }
    //else break; 
  } 
  for(auto&& child : children){
    child->lookup_helper(list, tmp_index, word);
  }
  //if this is an actual word and we matched +/- 1 number of letters
  if (print && ((tmp_index <= (word.length() + 1)) && (tmp_index >= (word.length() - 1)))){
    Node* tmp = parent;
    std::string t_s = rep;
    //std::cout << "t_i: " << tmp_index << " rep: " << rep << std::endl;
    while(tmp != nullptr){
      std::string s_tmp = tmp->rep + t_s;
      t_s = s_tmp;
      tmp = tmp->parent;
    }
    //more culling, is the length of the resultant word for this slot +/- in length to the searched word?
    if((t_s.length() <= word.length() + 1) && (t_s.length() >= word.length() - 1))  list.push_back(t_s);
  }
}

void RadixTree::lookup(const std::string& item) noexcept {
  std::cout << "Searching for " << item << std::endl;
  if(exists(item)) {
    std::cout << "RadixTree contains: " << item << std::endl;
    return;
  }

  std::vector<std::string> hits{};
  for(auto&& node : _root->children){
    if(node->rep[0] == item[0]){
      node->lookup_helper(hits, 0, item);
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

void RadixTree::Node::print_helper(std::ostream& os) const noexcept {
  static size_t indent = 0;
  os << rep << "[" << ((print)? "1" : "0") << "]\n";
  for(auto&& node : children){
    if(node != nullptr){
      indent++;
      for(size_t i=0;i<indent;i++) os << "  ";
      os << "-> ";
      node->print_helper(os);
      indent--;
    } 
  }
}

void RadixTree::Node::word_list_helper(std::vector<std::string>& list) noexcept{
  if(print){
    Node* tmp = parent;
    std::string t_s = rep;
    while(tmp != nullptr){
      std::string s_tmp = tmp->rep + t_s;
      t_s = s_tmp;
      tmp = tmp->parent;
    }
    list.push_back(t_s);
  }
  for(auto&& node : children){
    node->word_list_helper(list);
  }
}

std::vector<std::string> RadixTree::word_list() noexcept{
  std::vector<std::string> list{};
  for(auto&& node : _root->children){
    if (node) node->word_list_helper(list);
  }
  return list;
}

std::ostream& operator<<(std::ostream& os, const RadixTree& rhs){
  std::cout << rhs._size << " words in radixTree:\n";
  for(auto&& node : rhs._root->children){
    if(node) node->print_helper(os);
  }
  return os;
}

#endif
