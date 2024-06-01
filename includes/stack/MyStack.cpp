#ifndef MYSTACK_CPP
#define MYSTACK_CPP

#include "MyStack.h"

template <typename ITEM_TYPE>
Stack<ITEM_TYPE>::Stack(){
    _top = nullptr;
    _size = 0;
};

//worked!!!
template <typename ITEM_TYPE>
Stack<ITEM_TYPE>::Stack(const Stack<ITEM_TYPE>& copyMe){
    // _front = _copy_list(copyMe._front);
    // node<T>* walker = _front;
    // while(walker->_next != nullptr){
    //     walker = walker -> _next;
    // }
    // _rear = walker;
    // _size = copyMe._size;

    if(copyMe._top != nullptr){
        if(copyMe._top->_prev != nullptr){
            node<ITEM_TYPE>* head = copyMe._top;
            // std::cout << "CNM";
            while(head->_prev != nullptr){
                head = _previous_node(head);
            }
            // std::cout << "CNM";
            node<ITEM_TYPE>* newHead = _copy_list(head);
            
            // std::cout << "CNM";
            while(newHead->_next != nullptr){
                newHead = newHead->_next;
            }
            _top = newHead;
            _size = copyMe._size;
        }else{
            node<ITEM_TYPE>* newNode = new node<ITEM_TYPE>;
            newNode->_item = copyMe._top->_item;
            newNode->_next = nullptr;
            newNode->_prev = nullptr;
            _top = newNode;
        }
    }else{
        _top = nullptr;
        _size = 0;
    }
    // std::cout << "CNM";
    // node<ITEM_TYPE>* headOfCopyMe = copyMe._top;
    // node<ITEM_TYPE>* walker = copyMe._top;
    // while(walker->_prev != nullptr){
    //     walker = walker->_prev;
    // }
    // while(walker != nullptr){
    //     this->push(walker->_item);
    //     walker = walker->_next;
    // }

}


template <typename ITEM_TYPE>
Stack<ITEM_TYPE>::~Stack(){
    if(_top != nullptr){
        if(_top->_prev == nullptr){
            delete(_top);
        }else{
        // std::cout << "WDNMD";
        node<ITEM_TYPE>* head = _previous_node(_top);
        while(head->_prev != nullptr){
            head = _previous_node(head);
        }
        //std::cout << "hello";
        _clear_list(head);
        }
    }
}


template <typename ITEM_TYPE>
Stack<ITEM_TYPE>& Stack<ITEM_TYPE>::operator=(const Stack<ITEM_TYPE>& RHS){
    //assert(RHS._size != 0 && "Empty copy stack object(=operator)");
    if(this == &RHS){

    }else if(RHS._size != 0){
        node<ITEM_TYPE>* RHShead = RHS._top;
        while(RHShead->_prev != nullptr){
            RHShead = _previous_node(RHShead);
        }

        node<ITEM_TYPE>* LHShead = _top;
        if(_top == nullptr){
            _top = _copy_list(LHShead, RHShead);
            _size = RHS._size;
        }else{
            while(LHShead->_prev != nullptr){
                LHShead = _previous_node(LHShead);
            }
            _top = _copy_list(LHShead, RHShead);
            _size = RHS._size;
        }
    }else{
        _top = nullptr;
        _size = 0;
    }
    
    return *this;
}

template <typename ITEM_TYPE>
ITEM_TYPE Stack<ITEM_TYPE>::top(){
    assert(_top != nullptr && "The Stack is Empty(top())");
    return _top->_item;
}


template <typename ITEM_TYPE>
bool Stack<ITEM_TYPE>::empty(){
    // node<ITEM_TYPE>* head = _previous_node(_top);
    // while(head->_prev != nullptr){
    //     head = _previous_node(head);
    // }
    // _clear_list(head);
    return _top == nullptr;
}


//checked
template <typename ITEM_TYPE>
void Stack<ITEM_TYPE>::push(ITEM_TYPE item){
    // node<ITEM_TYPE>* head =_top;
    if(_top!=nullptr){
        // node<ITEM_TYPE>* head = _previous_node(_top);
        // while(head->_prev != nullptr){
        //     head = _previous_node(head);
        // }
        // _top = _insert_after(head, _top, item);
        node<ITEM_TYPE>* newNode = new node<ITEM_TYPE>;
        newNode->_item = item;
        newNode->_next = nullptr;
        newNode->_prev = _top;
        _top->_next = newNode;
        _top = newNode;
        
        //std::cout << "CNM";
    }else if(_top==nullptr){
        node<ITEM_TYPE>* newNode = new node<ITEM_TYPE>;
        _top = newNode;
        _top ->_item = item;
        _top ->_next = nullptr;
        _top ->_prev = nullptr;
    }
    _size++;
}


template <typename ITEM_TYPE>
ITEM_TYPE Stack<ITEM_TYPE>::pop(){
    assert(_top != nullptr && "The Stack is Empty(pop())");
    //ITEM_TYPE item;
    node<ITEM_TYPE>* temp = _top;
    node<ITEM_TYPE>* head = _top;
    while(head->_prev != nullptr){
        head = _previous_node(head);
    }

    _top = _top->_prev;
    //_delete_node(head,_top);
    
    _size--;

    return _delete_node(head,temp);
}


template<typename T>
ostream& operator<<(ostream& outs, const Stack<T>& printMe){
    
    node<T>* _temp = printMe._top;
    outs << "Stack:Head->";

    while(_temp != nullptr){
        outs << "<-[" << _temp->_item << "]-> ";
        _temp = _temp->_prev;
    }
    outs << "|||";
    return outs;
}


template <class ITEM_TYPE>
typename Stack<ITEM_TYPE>::Iterator Stack<ITEM_TYPE>::begin() const{
    return Iterator(_top);
}                   //Iterator to the head node


template <class ITEM_TYPE>
typename Stack<ITEM_TYPE>::Iterator Stack<ITEM_TYPE>::end() const{
    // node<ITEM_TYPE>* head = _top;
    // while(head->_prev != nullptr){
    //     head = _previous_node(head);
    // }
    return Iterator(nullptr);
}                      //Iterator to NULL    



#endif