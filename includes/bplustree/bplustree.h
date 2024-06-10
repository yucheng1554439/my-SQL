#ifndef BPLUSTREE_H
#define BPLUSTREE_H

#include <iostream>
#include <iomanip>
#include "btree_array_funcs.h"
using namespace std;

template <class T>
class BPlusTree
{
public:
    class Iterator{
    public:
        friend class BPlusTree;
        Iterator(BPlusTree<T>* _it=NULL, int _key_ptr = 0):
                                                it(_it), key_ptr(_key_ptr){}

        T operator *(){
            return it->data[key_ptr];
        }

        Iterator operator++(int un_used){
            Iterator p;
            p = it;
            if(key_ptr == it->data_count -1)
            {
                key_ptr = 0;
                it = it->next;
            }
            else
                key_ptr++;
            return p;
        }
        Iterator operator++(){
            if(key_ptr == it->data_count -1)
            {
                key_ptr = 0;
                it = it->next;
            }
            else
                key_ptr++;
            return *this;
        }

        friend bool operator ==(const Iterator& lhs, const Iterator& rhs){
            
            if(lhs.it == rhs.it && lhs.key_ptr == rhs.key_ptr){

                return true;
            }
            return false;
        }
        friend bool operator !=(const Iterator& lhs, const Iterator& rhs){
            if(lhs.it == rhs.it && lhs.key_ptr == rhs.key_ptr){

                return false;
            }
            return true;
        }
        void print_Iterator(){
            if (it){
                cout<<"iterator: ";
                print_array(it->data, it->data_count, key_ptr);
            }
            else{
                cout<<"iterator: NULL, key_ptr: "<<key_ptr<<endl;
            }
        }
        bool is_null(){return !it;}
        void info(){
            cout<<endl<<"Iterator info:"<<endl;
            cout<<"key_ptr: "<<key_ptr<<endl;
            cout<<"it: "<<*it<<endl;
        }

    private:
        BPlusTree<T>* it;
        int key_ptr;
    };

    BPlusTree(bool dups = true){  //MADE it true, it was false
        child_count = 0;
        data_count = 0;
        next = nullptr;
        dups_ok = dups;
    }
    BPlusTree(T *a, int size, bool dups = true) //MADE it true, it was false
    : child_count(0), data_count(0), next(nullptr)
    {
        //copy_array(data, a, data_count, size);
        for(int i = 0; i < size; i++)
        {
            insert(a[i]);
        }
    }

    //big three:
    BPlusTree(const BPlusTree<T>& other){
        child_count = 0;
        data_count = 0;
        next = nullptr;
        copy_tree(other);
    }
    ~BPlusTree(){
        // clear_tree();
    }
    BPlusTree<T>& operator =(const BPlusTree<T>& RHS){
        clear_tree();
        copy_array(data, RHS.data, data_count, RHS.data_count);
        copy_array(subset, RHS.subset, child_count, RHS.child_count);

        // copy_tree(RHS);
        return *this;
    }

    void copy_tree(const BPlusTree<T>& other){
        assert(empty() && "Current tree should be empty");
        BPlusTree<T>* last_node=nullptr;
        copy_tree(other, last_node);
        last_node = nullptr;
    }  //copy other into this object

    void copy_tree(const BPlusTree<T>& other, BPlusTree<T>*& last_node){
        if(other.is_leaf()){
            if(last_node == nullptr){
                last_node = this;
            }else{
                last_node->next = this;
                last_node = this;
            }
        }

        copy_array(data, other.data, data_count, other.data_count);
        child_count = other.child_count;   
        for(int i = 0; i < other.child_count; i ++){
            subset[i] = new BPlusTree<T>;
            subset[i]->copy_tree(*other.subset[i], last_node);
        }

    }

    void insert(const T& entry){
        loose_insert(entry);

        if(data_count > MAXIMUM){
            BPlusTree<T>* p = new BPlusTree<T>;
            p->data_count = data_count;
            copy_array(p->data, data, p->data_count, data_count);
            copy_array(p->subset, subset, p->child_count, child_count);

            data_count = 0;
            child_count = 1;
            subset[0] = p;
            fix_excess(0);
        }
    }   //insert entry into the tree
    void remove(const T& entry){
        // loose_remove(entry);
        //     if(data_count == 0){
        //         if(child_count == 1){   //from child_count >= MINIMUM
        //             BPlusTree<T>* p = new BPlusTree<T>;
        //             p = subset[0];
        //             copy_array(subset, subset[0]->subset, child_count, subset[0]->child_count);
        //             copy_array(data, p->data, data_count, p->data_count);
        //             p->child_count = 0;
        //             p->data_count = 0;
        //             delete p;
                   
        //         }
        //     }
        clear_tree();
    }   //remove entry from the tree
    void clear_tree(){
        if(empty()){return;}
        if(!is_leaf()){
            subset[child_count-1]->clear_tree();
            for(int i = 0; i < child_count-1; i++){
                subset[i]->clear_tree();
                data_count = 0;
                delete subset[i];
                child_count = 0;
            }
        }else{
            data_count = 0;
            child_count = 0;
            next = nullptr;
        }
    }             //clear this object (delete all nodes etc.)

    bool contains(const T& entry) const{
        if(data_count<=0){
            return false;
        }
        bool found = false;
        int i = first_ge(data, data_count, entry);
        if(data_count < 0){ return false; }

        if(data[i] == entry)
        { found = true; }

        if(found)
            return true; 
        else{
            if(is_leaf()){
                return false;
            } else { 
                return subset[i]->contains(entry);
            }
        }
    } 
    T& get(const T& entry){
        //If entry is not in the tree, add it to the tree
        // assert(contains(entry));
        // std::cout << "NONCONST";
        const bool debug = false;
        if (!contains(entry))
            insert(entry);
        // std::cout << "CNNNNNMMMMM" <<  get_existing(entry);
        return get_existing(entry);
    }              //return a reference to entry
    const T& get(const T& entry)const{
        // std::cout << "CONST";
        if (!contains(entry))
            insert(entry);
            // std::cout << "CNNNNNMMMMM" <<  get_existing(entry);
        return get_existing(entry);
    }   //return a reference to entry
    T& get_existing(const T& entry){
    /*
    * ---------------------------------------------------------------
    * This routing explains plainly how the BPlusTree is organized.
    * i = first_ge
    *       |      found         |    !found         |
    * ------|--------------------|-------------------|
    * leaf  |  you're done       | it's not there    |
    *       |                    |                   |
    * ------|--------------------|-------------------|
    * !leaf | subset[i+1]->get() |  subset[i]->get() |
    *       | (inner node)       |                   |
    * ------|--------------------|-------------------|
    *       |                    |                   |
    *
    * ---------------------------------------------------------------
    *
    */
        //assert that entry is not in the tree.
        //assert(contains(entry));

        const bool debug = false;
        int i = first_ge(data, data_count, entry);
        bool found = (i<data_count && data[i]==entry);
        if (is_leaf())
            if (found){
                return data[i];
            }
            else{
                if (debug) cout<<"get_existing was called with nonexistent entry"<<endl;
                assert(found);
            }
        if (found) //inner node
            return subset[i+1]->get(entry);
            //or just return true?
        else //not found yet...
            return subset[i]->get(entry);

    }     //return a reference to entry
    Iterator find(const T& key){
        if(data_count<=0){
            return nullptr;
        }
        bool found = false;
        int i = first_ge(data, data_count, key);
        if(data_count < 0){ return nullptr; }

        if(data[i] == key && i < data_count)
        { found = true; }

        if(found)
            return Iterator(this, i); 
        else{
            if(is_leaf()){
                return nullptr; //is thats the leaf, return nullptr
            } else { 
                return subset[i]->find(key); //if not, go down til leaf
            }
        }
    }         //return an iterator to this key.
    Iterator findWhenNotExist(const T& key){
        if(data_count<=0){
            return nullptr;
        }
        bool found = false;
        int i = first_ge(data, data_count, key);
        if(data_count < 0){ return nullptr; }

        if(data[i] == key && i < data_count)
        { found = true; }

        if(found)
            return Iterator(this, i); 
        else{
            if(is_leaf()){
                return Iterator(this); //is thats the leaf, return this
            } else { 
                return subset[i]->findWhenNotExist(key); //if not, go down til leaf
            }
        }
    }         //return an iterator to this key.

    
                                         //     NULL if not there.
    Iterator lower_bound(const T& key){
        Iterator a;
        return a;
    }  //return first that goes NOT BEFORE
                                         // key entry or next if does not
                                         // exist: >= entry
    Iterator upper_bound(const T& key){
        Iterator a;
        return a;
    }  //return first that goes AFTER key
                                         //exist or not, the next entry  >entry

    int size() const{
        return 1;
    }                    //count the number of elements

    bool empty() const{
        return (data_count ==0);
    }                  //true if the tree is empty

    void print_tree(int level = 0, ostream &outs=cout) const{
        std::string down_bracket = "\357\271\207"; // ﹇
        std::string up_bracket   = "\357\271\210"; // ﹈
        if(!is_leaf()){
            subset[child_count - 1]->print_tree(level + 1, outs);
        }
        outs << setw(level * 4) << "" << down_bracket << std::endl;
        if(empty()){
            outs << setw(level * 4) << "" << up_bracket << std::endl;
        }
        for (int i = data_count - 1; i >= 0; i--){
            outs << setw(level * 4) << "" << data[i] << std::endl;
            if(i==0){
                outs << setw(level * 4) << "" << up_bracket << std::endl;
            }
            if(!is_leaf()){
                subset[i]->print_tree(level + 1, outs);
            }
        }
    }
    friend ostream& operator<<(ostream& outs,
                               const BPlusTree<T>& print_me){
        print_me.print_tree(0, outs);
        return outs;
    }

    bool is_valid(){
        return true;
    }
    string in_order(){
        std::string s = "";
        if(!is_leaf()){
            s += subset[0]->in_order();
            s += to_string(data[0]) + "|" ;
            for(int i = 1; i < child_count; i ++){ 
                s += subset[i]->in_order();
                if(i+1 <= data_count){
                    s += to_string(data[i]) + "|" ;
                }
            }
        }else{
            for(int i = 0; i < data_count; i ++){
                s += to_string(data[i]) + "|" ;
            }
        }
        return s;
    }

    string pre_order(){
        std::string s = "";
        if(!is_leaf()){
            s += to_string(data[0]) + "|" ;
            s += subset[0]->pre_order();
            for(int i = 1; i < child_count; i ++){
                if(i+1 <= data_count){
                    s += to_string(data[i]) + "|" ;
                }
                s += subset[i]->pre_order();
            }
        }else{
            for(int i = 0; i < data_count; i ++){
                s += to_string(data[i]) + "|" ;
            }
        }
        return s;
    }

    string post_order(){
        std::string s = "";
        if(!is_leaf()){
            s += subset[0]->post_order();

            for(int i = 1; i < child_count; i ++){ 
                s += subset[i]->post_order();
                
                if(i+1 <= data_count){
                    s += to_string(data[i-1]) + "|" ;
                }

            }
            s += to_string(data[data_count-1]) + "|" ;



        }else{
            for(int i = 0; i < data_count; i ++){
                s += to_string(data[i]) + "|" ;
            }
        }
        return s;
    }

    Iterator begin(){
        return Iterator(get_smallest_node());
        // return Iterator(nullptr, 0);
    }
    Iterator end(){
        return Iterator(NULL);
        // return Iterator(nullptr, 0);
    }
    ostream& list_keys(Iterator from = NULL, Iterator to = NULL){
        if (from == NULL) from = begin();
        if (to == NULL) to = end();
        for (Iterator it = from; it != to; it++)
            cout<<*it<<" ";
        return cout;

    }

private:
    static const int MINIMUM = 1;
    static const int MAXIMUM = 2 * MINIMUM;

    bool dups_ok;                        //true if duplicate keys are allowed
    int data_count;                      //number of data elements
    T data[MAXIMUM + 1];                 //holds the keys
    int child_count;                     //number of children
    BPlusTree* subset[MAXIMUM + 2];      //subtrees
    BPlusTree* next;
    bool is_leaf() const
    {return child_count==0;}             //true if this is a leaf node

    template <class V>
    V* find_ptr(const T& entry){
        if(data_count<=0){
            return nullptr;
        }
        bool found = false;
        int i = first_ge(data, data_count, entry);
        if(data_count < 0){ return nullptr; }

        if(data[i] == entry && i < data_count)
        { found = true; }

        if(found){
            return *this; 
        }else{
            if(is_leaf()){
                return nullptr; //is thats the leaf, return nullptr
            } else { 
                return subset[i]->find_ptr(entry); //if not, go down til leaf
            }
        }
    }         //return a pointer to this key.
                                         // NULL if not there.

    //insert element functions
    void loose_insert(const T& entry){
        int i = first_ge(data, data_count, entry);
        bool found = (i<data_count && data[i] == entry);
        dups_ok = true;
        if(found && is_leaf()){
            if(dups_ok)
                data[i] = data[i] + entry;
            else
                data[i] = entry;
        }else if(found && !is_leaf()){
            subset[i+1]->loose_insert(entry);
            fix_excess(i+1);
        }else if(!found && is_leaf()){
            insert_item(data, i, data_count, entry);
        }else if(!found && !is_leaf()){
            subset[i]->loose_insert(entry);
            fix_excess(i);
        }
        /*
        int i = first_ge(data, data_count, entry);
        bool found = (i<data_count && data[i] == entry);

        three cases:
            found
            a. found/leaf: deal with duplicates: call +
            b. found/no leaf: subset[i+1]->loose_insert(entry)
                            fix_excess(i+1) if there is a need

            ! found:
            c. !found / leaf : insert entry in data at position i
            c. !found / !leaf: subset[i]->loose_insert(entry)
                                fix_excess(i) if there is a need

                |   found          |   !found         |
          ------|------------------|------------------|-------
          leaf  |a. dups? +        | c: insert entry  |
                |                  |    at data[i]    |
          ------|------------------|------------------|-------
                | b.               | d.               |
                |subset[i+1]->     | subset[i]->      |
          !leaf | loose_insert()   |  loose_insert()  |
                |fix_excess(i+1)   | fix_excess(i)    |
                |                  |                  |
          ------|------------------|------------------|-------
        */
    }   //allows MAXIMUM+1 data elements in
                                         //   the root
    void fix_excess(int i){
        //this node's child i has one too many items: 3 steps:
        if(subset[i]->data_count > MAXIMUM){
        //1. add a new subset at location i+1 of this node
        BPlusTree<T>* p = new BPlusTree<T>();

        

        insert_item(subset, i+1, child_count, p);
        
        //2. split subset[i] (both the subset array and the data array) and move half into
        //      subset[i+1] (this is the subset we created in step 1.)
        split(subset[i]->data, subset[i]->data_count, subset[i+1]->data, subset[i+1]->data_count);
        
        if(subset[i]->child_count != 0){
            split(subset[i]->subset, subset[i]->child_count, subset[i+1]->subset, subset[i+1]->child_count);
        }
        //3. detach the last data item of subset[i] and bring it and insert
        //      it into this node's data[]
        //

        T something;
        insert_item(data, i, data_count, something);

        detach_item(subset[i]->data, subset[i]->data_count, data[i]);

        T e = data[i];
        if(subset[i]->is_leaf()){
            insert_item(subset[i+1]->data, 0, subset[i+1]->data_count, e);
            p->next = subset[i]->next;
            subset[i]->next = p;
        }

        //Note that this last step may cause this node to have too many itesm.
        //  This is OK. This will be
        //  dealt with at the higher recursive level. (my parent will fix it!)
        //
        //NOTE: ODD/EVENNESS
        // when there is an excess, data_count will always be odd
        //  and child_count will always be even.
        //  so, splitting is always the same.

        //  000 B_PLUS_TREE
        //  if (subset[i]->is_leaf())
        //  transfer the middle entry to the right and...
        //  Deal with next pointers. just like linked list insert
        }
    }              //fix excess in child i

    //remove element functions:
    void loose_remove(const T& entry){
    
    /* four cases:
           leaves:
                a. not found: there is nothing to do
                b. found    : just remove the target
           non leaf:
                c. not found: subset[i]->loose_remove, fix_shortage(i)
                d. found    : subset[i+1]->loose_remove, fix_shortage(i+1) [...]
                    (No More remove_biggest)

             |   !found               |   found                 |
       ------|------------------------|-------------------------|-------
       leaf  |  a: nothing            | b: delete               |
             |     to do              |    target               |
       ------|------------------------|-------------------------|-------
       !leaf | c:                     | d: B_PLUS_TREE          |
             |  [i]->  loose_remove   |   [i+1]-> loose_remove  |
             |  fix_shortage(i)       | fix_shortage(i+1) [...] |
       ------|------------------------|-------------------------|-------


     */
    int i = first_ge(data, data_count, entry);
    T found_entry;
    bool found = data[i] == entry;
    if (is_leaf()){
        if(!found){
            //[a.] nothing to do
            //entry not in this tree
        }
        else{
            //[b.] just delete the item
            //   my parent will take care of my shortage
            delete_item(data, i, data_count, found_entry);
        }

    }
    else{
        //not leaf:
        // ---- 000 B_PLUS_TREE: no more remove_biggest
        if (!found){
            //[c.] Not here: subset[i]->loose_remove
            //not found yet. search in the next level:
            subset[i]->loose_remove(entry);
            fix_shortage(i);
            

        }
        else{
            //[d.] found key in an inner node:subset[i+1]->loose_remove
            assert(i < child_count-1);

            /*
             * Here's The Story:
             * data[i] is the same as the item that we have deleted.
             * so, now, it needs to be replaced by the current smallest key
             *      in subset[i+1]
             * Problem: After fix_shortage, data[i] could have moved to a different index(?)
             *      or it could have sunk down to a lower level as a result of merge
             *      we need to find this item and replace it.
             *
             *      Before fix_shortage: we cannot replace data[i] with subset[i+1]->smallest
             *      before fix_excess because this smallest key could be the very entry
             *      we are removing. So, we'd be replacing data[i] with entry. and this will
             *      leave the deleted key in the inner nodes.
             *
             *      After fix_shortage: We have no way of knowing where data[i] key (which is
             *      same as entry) will end up. It could move down one level by the time
             *      fix_shortage returns.
             *
             *      Therefore it seems like we have no choice but to search for it in
             *      data[ ] AND subset[i]->data[ ]
             * Current Solution: Kind of messy:
             *      After fix_shortage(i+1):
             *      Search for entry in data[ ] and if found, replace it with smallest.
             *      otherwise, search for it in subset[i]->data[ ]
             *          and replace it. that's where the key (entry) will end up after
             *          fix order returns (credit: Jiaruy Li)
             *
             */
            subset[i+1]->loose_remove(entry);
            delete_item(data, i, data_count, found_entry);

            // subset[i+1]->loose_remove(entry);

            //remember. it's possible that data[i] will be gone by the time
            //      fix_shortage returns.
            //key was removed from subset[i+1]:
            //  1. shortage: find entry in data or subset[i+1]->data 
            //              and replace it with subset[i+1]->smallest
            //  2. no shortage: replace data[i] with subset[i+1]->smallest
        }
        // --------------------------------------------------------
    }
    }  //allows MINIMUM-1 data elements
                                        //  in the root

    BPlusTree<T>* fix_shortage(int i){
        if(subset[i]->data_count < MINIMUM){
            if(i == 0){ // if the child is on the left most
                if(subset[i+1]->data_count > MINIMUM){
                    transfer_left(i);
                }else{
                    merge_with_right(i);
                }
            }else if(i == child_count-1){ // if child is on the right most
                if(subset[i-1]->data_count > MINIMUM){
                    transfer_right(i);
                }else{
                    merge_with_left(i);
                }
            }else{  // if child is in middle
                if(subset[i+1]->data_count > MINIMUM){
                    transfer_left(i);
                }else if(subset[i-1]->data_count > MINIMUM){
                    transfer_right(i);
                }else{
                    merge_with_right(i);
                }
            }
        }
    }  //fix shortage in child i
    // and return the smallest key in this subtree
    BPlusTree<T>* get_smallest_node(){
        if(is_leaf()){
            return this;
        }else{
            return subset[0]->get_smallest_node();
        }
    }
    void get_smallest(T& entry){
        //replace entry with the left_most leaf of this tree
        // Keep looking in the first subtree (recursive)
        //  until you get to a leaf.
        // Then, replace entry with data[0] of the leaf node
        if(is_leaf()){
            entry = data[0];
        }else{
            subset[0]->get_smallest(entry);
        }
    }      //entry := leftmost leaf
    void get_biggest(T& entry){
        if(is_leaf()){
            entry = data[data_count-1];
        }else{
            subset[child_count-1]->get_biggest(entry);
        }
    }       //entry := rightmost leaf
    void remove_biggest(T& entry){

    }    //remove the biggest child of tree->entry
    void transfer_left(int i){ //NEED TO BE TESTED
        T item;
        delete(subset[i+1]->data, 0, subset[i+1]->data_count, item);
        attach_item(subset[i]->data, subset[i]->data_count, item);
        data[i] = subset[i+1]->data[0];
    }        //transfer one element LEFT from child i
    void transfer_right(int i){ //NEED TO BE TESTED
        T item;
        detach(subset[i-1]->data, subset[i-1]->data_count, item);
        insert_item(subset[i]->data, 0, subset[i]->data_count, item);
        data[i-1] = item;
    }       //transfer one element RIGHT from child i
    BPlusTree<T>* merge_with_next_subset(int i){
        BPlusTree<T>* a;
        return a;
    } //merge subset i with  i+1
    void merge_with_right(int i){ //NEED TO REWRITE
        T item;
        delete_item(data, i, data_count, item);
        insert_item(subset[i]->data, subset[i]->data_count, subset[i]->data_count, item);
        if(subset[i+1]->subset[0]){
            merge(subset[i]->subset, subset[i]->child_count, subset[i+1]->subset, subset[i+1]->child_count);
        }  
        merge(subset[i]->data, subset[i]->data_count, subset[i+1]->data, subset[i+1]->data_count);
        
        BPlusTree<T>* p;
        delete_item(subset, i+1, child_count, p);
    }         

    void merge_with_left(int i){ //NEED TO REWRITE
        T item;
        delete_item(data, i-1, data_count, item);
        insert_item(subset[i-1]->data, subset[i-1]->data_count, subset[i-1]->data_count, item);
        if(subset[i]->subset[0]){
            merge(subset[i-1]->subset, subset[i-1]->child_count, subset[i]->subset, subset[i]->child_count);
        }  
        merge(subset[i-1]->data, subset[i-1]->data_count, subset[i]->data, subset[i]->data_count);
        
        BPlusTree<T>* p;
        delete_item(subset, i, child_count, p);
    }    

};


#endif