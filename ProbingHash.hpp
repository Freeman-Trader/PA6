#pragma once

#ifndef __PROBING_HASH_H
#define __PROBING_HASH_H

#include <vector>
#include <stdexcept>

#include "Hash.hpp"

using std::vector;
using std::pair;

// Can be used for tracking lazy deletion for each element in your table
enum EntryState {
    EMPTY = 0,
    VALID = 1,
    DELETED = 2
};

template<typename K, typename V>
class ProbingHash : public Hash<K,V> { // derived from Hash
private:
    // Needs a table and a size.
    // Table should be a vector of std::pairs for lazy deletion
    std::vector<std::pair<EntryState, V>> Table;
    int numElements;

public:
    ProbingHash(int n = 11) {
        //this->tableSize = n;
        this->Table.resize(n);
        this->numElements = 0;
    }

    ~ProbingHash() {
        // Needs to actually free all table contents
        //this->tableSize = 0;
        //this->Table.resize(0);
        this->clear();
    }

    bool empty() {
        for(long unsigned int I = 0; I < this->Table.size(); I++) {
            if(this->Table[I].first == VALID)
                return false;
        }
        return true;
    }

    int size() {
        return this->numElements;
    }

    V& at(const K& key) {
        int Index = this->hash(key);
        for(long unsigned int I = 0; I < this->Table.size(); I++) {
            if(this->Table[(Index + I) % this->Table.size()].first == VALID && this->Table[(Index + I) % this->Table.size()].second == key)
                return this->Table[(Index + I) % this->Table.size()].second;
        }
        //throw std::out_of_range("Key not in hash");
    }

    V& operator[](const K& key) {
        int Index = this->hash(key);
        for(long unsigned int I = 0; I < this->Table.size(); I++) {
            if(this->Table[(Index + I) % this->Table.size()].first == VALID && this->Table[(Index + I) % this->Table.size()].second == key)
                return this->Table[(Index + I) % this->Table.size()].second;
        }
        //throw std::out_of_range("Key not in hash");
    }

    int count(const K& key) {
        int Size = 0;
        int Index = this->hash(key);
        for(long unsigned int I = 0; I < this->Table.size(); I++) {
            if(this->Table[(Index + I) % this->Table.size()].first == key)
                Size += 1;
        }
        return Size;
    }

    void emplace(K key, V value) {
        int Index = this->hash(key);
        for(long unsigned int I = 0; I < this->Table.size(); I++) {
            if(this->Table[(Index + I) % this->Table.size()].first == EMPTY || this->Table[(Index + I) % this->Table.size()].first == DELETED) {
                this->Table[(Index + I) % this->Table.size()].first = VALID;
                this->Table[(Index + I) % this->Table.size()].second = value;
                this->numElements += 1;
                if(this->load_factor() > .75)
                    this->rehash();
                return;
            }
        }
    }

    void insert(const std::pair<K, V>& pair) {
        int Index = this->hash(pair.first);
        for(long unsigned int I = 0; I < this->Table.size(); I++) {
            if(this->Table[(Index + I) % this->Table.size()].first == EMPTY || this->Table[(Index + I) % this->Table.size()].first == DELETED) {
                this->Table[(Index + I) % this->Table.size()].first = VALID;
                this->Table[(Index + I) % this->Table.size()].second = pair.second;
                this->numElements += 1;
                if(this->load_factor() > .75)
                    this->rehash();
                return;
            }
        }
    }

    void erase(const K& key) {
        int Index = this->hash(key);
        for(long unsigned int I = 0; I < this->Table.size(); I++) {
            if(this->Table[(Index + I) % this->Table.size()].first == VALID && this->Table[(Index + I) % this->Table.size()].second == key) {
                this->Table[(Index + I) % this->Table.size()].first = DELETED;
                this->numElements -= 1;
            }    
        }
    }

    void clear() {
        this->Table.clear();
    }

    int bucket_count() {
        return this->Table.size();
    }

    int bucket_size(int n) {
        if(this->Table[n].first == 1)
            return 1;
        return 0;
    }

    int bucket(const K& key) {
        int Index = this->hash(key);
        for(long unsigned int I = 0; I < this->Table.size(); I++) {
            if(this->Table[(Index + I) % this->Table.size()].first == key) {
                return (Index + I) % this->Table.size();
            }    
        }
        throw std::out_of_range("Key not in hash");
    }

    float load_factor() {
        return (float)this->numElements / (float)this->Table.size();
    }

    void rehash() {
        int nSize = this->findNextPrime(2 * this->Table.size());
        std::vector<std::pair<EntryState, V>> nTable;
        nTable.resize(nSize);
        for(long unsigned int I = 0; I < this->Table.size(); I++) {
            if(this->Table[I].first == VALID) {
                int Index = this->hash(this->Table[I].second);
                long unsigned int L = 0;
                for(L = 0; nTable[(Index + L) % nSize].first != EMPTY && nTable[(Index + L) % nSize].first != DELETED; L++) {}
                nTable[(Index + L) % nSize].first = VALID;
                nTable[(Index + L) % nSize].second = this->Table[I].second;
            }

        }  
        this->Table = nTable;
    }
        
    void rehash(int n) {
        int nSize = this->findNextPrime(n);
        std::vector<std::pair<EntryState, V>> nTable;
        nTable.resize(nSize);
        for(long unsigned int I = 0; I < this->Table.size(); I++) {
            if(this->Table[I].first == VALID) {
                int Index = this->hash(this->Table[I].second);
                long unsigned int L = 0;
                for(L = 0; nTable[(Index + L) % nSize].first != EMPTY && nTable[(Index + L) % nSize].first != DELETED; L++) {}
                nTable[(Index + L) % nSize].first = VALID;
                nTable[(Index + L) % nSize].second = this->Table[I].second;
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
        for (int i = 2; i <= sqrt(n); i++)
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

#endif //__PROBING_HASH_H
