#ifndef MYQUEUE_CPP
#define MYQUEUE_CPP

#include "MyQueue.h"
template <typename T>
Queue<T>::Queue(){
    _front = nullptr;
    _rear = nullptr;
    _size = 0;
}


template <typename T>
Queue<T>::Queue(const Queue<T>& copyMe){
    _front = _copy_list(copyMe._front);
    node<T>* walker = _front;
    while(walker->_next != nullptr){
        walker = walker -> _next;
    }
    _rear = walker;
    _size = copyMe._size;
}
template <typename T>
Queue<T>::~Queue(){
    if(_front != nullptr){
        _clear_list(_front);
    }
    
}
template <typename T>
Queue<T>& Queue<T>::operator=(const Queue<T>& RHS){
    
    _rear = _copy_list(_front, RHS._front);
    _size = RHS._size;
    return *this;
}

template <typename T>
bool Queue<T>::empty(){
    return _front == nullptr;
}
template <typename T>
T Queue<T>::front(){
    return _front->_item;
}
template <typename T>
T Queue<T>::back(){
    return _rear->_item;
}

template <typename T>
void Queue<T>::push(T item){
    if(_rear==nullptr){
        node<T>* newNode = new node<T>;
        _rear = newNode;
        _front = newNode;
        _rear ->_item = item;
        _rear ->_next = nullptr;
        _rear ->_prev = nullptr;
    }else{
        node<T>* newNode = new node<T>;
        newNode->_item = item;
        newNode->_next = nullptr;
        newNode->_prev = _rear;
        _rear->_next = newNode;
        _rear = newNode;
    }
    _size++;

}
template <typename T>
T Queue<T>::pop(){
    assert(_front != nullptr && "The Queue is Empty(pop())");
    node<T>* temp = _front->_next;
    T item = _delete_node(_front,_front);
    _front = temp;
    if(_front == nullptr){
        _rear = nullptr;
    }
    _size--;

    return item;
}

template <typename T>
typename Queue<T>::Iterator Queue<T>::begin() const{
        return Iterator(_front);
}                                     //Iterator to the head node
template <typename T>
typename Queue<T>::Iterator Queue<T>::end() const{
    return Iterator(nullptr);
}                                       //Iterator to NULL
template <typename T>
void Queue<T>::print_pointers(){
    _print_list(_front);
}

template <typename TT>
ostream& operator << (ostream& outs, const Queue<TT>& printMe){
    node<TT>* _temp = printMe._front;
    outs << "Queue:Head->";
    while(_temp != nullptr){
        outs << "<-[" << _temp->_item << "]-> ";
        _temp = _temp->_next;
    }
    outs << "|||";
    return outs;
}

#endif