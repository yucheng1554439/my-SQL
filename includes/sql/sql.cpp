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
        // if(_valid_String){
            std::cout << table;
            if(select_recnos().size() != 0){
                std::cout << "records selected: "<< select_recnos() << endl;
            }
        // }else if(!_valid_String){
        //     std::cout << "Invalid String\n";
        // }
        std::cout << "Input your command.(\"exit\" to quit)\n";
        std::cout << ">";
        std::getline(std::cin, commandString);
    };

    std::cout << "\n\n------------------ SQL End ------------------\n\n";
}

void SQL::batch(){
    const vector<string> command_list = {
"select last, first, major from student where ((lname=Yang or major=CS) and age<23 )or lname=Jackson",
// 2. Expected comma:
"select last first, major from student where ((lname=Yang or major=CS) and age<23 )or lname=Jackson",
// 3. Expected: Missing field name
"select last, , major from student where ((lname=Yang or major=CS) and age<23 )or lname=Jackson",
// 4. Expected from:
"select last, first, major  student where ((lname=Yang or major=CS) and age<23 )or lname=Jackson",
// 5. Expected table name:
"select last, first, major from  where ((lname=Yang or major=CS) and age<23 )or lname=Jackson",
// 6. Expected condition:
"select last, first, major from student where ",
// 7. Missing left paren:
"select last, first, major from student where (lname=Yang or major=CS) and age<23 )or lname=Jackson",
// 8. Missing right paren:
"select last, first, major from student where ((lname=Yang or major=CS and age<23 )or lname=Jackson",
// 9. :
"select last, first, major from student where ((lname= or major=CS) and age<23 )or lname=Jackson",
// 10. :
"select last, first, major from student where ((lname=Yang or major=CS) and age<23 )or lname=Jackson",
// 11. :
"select last, first, major from student where ((lname=Yang  major=CS) and age<23 )or lname=Jackson",
// 12. :
"select last, first, major from student where ((lname=Yang or ) and age<23 )or lname=Jackson",
// 13. :
"select last, first, major from student where ((lname=Yang or major=CS) and age<23 )or ",
// 14. :
"select last, first, major from student where ((lname=Yang or major=CS) and age<23 )or lname=Jackson"
};
    const int MAKE_TABLE_COMMANDS = 10;
    const int SELECT_COMMANDS = 14;

    for (int i = 0; i < MAKE_TABLE_COMMANDS; i++)
     {
          cout << ">" << command_list[i] << endl;
          command(command_list[i]);
     }

     cout << endl
          << endl;
     for (int i = MAKE_TABLE_COMMANDS; i < SELECT_COMMANDS; i++)
     {
          cout << "\n>" << command_list[i] << endl;
               cout<< command(command_list[i])<<endl;
          cout << "basic_test: records selected: "<< select_recnos() << endl;
     }



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
















// // // new version
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
//             std::cout << "CNM";
//             Table temp(tableName);
//             _table_map[tableName] = temp;
//         }
//         // file.close();
//     } //else{
//         // ofstream f;
//         // f.open((_table_name_list + ".txt").c_str());
//         // f.close();
//         file.close();
//     // }

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

//     while((toupper(commandString) != "EXIT") && (tolower(commandString) != "quit") && (tolower(commandString) != "clear")){
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
//         std::cout << "Input your command.(\"exit\" to quit, clear to delete all table)\n";
//         std::cout << ">";
//         std::getline(std::cin, commandString);
//     };
//     if(tolower(commandString) == "clear"){
//         clearTableNameTxtFile();
//     }

//     std::cout << "\n\n------------------ SQL End ------------------\n\n";
// }

// void SQL::batch(){
//     const vector<string> command_list = {
// "select last, first, major from student where ((lname=Yang or major=CS) and age<23 )or lname=Jackson",
// // 2. Expected comma:
// "select last first, major from student where ((lname=Yang or major=CS) and age<23 )or lname=Jackson",
// // 3. Expected: Missing field name
// "select last, , major from student where ((lname=Yang or major=CS) and age<23 )or lname=Jackson",
// // 4. Expected from:
// "select last, first, major  student where ((lname=Yang or major=CS) and age<23 )or lname=Jackson",
// // 5. Expected table name:
// "select last, first, major from  where ((lname=Yang or major=CS) and age<23 )or lname=Jackson",
// // 6. Expected condition:
// "select last, first, major from student where ",
// // 7. Missing left paren:
// "select last, first, major from student where (lname=Yang or major=CS) and age<23 )or lname=Jackson",
// // 8. Missing right paren:
// "select last, first, major from student where ((lname=Yang or major=CS and age<23 )or lname=Jackson",
// // 9. :
// "select last, first, major from student where ((lname= or major=CS) and age<23 )or lname=Jackson",
// // 10. :
// "select last, first, major from student where ((lname=Yang or major=CS) and age<23 )or lname=Jackson",
// // 11. :
// "select last, first, major from student where ((lname=Yang  major=CS) and age<23 )or lname=Jackson",
// // 12. :
// "select last, first, major from student where ((lname=Yang or ) and age<23 )or lname=Jackson",
// // 13. :
// "select last, first, major from student where ((lname=Yang or major=CS) and age<23 )or ",
// // 14. :
// "select last, first, major from student where ((lname=Yang or major=CS) and age<23 )or lname=Jackson"
// };
//     const int MAKE_TABLE_COMMANDS = 0;
//     const int SELECT_COMMANDS = 13;

//     for (int i = 0; i < MAKE_TABLE_COMMANDS; i++)
//      {
//           cout << ">" << command_list[i] << endl;
//           command(command_list[i]);
//      }

//      cout << endl
//           << endl;
//      for (int i = MAKE_TABLE_COMMANDS; i < SELECT_COMMANDS; i++)
//      {
//           cout << "\n>" << command_list[i] << endl;
//                cout<< command(command_list[i])<<endl;
//           cout << "basic_test: records selected: "<< select_recnos() << endl;
//      }
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
//                 // fstream file;
//                 // vectorstr tablenames;
//                 // std::string tableName;

//                 //this mighr be wrong
//                 // file.open((_table_name_list + ".txt").c_str());
//                 //geting the table names from the txt file
//                 // while(getline(file, tableName)){
//                 //     tablenames.push_back(tableName);
//                 // }
//                 // file.close();

//                 // tablenames.push_back(ptree["table_name"][0]);

//                 f.open((_table_name_list + ".txt").c_str(), ios::app);
//                 //input the table names into the txt file
//                 // int size = tablenames.size();
//                 // for(int i = 0; i < size; i++){
//                  f << ptree["table_name"][0] << "\n";
//                 // }
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
//     if(file){
//         while(getline(file, tableName)){
//             std::cout << setw(20) <<tableName << endl;
//         }
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
//     file.open((_table_name_list+ ".txt").c_str(), std::ofstream::out | std::ofstream::trunc);
//     // remove(_table_name_list+ ".txt");
//     file.close();
// }


// #endif






