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
using std::copy;
#include<sstream>
using std::ostringstream;

//
// Node
//
template<typename K, typename V>
struct Node {
    K first;
    V second;
    Node() = default;
    Node(K,V);
    bool operator<(const Node&) const;
    bool operator==(const Node&) const;
    friend ostream& operator<<(ostream &out, const Node &n){
        return out << n.first << ":" << n.second;
    }
};

template<typename K, typename V>
Node<K,V>::Node(K key, V value) : first(key), second(value) {}

template<typename K, typename V>
bool Node<K,V>::operator<(const Node &n) const{
    return first < n.first;
}

template<typename K, typename V>
bool Node<K,V>::operator==(const Node &n) const{
    return first == n.first;
}

//
// MapSet
// 
template<typename K, typename V>
class MapSet{
private:
    Node<K,V>* ary_;
    size_t last_;
    size_t capacity_;
    Node<K,V>* find_key(K);
    void grow();
public:
    MapSet(int sz = 2);
    MapSet(initializer_list< Node<K,V> >);
    MapSet(const MapSet&);
    MapSet operator=(MapSet);
    ~MapSet();
    size_t size();
    bool remove(K);  
    bool add(Node<K,V>);
    Node<K,V> get(K);
    bool update(K,V);  
    int compare(MapSet&);
    MapSet mapset_union(MapSet&);
    MapSet mapset_intersection(MapSet&);

    friend ostream& operator<<(ostream &out, const MapSet &ms){
        if (!ms.last_) {
            return out;
        }
        // output all but the last node
        for_each(ms.begin(), ms.end() - 1, [&out](const Node<K,V>& n) { out << n << ", "; });

        // output the last node without the trailing comma
        return out << ms.back();
    }  

    static K KEY(const Node<K, V>& p) { return p.first; }
    static K& KEY(Node<K,V>& p) { return p.first; }
    static V VAL(const Node<K, V>& p) { return p.second; }
    static V& VAL(Node<K,V>& p) { return p.second; }

    Node<K,V>* end() const { return ary_ + last_; }
    Node<K,V>* begin() const { return ary_; }
    Node<K,V> back() const { return *(ary_+last_-1); }

    bool key_at_iter(const Node<K,V>* iter, const K &key) {
        return (iter != nullptr) && (iter != end()) && (KEY(*iter) == key);
    }
};

template<typename K, typename V>
MapSet<K,V>::MapSet(int sz){
    if (sz <= 0) {
        sz = 1;
    }
    capacity_ = sz;
    last_ = 0;
    ary_ = new Node<K, V>[capacity_];
}

template<typename K, typename V>
MapSet<K,V>::MapSet(initializer_list< Node<K,V> > il){
    capacity_ = il.size();
    if (capacity_ == 0) {
        capacity_ = 1;
    }
    last_ = 0;
    ary_ = new Node<K, V>[capacity_];
    // Add each element of the initializer_list to the mapset
    for (const Node<K,V>& el : il) {
        add(el);
    }
}

template<typename K, typename V>
MapSet<K,V>::MapSet(const MapSet &ms){
    capacity_ = ms.capacity_;
    last_ = ms.last_;
    ary_ = new Node<K, V>[capacity_];
    copy(ms.begin(), ms.end(), begin());
}

// copy and swap
template<typename K, typename V>
MapSet<K,V> MapSet<K,V>::operator=(MapSet<K,V> ms){
    last_ = ms.last_;
    capacity_ = ms.capacity_;
    if (ary_) {
        delete[] ary_;
    }
    ary_ = new Node<K, V>[capacity_];
    copy(ms.begin(), ms.end(), begin());
    return *this;
}

template<typename K, typename V>
MapSet<K,V>::~MapSet(){
    delete[] ary_;
}

template<typename K, typename V>
size_t MapSet<K,V>::size(){
    return last_;
}

template<typename K, typename V>
void MapSet<K,V>::grow(){
    capacity_ *= 2;
    auto *temp = new Node<K, V>[capacity_];
    copy(begin(), end(), temp);
    delete[] ary_;
    ary_ = temp;
}

template<typename K, typename V>
Node<K,V>* MapSet<K,V>::find_key(K key){
    Node<K, V> node;
    KEY(node) = key;
    return lower_bound(begin(), end(), node);
}

template<typename K, typename V>
bool MapSet<K,V>::add(Node<K,V> n){
    int p = find_key(KEY(n)) - begin();

    // If key is in the mapset, then do not add a node to the mapset
    if (KEY(ary_[p]) == KEY(n)) {
        return false;
    }

    if (capacity_ == last_) {
        grow();
    }

    for (int i = last_; i > p; --i) {
        ary_[i] = ary_[i - 1];
    }
    ary_[p] = n;
    last_++;

    return true;
}

template<typename K, typename V>
bool MapSet<K,V>::remove(K key){
    auto *p = find_key(key);

    // If key is not in the mapset, then do not try to remove a node
    if (!key_at_iter(p, key)) {
        return false;
    }

    copy(p + 1, end(), p);
    last_--;

    return true;
}

template<typename K, typename V>
Node<K,V> MapSet<K,V>::get(K key){
    auto *p = find_key(key);

    // if key is not in the mapset, then return an empty node
    if (!key_at_iter(p, key)) {
        return Node<K, V>();
    }

    return *p;
}

template<typename K, typename V>
bool MapSet<K,V>::update(K key, V value){
    auto *p = find_key(key);

    // if key is not in the mapset, then do not try to update a node
    if (!key_at_iter(p, key)) {
        return false;
    }

    KEY(*p) = key;
    VAL(*p) = value;
    return true;
}

template<typename K, typename V>
int MapSet<K,V>::compare(MapSet &ms){
    // Check for equal keys
    bool v_eq_m = equal(begin(), end(), ms.begin(), ms.end());

    if (v_eq_m) {
        return 0;
    }

    // true if there is an i s.t. KEY(ary_[i]) < KEY(ms.ary_[i]), otherwise true if size() < ms.size()
    bool v_lt_m = lexicographical_compare(begin(), end(), ms.begin(), ms.end());

    if (v_lt_m) {
        return -1;
    }
    return 1;
}

template<typename K, typename V>
MapSet<K,V> MapSet<K,V>::mapset_union(MapSet<K,V> &ms){
    MapSet<K, V> _union = *this;
    for (size_t i = 0; i < ms.last_; ++i) {
        _union.add(ms.ary_[i]);
    }
    return _union;
}

template<typename K, typename V>
MapSet<K,V> MapSet<K,V>::mapset_intersection(MapSet<K,V> &ms){
    MapSet<K, V> intersection;
    for (size_t i = 0; i < ms.last_; ++i) {
        K key = KEY(ms.ary_[i]);
        auto *iter = find_key(key);
        if (key_at_iter(iter, key)) {
            intersection.add(*iter);
        }
    }
    return intersection;
}

#endif
  
