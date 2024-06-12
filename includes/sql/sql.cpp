#ifndef SQL_CPP
#define SQL_CPP

#include "sql.h"



SQL::SQL(){
    _valid_String = false;
    _recnos_selected.clear();
}

void SQL::run(){
    std::cout << "\n\n---------------- SQL Running ----------------\n\n";
    std::string commandString;
    std::cout << "Input your command.(\"exit\" to quit)\n";
    std::cout << ">";
    std::getline(std::cin, commandString);
    while((toupper(commandString) != "EXIT") && (tolower(commandString) != "quit")){
        
        // std::cout << "COMMAND: " <<commandString <<endl;
        Table table = command(commandString);
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

string SQL::toupper(string string){
    transform(string.begin(), string.end(), string.begin(), ::toupper); 
    return string;
}
string SQL::tolower(string string){
    transform(string.begin(), string.end(), string.begin(), ::tolower); 
    return string;
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
        // std::cout << "ptree[values].size()" << ptree["values"].size() << endl;

        // std::cout << "\nptreee" << ptree << "\n\n";
        if(ptree["command"][0] == "make")
        {
            Table table(ptree["table_name"][0], ptree["col"]);
            return table;
        }
        if(ptree["command"][0] == "insert")
        {
            // std::cout << "WCNM";
            Table table(ptree["table_name"][0]);
            table.insert_into(ptree["values"]);

            return table;
        }
        if(ptree["command"][0] == "select")
        {

            std::string tableName = ptree["table_name"][0];
            Table table(tableName);

            if(ptree["fields"][0] == "*"){
                if(ptree.contains("where")){
                    // std::cout << "\nptree[condition]" << ptree["condition"] << endl;
                    // table.select(table.get_field_names(), ptree["condition"]);
                    // cout<<"_recnos_selected: "<<_recnos_selected<<endl;
                    // std::cout << "FIRST PARE: " << table.get_field_names() << endl;
                    // std::cout << "SECOND PARE: " << ptree["condition"] << endl;


                    Table temp = table.select(table.get_field_names(), ptree["condition"]);
                    _recnos_selected = table.select_recnos();

                    // cout<<"--------Printing directly from select"<<endl;
                    // cout<<"fielddd_names.size(): "<<fielddd_names.size()<<endl;
                    // cout<< table.select(fielddd_names, ptree_condition);
                    // cout<<"-----------temp:\n" <<temp<<endl;
                    
                    return temp;
                }else{

                    Table temp = table.select_all();
                    _recnos_selected = table.select_recnos();

                    return temp;
                }
            }else{
                if(ptree.contains("where")){
                    // table.select(ptree["fields"], ptree["condition"]);
                    // std::cout << table.select_recnos() << endl;

                    Table temp = table.select(ptree["fields"], ptree["condition"]);
                    _recnos_selected = table.select_recnos();

                    return temp;
                }else{
                    //this this the case where you just reduce the num of the fields, need to work on
                    // Queue<Token*> temp;
                    // table.select(ptree["fields"], temp);
                    // std::cout << table.select_recnos() << endl;
                    Table temp = table.select(ptree["fields"]);

                    _recnos_selected = table.select_recnos();
                    // return table.select(ptree["fields"], temp);

                    return temp;
                }
            }
        }
    }
}

vector<long> SQL::select_recnos(){
    return _recnos_selected;
}



#endif




