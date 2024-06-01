#include "token.h"

Token::Token(): _token(""), _type(0) {}
Token::Token(string str, int type){
    _token = str;
    _type = type;
}
ostream& operator <<(ostream& outs, const Token& t){
    outs << t._token;
    return outs;
}
int Token::type() const{
    return _type;
}

string Token::type_string() const{
    switch(_type){
        case TOKEN_SPACE: return "SPACE";
        break;
        case TOKEN_NUMBER: return "NUMBER";
        break;
        case TOKEN_ALPHA: return "ALPHA";
        break;
        case TOKEN_OPERATOR: return "OPERATOR";
        break;
        case TOKEN_PUNC: return "PUNCTUATION";
        break;
        case TOKEN: return "TOKEN";
        break;
        case LPAREN: return "LPAREN";
        break;
        case RPAREN: return "RPAREN";
        break;
        case TOKEN_STR: return "TOKEN_STR";
        break;
        case RELATIONAL: return "RELATIONAL";
        break;
        case LOGICAL: return "LOGICAL";
        break;
        case RESULT_SET: return "RESULT_SET";
        break;
        default: return "UNKNOWN";
        break;
    }

}

void Token::setType(int type){
    _type = type;
}

void Token::setToken(string str){
    _token = str;
}


string Token::token_str() const{
    return _token;
}