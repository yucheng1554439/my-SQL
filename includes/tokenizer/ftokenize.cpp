#include "ftokenize.h"

FTokenizer::FTokenizer(char* fname){
    _f.open(fname);
    if(!_f.is_open()){ 
        std::cout << "File is not found";
        exit(2);
        
    }
    _more = true;    //CHANGED
    _pos = 0;
    get_new_block();

    
    
    // _more = _f.eof();
}

Token FTokenizer::next_token(){
        Token t;
        _stk >> t;
        return t;
    // }
}
bool FTokenizer::more(){
    //return _more;
    return (!_f.eof() || !_stk.done()); //&& get_new_block()&& 
}        //returns the current value of _more

bool FTokenizer::done(){
    return _stk.done();
}

int FTokenizer::pos(){
    return _pos;
}          //returns the value of _pos

int FTokenizer::block_pos(){
    return _blockPos;
}     //returns the value of _blockPos

FTokenizer& operator >> (FTokenizer& f, Token& t){
    // if(f.done()){
    //     f.get_new_block();
    // }
    t = f.next_token();

    if(t.token_str().empty()) 
    {
        if(f.get_new_block()){
            t = f.next_token();
        }
    }

    // t = f.next_token();
    return f;
}

bool FTokenizer::get_new_block(){
    if(!more()){
        return false;
    }
    char _buffer[MAX_BUFFER];
    _f.read(_buffer, MAX_BUFFER-1);

    if(_f.gcount() == 0) { 
        // std::cout << "!!!!!!!!!!!!"; 
        _more = false;
        return false; 
    }


    _buffer[_f.gcount()] = '\0';
    //_stk.set_pos(0);
    _stk.set_string(_buffer);

    std::cout << "\n==========NEW BLOCK==========\n";
    std::cout << _buffer << std::endl;
    cout<< "[" <<_f.gcount() <<"]" <<endl;

    return true;
    // char _buffer[MAX_BUFFER+1];
    // _f.read(_buffer, MAX_BUFFER);

    // if(_f.gcount() == 0) { 
    //     // std::cout << "!!!!!!!!!!!!"; 
    //     return false; 
    // }


    // _buffer[_f.gcount()] = '\0';
    // _stk.set_pos(0);
    // _stk.set_string(_buffer);
    // std::cout << "\n==========NEW BLOCK==========\n";
    // std::cout << _buffer << std::endl;
    
    // return true;
} //gets the new block from the file

    