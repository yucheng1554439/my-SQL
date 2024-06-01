#ifndef MYSTACK_H
#define MYSTACK_H
#include <iostream>
using namespace std;
#include "../../includes/node/node.h"
#include "../../includes/linked_list_functions/linkedlistfunctionsTemplated.h"
template <typename ITEM_TYPE>
class Stack{
public:
    class Iterator{
    public:
        friend class Stack;                 //give access to list to access _ptr
        Iterator(){_ptr = NULL;}            //default ctor
        Iterator(node<ITEM_TYPE>* p){
            _ptr = p;
        }      //Point Iterator to where
                //  p is pointing to
        ITEM_TYPE operator *(){
            return _ptr->_item;
        }    //dereference operator
        bool is_null(){return _ptr == NULL;}            //true if _ptr is NULL
        friend bool operator !=(const Iterator& left,
                                const Iterator& right)  //true if left != right
        {return left._ptr != right._ptr;}

        friend bool operator ==(const Iterator& left,
                                const Iterator& right)  //true if left == right
        {return left._ptr == right._ptr;}


        Iterator& operator++(){                        //member operator:++it
            //_ptr = _ptr->_next;
            return *this;                             // or ++it = new_value
        }

        friend Iterator operator++(Iterator& it,
                                   int unused){        //friend operator: it++
            assert(it._ptr!=NULL && "++ worng");
            it._ptr = it._ptr->_prev;  
            return ++it;
        }

    private:
        node<ITEM_TYPE>* _ptr;    //pointer being encapsulated
    
    };



    Stack();
    
    Stack(const Stack<ITEM_TYPE>& copyMe);
    ~Stack();
    Stack<ITEM_TYPE>& operator=(const Stack<ITEM_TYPE>& RHS);
    ITEM_TYPE top();
    bool empty();
    void push(ITEM_TYPE item);
    ITEM_TYPE pop();
    template<typename T>
    friend ostream& operator<<(ostream& outs, const Stack<T>& printMe);
    Iterator begin() const;                   //Iterator to the head node
    Iterator end() const;                     //Iterator to NULL
    int size() const { return _size; }

private:
    node<ITEM_TYPE>* _top;
    int _size;
};

#include "MyStack.cpp"
#endif