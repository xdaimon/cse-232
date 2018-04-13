/*
Bradley Bauer
Section 14
04-12-18
Project 10

This code implements a mapset by using dynamic memory instead of STL vectors.
*/

#ifndef MAP_SET
#define MAP_SET

#include<iostream>
using std::ostream;
#include<string>
using std::string;
#include<vector>
using std::vector;
#include<initializer_list>
using std::initializer_list;
#include<algorithm>
using std::sort; using std::lower_bound;
using std::copy; using std::swap;
#include<sstream>
using std::ostringstream;

//
// Node
//
template<typename K, typename V>
struct Node {
    K first; // key
    V second; // value

    Node() = default;
    Node(K k, V v) : first(k), second(v) {}

    bool operator<(const Node& n) const { return first < n.first; }
    bool operator==(const Node& n) const { return first == n.first; }

    friend ostream& operator<<(ostream &out, const Node &n) {
        return out << n.first << ":" << n.second;
    }
};

//
// MapSet
// 
template<typename K, typename V>
class MapSet {
private:
    Node<K, V>* ary_;
    size_t last_;
    size_t capacity_;
    Node<K, V>* find_key(K);
    void grow();
public:
    MapSet(int sz = 2);
    MapSet(initializer_list< Node<K, V> >);
    MapSet(const MapSet&);
    MapSet& operator=(MapSet); // copy and swap
    ~MapSet();

    size_t size();
    bool remove(K);
    bool add(Node<K, V>);
    Node<K, V> get(K);
    bool update(K, V);
    int compare(MapSet&);
    MapSet mapset_union(MapSet&);
    MapSet mapset_intersection(MapSet&);

    friend ostream& operator<<(ostream &out, const MapSet &ms) {
        if (!ms.last_) {
            return out;
        }
        // output all but the last node
        for_each(ms.begin(), ms.end() - 1, [&out](const Node<K, V>& n) { out << n << ", "; });

        // output the last node without the trailing comma
        return out << *(ms.end() - 1);
    }

    // Key-value convenience funcs
    static K KEY(const Node<K, V>& p) { return p.first; }
    static K& KEY(Node<K, V>& p) { return p.first; }
    static V& VAL(Node<K, V>& p) { return p.second; }

    Node<K, V>* end() const { return ary_ + last_; }
    Node<K, V>* begin() const { return ary_; }
    
    // Check if the node pointed to by iter contains key
    bool key_at_iter(const Node<K, V>* iter, const K &key) {
        return (iter != nullptr) && (iter != end()) && (KEY(*iter) == key);
    }
};

/*
Initialize an empty mapset with sz capacity
Inputs:
    sz (int): capacity of the mapset
*/
template<typename K, typename V>
MapSet<K, V>::MapSet(int sz) {
    // grow() fails with capacity_ == 0
    if (sz <= 0) {
        sz = 1;
    }
    capacity_ = sz;
    last_ = 0;
    ary_ = new Node<K, V>[capacity_];
}

/*
Initializes the mapset with nodes from an initializer list
Inputs:
    il (initializer_list<pr>): an list of nodes to put into the mapset
*/
template<typename K, typename V>
MapSet<K, V>::MapSet(initializer_list< Node<K, V> > il) : MapSet(il.size()) {
    // Add each element of the initializer_list to the mapset
    for (const Node<K, V>& el : il) {
        add(el);
    }
}

/*
Copy contstructs a new mapset
Inputs:
    ms (const MapSet &): the mapset to copy
*/
template<typename K, typename V>
MapSet<K, V>::MapSet(const MapSet &ms) {
    capacity_ = ms.capacity_;
    last_ = ms.last_;
    ary_ = new Node<K, V>[capacity_];
    copy(ms.begin(), ms.end(), begin());
}

/*
Assign the input mapset to *this by copy and swap
Inputs:
    ms (MapSet): a copy of some mapset
*/
template<typename K, typename V>
MapSet<K, V>& MapSet<K, V>::operator=(MapSet<K, V> ms) {
    swap(this->ary_, ms.ary_);
    swap(this->capacity_, ms.capacity_);
    swap(this->last_, ms.last_);
    return *this;
    // ms destructor deletes memory once owned by *this
}

/*
Delete the ary_ memory
*/
template<typename K, typename V>
MapSet<K, V>::~MapSet() {
    delete[] ary_;
}

/*
Gives the number of nodes in the mapset
*/
template<typename K, typename V>
size_t MapSet<K, V>::size() {
    return last_;
}

/*
Doubles the capacity of the mapset
*/
template<typename K, typename V>
void MapSet<K, V>::grow() {
    capacity_ *= 2;
    auto *temp = new Node<K, V>[capacity_];
    copy(begin(), end(), temp);
    delete[] ary_;
    ary_ = temp;
}

/*
Find the position of the first node whose key is greater than or equal to (not less than) the input key
Inputs:
    key (K): the key to find a position for
Outputs:
    (Node<K,V>*): a pointer into the mapset's internal buffer
*/
template<typename K, typename V>
Node<K, V>* MapSet<K, V>::find_key(K key) {
    return lower_bound(begin(), end(), Node<K, V>{ key, V() });
}

/*
Adds a node to the mapset
Inputs:
    p (Node): the node to add
Outputs:
    (bool): whether a node was added to the mapset
*/
template<typename K, typename V>
bool MapSet<K, V>::add(Node<K, V> n) {
    // stores an index because grow() may invalidate pointer
    int p = find_key(KEY(n)) - begin();

    // If key is in the mapset, then do not add a node to the mapset
    if (key_at_iter(ary_+p, KEY(n))) {
        return false;
    }

    // If there is no more room
    if (capacity_ == last_) {
        grow();
    }

    // Shift each element in range [p, end) to the right by one
    for (int i = last_; i > p; --i) {
        ary_[i] = ary_[i - 1];
    }
    
    ary_[p] = n;
    last_++;

    return true;
}

/*
Removes a node from the mapset
Inputs:
    key (string): the key of the node to remove
Outputs:
    (bool): whether a node was removed from the mapset
*/
template<typename K, typename V>
bool MapSet<K, V>::remove(K key) {
    auto *p = find_key(key);

    // If key is not in the mapset, then do not try to remove a node
    if (!key_at_iter(p, key)) {
        return false;
    }

    // Shift each element in the range [p+1, end) to the left by one
    copy(p + 1, end(), p);

    last_--;

    return true;
}

/*
Return the node with the given key from the mapset
If the key is not found return an empty node
Inputs:
    key (string): the key to lookup in the mapset
Outputs:
    (Node): the node with node.key == key, or an empty node
*/
template<typename K, typename V>
Node<K, V> MapSet<K, V>::get(K key) {
    auto *p = find_key(key);

    // return an empty node if key is not found
    if (!key_at_iter(p, key)) {
        return Node<K, V>();
    }

    return *p;
}

/*
Set the value of the node with the given key to the given value if the node is found in the mapset
Inputs:
    key (string): the key to look for in the mapset
    val (long): the new value of the node
Outputs:
    (bool): whether a node with the given key was found in the mapset
*/
template<typename K, typename V>
bool MapSet<K, V>::update(K key, V value) {
    auto *p = find_key(key);

    // if key is not in the mapset, then do not update a node
    if (!key_at_iter(p, key)) {
        return false;
    }

    KEY(*p) = key;
    VAL(*p) = value;

    return true;
}

/*
Compares the calling mapset's keys with the input mapset's keys
Similar to comparing lists of strings in python
Inputs:
    m (MapSet&): the mapset to compare against
Outputs:
    (int): -1 if this.keys() < ms.keys() (or if each key compares equal but v_ has less number of keys)
            0 if this.keys() = ms.keys() (each have same number of keys and they all compare equal)
            1 if this.keys() > ms.keys() (or if each key compares equal but m.v_ has less number of keys)
*/
template<typename K, typename V>
int MapSet<K, V>::compare(MapSet &ms) {
    // Check for equal keys
    bool v_eq_m = equal(begin(), end(), ms.begin(), ms.end());
    if (v_eq_m) {
        return 0;
    }

    // true if there is an i s.t. KEY(ary_[i]) < KEY(ms.ary_[i]),
    // otherwise true if size() < ms.size() and KEY(ary_[i]) == key(ms.ary_[i]) for all i
    bool v_lt_m = lexicographical_compare(begin(), end(), ms.begin(), ms.end());
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
template<typename K, typename V>
MapSet<K, V> MapSet<K, V>::mapset_union(MapSet<K, V> &ms) {
    MapSet<K, V> _union = *this;
    for (size_t i = 0; i < ms.last_; ++i) {
        _union.add(ms.ary_[i]);
    }
    return _union;
}

/*
Computes the intersection of the calling mapset and an input mapset
Inputs:
    m (MapSet&): the mapset to intersect with the calling mapset
Outputs:
    intersection (MapSet): the intersect of the calling mapset and input mapset
*/
template<typename K, typename V>
MapSet<K, V> MapSet<K, V>::mapset_intersection(MapSet<K, V> &ms) {
    MapSet<K, V> intersection;

    // For each element in ms
    for (size_t i = 0; i < ms.last_; ++i) {
        const K& key = KEY(ms.ary_[i]);

        // if there is a node in *this with key
        auto *iter = find_key(key);
        if (key_at_iter(iter, key)) {
            // then add the node to the intersection
            intersection.add(*iter);
        }
    }

    return intersection;
}

#endif

