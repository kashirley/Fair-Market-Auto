#pragma once
#include <map>
#include <vector>
#include <string>
#include "car.h"
//TESTER TREEMAP.H FILE -  PLEASE DEBUG 
template<typename K, typename V>
class TreeMap {
private:
    map<K, vector<V>> tree;
    
public:
    void insert(const K& key, const V& value) {
        tree[key].push_back(value);
    }
    
    vector<V> search(const K& key) const {
        auto it = tree.find(key);
        if (it != tree.end()) {
            return it->second;
        }
        return vector<V>();
    }
};