/*
Bradley Bauer
Section 14
04-12-18
Project 11

This code implements a mapset by using a linked list and dynamic memory instead of STL vectors.
*/

#ifndef MAP_SET
#define MAP_SET

#include<iostream>
using std::ostream;
#include<string>
using std::string;
#include<utility>
using std::pair;
using std::swap;
#include<initializer_list>
using std::initializer_list;
#include<sstream>
using std::ostringstream;

//
// Node
//
template<typename K, typename V>
struct Node {
    K first;
    V second;
    Node *next = nullptr;

    Node() = default;
    Node(K k, V v) : first(k), second(v) {};

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
    Node<K,V>* head_ = nullptr;
    size_t sz_ = 0;
    pair<Node<K,V>*, Node<K,V>*> find_key(K);

 public:
    MapSet() = default;
    MapSet(initializer_list< Node<K, V> >);
    MapSet(const MapSet&);
    MapSet& operator=(MapSet);
    ~MapSet();

    size_t size() const;
    bool remove(K);
    bool add(Node<K, V>);
    Node<K, V> get(K);
    bool update(K, V);
    int compare(MapSet&);
    MapSet mapset_union(MapSet&);
    MapSet mapset_intersection(MapSet&);

    // Print the mapset to an out stream
    friend ostream& operator<<(ostream &out, const MapSet &ms) {
        // output all but the last node
        size_t n = 1;
        auto *p = ms.head_;
        while (n < ms.size()) {
            out << *p << ", ";
            p = p->next;
            ++n;
        }

        // output last node without trailing comma
        if (p != nullptr) {
            out << *p;
        }

        return out;
    }

    // Key-Value convenience funcs
    static K KEY(const Node<K, V>& p) { return p.first; }
    static K& KEY(Node<K, V>& p) { return p.first; }
    static V& VAL(Node<K, V>& p) { return p.second; }

    bool key_at_node(const Node<K, V>* iter, const K &key) {
        return (iter != nullptr) && (KEY(*iter) == key);
    }
};

/*
Initializes the mapset with nodes from an initializer list
Inputs:
    il (initializer_list<pr>): an list of nodes to put into the mapset
*/
template<typename K, typename V>
MapSet<K, V>::MapSet(initializer_list< Node<K, V> > il) {
    for (auto &el : il) {
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
    for (auto *p = ms.head_; p != nullptr; p=p->next) {
        add(*p);
    }
}

/*
Assign the input mapset to *this by copy and swap
Inputs:
    ms (MapSet): a copy of some mapset
*/
template<typename K, typename V>
MapSet<K, V>& MapSet<K, V>::operator=(MapSet ms) {
    swap(ms.sz_, sz_);
    swap(ms.head_, head_);
    return *this;
    // ms destructor deletes memory once owned by *this
}

/*
Delete each node in the linked list
*/
template<typename K, typename V>
MapSet<K, V>::~MapSet() {
    // iteratively delete head ptr
    for (auto *p = head_; p != nullptr; p = head_) {
        head_ = p->next;
        delete p;
    }
}

/*
Gives the number of nodes in the mapset
*/
template<typename K, typename V>
size_t MapSet<K, V>::size() const {
    return sz_;
}

/*
Find the position of the first node whose key is greater than or equal to (not less than) the input key
Inputs:
    key (K): the key to find a position for
Outputs:
    prev_next (pair<Node<K,V>*,Node<K,V>*>): a node with the desired key and the previous node
*/
template<typename K, typename V>
pair<Node<K,V>*, Node<K,V>*> MapSet<K, V>::find_key(K key) {
    // A pointer to node p and the node that precedes p in the linked list
    pair<Node<K,V>*, Node<K,V>*> prev_next {nullptr, head_};

    for (auto *p = head_; p != nullptr; p = p->next) {
        if (KEY(*p) >= key) {
            break;
        }
        prev_next.first = p;
        prev_next.second = p->next;
    }

    // if mapset is empty then returns {nullptr, nullptr}
    // if sought node is at end then returns {ptr, nullptr}
    // if sought node is at begin then returns {nullptr, head_}
    return prev_next;
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
    auto prev_next = find_key(KEY(n));

    // if key is in the mapset, then do not add a node
    auto *frwd = prev_next.second;
    auto *back = prev_next.first;
    if (key_at_node(frwd, KEY(n))) {
        return false;
    }

    // put the given node on the heap
    auto *p = new Node<K,V>(n);

    // insert between back and frwd
    p->next = frwd;

    // link preceding node to p if it exists
    if (back) {
        back->next = p;
    }
    else if (frwd == head_) { // if at beginning (or if mapset is empty)
        head_ = p;
    }

    ++sz_;

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
    auto prev_next = find_key(key);

    // if key is not in the mapset, then do not remove a node
    auto *frwd = prev_next.second;
    auto *back = prev_next.first;
    if (!key_at_node(frwd, key)) {
        return false;
    }
    // frwd != nullptr
    // and KEY(*frwd) == key


    // unlink preceding node from frwd if it exists
    if (back) {
        back->next = frwd->next;
    }
    else if (frwd == head_) { // if at beginning (or if mapset is empty)
        head_ = frwd->next;
    }

    delete frwd;

    --sz_;

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
    auto prev_next = find_key(key);

    // return an empty node if key is not found
    auto *frwd = prev_next.second;
    if (!key_at_node(frwd, key)) {
        return Node<K,V>();
    }

    return *frwd;
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
    auto prev_next = find_key(key);

    // if key is not in the mapset, then do not update a node
    auto *frwd = prev_next.second;
    if (!key_at_node(frwd, key)) {
        return false;
    }

    KEY(*frwd) = key;
    VAL(*frwd) = value;

    return true;
}

/*
Compares the calling mapset's keys with the input mapset's keys
Similar to comparing lists of strings in python
Inputs:
    m (MapSet&): the mapset to compare against
Outputs:
    (int): -1 if this.keys() < ms.keys() (or if each key compares equal but this has less number of keys)
            0 if this.keys() = ms.keys() (each have same number of keys and they all compare equal)
            1 if this.keys() > ms.keys() (or if each key compares equal but ms has less number of keys)
*/
template<typename K, typename V>
int MapSet<K, V>::compare(MapSet &ms) {
    // Find the first difference between keys
    auto *p = head_;
    auto *msp = ms.head_;
    while (p != nullptr && msp != nullptr) {
        // if ms key is 'smaller'
        if (KEY(*p) > KEY(*msp)) {
            return 1;
        }
        // if this key is 'smaller'
        if (KEY(*p) < KEY(*msp)) {
            return -1;
        }
        p = p->next;
        msp = msp->next;
    }
    
    // this is shorter
    if (size() < ms.size()) {
        return -1;
    }

    // ms is shorter
    if (size() > ms.size()) {
        return 1;
    }

    // equal keys and equal sizes
    return 0;
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
    for (auto *p = ms.head_; p != nullptr; p = p->next) {
        _union.add(*p);
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
    for (auto *p = ms.head_; p != nullptr; p = p->next) {
        const K& key = KEY(*p);

        // if there is a node in *this with key
        auto prev_next = find_key(key);
        auto *frwd = prev_next.second;
        if (key_at_node(frwd, key)) {
            // then add the node to the intersection
            intersection.add(*frwd);
        }
    }

    return intersection;
}

#endif
