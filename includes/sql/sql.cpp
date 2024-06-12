#ifndef SQL_CPP
#define SQL_CPP

#include "sql.h"

SQL::SQL(){
    // _valid_String = false;
    // _recnos_selected.clear();
}

void SQL::run(){
    std::cout << "\n\n---------------- SQL Running ----------------\n\n";
    std::string commandString;
    std::cout << "Input your command.(\"exit\" to quit)\n";
    std::cout << ">";
    std::getline(std::cin, commandString);

    while((toupper(commandString) != "EXIT") && (tolower(commandString) != "quit")){
        Table table = command(commandString);
        //after parse the string, if it is valid string
        if(_valid_String){
            std::cout << table;
            if(select_recnos().size() != 0){
                std::cout << "records selected: "<< select_recnos() << endl;
            }
        }
        std::cout << "Input your command.(\"exit\" to quit)\n";
        std::cout << ">";
        std::getline(std::cin, commandString);
    };

    std::cout << "\n\n------------------ SQL End ------------------\n\n";
}

Table SQL::command(string string){
    Parser parser(string);
    _valid_String = parser.is_valid();
    // _recnos_selected.clear();  //COMMNETED OUT
    if(!_valid_String){
        Table temp;
        std::cout << "Invalid Command\n";
        return temp;
    }else{
        MMap<std::string, std::string> ptree = parser.parse_tree();
        //----- debugging -----
        // std::cout << "\nptreee" << ptree << "\n\n";
        if(ptree["command"][0] == "make" || ptree["command"][0] == "create")
        {
            Table table(ptree["table_name"][0], ptree["col"]);
            return table;
        }
        if(ptree["command"][0] == "insert")
        {
            Table table(ptree["table_name"][0]);
            table.insert_into(ptree["values"]);

            return table;
        }
        if(ptree["command"][0] == "select")
        {

            std::string tableName = ptree["table_name"][0];
            Table table(tableName);
            _table = table; //new added
            if(ptree["fields"][0] == "*"){
                if(ptree.contains("where")){
                    //select the records from the table with condition
                    Table temp = table.select(table.get_field_names(), ptree["condition"]);
                    // _recnos_selected = table.select_recnos(); //old version
                    _table.set_selected_recnos(table.select_recnos());
                    return temp;
                }else{
                    //select all the records from the table
                    Table temp = table.select_all();
                    // _recnos_selected = table.select_recnos(); //old version
                    _table.set_selected_recnos(table.select_recnos());
                    return temp;
                }
            }else{
                if(ptree.contains("where")){
                    //the chosen fields with the conditions
                    Table temp = table.select(ptree["fields"], ptree["condition"]);
                    // _recnos_selected = table.select_recnos(); //old version
                    _table.set_selected_recnos(table.select_recnos());
                    return temp;
                }else{
                    //only select certain fields
                    Table temp = table.select(ptree["fields"]);
                    // _recnos_selected = table.select_recnos(); //old version
                    _table.set_selected_recnos(table.select_recnos());
                    return temp;
                }
            }
        }
    }
}

vector<long> SQL::select_recnos(){
    return _table.select_recnos();
}

string SQL::toupper(string string){
    transform(string.begin(), string.end(), string.begin(), ::toupper); 
    return string;
}
string SQL::tolower(string string){
    transform(string.begin(), string.end(), string.begin(), ::tolower); 
    return string;
}



#endif




