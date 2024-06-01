#ifndef STOKENIZE_H
#define STOKENIZE_H
#include "token.h"
#include "constants.h"
#include <iostream>
#include "state_machine_functions.h"
using namespace std;
class STokenizer
{
public:
    STokenizer();
    STokenizer(char str[]);
    STokenizer(vector<string> vector_enum);
    bool done();            //true: there are no more tokens
    bool more();            //true: there are more tokens
    //
    //---------------
    //extract one token (very similar to the way cin >> works)
    friend STokenizer& operator >> (STokenizer& s, Token& t);
    //set a new string as the input string
    void set_string(char str[]);
    void set_pos(int pos);
    bool contains(const char array[], char element);
    void make_adjacency_table();
    bool isSuccess(int state){return is_success(_table, state);}
    
    int get(int state, keys column_no);
    static int _table[MAX_ROWS][MAX_COLUMNS];

private:
    //create table for all the tokens we will recognize
    //                      (e.g. doubles, words, etc.)
    void make_table(int _table[][MAX_COLUMNS]);
    void make_table_enum(int _table[][MAX_COLUMNS]);
    //extract the longest string that match
    //     one of the acceptable token types
    bool get_token1(string input, int& pos, int &state, int table[][MAX_COLUMNS], string &t);

    bool get_token(int& start_state, string& token);
    //---------------------------------
    char _buffer[MAX_BUFFER];       //input string buffer is the cstring which is the user input line
    int _pos;                       //current position in the string
    
};

#endif