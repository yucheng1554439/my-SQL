#ifndef SQL_CPP
#define SQL_CPP

#include "sql.h"

SQL::SQL(){
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
    // if(!_valid_String){
    //     Table temp;
    //     std::cout << "Invalid Command\n";
    //     return temp;
    // }else{
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
            if(ptree["fields"][0] == "*"){
                if(ptree.contains("where")){
                    //select the records from the table with condition
                    Table temp = table.select(table.get_field_names(), ptree["condition"]);
                    _recnos_selected = table.select_recnos(); 
                    return temp;
                }else{
                    //select all the records from the table
                    Table temp = table.select_all();
                    _recnos_selected = table.select_recnos(); 
                    return temp;
                }
            }else{
                if(ptree.contains("where")){
                    //the chosen fields with the conditions
                    Table temp = table.select(ptree["fields"], ptree["condition"]);
                    _recnos_selected = table.select_recnos(); 
                    return temp;
                }else{
                    //only select certain fields
                    Table temp = table.select(ptree["fields"]);
                    _recnos_selected = table.select_recnos();
                    return temp;
                }
            }
        }
    }
// }

vector<long> SQL::select_recnos(){
    return _recnos_selected;
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
















// // new version
// #ifndef SQL_CPP
// #define SQL_CPP

// #include "sql.h"

// SQL::SQL(){
//     //trying to get the table from previous run
//     _table_name_list = "_table_name_list";
//     fstream file;
//     string tableName;
    
//     file.open((_table_name_list + ".txt").c_str());
//     if(file){
//         // geting the table names from the txt file
//         while(getline(file, tableName)){
//             Table temp(tableName);
//             _table_map[tableName] = temp;
//         }
//         file.close();
//     }else{
//         ofstream f;
//         f.open((_table_name_list + ".txt").c_str());
//         f.close();
//     }

//     //new version with no memory feature
//     // clearTableNameTxtFile();
// }

// void SQL::run(){
//     std::cout << "\n\n---------------- SQL Running ----------------\n\n";
//     std::string commandString;
//     std::cout << "Input your command.(\"exit\" to quit)\n";
//     std::cout << ">";
//     // std::cout << "TEST\n" << _table_map << endl;
//     std::getline(std::cin, commandString);

//     while((toupper(commandString) != "EXIT") && (tolower(commandString) != "quit")){
//         Table table = command(commandString);
//         //after parse the string, if it is valid string
//         if(_valid_String){
//             std::cout << table;
//             if(select_recnos().size() != 0){
//                 std::cout << "records selected: "<< select_recnos() << endl;
//             }
//         }
//         std::cout << "\n\n\n***** Current Existing Tables *****\n";
//         currentExistTables();
//         std::cout << "\n***********************************\n\n";
//         std::cout << "Input your command.(\"exit\" to quit)\n";
//         std::cout << ">";
//         std::getline(std::cin, commandString);
//     };

//     std::cout << "\n\n------------------ SQL End ------------------\n\n";
// }

// Table SQL::command(string string){
//     Parser parser(string);
//     _valid_String = parser.is_valid();
//     // _recnos_selected.clear();  //COMMNETED OUT
//     if(!_valid_String){
//         Table temp;
//         std::cout << "MESSAGE: (Invalid Command)\n";
//         return temp;
//     }else{
//         MMap<std::string, std::string> ptree = parser.parse_tree();
//         //----- debugging -----
//         // std::cout << "\nptreee" << ptree << "\n\n";
//         if(ptree["command"][0] == "make" || ptree["command"][0] == "create")
//         {
//             if(!_table_map.contains(ptree["table_name"][0])){
//                 ofstream f;
//                 fstream file;
//                 vectorstr tablenames;
//                 std::string tableName;

//                 file.open((_table_name_list + ".txt").c_str());
//                 //geting the table names from the txt file
//                 while(getline(file, tableName)){
//                     tablenames.push_back(tableName);
//                 }
//                 file.close();

//                 tablenames.push_back(ptree["table_name"][0]);

//                 f.open((_table_name_list + ".txt").c_str());
//                 //input the table names into the txt file
//                 int size = tablenames.size();
//                 for(int i = 0; i < size; i++){
//                     f << tablenames[i] << "\n";
//                 }
//                 f.close();
//             }else{
//                 std::cout << "MESSAGE: (OverWriting The Old Table with New Fields)\n";
//             }
//             Table table(ptree["table_name"][0], ptree["col"]);
//             _table_map[ptree["table_name"][0]] = table;
//             return _table_map[ptree["table_name"][0]];
//         }
//         if(ptree["command"][0] == "insert")
//         {
//             if(!_table_map.contains(ptree["table_name"][0])){
//                 std::cout << "MESSAGE: (Can not insert to a non-exist Table)\n";
//                 // Table temp;
//                 // return temp;
//             }else{
//                 _table_map[ptree["table_name"][0]].insert_into(ptree["values"]);
                
//             }
//             return _table_map[ptree["table_name"][0]];
            
            
//         }
//         if(ptree["command"][0] == "select")
//         {
//             std::string tableName = ptree["table_name"][0];
//             if(ptree["fields"][0] == "*"){
//                 if(ptree.contains("where")){
//                     //select the records from the table with condition
//                     Table temp = _table_map[tableName].select(_table_map[tableName].get_field_names(), ptree["condition"]);
//                     _recnos_selected = _table_map[tableName].select_recnos(); 
//                     return temp;
//                 }else{
//                     //select all the records from the table
//                     Table temp = _table_map[tableName].select_all();
//                     _recnos_selected = _table_map[tableName].select_recnos(); 
//                     return temp;
//                 }
//             }else{
//                 if(ptree.contains("where")){
//                     //the chosen fields with the conditions
//                     Table temp = _table_map[tableName].select(ptree["fields"], ptree["condition"]);
//                     _recnos_selected = _table_map[tableName].select_recnos(); 
//                     return temp;
//                 }else{
//                     //only select certain fields
//                     Table temp = _table_map[tableName].select(ptree["fields"]);
//                     _recnos_selected = _table_map[tableName].select_recnos();
//                     return temp;
//                 }
//             }
//         }
//     }
// }

// vector<long> SQL::select_recnos(){
//     return _recnos_selected;
// }

// //print the names of all existing table
// void SQL::currentExistTables(){
//     string tableName;
//     fstream file;
//     file.open((_table_name_list + ".txt").c_str());
//     //geting the field names from the txt file
//     std::cout << endl;
//     while(getline(file, tableName)){
//         std::cout << setw(20) <<tableName << endl;
//     }
//     file.close();
// }


// string SQL::toupper(string string){
//     transform(string.begin(), string.end(), string.begin(), ::toupper); 
//     return string;
// }
// string SQL::tolower(string string){
//     transform(string.begin(), string.end(), string.begin(), ::tolower); 
//     return string;
// }

// //feature when user want to clear the txt file with table and also delete previous tables
// void SQL::clearTableNameTxtFile()
// {
//     fstream file;
//     file.open(_table_name_list+ ".txt", std::ofstream::out | std::ofstream::trunc);
//     file.close();
// }


// #endif






