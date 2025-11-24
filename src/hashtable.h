#pragma once
#include <unordered_map>
#include <vector>
#include <string>
#include "car.h"

using namespace std;

// Simple hash table wrapper 
// Stores a vector<V> for each key so multiple cars can share the same key
template<typename K, typename V>
class HashTable {
private:
    unordered_map<K, vector<V>> table;

public:
    void insert(const K& key, const V& value) {
        table[key].push_back(value);
    }

    vector<V> search(const K& key) const {
        auto it = table.find(key);
        if (it != table.end()) 
            return it->second;
        return vector<V>();
    }

    size_t size() const {
        return table.size();
    }
};
