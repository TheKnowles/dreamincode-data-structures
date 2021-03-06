#include "Table.h"

int main(){
  vector<Interval> intervals;
  Interval one(3,9);
  Interval two(1,6);
  Interval three(4,8);
  intervals.push_back(one);
  intervals.push_back(two);
  intervals.push_back(three);

  Table table(find_storage_size(intervals));
  for(Interval& in : intervals) table.add_interval(in);
  cout << table;
  cout << "\n\n";
  cout << "Longest intersection of any 2 lines:\n";
  Interval result = table.find_max_overlap(2);
  cout << result;
  cout << "Longest intersection of any 3 lines:\n";
  result = table.find_max_overlap(3);
  cout << result;

  cout << "\n\n";
  intervals.push_back(Interval(1,9));
  Table table2(find_storage_size(intervals));
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


  vector<Interval> intervals3;
  intervals3.push_back(Interval(4, 6)); 
  intervals3.push_back(Interval(2, 5));
  intervals3.push_back(Interval(3, 4)); 
  intervals3.push_back(Interval(3, 5)); 
  intervals3.push_back(Interval(3, 6)); 
  intervals3.push_back(Interval(5, 8)); 
  intervals3.push_back(Interval(4, 7)); 
  intervals3.push_back(Interval(1, 5)); 
  intervals3.push_back(Interval(4, 6)); 
  intervals3.push_back(Interval(3, 6)); 
  intervals3.push_back(Interval(2, 8)); 
  //intervals3.push_back(Interval(2, 8)); 
  intervals3.push_back(Interval(1, 2)); 
  intervals3.push_back(Interval(4, 5)); 
  intervals3.push_back(Interval(5, 7)); 
  intervals3.push_back(Interval(1, 4)); 
  intervals3.push_back(Interval(1, 6)); 
  //intervals3.push_back(Interval(3, 5)); 
  intervals3.push_back(Interval(2, 4)); 
  //intervals3.push_back(Interval(3, 5));

  
  Table table4(find_storage_size(intervals3));
  for(Interval& in : intervals3) table4.add_interval(in);

  cout << table4;
  cout << "\n\n";
  cout << "Longest intersection of any 8 lines:\n";
  result = table4.find_max_overlap(8);
  cout << result;
  
  return 0;
}
