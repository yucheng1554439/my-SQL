#ifndef SQL_H
#define SQL_H

#include "../parser/parser.h"
#include "../table/table.h"


using namespace std;

class SQL{
    public:
    SQL();
    Table command(string string);
    void run();
    vector<long> select_recnos();

    private:
    string toupper(string string);
    string tolower(string string);
    vector<long> _recnos_selected;
    // Table _table;
    bool _valid_String;

};


#endif