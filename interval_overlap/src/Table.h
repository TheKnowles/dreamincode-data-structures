#ifndef __TABLE_H
#define __TABLE_H
#include <iostream>
#include <vector>
#include <cmath>
#include <bitset>

using namespace std;

struct Interval {
  Interval():start(0),end(0),length(0){}
  Interval(int a, int b):start(a),end(b), length(b-a+2){}
  bool contains(int c) {return ((c >= start && c <= end));}
  int start;
  int end;
  int length;
  friend ostream& operator<<(ostream& os, Interval& rhs);
};

ostream& operator<<(ostream& os, Interval& rhs){
  os << "[" << rhs.start << "," << rhs.end << "]\n";
  return os;
}

struct Slot {
  Slot():intervals(0),start(false),end(false),part_of(0x0) {}
  unsigned int intervals;
  bool start, end;
  bitset<50> part_of; //bit mask what Intervals this slot is a part of
};

//table requires knowledge of the max elements it can have from any interval
struct Table {
  Table(int r):range(r){ intervals = new Slot[range];}
  ~Table() {delete[] intervals;}
  int range;
  Slot* intervals;
  friend ostream& operator<<(ostream& os, Table& rhs);
  void add_interval(const Interval& rhs){
    static unsigned int mask = 0x00;
    for(int i = rhs.start; i <= rhs.end; i++) {
      intervals[i].intervals++;
      if (i == rhs.start){
        intervals[i].start = true;
      }
      if (i == rhs.end) {
        intervals[i].end = true;
      }
      intervals[i].part_of |= (int)pow(2.0, (int)mask); 
    }
    ++mask;
  }
  void print_starts(){
    cout << "Starts:\n";
    for(int i = 1; i < range; i++) if (intervals[i].start == true) cout << i << endl;
  } 

  void print_ends(){
    cout << "Ends:\n";
    for(int i = 1; i < range; i++) if (intervals[i].end == true) cout << i << endl;
  } 

  Interval find_max_overlap(int k){
    int max = 0;
    int max_start = 0;
    int max_end = 0;
    Interval result;
    //throw out anything < k  
    //search from start to end
    for(int i = 1; i < range; i++){
      if(intervals[i].intervals < k) continue;
      if (intervals[i].start){
        int j = i+1;
        int tmp = 0;
        while(intervals[j].intervals >= k && j < range) {
          if(intervals[j].end == true && ((intervals[i].part_of & intervals[j].part_of) == intervals[j].part_of)){
            if (tmp > max){
              max = tmp;
              max_start = i;
              max_end = j;
            }
          }
          tmp = (j-i+1);
          j++;
        }//while
      }
    }//linear pass
    result.start = max_start;
    result.end = max_end;
    result.length = result.end-result.start+2;
    return result;
  }
};

ostream& operator<<(ostream& os, Table& rhs){
  os << "\nnumber line | ";
  for(int i = 1; i < rhs.range; i++) os << i << " | "; 
  os << "\n---------------------------------------------------------------\n";
  os << "intervals   | ";
  for(int i = 1; i < rhs.range; i++) os << rhs.intervals[i].intervals << " | ";
  os << "\n";
  return os; 
}

int find_storage_size(const vector<Interval>& intervals){
  int max = 0;
  for(const Interval& interval : intervals){
    if (interval.end > max) max = interval.end;
  }
  return (max+1);//to 1 base the array
}

#endif
