#ifndef MULTIMMap_H
#define MULTIMMap_H

#include <iostream>
#include <vector>
#include <iomanip>
#include <string>
#include <cmath>
#include <cassert>

#include "bplustree.h"
#include "btree_array_funcs.h"

using namespace std;



template <typename K, typename V>
struct MPair{
    K key;
    vector<V> value_list;

    //--------------------------------------------------------------------------------
    /*      MPair CTORs:
     *  must have these CTORs:
     *  - default CTOR / CTOR with a key and no value: this will create an empty vector
     *  - CTOR with a key AND a value: pushes the value into the value_list
     *  _ CTOR with a key and a vector of values (to replace value_list)
     */
    //--------------------------------------------------------------------------------
    MPair(const K& k=K()){
        key = k;
        value_list.clear();
    }
    MPair(const K& k, const V& v)
        : key(k)
    {
        // k = K();
        // v = V();
        value_list.clear();
        value_list.push_back(v);
    }
    MPair(const K& k, const vector<V>& vlist){
        key = k;
        value_list = vlist;
    }
    //--------------------------------------------------------------------------------

    //You'll need to overlod << for your vector:
    friend ostream& operator <<(ostream& outs, const MPair<K, V>& print_me){
        string key = print_me.key;
        // cout<<"|~"<<key<<"~|  :"<<(key == string("20"))<<"size: "<<key.size();
        // for (int i=0; i<key.size(); i++){
        //     cout<<"["<<key[i]<<"]";
        // }
        cout<<endl;
        outs <<"|"<<print_me.key << ":" << print_me.value_list<<"|";
        return outs;   
    }
    friend bool operator ==(const MPair<K, V>& lhs, const MPair<K, V>& rhs){
        return lhs.key == rhs.key;
    }
    friend bool operator < (const MPair<K, V>& lhs, const MPair<K, V>& rhs){
        return lhs.key < rhs.key;
    }
    friend bool operator > (const MPair<K, V>& lhs, const MPair<K, V>& rhs){
        return lhs.key > rhs.key;
    }
    friend bool operator <= (const MPair<K, V>& lhs, const MPair<K, V>& rhs){
        return lhs.key <= rhs.key;
    }
    friend bool operator >= (const MPair<K, V>& lhs, const MPair<K, V>& rhs){
        return lhs.key >= rhs.key;
    }
    friend MPair<K, V> operator + (const MPair<K, V>& lhs, const MPair<K, V>& rhs){
        MPair<K,V> a = MPair<K, V>(lhs.key, lhs.value_list.pushback(rhs.value_list));
        return a;
    }
};


template <typename K, typename V>
class MMap
{
public:
    typedef BPlusTree<MPair<K, V> > MMap_base;
    class Iterator{
    public:
        friend class MMap;
        Iterator(){

        }
        Iterator(typename MMap_base::Iterator it){
            _it = it;
        }
        Iterator operator ++(int unused){
            return _it++;
        }
        Iterator operator ++(){
            return ++_it;
        }
        Iterator operator --(){
            return --_it;
        }
         Iterator operator --(int unused){
            return _it--;
        }
        MPair<K, V> operator *(){
            return *_it;
        }
        friend bool operator ==(const Iterator& lhs, const Iterator& rhs){
            if(lhs._it == rhs._it){
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
        typename MMap_base::Iterator _it;
    };

    MMap(){
        key_count = 0;
    }
//  Iterators
    Iterator begin(){
        return mmap.begin();
    }
    Iterator end(){
        return mmap.end();
    }

//  Capacity
    int size() const{
        return mmap.size();
    }
    bool empty() const{
        return mmap.empty();
    }

//  Element Access
    const vector<V>& operator[](const K& key) const{
        // if(!conatins(key)){
        //     MPair<K, V> temp = MPair<K, V>(key);
        //     mmap.insert(temp);
        //     key_count++;
        //     // vector<V> tempV;
        //     return;
        // }
        return mmap.get(MPair<K,V>(key)).value_list;
        // return *this->get(key);
    }
    vector<V>& at(const K& key){
        return mmap.get(MPair<K,V>(key)).value_list;
    }
    vector<V>& operator[](const K& key){
        // if(!contains(key)){
        //     MPair<K, V> temp = MPair<K, V>(key);
        //     mmap.insert(temp);
        //     key_count++;

        // }else{
            return mmap.get(MPair<K,V>(key)).value_list;
        // }
    }


    


//  Modifiers
    void insert(const K& k, const V& v){
        if(mmap.contains(k)){
            get(k).push_back(v) ;
        }
        else{
            MPair<K, V> temp = MPair<K, V>(k, v);
            mmap.insert(temp);
            key_count++;
        }

    }
    void insert(MPair<K, V> temp){
        mmap.insert(temp);
        key_count++;

    }
    void erase(const K& key){
        mmap.remove(MPair<K,V>(key));
        key_count--;
    }
    void clear(){
        mmap.clear_tree();
        key_count = 0;
    }
    vector<V> &get(const K& key){
        return mmap.get(MPair<K,V>(key)).value_list;
    }

//  Operations:
    Iterator find(const K& key){
        return Iterator(mmap.find(MPair<K,V>(key)));
    }
    Iterator findWhenNotExist(const K& key){
        return Iterator(mmap.findWhenNotExist(MPair<K,V>(key)));
    }
    bool contains(const MPair<K, V>& target) const{
        return mmap.contains(target);
    }
    bool contains(const K& target) const{

        MPair<K, V> target1(target);
        return mmap.contains(target1);
    }

    // I have not writtent hese yet, but we will need them:
    //    int count(const K& key);
    //    lower_bound
    //    upper_bound
    //    equal_range:
    
    void print_lookup(){        
        if(mmap.get(MPair<string,string>("command")).value_list[0] == "select"){
            std::cout << "command :\t" << mmap.get(MPair<string,string>("command")).value_list << endl;
            if(contains("condition")){
                std::cout << "condition :\t" << mmap.get(MPair<string,string>("condition")).value_list << endl;
            }
            std::cout << "fields :\t" << mmap.get(MPair<string,string>("fields")).value_list << endl;
            std::cout << "table_name :\t" << mmap.get(MPair<string,string>("table")).value_list << endl;
            if(contains("where")){
                std::cout << "where :\t" << "yes" << endl;
            }

        }else if(mmap.get(MPair<string,string>("command")).value_list[0] == "insert"){
            std::cout << "command :\t" << mmap.get(MPair<string,string>("command")).value_list << endl;
            std::cout << "table_name :\t" << mmap.get(MPair<string,string>("table")).value_list << endl;
            std::cout << "values :\t" << mmap.get(MPair<string,string>("condition")).value_list << endl;

        }else if(mmap.get(MPair<string,string>("command")).value_list[0] == "make"){
            std::cout << "col :\t" << mmap.get(MPair<string,string>("fields")).value_list << endl;
            std::cout << "command :\t" << mmap.get(MPair<string,string>("command")).value_list << endl;
            std::cout << "table_name:\t" << mmap.get(MPair<string,string>("table")).value_list << endl;
        }
    }


    //NEED TO BE FIXED
    Iterator lower_bound(const K& key){

        // Iterator foundedKey = find(key);
        // if(foundedKey == Iterator(NULL)){
        //     return findWhenNotExist(key);
        // }
        // return foundedKey;

        for(Iterator it = begin(); it != end(); it++)
        {
            MPair<K, V> MPair = *it;
            if(key <= MPair.key) 
            {
                return it;
            }
        }
        return end();
    }  //return first that goes NOT BEFORE
                                         // key entry or next if does not
                                         // exist: >= entry
    //NEED TO BE FIXED
    Iterator upper_bound(const K& key){

        // Iterator foundedKey = find(key);
        // if(foundedKey == Iterator(NULL)){
        //     return findWhenNotExist(key);
        // }
        // foundedKey++;
        // return foundedKey;

        for(Iterator it = begin(); it != end(); it++)
        {
            MPair<K, V> MPair = *it;
            if(key < MPair.key)  //go for larger one after 
            {
                return it;
            }
        }
        return end();
    }  //return first that goes AFTER key
                                         //exist or not, the next entry  >entry

    int count(const K& key) const{
        return 1;
    }  

    bool is_valid(){return mmap.is_valid();}

    MMap& operator =(const MMap& RHS){
        key_count = RHS.key_count;
        mmap = RHS.mmap;
        return *this;
    }   

    friend ostream& operator<<(ostream& outs, const MMap<K, V>& print_me){
        outs<<print_me.mmap<<endl;
        return outs;
    }
private:
    int key_count;
    BPlusTree<MPair<K, V> > mmap;
};


#endif