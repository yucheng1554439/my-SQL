#ifndef TOKEN_H
#define TOKEN_H
#include <iostream>
#include "constants.h"
#include "../bplustree/map.h" 
#include "../bplustree/multimap.h"
enum keys{ZERO, SELECT, FROM, WHERE, INSERT, MAKE, INTO, TABLE, FIELDS, CONDITION, SYM, VALUES, STARR};

using namespace std;
class Token
{
    public:
        Token();
        Token(string str, int type);
        friend ostream& operator <<(ostream& outs, const Token& t);
        int type() const;
        virtual vector<long> evaluate() const{
            vector<long> temp;
            return temp;
        }
        virtual vector<long> evaluate(const Token *lhs, const Token *rhs){
            vector<long> temp;
            return temp;
        }
        virtual vector<long> evaluate(const Token *lhs, const Token *rhs, vector<MMap<string, long> > _indices_recno,  vector<string> fieldNames, const fstream &file){
            vector<long> temp;
            return temp;
        }
        virtual std::string getString() const{
            return "CNM";
        }
        string type_string() const;
        void setToken(string str);
        void setType(int type);
        string token_str() const;
    private:
        string _token;
        int _type;
};

#endif