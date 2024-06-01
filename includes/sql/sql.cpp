#ifndef SQL_CPP
#define SQL_CPP

#include "sql.h"



SQL::SQL(){

}
Table SQL::command(string string){
    Parser parser(string);
    MMap<std::string, std::string> ptree = parser.parse_tree();
    if(ptree["command"][0] == "make")
    {
        Table table(ptree["table"][0], ptree["fields"]);
        return table;
    }
    if(ptree["command"][0] == "insert")
    {
        Table table(ptree["table"][0]);
        table.insert_into(ptree["condition"]);
        return table;
    }
    if(ptree["command"][0] == "select")
    {
        Table table(ptree["table"][0]);
        if(ptree["fields"][0] == "*"){
            if(ptree.contains("condition")){
                // std::cout << "ptree[condition]" << ptree["condition"];
                // table.select(table.get_field_names(), ptree["condition"]);
                // _recnos_selected = table.select_recnos();
                // std::cout << "CNCCNCNNCNCNCN";
                return table.select(table.get_field_names(), ptree["condition"]);
            }else{
                // table.select_all();
                // _recnos_selected = table.select_recnos();
                return table.select_all();
            }
        }else{
            if(ptree.contains("condition")){
                // table.select(ptree["fields"], ptree["condition"]);
                // std::cout << table.select_recnos() << endl;
                // _recnos_selected = table.select_recnos();
                return table.select(ptree["fields"], ptree["condition"]);
            }else{
                //this this the case where you just reduce the num of the fields, need to work on
                // Queue<Token*> temp;
                // table.select(ptree["fields"], temp);
                // std::cout << table.select_recnos() << endl;
                // _recnos_selected = table.select_recnos();
                // return table.select(ptree["fields"], temp);
                return table.select_all();
            }
        }
    }

}

vector<long> SQL::select_recnos(){
    return _recnos_selected;
}



#endif