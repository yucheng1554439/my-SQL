#ifndef LINKED_LIST_FUNCTIONS_H
#define LINKED_LIST_FUNCTIONS_H

#include "assert.h"
#include "../../includes/node/node.h"

template <typename T>
void _print_list(node<T>* head){
    //make sure that the head pointer is not pointing at nothing(means its empty list)
    //assert(head != nullptr && "linkedlist cant be empty(_print_list)");
    
    node<T>* walker = head;
    while (walker != nullptr){
        if(walker == head){
            std::cout << "Head-><-[" << walker->_item << "]-> ";
        }else if(walker->_next == nullptr){
            std::cout << "<-[" << walker->_item << "]-> |||";
        }else{
            std::cout << "<-[" << walker->_item << "]-> ";
        }
        walker = walker->_next;
    }
    std::cout << std::endl;
}

//recursive fun! :)
template <typename T>
void _print_list_backwards(node<T> *head){
    //make sure that the head pointer is not pointing at nothing(means its empty list)
    assert(head != nullptr && "linkedlist cant be empty(_print_list_backwards)");
    if(head->_next != nullptr){
        _print_list_backwards(head->_next);
    }
    std::cout << (head->_item) << " ";
    
}

//return ptr to key or NULL
template <typename T>
node<T>* _search_list(node<T>* head, T key){
    //make sure that the head pointer is not pointing at nothing(means its empty list)
    assert(head != nullptr && "linkedlist cant be empty(_search_list)");
    node<T>* walker = head;
    while(walker != nullptr){
        if(walker->_item == key){
            return walker;
        }
        walker = walker->_next;
    }
    return walker;
}


template <typename T>
//when you dereference nullptr, computer does not like it
node<T>* _insert_head(node<T> *&head, T insert_this){
    node<T>* newNode = new node<T>;
    newNode->_item = insert_this;
    newNode->_next = head;
    newNode->_prev = nullptr;
    head = newNode;
    
    if(head->_next != nullptr){
        head->_next->_prev = head;
    }

    //******question: Do I return head????? or something else????

    return head;
}

//insert after ptr: insert head if marker null
template <typename T>
node<T>* _insert_after(node<T>*& head, node<T> *after_this, T insert_this){
    //make sure that the head pointer is not pointing at nothing(means its empty list)
    assert(head != nullptr && "linkedlist cant be empty(_insert_after)");
    //check if the after this node is in the list
    node<T>* walker = head;
    bool newNodeInTheList = false;
    while(walker != nullptr){
        if(walker == after_this){
            newNodeInTheList = true;
            break;
        }
        walker = walker->_next;
    }
    //make sure the after this node is in the list
    assert(newNodeInTheList && "The After This Node is not in the List(_insert_after)");
    node<T>* newNode = new node<T>;
    if(after_this->_next != nullptr){
        newNode->_item = insert_this;
        newNode->_next = after_this->_next;
        after_this->_next->_prev = newNode;
        after_this->_next = newNode;
        newNode->_prev = after_this;
    }else{
        newNode->_item = insert_this;
        newNode->_next = after_this->_next;
        after_this->_next = newNode;
        newNode->_prev = after_this;
    }
    
    //******question: Do I return head????? or something else????

    return newNode;
}

//insert before ptr: insert head if marker null
template <typename T>
node<T>* _insert_before(node<T>*& head, node<T>* before_this, T insert_this){
    //make sure that the head pointer is not pointing at nothing(means its empty list)
    assert(head != nullptr && "linkedlist cant be empty(_insert_before)");
    
    //check if the Before this node is in the list
    node<T>* walker = head;
    bool newNodeInTheList = false;
    while(walker != nullptr){
        if(walker == before_this){
            newNodeInTheList = true;
            break;
        }
        walker = walker->_next;
    }

    //make sure the Before this node is in the list
    assert(newNodeInTheList && "The Before This Node is not in the List(_insert_before)");
    
    walker = head; 
    // while(walker != nullptr){
    //     if(walker->_next == before_this){
            // node<T>* newNode = new node<T>;
            // newNode->_item = insert_this;
            // newNode->_next = walker->_next;
            // before_this->_prev = newNode;
            // walker->_next = newNode;
            // newNode->_prev = walker;

    //     }
    //     walker = walker->_next;
    // }
    node<T>* newNode = new node<T>;
    if(before_this->_prev!=nullptr){
        newNode->_item = insert_this;
        newNode->_next = before_this;
        before_this->_prev->_next = newNode;
        newNode->_prev = before_this->_prev;
        before_this->_prev = newNode;
    }else{
        newNode->_item = insert_this;
        newNode->_next = before_this;
        before_this->_prev = newNode;
        newNode->_prev = nullptr;
        head = newNode;
    }
    

    return newNode;
}

//ptr to previous node
template <typename T>
node<T>* _previous_node(node<T>* prev_to_this){
    if(prev_to_this != nullptr){
        return prev_to_this->_prev;
    }
    return prev_to_this;
}

//delete, return item
template <typename T>
T _delete_node(node<T>*& head, node<T>* delete_this){
    //make sure that the head pointer is not pointing at nothing(means its empty list)
    assert(head != nullptr && "linkedlist cant be empty(_delete_node)");
    T item;
    node<T>* walker = head;
    
    if(walker == delete_this){
        if(walker->_next != nullptr){
            walker->_next->_prev = nullptr;
            head = walker->_next;
            item = walker->_item;
            delete(walker);
            return item;
        }else{
            item = head->_item;
            delete(head);
            head = nullptr;
        }
        // item = walker->_item;
        // delete(walker);
        return item;
    }
    while(walker->_next != nullptr){
        if(walker->_next == delete_this){
            node<T>* temp = walker->_next->_next;
            item = walker->_next->_item;
            delete(walker->_next);
            walker->_next = temp;
            //temp->_prev = walker;
            break;
        }
        walker = walker->_next;
    }
    //if the null pointer is looped through and the while loop is not break then,
    assert(walker != nullptr && "delete target is not in list(_delete_node)");
    return item;
}

//checked
//duplicate the list...
template <typename T>
node<T>* _copy_list(node<T>* head){
    if(head != nullptr){
        node<T>* walker = head;
        
        node<T>* oldNode = new node<T>;
        oldNode->_item = walker->_item;
        oldNode->_prev = nullptr;
        oldNode->_next = nullptr;
        node<T>* newHead = oldNode;
        walker = walker->_next;

        while(walker != nullptr){
            node<T>* newNode = new node<T>;
            newNode->_item = walker->_item;
            newNode->_next = nullptr;
            newNode->_prev = oldNode;
            oldNode->_next = newNode;
            oldNode = newNode;
            walker = walker->_next;
        }
        return newHead;
    }
    return nullptr;
}

//duplicate the list, return pointer to last node in destination... 
//     use this function in your queue big three //source
// copy the src list to the dest list and return the tail of dest list, given dest head
// * p means the address where the pointer points at, *& p means the pointer it self which means we can change where it points at

template <typename T>
node<T> *_copy_list(node<T> *&dest, node<T> *src){
    node<T>* destTail = nullptr;
    if(dest != nullptr){
        _clear_list(dest);
    }

    if(src != nullptr){

        node<T>* walker = src;
        node<T>* oldNode = new node<T>;
        oldNode->_item = walker->_item;
        dest = oldNode;
        walker = walker->_next;

        while(walker != nullptr){
            node<T>* newNode = new node<T>;
            newNode->_item = walker->_item;
            oldNode->_next = newNode;
            newNode->_prev = oldNode;
            oldNode = newNode;
            walker = walker->_next;
            if(walker == nullptr){
                destTail = newNode;
                newNode->_next = nullptr;
            }
        }

    }else{
        
        dest = nullptr;

    }

    //Have Changed the dest point to the head of the list
    //return Tail of the list


    return destTail;
}



//delete all the nodes
template <typename T>
void _clear_list(node<T>*& head){
    //make sure that the head pointer is not pointing at nothing(means its empty list)
    assert(head != nullptr && "linkedlist cant be empty(_clear_list)");
    node<T>* walker = head;
    while(walker != nullptr){
        node<T>* temp = walker;
        walker = walker->_next;
        head = walker;
        delete(temp);
    }
}

//_item at this position
template <typename T>
T& _at(node<T>* head, int pos){
    //make sure that the head pointer is not pointing at nothing(means its empty list)
    assert(head != nullptr && "linkedlist cant be empty(_at)");
    T item;

    //*******Start from 0 or 1 ?????????

    int position = 0;
    node<T>* walker = head;
    while(walker != nullptr){
        if(position == pos){
            return walker->_item;
        }
        position ++;
        walker = walker->_next;
    }
    assert("Position has no item/Wrong position entered(_at)");

}


#endif