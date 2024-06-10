#ifndef TABLE_H
#define TABLE_H
// #include <cmath>
// #include <iostream>
// #include <iomanip>
// #include <set>
#include <iostream>
#include <fstream>
#include <vector>
#include "typedefs.h"


using namespace std;

class Table{
    public:
    Table();
    Table(string a);
    Table(string a, vectorstr b);

    // Table(const Table& a);
    // ~Table();
    // Table& operator =(const Table& RHS);

    void insert_into(vectorstr b);
    Table select(vectorstr fieldnames, Queue<Token*> queue_of_compar);
    Table select(vectorstr fieldnames, vector<string> string_of_compar);
    Table select(vectorstr fieldnames, string field_searching, string operatr, string value_searching);
    Table select(vectorstr fieldnames);
    Table select_all();
    vectorstr get_field_names();
    Table vector_table(vector<long> vec, string fileName, int fieldNameLength);
    vector<long> select_recnos(); //return the record numbers
    friend ostream& operator <<(ostream& outs, const Table& print_me);
    static int sequenceNumber;
    
    
    private:
    vectorstr field_names;
    string title;
    vector<MMap<string, long> > _indices_recno;
    Map<string, int> index;
    vector<long> recnoVec;
    vector<long> selected_recnos;
    int numOfRec;
};





#endif