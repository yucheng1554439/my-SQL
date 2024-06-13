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
    void run();
    void batch();
    

    private:
    string _table_name_list;
    vector<long> _recnos_selected;
    Map<string, Table> _table_map;
    bool _valid_String;


    void currentExistTables();
    void clearTableNameTxtFile();
    string toupper(string string);
    string tolower(string string);
    

};


#endif