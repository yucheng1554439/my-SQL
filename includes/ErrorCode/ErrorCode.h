#ifndef ERRORCODE_H
#define ERRORCODE_H

#include <iostream>
#include "../table/typedefs.h"


struct ErrorCode{

    static std::string getCodeString(Code code){
    switch(code){
        case missing_from: return "Error: (Missing From)\n\n";
        break;
        case missing_table_name: return "Error: (Missing Table Name)\n\n";
        break;
        case missing_codition: return "Error: (Missing Condition)\n\n";
        break;
        case command_syntax_error: return "Error: (Syntax Error)\n\n";
        break;
        case invalid_condition: return "Error: (Invalid Condition)\n\n";
        break;
        case missing_left_parenthesis: return "Error: (Missing Left Parenthesis)\n\n";
        break;
        case missing_right_parenthesis: return "Error: (Missing Right Parenthesis)\n\n";
        break;
        case missing_comma: return "Error: (Missing Comma)\n\n";
        break;
        case invalid_field_name: return "Error: (Invalid Field Names)\n\n";
        break;
        case missing_relational_operator_compare: return "Error: (Missing one side of the relational operator)\n\n";
        break;
        case missing_logical_operator_compare: return "Error: (Missing one side of the logical operator)\n\n";
        break;
        default: return "Error: Invalid Command\n\n";
        break;
    }
}

};




#endif