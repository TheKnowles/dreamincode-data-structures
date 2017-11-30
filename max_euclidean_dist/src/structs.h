#ifndef __STRUCTS_H
#define __STRUCTS_H

#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <set>
#include <iterator>
#include <bitset>

using namespace std;

struct Movement{
  Movement(int a, int b):x(a),y(b){}
  int x;
  int y;
  friend ostream& operator<<(ostream&, const Movement&);
};

bool operator <(const Movement& lhs, const Movement& rhs){
  return lhs.x < rhs.x;
}

bool operator ==(const Movement& lhs, const Movement& rhs){
  return ((lhs.x == rhs.x) && (lhs.y == rhs.y));
}

ostream& operator<<(ostream& os, const Movement& rhs){
  os << "(" << rhs.x << "," << rhs.y << ")";
  return os; 
}

struct Point {
  Point():x(0),y(0) {}
  Point(int a, int b):x(a),y(b) {}
  Point(const Point& p):x(p.x),y(p.y) {}
  int x;
  int y;
  friend ostream& operator<<(ostream&, const Point&);
  Point& operator+=(const Movement& rhs){
    this->x += rhs.x;
    this->y += rhs.y;
    return *this;
  }
};

ostream& operator<<(ostream& os, const Point& rhs){
  os << "[" << rhs.x << "," << rhs.y << "]";
  return os; 
}

struct Result {
  Result():dist(0),moves(0) {}
  int dist;
  int moves;
  vector<Movement> movements;
  friend ostream& operator<<(ostream&, const Result&);
};

ostream& operator<<(ostream& os, const Result& rhs){
  os << "Result: dist:" << rhs.dist << " moves: " << rhs.moves << " >> (0,0) ";
  for(const Movement& m : rhs.movements) os << m << " ";
  return os; 
}

//comparison functor to put Results in a set
//check the dist/max, then check length of movements
//then check if it's a permuation and throw it away if it is - path ordering does not matter
struct result_cmp {
  bool operator()(const Result& lhs, const Result& rhs){
    if (lhs.moves < rhs.moves) return true;
    if (lhs.dist < rhs.dist) return true;
    if(lhs.movements.size() < rhs.movements.size()) return true;
    if(lhs.movements.size() == rhs.movements.size()) {
      if (is_permutation(lhs.movements.begin(), lhs.movements.end(), rhs.movements.begin())){
        return false;
      }
      //no perm, enforce strict weak ordering
      for(int i = 0; i < lhs.movements.size(); i++){
        if(lhs.movements.at(i) < rhs.movements.at(i)) return true;
      }
    }
    return false;
  }
};


#endif
