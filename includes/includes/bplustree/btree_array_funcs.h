#ifndef BTREE_ARRAY_FUNCTIONS_H
#define BTREE_ARRAY_FUNCTIONS_H
#include <iostream>
#include <cassert>
#include "vector"


using namespace std;

template <class T>
T maximal(const T& a, const T& b){
    if(a >= b)
        return a;
    else
        return b;
}                      //return the larger of the two items

template <class T>
void swap(T& a, T& b){
    T temp = a;
    a = b;
    b = temp;
}  //swap the two items

template <class T>
int index_of_maximal(T data[ ], int n){
    T max = data[0];
    int index = 0;

    for(int i = 0; i < n; i++)
    {
        if(data[i] > max)
        {    
            max = data[i];
            index = i;
        }
    }

    return index;
}                 //return index of the largest item in data

template <class T>
void ordered_insert(T data[ ], int& n, T entry){
    if(n < 0)
      return;
    int insert_here = first_ge(data, n, entry);

    if(insert_here < 0)//data count = 0
        insert_here = 0;

    if(entry == data[insert_here])
        return;
    insert_item(data, insert_here, n, entry);
}        //insert entry into the sorted array
                                                        //data with length n

template <class T>
int first_ge(const T data[ ], int n, const T& entry){
    if(n==0){return 0;}
    for(int i = 0; i < n; i++){
        if(data[i]>=entry){
            return i;
        }
    }   
    return n;
}   //return the first element in data that is not less than entry

template <class T>
void attach_item(T data[ ], int& n, const T& entry){
    data[n] = entry;
    n++;
}    //append entry to the right of data

template <class T>
void insert_item(T data[ ], int i, int& n, T entry){
    for (int j = n - 1; j >= i; j--)
        data[j + 1] = data[j];
    data[i] = entry;
    n++;

}    //insert entry at index i in data

template <class T>
void detach_item(T data[ ], int& n, T& entry){
    entry = data[n-1];
    // std::cout << "Detach Item Entry: " << entry << endl;
    // int i = first_ge(data, n, entry);
    // insert_item(data, i, n, entry);
    // std::cout << "entry is: " << entry << "\n";
    n--;
    // std::cout << "n is: " << n << "\n";
}          //remove the last element in data and place it in entry

template <class T>
void delete_item(T data[ ], int i, int& n, T& entry){
    if(i >= 0){
        T item = data[i];
        for(int j = i; j < n-1; j++){
            data[j] = data[j+1];
        }
        n--;
        entry = item;
    }
    
}   //delete item at index i and place it in entry

template <class T>
void merge(T data1[ ], int& n1, T data2[ ], int& n2){
    for(int i = 0; i < n2; i++){
        data1[n1+i] = data2[i];
    }
    n1 = n1+n2;
    n2 = 0;
}   //append data2 to the right of data1

template <class T>
void split(T data1[ ], int& n1, T data2[ ], int& n2){
    assert(n1 != 0 && "SPLIT CANT BE EMPTY");
    for(int i = n1-n1/2; i < n1; i++){
        insert_item(data2, i-n1+n1/2, n2, data1[i]);
        // std::cout << "\ni-n1-n1/2 is: " << i-n1+n1/2 << " data2[i-n1-n1/2] is : " << data2[i-n1-n1/2] << "\n";
        // std::cout << "\ndata1[n1-1] is: " << data1[n1-1] << endl;
    }
    n1 -= n1/2;
    // std::cout << "\ndata2[n2] is : " << data2[n2-1] << "\n";
    // std::cout << "\ndata1[n1] is: " << data1[n1-1] << endl;
    // std::cout << "\nn1 is : " << n1 << "\n";
    // // n2 = n1/2;
    // std::cout << "\nn2 is : " << n2 << "\n";
}   //move n/2 elements from the right of data1 and move to data2

template <class T>
void copy_array(T dest[], const T src[],
                int& dest_size, int src_size){
                    dest_size = src_size;
                    for(int i = 0; i < src_size; i++){
                        dest[i] = src[i];
                    }
                    
}              //copy src[] into dest[]

template <class T>
void print_array(const T data[], int n, int pos = -1){

}  //print array data

template <class T>
bool is_gt(const T data[], int n, const T& item){
    return item > data[n];
}       //item > all data[i]

template <class T>
bool is_le(const T data[], int n, const T& item){
    return item <= data[n];
}       //item <= all data[i]

//-------------- Vector Extra operators: ---------------------

template <typename T>
ostream& operator <<(ostream& outs, const vector<T>& list){
    for (int i = 0; i < list.size(); i++) {
        outs << list[i] << " ";
    }
    return outs;
} //print vector list

template <typename T>
vector<T>& operator +=(vector<T>& list, const T& addme){
    list.push_back(addme);
    return list;
} //list.push_back addme

#endif