#ifndef PARSER_CPP
#define PARSER_CPP

#include "parser.h"



Parser::Parser(){
}
Parser::Parser(string string){
    //set the member buffer to the input

    _token_holder.clear();
    _token_holder_enum_type.clear();
    _buffer = string;
    //make the table for enum
    _table_for_enum.make_adjacency_table();
    //building the keyword list
    keyword.insert("SELECT", SELECT);
    keyword.insert("FROM", FROM);
    keyword.insert("WHERE", WHERE);
    keyword.insert("INSERT", INSERT);
    keyword.insert("MAKE", MAKE);
    keyword.insert("INTO", INTO);
    keyword.insert("TABLE", TABLE);
    keyword.insert("FIELDS", FIELDS);
    keyword.insert("VALUES", VALUES);
    keyword.insert("CONDITION", CONDITION);
    keyword.insert("SYM", SYM);
    keyword.insert("*", STARR);
    


    //tokenize the input into our vector string _token_holder
    set_string(string);

}
mmap_ss Parser::parse_tree(){
    MMap<string,string> temp;
    if(ptree["command"][0] == "select"){
        temp.insert("command", "select");
        if(ptree.contains("condition")){
            for(int i = 0; i < ptree["condition"].size(); i++){
                temp.insert("condition", ptree["condition"][i]);
            }
        }
        for(int i = 0; i < ptree["fields"].size(); i++){
            temp.insert("fields", ptree["fields"][i]);
        }
        for(int i = 0; i < ptree["table_name"].size(); i++){
            temp.insert("table_name", ptree["table_name"][i]);
        }
        if(ptree.contains("where")){
            temp.insert("where", ptree["where"][0]);
        }        
    }else if(ptree["command"][0] == "insert"){
        temp.insert("command", ptree["command"][0]);
        for(int i = 0; i < ptree["table_name"].size(); i++){
            temp.insert("table_name", ptree["table_name"][i]);
        }
        for(int i = 0; i < ptree["values"].size(); i++){
            temp.insert("values", ptree["values"][i]);
        }
    }else if(ptree["command"][0] == "make"){
        for(int i = 0; i < ptree["col"].size(); i++){
            temp.insert("col", ptree["col"][i]);
        }
        temp.insert("command", ptree["command"][0]); //
        for(int i = 0; i < ptree["table_name"].size(); i++){
            temp.insert("table_name", ptree["table_name"][i]);
        }
    }
    return temp;
}
void Parser::set_string(string string){
    char charArr[string.length()+1];
	strcpy(charArr, string.c_str());
    
    STokenizer stringTokenizer(charArr);
    _token_holder.clear();

    Token t;
    stringTokenizer>>t;
    _token_holder.push_back(t);
    //get the token into the vector
    while(stringTokenizer.more()){
        stringTokenizer>>t;
        _token_holder.push_back(t);
    }
    
    vector<Token> temp;
    temp.clear();
    //filter thru the raw vector
    int vecLength = _token_holder.size();
    for(int i = 0; i < vecLength; i++){
        if(_token_holder[i].token_str() != "," && _token_holder[i].token_str() != " "){
            temp.push_back(_token_holder[i]);
            // std::cout << _token_holder[i] <<endl;
        }
    }
    _token_holder.clear();
    _token_holder = temp;



    // std::cout << _token_holder << endl;

    // ptree
    ptree.clear();
    get_parse_tree();
    // cout << ptree;
}

keys Parser::get_column(Token token){
    if(token.token_str()=="*"){
        return STARR;
    }
    // if(token.token_str()=="."){
    //     return SYM;
    // }

    Pair<string, keys> temp(toupper(token.token_str()));
    if(keyword.contains(temp)){
        return keyword[toupper(token.token_str())];
    }
    return SYM;
}
bool Parser::get_parse_tree(){
    //creating the structure of the ptreee
    std::string x = "command";
    MPair<std::string, std::string > tem12p(x);
    ptree.insert(tem12p);
    x = "table_name";
    MPair<std::string, std::string > te2p(x);
    ptree.insert(te2p);
    x = "fields";
    MPair<std::string, std::string > t2p(x);
    ptree.insert(t2p);
    x = "condition";
    MPair<std::string, std::string > tp(x);
    ptree.insert(tp);
    
    int vecLength = _token_holder.size()-1;

    _table_for_enum.make_adjacency_table();
    int state = 0;
    for(int i = 0; i < vecLength; i++){

        Token token = _token_holder[i];
        std::string yes = "yes";
        state = _table_for_enum.get(state, get_column(token));
        // std::cout << get_column(token) << endl;

        switch(state)
        {
        case 1:
            ptree["command"] += token.token_str();
            break;
        case 3:
            ptree["table_name"] += token.token_str();
            break;
        case 5:
            ptree["col"] += token.token_str();
            break;
        case 6:
            ptree["command"] += token.token_str();
            break;
        case 8:
            ptree["table_name"] += token.token_str();
            break;
        case 9:
            ptree["command"] += token.token_str();
            break;
        case 10:
            ptree["fields"] += token.token_str();
            break;
        case 12:
            ptree["table_name"] += token.token_str();
            break;
        case 13:
            ptree["fields"] += token.token_str();
            break;
        case 15:
            ptree["where"] += yes;
            break;
        case 16:
            if(token.token_str()[0] == '\"'){
                ptree["condition"] += token.token_str().substr(1, token.token_str().length()-2);
            }else{ptree["condition"] += token.token_str();}
            break;
        case 18:
            if(token.token_str()[0] == '\"'){
                ptree["values"] += token.token_str().substr(1, token.token_str().length()-2);
            }else{ptree["values"] += token.token_str();}
            break;
        default:
            break;
        }


        
    }
    // if(state == 1){
    //     return true;
    // }else{
    //     ptree.clear();
    //     return false;
    // }



}
string Parser::toupper(string string){
    transform(string.begin(), string.end(), string.begin(), ::toupper); 
    return string;
}






#endif