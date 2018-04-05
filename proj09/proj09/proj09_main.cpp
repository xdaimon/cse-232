#include "proj09_mapset.h"
using namespace std;

int main (){
  pair<string,long> itr;
  cout << boolalpha;
  
  MapSet ms ({ {"bill", 1}, {"fred", 2}, {"alan", 3}, {"bob", 4} });
  cout << "Size:"<<ms.size() << endl;

  MapSet ms_copy(ms);

  cout << "ms:"<<ms<<endl;
  cout << "add ed: " << ms.add({"ed", 5}) << endl;
  cout << "add bob: " << ms.add({"bob", 5}) << endl;
  cout << "ms: "<< ms << endl;
  
  cout << "bob update:" << ms.update("bob", 100) << endl;
  cout <<  "george update:" << ms.update("george", 10) << endl;
  cout << "ms: "<< ms << endl;
  cout << "ms_copy: " << ms_copy << endl;

  cout << "remove fred: " << ms_copy.remove("fred") << endl;
  cout << "remove george: " << ms_copy.remove("george") << endl;  

  cout << endl;
  cout << "ms:"<< ms << endl;  
  cout << "copy:"<< ms_copy << endl;  
  cout << endl;

  cout << "copy2ms: "<< ms_copy.compare(ms) << endl;
  cout << "ms2copy: "<<ms.compare(ms_copy) << endl;
  cout << "ms2ms: "<<ms.compare(ms) << endl;
  
  auto union_ms = ms.mapset_union(ms_copy);
  cout << "union: " <<union_ms << endl;
  auto intersect_ms = ms.mapset_intersection(ms_copy);
  cout << "intersection: "<< intersect_ms <<endl;

  MapSet ms1({ {"bill",1},{"alan",2},{"abby",3} });
  MapSet ms2({ {"alan",2},{"abby", 3},{"john",4} });
  MapSet ms3({ {"abby",5}, {"alan",17} });

  auto ASSERT_EQ = [&](int x, int y) { 
      if (x != y)
          cout << x << endl;
      else
          cout << true << endl;
  };
  int result1 = ms1.compare(ms2);
  int ans1 = -1;
  ASSERT_EQ(result1,ans1);
  int result2 = ms2.compare(ms1);
  int ans2 = 1;
  ASSERT_EQ(result2,ans2);
  int result3 = ms1.compare(ms1);
  int ans3 = 0;
  ASSERT_EQ(result3,ans3);
  int result4 = ms1.compare(ms3);
  int ans4 = 1;
  ASSERT_EQ(result4, ans4);
  int result5 = ms3.compare(ms1);
  int ans5 = -1;
  ASSERT_EQ(result5, ans5);

}
  
