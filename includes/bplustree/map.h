#ifndef MAP_H
#define MAP_H

#include <iostream>
#include "bplustree.h"
// #include "btree_array_funcs.h"


// using namespace std;


template <typename K, typename V>
struct Pair{
    K key;
    V value;

    Pair(const K& k=K(), const V& v=V())
    : key(k), value(v){

    }
    friend ostream& operator <<(ostream& outs, const Pair<K, V>& print_me){
        outs << print_me.key << ":" << print_me.value;
        return outs;   
    }
    friend bool operator ==(const Pair<K, V>& lhs, const Pair<K, V>& rhs){
        return lhs.key == rhs.key;
    }
    friend bool operator < (const Pair<K, V>& lhs, const Pair<K, V>& rhs){
        return lhs.key < rhs.key;
    }
    friend bool operator > (const Pair<K, V>& lhs, const Pair<K, V>& rhs){
        return lhs.key > rhs.key;
    }
    friend bool operator <= (const Pair<K, V>& lhs, const Pair<K, V>& rhs){
        return lhs.key <= rhs.key;
    }
    friend bool operator >= (const Pair<K, V>& lhs, const Pair<K, V>& rhs){
        return lhs.key >= rhs.key;
    }
    friend Pair<K, V> operator + (const Pair<K, V>& lhs, const Pair<K, V>& rhs){
        Pair<K,V> a = Pair<K, V>(lhs.key, lhs.value + rhs.value);
        return a;
    }
};


template <typename K, typename V>
class Map
{
public:
    typedef BPlusTree<Pair<K, V> > map_base;
    class Iterator{
    public:
        friend class Map;
        Iterator(){

        }
        Iterator(typename map_base::Iterator it){
            _it = it;
        }
        Iterator operator ++(int unused){
            return _it++;
        }
        Iterator operator ++(){
            return ++_it;
        }
        Pair<K, V> operator *(){
            return *_it;
        }
        friend bool operator ==(const Iterator& lhs, const Iterator& rhs){
            if(lhs.it == rhs.it){

                return true;
            }
            return false;
        }
        friend bool operator !=(const Iterator& lhs, const Iterator& rhs){
            if(lhs._it == rhs._it){

                return false;
            }
            return true;
        }
    private:
        typename map_base::Iterator _it;
    };

    Map(){
        key_count = 0;
    }
//  Iterators
    Iterator begin(){
        return map.begin();
    }
    Iterator end(){
        return map.end();
    }

//  Capacity
    int size() const{
        return map.size();
    }
    bool empty() const{
        return map.empty();
    }

//  Element Access
    V& operator[](const K& key){
        return map.get(Pair<K,V>(key)).value;
    }
    V& at(const K& key){
        return map.get(Pair<K,V>(key)).value;
    }
    const V& at(const K& key) const{
        return map.get(Pair<K,V>(key)).value;
    }


//  Modifiers
    void insert(const K& k, const V& v){
        if(map.contains(k)){
            map.get(Pair<K,V>(k)).value = v;
        }
        else{
            Pair<K, V> temp = Pair<K, V>(k, v);
            map.insert(temp);
            key_count++;
        }
    }
    void erase(const K& key){
        map.remove(Pair<K,V>(key));
        key_count--;
    }
    void clear(){
        map.clear_tree();
        key_count = 0;
    }
    V get(const K& key){
        return map.get(Pair<K,V>(key));
    }

//  Operations:
    Iterator find(const K& key){
        return map.find(Pair<K,V>(key));
    }
    bool contains(const Pair<K, V>& target) const{
        return map.contains(Pair<K,V>(target));
    }

    // I have not writtent hese yet, but we will need them:
    //    int count(const K& key);
    //    lower_bound
    //    upper_bound
    //    equal_range:
    

    Iterator lower_bound(const K& key){
        for(Iterator it = begin(); it != end(); it++)
        {
            Pair<K, V> pair = *it;
            if(key <= pair.key) 
            {
                return it;
            }
        }
        return Iterator(nullptr);
    }  //return first that goes NOT BEFORE
                                         // key entry or next if does not
                                         // exist: >= entry
    Iterator upper_bound(const K& key){
        for(Iterator it = begin(); it != end(); it++)
        {
            Pair<K, V> pair = *it;
            if(key < pair.key)  //go for larger one after 
            {
                return it;
            }
        }
        return Iterator(nullptr);
    }  //return first that goes AFTER key
                                         //exist or not, the next entry  >entry

    int count(const K& key) const{
        return 1;
    }  

    Map& operator =(const Map& RHS){
        key_count = RHS.key_count;
        map = RHS.map;
        return *this;
    }   

    bool is_valid(){return map.is_valid();}

    friend ostream& operator<<(ostream& outs, const Map<K, V>& print_me){
        outs<<print_me.map<<endl;
        return outs;
    }
private:
    int key_count;
    BPlusTree<Pair<K, V> > map;
};



#endif