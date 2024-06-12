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
    keyword.insert("CREATE", CREATE);


    //tokenize the input into our vector string _token_holder
    _valid = set_string(string);
}

bool Parser::is_valid(){
    return _valid;
}

mmap_ss Parser::parse_tree(){
    MMap<string,string> temp;
    //depends on different values insert into the mmap(ptree)
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
    }else if(ptree["command"][0] == "make" || ptree["command"][0] == "create"){
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
bool Parser::set_string(string string){
    char charArr[string.length()];
	strcpy(charArr, string.c_str());
    //tokenize the string
    STokenizer stringTokenizer(charArr);
    _token_holder.clear();

    Token t;
    stringTokenizer>>t;
    //if it is space, we don't include them in our token holder
    if(t.type_string()!= "SPACE")
        _token_holder.push_back(t);

    
    //get the token into the vector
    while(stringTokenizer.more()){
        // ---- debugging ----
        // cout << "\nTOken:|" << t << "|TOKENTYPE:|" << setw(25)<<t.type_string() << "|\t\n";
        stringTokenizer>>t;
        if(t.type_string()!= "SPACE")
            _token_holder.push_back(t);
    }
    
    vector<Token> temp;
    temp.clear();
    //filter thru the raw vector
    int vecLength = _token_holder.size();
    for(int i = 0; i < vecLength; i++){
        //we are going to ignore the spaces and the punctuations, only push the tokens we want
        if(_token_holder[i].type_string() != "SPACE" && _token_holder[i].type_string() != "PUNCTUATION"){
            
            temp.push_back(_token_holder[i]);
            
            //old version
            // if(_token_holder[i].token_str() == "*"){
            //     temp.push_back(_token_holder[i]);
            // }else if(tolower(_token_holder[i].token_str()) != "select" && tolower(_token_holder[i].token_str()) != "insert" && tolower(_token_holder[i].token_str()) != "make" && tolower(_token_holder[i].token_str()) != "from" && tolower(_token_holder[i].token_str()) != "where" && tolower(_token_holder[i].token_str()) != "into"&& tolower(_token_holder[i].token_str()) != "values"&& tolower(_token_holder[i].token_str()) != "fields"&& tolower(_token_holder[i].token_str()) != "table"){
            //     temp.push_back(_token_holder[i]);
            // }else{
            //     //we are puting all the command and keywords into a token
            //     Token temp1(tolower(_token_holder[i].token_str()), _token_holder[i].type());
            //     temp.push_back(temp1);
            // }
        }
    }
    _token_holder.clear();
    _token_holder = temp;

    //ptree
    ptree.clear();
    
    
    return get_parse_tree();
}

keys Parser::get_column(Token token){
    //marking the Enum of the given token
    if(token.token_str()=="*"){
        return STARR;
    }
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
    std::string yes = "yes";
    bool valid = false;

    //find the correct state from the state machine and insert into corresponded index of the ptree
    for(int i = 0; i < vecLength; i++){

        Token token = _token_holder[i];
        string string = token.token_str();
        state = _table_for_enum.get(state, get_column(token));
        valid = _table_for_enum.isSuccess(state);

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
    return valid;

}
//Get the upper case of a string
string Parser::toupper(string string){
    transform(string.begin(), string.end(), string.begin(), ::toupper); 
    return string;
}
//Get the lower case of a string
string Parser::tolower(string string){
    transform(string.begin(), string.end(), string.begin(), ::tolower); 
    return string;
}






#endif