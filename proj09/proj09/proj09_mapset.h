#ifndef MAP_SET
#define MAP_SET

#include<iostream>
using std::ostream;
#include<string>
using std::string;
#include<utility>
using std::pair;
#include<vector>
using std::vector;
#include<initializer_list>
using std::initializer_list;
#include<algorithm>
using std::lexicographical_compare;
using std::lower_bound;
using std::equal;
using std::set_intersection;
using std::set_union;
using std::for_each;
#include <iterator>
using std::back_inserter;

class MapSet{
 private:
  vector< pair<string,long> > v_;
  vector< pair <string, long> >::iterator find_key(string);
 public:
  MapSet() = default;
  MapSet(initializer_list< pair<string,long> >);

  size_t size();
  pair<string,long> get(string);
  bool update(string,long);
  bool remove (string);
  bool add(pair<string,long>);
  int compare(MapSet&);
  MapSet mapset_union (MapSet&);
  MapSet mapset_intersection(MapSet&);
  friend ostream& operator<<(ostream&, MapSet&);  

};

ostream& operator<<(ostream&, MapSet&);

#endif
  
