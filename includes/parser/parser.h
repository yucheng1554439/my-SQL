#ifndef PARSER_H
#define PARSER_H

#include <iostream>
#include "../../includes/table/typedefs.h"
#include "../tokenizer/stokenize.h"
#include "fstream"
// #include "../binary_files/file_record.h"
// #include "../binary_files/utilities.h"
// #include "../table/table.h"
using namespace std;



class Parser{
    public:
    Parser();
    Parser(string string);
    mmap_ss parse_tree();
    void set_string(string string);
    keys get_column(Token token);
    bool get_parse_tree();
    // void make

    private:

    string toupper(string string);

    mmap_ss ptree;
    Map<string, keys> keyword;
    bool flag;
    STokenizer _table_for_enum; //state machine for enumed vector string
    vector<Token> _token_holder;
    vector<keys> _token_holder_enum_type;
    string enum_form;
    string _buffer;
};





#endif