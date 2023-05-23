#pragma once

#ifndef __CHAINING_HASH_H
#define __CHAINING_HASH_H

// Standard library includes
#include <iostream>
#include <vector>
#include <list>
#include <stdexcept>
#include <math.h>

// Custom project includes
#include "Hash.hpp"

//
// Separate chaining based hash table - derived from Hash
//
template<typename K, typename V>
class ChainingHash : public Hash<K,V> {
private:
    std::vector<std::list<V>> Table;
    int numElements;

public:
    ChainingHash(int n = 11) {
        this->Table.resize(n);
        this->numElements = 0;
    }

    ~ChainingHash() {
        this->clear();
    }

    bool empty() {
        for(long unsigned int I = 0; I < this->Table.size(); I++) {
            if(!this->Table[I].empty())
                return false;
        }
        return true;
    }

    int size() {
        return this->numElements;
    }

    V& at(const K& key) {
        int Index = this->hash(key) % this->Table.size();
        for(auto it = this->Table[Index].begin(); it != this->Table[Index].end(); ++it) {
            if(*it == key)
                return *it;
        }
        //throw std::out_of_range("Key not in hash");
    }

    V& operator[](const K& key) {
        int Index = this->hash(key) % this->Table.size();
        for(auto it = this->Table[Index].begin(); it != this->Table[Index].end(); ++it) {
            if(*it == key)
                return *it;
        }
        //throw std::out_of_range("Key not in hash");
    }

    int count(const K& key) {
        int Size = 0;
        int Index = this->hash(key) % this->Table.size();
        for(auto it = this->Table[Index].begin(); it != this->Table[Index].end(); ++it) {
            ++Size;
        }
        return Size;
    }

    void emplace(K key, V value) {
        this->Table[this->hash(key) % this->Table.size()].push_back(value);
        this->numElements += 1;
        if(this->load_factor() > .75)
            this->rehash();
    }

    void insert(const std::pair<K, V>& pair) {
        this->Table[this->hash(pair.first) % this->Table.size()].push_back(pair.second);
        this->numElements += 1;
        if(this->load_factor() > .75)
            this->rehash();
    }

    void erase(const K& key) {
        int Index = this->hash(key) % this->Table.size();
        for(auto it = this->Table[Index].begin(); it != this->Table[Index].end(); ++it) {
            if(*it == key) {
                this->Table[Index].erase(it);
                this->numElements -= 1;
                return;
            }
        }
    }

    void clear() {
        for(long unsigned int I = 0; I < this->Table.size(); I++) {
            this->Table[I].clear();
        }
        this->Table.clear(); //////////////////////// 
    }

    int bucket_count() {
        return this->Table.size();
    }

    int bucket_size(int n) {
        return this->Table[n].size();
    }

    int bucket(const K& key) {
        int Index = this->hash(key) % this->Table.size();
        for(auto it = this->Table[Index].begin(); it != this->Table[Index].end(); ++it) {
            if(*it == key)
                return Index;
        }
        throw std::out_of_range("Key not in hash");
    }

    float load_factor() {
        return (float)this->numElements / (float)this->Table.size();
    }

    void rehash() {
        int nSize = this->findNextPrime(2 * this->Table.size());
        std::vector<std::list<V>> nTable;
        nTable.resize(nSize);
        
        for(long unsigned int I = 0; I < this->Table.size(); I++) {
            for(auto it = this->Table[I].begin(); it != this->Table[I].end(); ++it) {
                nTable[this->hash(*it) % nSize].push_back(*it);
            }
        }
        this->Table = nTable;
    }

    void rehash(int n) {
        int nSize = this->findNextPrime(n);
        std::vector<std::list<V>> nTable;
        nTable.resize(n);
        
        for(long unsigned int I = 0; I < this->Table.size(); I++) {
            for(auto it = this->Table[I].begin(); it != this->Table[I].end(); ++it) {
                nTable[this->hash(*it) % nSize].push_back(*it);
            }
        }
        this->Table = nTable;
    }


private:
    int findNextPrime(int n)
    {
        while (!isPrime(n))
        {
            n++;
        }
        return n;
    }

    int isPrime(int n)
    {
        for (int i = 2; i <= std::sqrt(n); i++)
        {
            if (n % i == 0)
            {
                return false;
            }
        }

        return true;
    }

    int hash(const K& key) {
        return (int)key;       
    }

};

#endif //__CHAINING_HASH_H
