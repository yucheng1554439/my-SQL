#include "stokenize.h"
int STokenizer::_table[MAX_ROWS][MAX_COLUMNS] = {};
STokenizer::STokenizer()
{
    set_string("");
    _pos = 0;
    make_table(_table);
}
STokenizer::STokenizer(char str[]){
    set_string(str);
    _pos = 0;
    make_table(_table);
}
// STokenizer::STokenizer(vector<string> vector_enum){
//     set_string(vector_enum);
//     _pos = 0;
//     make_table(_table);
// }
bool STokenizer::done(){
    if(_pos <= strlen(_buffer)){
        return false;
    }
    return true;
}            //true: there are no more tokens
bool STokenizer::more(){
    return !done();
}            //true: there are more tokens
//
//---------------
//extract one token (very similar to the way cin >> works)
STokenizer& operator>> (STokenizer& s, Token& t){
    string token; 
    int start_state = 0;
    if(s.get_token(start_state, token)){
        t.setToken(token);
        if(token.length() > 1){
            if(s.contains(ALFA, token[0])){
                t.setType(TOKEN_ALPHA);
            }else if(s.contains(DIGITS, token[0])||s.contains(DIGITS, token[1])){
                t.setType(TOKEN_NUMBER);
            }
        }else{
            if(s.contains(ALFA, token[0])){
                t.setType(TOKEN_ALPHA);
            }else if(s.contains(DIGITS, token[0])){
                t.setType(TOKEN_NUMBER);
            }else if(s.contains(SPACES, token[0])){
                t.setType(TOKEN_SPACE);
            }else if(s.contains(OPERATORS, token[0])){
                t.setType(TOKEN_OPERATOR);
            }else if(s.contains(PUNC, token[0])){
                t.setType(TOKEN_PUNC);
            }
        }
    }
    // std::cout << "Token: " << t << std::endl;
    // std::cout << "Position: " << s._pos << std::endl;
    return s;
}

bool STokenizer::contains(const char array[], char element){
    for(int i = 0; i < strlen(array); i ++){
        if(array[i] == element){
            return true;
        }
    }
    return false;
}

//set a new string as the input string
void STokenizer::set_string(char str[]){
    strcpy(_buffer, str);
    _pos = 0;
}

void STokenizer::set_pos(int pos){
    _pos = pos;
}


// OG TABLE
// void STokenizer::make_table(int _table[][MAX_COLUMNS]){
//     init_table(_table);

//     //doubles:
//     mark_fail(_table, 0);            //Mark states 0 and 2 as fail states
//     mark_success(_table, 1);         //Mark states 1 and 3 and 4 as success states
//     mark_fail(_table, 2);

//     mark_success(_table, 3);
//     mark_success(_table, 4);
//     mark_success(_table, 5);
//     mark_success(_table, 6);
//     mark_success(_table, 7);
//     mark_success(_table, 8);
//     // mark_success(_table, 9);

//     //Punctuation:
//     mark_cells(0, _table, PUNC, 6);    //state [0] --- PUNC ---> [6]

//     mark_cells(0, _table, DIGITS, 1);    //state [0] --- DIGITS ---> [1]
//     mark_cells(0, _table, '.', '.', 4);  //state [0] --- '.' ------> [4] 
//     mark_cells(1, _table, DIGITS, 1);    //state [1] --- DIGITS ---> [1]
//     mark_cells(1, _table, '.', '.', 2);  //state [1] --- '.' ------> [2] 
//     mark_cells(2, _table, DIGITS, 3);    //state [2] --- DIGITS ---> [3]
//     mark_cells(3, _table, DIGITS, 3);    //state [3] --- DIGITS ---> [3]
    
//     mark_cells(4, _table, DIGITS, 3);    //state [4] --- DIGITS ---> [3]
//     // mark_cells(9, _table, DIGITS, 9);

//     //Alpha:
//     mark_cells(0, _table, ALFA, 5);    //state [0] --- ALFA ---> [5]
//     mark_cells(5, _table, ALFA, 5);    //state [5] --- ALFA ---> [5]

    

//     //Space:
//     mark_cells(0, _table, SPACES, 7);    //state [0] --- SPACE ---> [7]
//     mark_cells(7, _table, SPACES, 7);    //state [7] --- SPACE ---> [7]


//     //Operator
//     mark_cells(0, _table, OPERATORS, 8);    //state [0] --- SPACE ---> [8]

// }

int STokenizer::get(int state, keys column_no){
    return _table[state][column_no];
}


void STokenizer::make_adjacency_table(){
    make_table_enum(_table);
}

void STokenizer::make_table(int _table[][MAX_COLUMNS]){
    init_table(_table);

    //doubles:
    mark_fail(_table, 0);            //Mark states 0 and 2 as fail states
    mark_success(_table, 1);         //Mark states 1 and 3 and 4 as success states
    mark_fail(_table, 2);
    mark_fail(_table, 9);

    mark_success(_table, 3);
    mark_success(_table, 4);
    mark_success(_table, 5);
    mark_success(_table, 6);
    mark_success(_table, 7);
    mark_success(_table, 8);
    mark_success(_table, 10);

    //Punctuation:
    mark_cells(0, _table, PUNC, 6);    //state [0] --- PUNC ---> [6]
    mark_cells(0, _table, STAR, 6);    //state [0] --- PUNC ---> [6]
    mark_cells(0, _table, PARENTHSIS, 6);    //state [0] --- PUNC ---> [6]


    mark_cells(0, _table, DIGITS, 1);    //state [0] --- DIGITS ---> [1]
    mark_cells(0, _table, '.', '.', 4);  //state [0] --- '.' ------> [4] 
    mark_cells(1, _table, DIGITS, 1);    //state [1] --- DIGITS ---> [1]
    mark_cells(1, _table, '.', '.', 2);  //state [1] --- '.' ------> [2] 
    mark_cells(2, _table, DIGITS, 3);    //state [2] --- DIGITS ---> [3]
    mark_cells(3, _table, DIGITS, 3);    //state [3] --- DIGITS ---> [3]
    mark_cells(4, _table, DIGITS, 3);    //state [4] --- DIGITS ---> [3]
    // mark_cells(9, _table, DIGITS, 9);

    //Alpha:
    mark_cells(0, _table, ALFA, 5);    //state [0] --- ALFA ---> [5]
    mark_cells(5, _table, ALFA, 5);    //state [5] --- ALFA ---> [5]

    //Quotation
    mark_cells(0, _table, QUOTATION, 9);    //state [0] --- ALFA ---> [5]
    mark_cells(9, _table, ALFA, 9);    //state [5] --- ALFA ---> [5]
    mark_cells(9, _table, SPACES, 9);    //state [5] --- ALFA ---> [5]
    mark_cells(9, _table, QUOTATION, 10);    //state [0] --- ALFA ---> [5]
    

    //Space:
    mark_cells(0, _table, SPACES, 7);    //state [0] --- SPACE ---> [7]
    mark_cells(7, _table, SPACES, 7);    //state [7] --- SPACE ---> [7]

    //Operator
    mark_cells(0, _table, OPERATORS, 8);    //state [0] --- SPACE ---> [8]
    mark_cells(8, _table, OPERATORS, 8);    //state [0] --- SPACE ---> [8]

}

void STokenizer::make_table_enum(int _table[][MAX_COLUMNS]){
    init_table(_table);

    //doubles:
    mark_fail(_table, 0);    
    mark_fail(_table, 1);  
    mark_fail(_table, 2);  
    mark_fail(_table, 3);  
    mark_fail(_table, 4);  
    mark_fail(_table, 6);  
    mark_fail(_table, 7);  
    mark_fail(_table, 8);  
    mark_fail(_table, 9);  
    mark_fail(_table, 11);  
    mark_fail(_table, 15);  
    mark_fail(_table, 13);  
    mark_fail(_table, 14);  
    mark_fail(_table, 17);



    mark_success(_table, 5);
    mark_success(_table, 18);
    mark_success(_table, 10);
    mark_success(_table, 12);
    mark_success(_table, 16);


    mark_cells(0, _table, SELECT, 9);
    mark_cells(9, _table, SYM, 13);
    mark_cells(13, _table, SYM, 13);
    mark_cells(13, _table, FROM, 14);
    mark_cells(14, _table, SYM, 12);
    mark_cells(9, _table, STARR, 10);
    mark_cells(10, _table, FROM, 11);
    mark_cells(11, _table, SYM, 12);
    mark_cells(12, _table, WHERE, 15);
    mark_cells(15, _table, SYM, 16);
    mark_cells(16, _table, SYM, 16);


    mark_cells(0, _table, INSERT, 6);
    mark_cells(6, _table, INTO, 7);
    mark_cells(7, _table, SYM, 8);
    mark_cells(8, _table, VALUES, 17);
    mark_cells(17, _table, SYM, 18);
    mark_cells(18, _table, SYM, 18);


    mark_cells(0, _table, MAKE, 1);
    mark_cells(1, _table, TABLE, 2);
    mark_cells(2, _table, SYM, 3);
    mark_cells(3, _table, FIELDS, 4);
    mark_cells(4, _table, SYM, 5);
    mark_cells(5, _table, SYM, 5);
}

// t needs to be changed to token
bool STokenizer::get_token1(string input, int& pos, int &state, int table[][MAX_COLUMNS], string &t){
    t = ""; 
    bool thereIs = true;
    int successStae = table[state][0];
    int oneAfyerSuccessPos = -1;



    if(table[state][input[pos]] == -1){
        return false;
    }
    int i = pos;
    do{
        //-------------------CHECK FOR SPECIAL CHARACTERS(SPANISH)-------------------
        if(!contains(ALFA, input[pos]) && !contains(DIGITS, input[pos]) && !contains(OPERATORS, input[pos]) && !contains(SPACES, input[pos]) && !contains(PUNC, input[pos])&& !contains(STAR, input[pos])&& !contains(PARENTHSIS, input[pos])&& !contains(QUOTATION, input[pos])){
            t = input[pos];
            // i++;
            // oneAfyerSuccessPos = pos+1;

            // pos = oneAfyerSuccessPos+1;
            pos += 1;
            return true;
            // return false;
        }
        state = table[state][input[i]];
        successStae = table[state][0];

        if(oneAfyerSuccessPos == -1 && successStae == 1){
            for(int a = pos; a <= i; a ++){
                t += input[a];
            }
            oneAfyerSuccessPos = i + 1;
        }
        else if(successStae == 1){
            for(int a = oneAfyerSuccessPos; a <= i; a ++){
                t += input[a];
            }
            oneAfyerSuccessPos = i + 1;
        }

        i++;
        // std::cout << "State" << state << std::endl;
        // std::cout << "table[state][input[i]]" << table[state][input[i]] << std::endl;

        if(table[state][input[i]] == -1){
            break;
        }
    }while(i < input.length());


    if(oneAfyerSuccessPos == -1){
        thereIs = false;
    }

    // std::cout << "Token: " << t << std::endl;
    // std::cout << "Position: " << oneAfyerSuccessPos << std::endl;
    pos = oneAfyerSuccessPos;
    return thereIs;
}

bool STokenizer::get_token(int& start_state, string& token){
    // std::string a = "";
    // for(int i = 0; i < strlen(_buffer); i++){
    //     a += _buffer[i];
    // }
    //string input, int pos, int state, int table[][128], string &t
    return get_token1(_buffer, _pos, start_state, _table, token);
}

