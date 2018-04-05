/*
Bradley Bauer
04-01-2018
Project 9
Section 14

This file implements a MapSet class.
A mapset class acts like a map since it stores key-value pairs and it acts
like a set because no two stored pairs have the same key.
The mapset's data is ordered such that key-value pairs with smaller keys appear earlier in the internal buffer.
*/

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

#include "proj09_mapset.h"

using cpr = const pair<string, long>&;
using pr = pair<string, long>&;

// Key-value pair convenience functions
static string KEY(cpr p) { return p.first; }
static string& KEY(pr p) { return p.first; }
static long VAL(cpr p) { return p.second; }
static long& VAL(pr p) { return p.second; }
static bool key_less(cpr lhs, cpr rhs) { return KEY(lhs) < KEY(rhs); }
static bool key_equal(cpr lhs, cpr rhs) { return KEY(lhs) == KEY(rhs); }

/*
Check if the iter points to a key-value pair whose key is equal to the input key
*/
using vpi = vector<pair<string, long>>::iterator;
static bool key_in_map(const vpi &iter, const vpi &end, const string &key) {
    return iter != end && KEY(*iter) == key;
}

/*
Find the position of the first pair whose key is greater than or equal to (not less than) the input key
Inputs:
    key (string): the key to find a position for
Outputs:
    (vector<pair<string,long>>::iterator): an iterator of the mapset's internal buffer
*/
vpi MapSet::find_key(string key) {
    // returns v_.end() if input key is greater than all keys in v_
    // returns v_.begin() if input key is less than or equal to KEY(v_[0])
    return lower_bound(v_.begin(), v_.end(), key, [](cpr p, const string & key) { return KEY(p) < key; });
}

/*
Initializes the mapset with pairs from an initializer list
Inputs:
    il (initializer_list<pr>): an list of pairs to put into the mapset
*/
MapSet::MapSet(initializer_list<pair<string, long>> il) {
    // Add each element of the initializer_list to the mapset
    for (const auto & el : il) {
        add(el);
    }
}

/*
Gives the number of pairs currently in the mapset
*/
size_t MapSet::size() {
    return v_.size();
}

/*
Return the pair with the given key from the mapset
If the key is not found return an empty pair
Inputs:
    key (string): the key to lookup in the mapset
Outputs:
    (pair<string, long>): the pair with pair.key == key, or an empty pair
*/
pair<string, long> MapSet::get(string key) {
    vpi p_iter = find_key(key);

    // if key is not in the mapset, then return an empty pair
    if (!key_in_map(p_iter, v_.end(), key)) {
        return pair<string, long>{"", 0};
    }

    return *p_iter;
}

/*
Set the value of the pair with the given key to the given value if the pair is found in the mapset
Inputs:
    key (string): the key to look for in the mapset
    val (long): the new value of the key-value pair
Outputs:
    (bool): whether a key-value pair with the given key was found in the mapset
*/
bool MapSet::update(string key, long val) {
    vpi p_iter = find_key(key);

    // if key is not in the mapset, then do not try to update a pair
    if (!key_in_map(p_iter, v_.end(), key)) {
        return false;
    }

    KEY(*p_iter) = key;
    VAL(*p_iter) = val;
    return true;
}

/*
Removes a key-value pair from the mapset
Inputs:
    key (string): the key of the pair to remove
Outputs:
    (bool): whether a pair was removed from the mapset
*/
bool MapSet::remove(string key) {
    vpi p_iter = find_key(key);

    // If key is not in the mapset, then do not try to remove a pair
    if (!key_in_map(p_iter, v_.end(), key)) {
        return false;
    }

    v_.erase(p_iter);
    return true;
}

/*
Adds a key-value pair to the mapset
Inputs:
    p (pair<string,long>): the pair to add
Outputs:
    (bool): whether a pair was added to the mapset
*/
bool MapSet::add(pair<string, long> p) {
    vpi p_iter = find_key(KEY(p));

    // If key is in the mapset, then do not add a pair to the mapset
    if (key_in_map(p_iter, v_.end(), KEY(p))) {
        return false;
    }

    v_.insert(p_iter, p);
    return true;
}

/*
Compares the calling mapset's keys with the input mapset's keys
Similar to comparing lists of strings in python
Inputs:
    m (MapSet&): the mapset to compare against
Outputs:
    (int): -1 if v_.keys() < m.v_.keys() (or if each key compares equal but v_ has less number of keys)
            0 if v_.keys() = m.v_.keys() (each have same number of keys and they all compare equal)
            1 if v_.keys() > m.v_.keys() (or if each key compares equal but m.v_ has less number of keys)
*/
int MapSet::compare(MapSet &m) {
    // Check for equal keys
    bool v_eq_m = equal(v_.begin(), v_.end(), m.v_.begin(), m.v_.end(), key_equal);

    if (v_eq_m) {
        return 0;
    }

    // true if there is an i s.t. KEY(v_[i]) < KEY(m.v_[i]), otherwise true if v_.size() < m.v_.size()
    bool v_lt_m = lexicographical_compare(v_.begin(), v_.end(), m.v_.begin(), m.v_.end(), key_less);

    if (v_lt_m) {
        return -1;
    }
    return 1;
}

/*
Computes the union of the calling mapset and an input mapset
Inputs:
    m (MapSet&): the mapset to combine with the calling mapset
Outputs:
    ms_union (MapSet): the union of the calling mapset and input mapset
*/
MapSet MapSet::mapset_union(MapSet &m) {
    MapSet ms_union;
    // set_union will not output duplicates since there are no duplicates in either v_ or m.v_
    set_union(v_.begin(), v_.end(), m.v_.begin(), m.v_.end(), back_inserter(ms_union.v_), key_less);
    return ms_union;
}

/*
Computes the intersection of the calling mapset and an input mapset
Inputs:
    m (MapSet&): the mapset to intersect with the calling mapset
Outputs:
    intersection (MapSet): the intersect of the calling mapset and input mapset
*/
MapSet MapSet::mapset_intersection(MapSet &m) {
    MapSet intersection;
    // set_intersection will not output duplicates since there are no duplicates in either v_ or m.v_
    set_intersection(v_.begin(), v_.end(), m.v_.begin(), m.v_.end(), back_inserter(intersection.v_), key_less);
    return intersection;
}

/*
Puts a string representation of the mapset into an out stream
Inputs:
    out (ostream&): the outstream to put the mapset's string representation
    m (MapSet&): the mapset to put into the stream
Outputs:
    out (ostream&): the provided outstream
*/
ostream & operator<<(ostream &out, MapSet &m) {
    // output all but the last pair
    for_each(m.v_.begin(), --m.v_.end(), [&out](cpr p) { out << KEY(p) << ":" << VAL(p) << ", "; });

    // output the last pair without the trailing comma
    cpr p = m.v_.back();
    return out << KEY(p) << ":" << VAL(p);
}
