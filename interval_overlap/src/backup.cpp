#include <iostream>
#include <vector>
#include <cmath>

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
  unsigned int part_of; //bit mask what Intervals this slot is a part of
};

//table requires knowledge of the max elements it can have from any interval
struct Table {
  Table(int r):range(r){ intervals = new Slot[range];}
  ~Table() {delete[] intervals;}
  int range;
  Slot* intervals; //for simplicity now
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
          //cout << "checking " << i << " " << j << endl;
          if(intervals[j].end == true && ((intervals[i].part_of & intervals[j].part_of) == intervals[j].part_of)){
            //cout << " i: " << i << " j: " << j << endl;
            //cout << " i mask: " << intervals[i].part_of << " j mask: " << intervals[j].part_of << endl;
            //cout << " i & j: " << (intervals[i].part_of & intervals[j].part_of) << endl;
            //cout << "found overlap of size: " << (j-i+1) << endl;
            if (tmp > max){
              //cout << "setting max to tmp\n";
              max = tmp;
              max_start = i;
              max_end = j;
            }
          }
          tmp = (j-i+1);
          j++;
          //if (j > 12) break;
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
  os << "intervals   | ";
  for(int i = 1; i < rhs.range; i++) os << rhs.intervals[i].intervals << " | ";
  os << "\n----------------------------------------------------------";
  os << "\nnumber line | ";
  for(int i = 1; i < rhs.range; i++) os << i << " | "; 
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

int main(){
  vector<Interval> intervals;
  Interval one(3,9);
  Interval two(1,6);
  Interval three(4,8);
  intervals.push_back(one);
  intervals.push_back(two);
  intervals.push_back(three);

  //for(Interval& in : intervals) cout << in;
  //build out 1D dynamic table
  Table table(find_storage_size(intervals));
  cout << "ex 1 storage size: " << find_storage_size(intervals) << endl;
  for(Interval& in : intervals) table.add_interval(in);
  cout << table;
  cout << "\n\n";
  //table.print_starts();
  //table.print_ends();
  cout << "Longest intersection of any 2 lines:\n";
  Interval result = table.find_max_overlap(2);
  cout << result;
  cout << "Longest intersection of any 3 lines:\n";
  result = table.find_max_overlap(3);
  cout << result;

  cout << "\n\n";
  intervals.push_back(Interval(1,9));
  Table table2(find_storage_size(intervals));
  cout << "ex 2 storage size: " << find_storage_size(intervals) << endl;
  //for(Interval& in : intervals) cout << in;
  for(Interval& in : intervals) table2.add_interval(in);
  cout << table2;
  cout << "\n\n";
  cout << "Longest intersection of any 2 lines:\n";
  result = table2.find_max_overlap(2);
  cout << result;
  cout << "Longest intersection of any 3 lines:\n";
  result = table2.find_max_overlap(3);
  cout << result;


  cout << "\n\n";
  //another example
  vector<Interval> intervals2;  
  Interval one2(3,8);
  Interval two2(4,12);
  Interval three2(2,6);
  Interval four(1,10);
  Interval five(5,9);
  Interval six(11,12);
  intervals2.push_back(one2);
  intervals2.push_back(two2);
  intervals2.push_back(three2);
  intervals2.push_back(four);
  intervals2.push_back(five);
  intervals2.push_back(six);

  Table table3(find_storage_size(intervals2));
  cout << "ex 3 storage size: " << find_storage_size(intervals2) << endl;
  //for(Interval& in : intervals) cout << in;
  for(Interval& in : intervals2) table3.add_interval(in);
  cout << table3;
  cout << "\n\n";
  cout << "Longest intersection of any 2 lines:\n";
  result = table3.find_max_overlap(2);
  cout << result;
  cout << "Longest intersection of any 3 lines:\n";
  result = table3.find_max_overlap(3);
  cout << result;
  cout << "Longest intersection of any 4 lines:\n";
  result = table3.find_max_overlap(4);
  cout << result;
  return 0;
}
