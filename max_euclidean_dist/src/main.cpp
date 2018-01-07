#include "structs.h"

int euc_dist(const Point& start, const Point& end){
  int dx = start.x - end.x;
  int dy = start.y - end.y;
  return sqrt(dx*dx + dy*dy);
}

void calculate_all_dists(const Point& start, vector<Movement>& movements, set<Result, result_cmp>& results){
  cout << "Starting at: " << start << " with " << movements.size() << " max moves\n";
  Point tmp(start);
  sort(movements.begin(), movements.end());
  int count  = 0;
  int x = 0, y = 0;
  Result result; 
  do {
    for(int i = 0; i < movements.size(); i++) {
      x += movements.at(i).x;
      y += movements.at(i).y;
      Point end(x,y);
      int tmp_dist = euc_dist(start, end);
      if (tmp_dist >= result.dist) {
        result.movements.clear();
        result.dist = tmp_dist;
        result.moves = i+1;
        for(int j = 0; j <= i; j++) result.movements.push_back(movements.at(j));
        results.insert(result);
      }
    }
    ++count;
  } while(next_permutation(movements.begin(), movements.end()));
  cout << "Total permutations: " << count << endl;
  cout << "Max distance: " << result << endl;
  //cull out any < max distance elements that were added before actual max was discovered
  set<Result, result_cmp>::iterator it = results.begin();
  while(it != results.end()){
    if (it->dist < result.dist) results.erase(it);
    it++;
  }
}

const size_t size = sizeof(int) * 8;
//const size_t size = 5;
typedef std::bitset<size> euc_bitset;

//calculate subset values, vector operations are associative and communative
void calculate_all_dist_opt(const Point& start, vector<Movement>& movements, set<Result, result_cmp>& results){
  cout << "[Opt] Starting at: " << start << " with " << movements.size() << " max moves\n";
  Point tmp(start);
  sort(movements.begin(), movements.end());
  int count  = 0;
  int x = 0, y = 0;
  Result result;
  int max_sets = pow(2.0, movements.size());
  //generate, but ignore empty set
  vector < euc_bitset > subsets;
  for(int i = 0; i < max_sets; i++){
    euc_bitset tmp;
    tmp = i;
    subsets.push_back(tmp);
  }

  //compare to permutations above
  cout << "Subsets generated: " << subsets.size() << endl;
  //for(euc_bitset set : subsets) cout << set << endl;

  int max_dist = 0;
  //find max dist
  for(euc_bitset set : subsets){
    int x_sum = 0;
    int y_sum = 0;
    for(int i = 0; i < movements.size(); i++){
      if(set[i]){
        x_sum += movements.at(i).x;
        y_sum += movements.at(i).y;
        result.movements.push_back(movements.at(i));
      }
    }
    Point end(x_sum, y_sum);
    result.dist = euc_dist(start, end);
    result.moves = result.movements.size();
    //cout << set << " ~~~ " << start <<  " -> " << end << " = " << euc_dist(start, end) << "  -----> " << result << endl;
    if(result.dist >= max_dist){
      results.insert(result);
      max_dist = result.dist;
    }
    result.movements.clear();
  }
  //cull out any < max distance elements that were added before actual max was discovered
  set<Result, result_cmp>::iterator it = results.begin();
  while(it != results.end()){
    if (it->dist < max_dist) results.erase(it);
    it++;
  }
}

//return the bearing degree (we'll subtract 180 to see if we're moving farther away from the origin)
double bearing(double a1, double a2, double b1, double b2) {
  static const double TWOPI = 6.2831853071795865;
  static const double RAD2DEG = 57.2957795130823209;
  double theta = atan2(b1 - a1, a2 - b2);
  if (theta < 0.0)
      theta += TWOPI;
  return RAD2DEG * theta;
}

void calculate_all_dist_polynomial_time(const Point& start, vector<Movement>& movements, set<Result, result_cmp>& results){
  cout << "[polynomial] Starting at: " << start << " with " << movements.size() << " max moves\n";
  Point tmp(start);
  int count  = 0;
  int max_dist = 0;
  // O(n^2)
  for(int i = 0; i < movements.size(); i++){
    Result result;
    int x_sum = 0;
    int y_sum = 0;
    Movement& one = movements.at(i);
    x_sum += movements.at(i).x;
    y_sum += movements.at(i).y;
    result.movements.push_back(movements.at(i));
    for(int j = 0; j < movements.size(); j++){
      Movement& two = movements.at(j);
      if(one == two) continue; //don't evaluate the same one, does not take care of edge case of multiple of the same value
      double degree = bearing(x_sum, y_sum, (x_sum+two.x), (y_sum+two.y));
      if ((degree - 180) >= 0){
        result.movements.push_back(movements.at(j));
	x_sum += movements.at(j).x;
	y_sum += movements.at(j).y;
	Point end(x_sum, y_sum);
	result.dist = euc_dist(start, end);
	result.moves = result.movements.size();
	if(result.dist >= max_dist){
	  results.insert(result);
	  max_dist = result.dist;
	}
      }
    }
  }
  //cull out any < max distance elements that were added before actual max was discovered
  set<Result, result_cmp>::iterator it = results.begin();
  while(it != results.end()){
    if (it->dist < max_dist) results.erase(it);
    it++;
  }
}

int main(){
  Point start(0,0);
  vector<Movement> movements;
  set<Result, result_cmp> results;
  //initial sample set
  movements.push_back(Movement(2,-2));
  movements.push_back(Movement(-2,-2));
  movements.push_back(Movement(0,2));
  movements.push_back(Movement(3,1));
  movements.push_back(Movement(-3,1));
  calculate_all_dists(start, movements, results);
  for(const Result& result : results) cout << result << endl;
  results.clear();
  calculate_all_dist_opt(start, movements, results);
  for(const Result& result : results) cout << result << endl;
  results.clear();
  calculate_all_dist_polynomial_time(start, movements, results);
  for(const Result& result : results) cout << result << endl;
  return 0;
}
