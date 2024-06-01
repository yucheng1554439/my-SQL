#ifndef CONSTANTS_H
#define CONSTANTS_H


const int MAX_COLUMNS = 256;
const int MAX_ROWS = 100;
const int MAX_BUFFER = 200;

const char ALFA[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
const char DIGITS[] = "0123456789";
const char OPERATORS[] = "><=!+-%&|";
const char SPACES[] = {' ', '\t', '\n', '\0'};
const char PUNC[] = "?,.:;'`~!";
const char PARENTHSIS[] = "()";
const char QUOTATION[] = "\"";
const char STAR[] = "*";

const int START_DOUBLE = 0;
const int START_SPACES = 4;
const int START_ALPHA = 6;
const int START_OPERATOR = 20;
const int START_PUNC = 10;

//token types:
const int TOKEN_NUMBER = 1;
const int TOKEN_ALPHA = 2;
const int TOKEN_SPACE = 3;
const int TOKEN_OPERATOR = 4;
const int TOKEN_PUNC = 5;
const int TOKEN = 6;
const int LPAREN = 7;
const int RPAREN = 8;
const int TOKEN_STR = 9;
const int OPERATOR = 10;
const int RELATIONAL = 11;
const int LOGICAL = 12;
const int RESULT_SET = 13;




const int TOKEN_UNKNOWN = -1;
const int TOKEN_END = -2;

#endif