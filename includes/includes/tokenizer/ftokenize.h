#ifndef FTOKENIZE_H
#define FTOKENIZE_H
#include <iostream>
#include <fstream>
#include "token.h"
#include "stokenize.h"
#include "constants.h"
#include "state_machine_functions.h"

class FTokenizer
{
public:
    //get a amount of buffer from the file and then get token return
    //, if the buffer is empty then get more buffer from the file
    const int MAX_BLOCK = MAX_BUFFER;
    FTokenizer(char* fname);
    Token next_token();
    bool more();        //returns the current value of _more
    bool done();
    int pos();          //returns the value of _pos
    int block_pos();     //returns the value of _blockPos
    friend FTokenizer& operator >> (FTokenizer& f, Token& t);
private:
    bool get_new_block(); //gets the new block from the file
    std::ifstream _f;   //file being tokenized
    STokenizer _stk;     //The STokenizer object to tokenize current block
    int _pos;           //Current position in the file
    int _blockPos;      //Current position in the current block
    bool _more;         //false if last token of the last block
                        //  has been processed and now we are at
                        //  the end of the last block.
    
};

#endif