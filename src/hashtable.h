#pragma once
#include <vector>
#include <list>
#include <string>
#include <cmath>
#include <functional> //added in after issue with hash function
#include "car.h"
//using namespace std;
//TESTER HASHTABLE.H FILE -  PLEASE DEBUG

template<typename K, typename V>
class HashTable {
private:
    std::vector<std::list<std::pair<K, std::vector<V>>>> table;
    int table_size;
    int num_elements;
    const double MAX_LOAD_FACTOR = 0.75;

    int hash(const K& key) const {
        typedef std::hash<K> HashFunc;
        HashFunc hasher;
        size_t hash_value = hasher(key);
        return static_cast<int>(hash_value % table_size);
    }

    bool isPrime(int n) const {
        if (n <= 1) return false;
        if (n <= 3) return true;
        if (n % 2 == 0 || n % 3 == 0) return false;
        for (int i = 5; i * i <= n; i += 6) {
            if (n % i == 0 || n % (i + 2) == 0) return false;
        }
        return true;
    }

    int nextPrime(int n) const {
        while (!isPrime(n)) n++;
        return n;
    }

    void rehash() {
        int old_size = table_size;
        table_size = nextPrime(table_size * 2);

        std::vector<std::list<std::pair<K, std::vector<V>>>> old_table = table;
        table.clear();
        table.resize(table_size);
        num_elements = 0;

        // Reinsert all elements
        for (const auto& bucket : old_table) {
            for (const auto& entry : bucket) {  // Changed 'pair' to 'entry'
                for (const auto& value : entry.second) {
                    insert(entry.first, value);
                }
            }
        }
    }

public:
    HashTable(int initial_size = 100001) //double check if this is okay - updated initial size
        : table_size(nextPrime(initial_size)), num_elements(0) {
        table.resize(table_size);
    }

    void insert(const K& key, const V& value) {
        // Check load factor ad rehash if needed
        if ((double)num_elements / table_size > MAX_LOAD_FACTOR) {
            rehash();
        }

        int index = hash(key);

        // if key exists
        for (auto& entry : table[index]) {  // Changed 'pair' to 'entry'
            if (entry.first == key) {
                entry.second.push_back(value);
                return;
            }
        }

		
        table[index].push_back({ key, {value} });
        num_elements++;
    }

    std::vector<V> search(const K& key) const {
        int index = hash(key);

        for (const auto& entry : table[index]) {  // Changed 'pair' to 'entry'
            if (entry.first == key) {
                return entry.second;
            }
        }

        return std::vector<V>();
    }

    double getLoadFactor() const {
        return (double)num_elements / table_size;
    }

    int getTableSize() const {
        return table_size;
    }

    int getNumElements() const {
        return num_elements;
    }
};