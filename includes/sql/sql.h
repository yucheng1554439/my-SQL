#ifndef SQL_H
#define SQL_H

#include "../parser/parser.h"
#include "../table/table.h"


using namespace std;

class SQL{
    public:
    SQL();
    Table command(string string);
    vector<long> select_recnos();

    private:
    vector<long> _recnos_selected;
    // Table _table;
};


#endif