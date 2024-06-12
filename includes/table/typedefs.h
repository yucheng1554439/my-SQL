#ifndef TYPEDEFS_H
#define TYPEDEFS_H

#include "../bplustree/map.h" 
#include "../bplustree/multimap.h"
#include "../queue/MyQueue.h"
#include "../stack/MyStack.h"
#include "../tokenizer/token.h"
#include <iostream>
#include <algorithm>
#include <string>
// #include "table.h"
#include <vector>
#include <queue>
using namespace std; 
typedef Map<string, string> map_ss;
typedef MMap<string, string> mmap_ss; 
typedef Map<string, long> map_sl;
typedef MMap<string, long> mmap_sl; 
typedef vector<string> vectorstr; 
typedef vector<long> vectorlong;
typedef Queue<string> queuestr;

// enum TokenType
// { TOKEN, //6
// LPAREN, //7
// RPAREN, //8
// TOKEN_STR, //9
// OPERATOR, //10
// RELATIONAL, //11
// LOGICAL, //12
// RESULT_SET, //13
// // TOKEN_NUMBER, 
// // TOKEN_ALPHA, 
// // TOKEN_SPACE, 
// // TOKEN_OPERATOR,
// // TOKEN_PUNC,
// // TOKEN_UNKNOWN,
// // TOKEN_END
// };

// enum PRECEDENCE

class TokenStr:public Token{
    public:
        TokenStr(std::string input){
            _string = input;
            setType(9);
        }
        virtual std::string getString() const{
            return _string;
        }
        
    private:
        std::string _string;
};

class LParen:public Token{
    public:
    LParen(std::string input){
        setType(7);
        _string = input;
    }
    std::string type_string(){
        return "LPAREN";
    }
    virtual std::string getString() const{
        return _string;
    }
    private:
        std::string _string;

};

class RParen:public Token{
    public:
    RParen(std::string input){
        setType(8);
        _string = input;
    }
    virtual std::string getString() const{
        return _string;
    }
    private:
        std::string _string;
};

class ResultSet:public Token{
    public:
    ResultSet(vector<long> input){
        _result_vecno = input;
        setType(13);
    }
    virtual vector<long> evaluate() const{
        return _result_vecno;
    }
    private: 
        vector<long> _result_vecno;
};

class Operator:public Token{
    public:
    Operator(){}
    Operator(std::string input){
        _string_oporater = input;
        setType(10);
    }
    virtual std::string getString() const{
        return _string_oporater;
    }
    private:
        std::string _string_oporater;
};

class Relational:public Operator{
    public:
    Relational(){}
    Relational(std::string input){
        setType(11);
        _string_relational = input;
    }
    virtual std::string getString() const{
        return _string_relational;
    }
    virtual vector<long> evaluate(const Token *lhs, const Token *rhs, vector<MMap<string, long> > _indices_recno, vector<string> fieldNames, int index){
        // std::cout << "RATION CALLED EVAL";
        string lhsString = lhs->getString();
        string rhsString = rhs->getString();
        vector<long> recVectr;
        recVectr.clear();
        int numOfField = fieldNames.size();
            if(_string_relational == "="){
                if(_indices_recno[index].contains(rhsString)){

                    //old versions
                    // recVectr = _indices_recno[index].get(rhsString);
                    // recVectr.insert(recVectr.end(), _indices_recno[index].get(rhsString).begin(), _indices_recno[index].get(rhsString).end());
                    
                    //get the pointer to that node and push back the valuelist at that node
                    MMap<string, long>::Iterator itertrLower = _indices_recno[index].lower_bound(rhsString);
                    for(int i = 0; i < ((*itertrLower).value_list).size(); i++){
                        //get the value list
                        recVectr.push_back(((*itertrLower).value_list)[i]);
                    }
                }
            }else if(_string_relational == "<"){
                        
                        MMap<string, long>::Iterator itertrLower = _indices_recno[index].lower_bound(rhsString);
                        // //loop through all the valuelist
                    //    if(_indices_recno[index].begin() == itertrLower){

                        // std::cout << "_indices_recno[index].begin()" << *_indices_recno[index].begin() << endl;
                        // std::cout << "itertrLower" << *itertrLower << endl;
                        for(MMap<string, long>::Iterator temp = _indices_recno[index].begin(); temp != itertrLower; temp++){
                            //push each other record number in the valuelist
                            for(int i = 0; i < ((*temp).value_list).size(); i++){
                                //get all the record numbers
                                recVectr.push_back(((*temp).value_list)[i]);
                            }
                        }
                    //    }
                        

            }else if(_string_relational == ">"){
                        MMap<string, long>::Iterator itertrUpper = _indices_recno[index].upper_bound(rhsString);
                        // itertrUpper++;
                        for(itertrUpper; itertrUpper != MMap<string, long>::Iterator(nullptr); itertrUpper++){
                            for(int i = 0; i < ((*itertrUpper).value_list).size(); i++){
                                //get all the record numbers
                                recVectr.push_back(((*itertrUpper).value_list)[i]);
                            }
                } 
            }else if(_string_relational == "<="){
                        MMap<string, long>::Iterator itertrLower = _indices_recno[index].upper_bound(rhsString);
                        for(MMap<string, long>::Iterator temp = _indices_recno[index].begin(); temp != itertrLower; temp++){
                            for(int i = 0; i < ((*temp).value_list).size(); i++){
                                //get all the record numbers
                                recVectr.push_back(((*temp).value_list)[i]);
                            }
                        }
            }else if(_string_relational == ">="){
                        MMap<string, long>::Iterator itertrUpper = _indices_recno[index].lower_bound(rhsString);

                        for(MMap<string, long>::Iterator walker = itertrUpper; walker != MMap<string, long>::Iterator(nullptr); walker++){
                            for(int i = 0; i < ((*walker).value_list).size(); i++){
                                //get all the record numbers
                                recVectr.push_back(((*walker).value_list)[i]);
                            }
                        }
            }
        return recVectr;
    }
    private:
        std::string _string_relational;
};

class Logical:public Operator{
    public:
    Logical(){}
    Logical(std::string input){
        setType(12);
        _string_logical = input;
    }
    virtual vector<long> evaluate(const Token *lhs, const Token *rhs){
        vector<long> lhsVector = lhs->evaluate();
        vector<long> rhsVector = rhs->evaluate();
        // std::cout << "\nRESULT Vector1: " << lhsVector << endl;
        // std::cout << "\nRESULT Vector2: " << rhsVector << endl;
        vector<long> resultVector;
        sort(lhsVector.begin(), lhsVector.end());
        sort(rhsVector.begin(), rhsVector.end());

        if(_string_logical == "and"){
            set_intersection(lhsVector.begin(), lhsVector.end(), rhsVector.begin(), rhsVector.end(), back_inserter(resultVector));

        }else if(_string_logical == "or"){
            set_union(lhsVector.begin(), lhsVector.end(), rhsVector.begin(), rhsVector.end(), back_inserter(resultVector));
            
        }
        
        return resultVector;
    }
    virtual std::string getString() const{
        return _string_logical;
    }

    private:
        std::string _string_logical;
};


#endif // TYPEDEFS H