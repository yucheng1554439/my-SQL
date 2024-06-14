#ifndef ERRORCODE_H
#define ERRORCODE_H

#include <iostream>
#include "../table/typedefs.h"


struct ErrorCode{

    static std::string getCodeString(Code code){
    switch(code){
        case missing_from: return "\nError: (Missing From)\n";
        break;
        case missing_table_name: return "\nError: (Missing Table Name)\n";
        break;
        case missing_codition: return "\nError: (Missing Condition)\n";
        break;
        case command_syntax_error: return "\nError: (Syntax Error)\n";
        break;
        case invalid_condition: return "\nError: (Invalid Condition)\n";
        break;
        case missing_left_parenthesis: return "\nError: (Missing Left Parenthesis)\n";
        break;
        case missing_right_parenthesis: return "\nError: (Missing Right Parenthesis)\n";
        break;
        case missing_comma: return "\nError: (Missing Comma)\n";
        break;
        case invalid_field_name: return "\nError: (Invalid Field Names)\n";
        break;
        default: return "\nError: Invalid Command\n";
        break;
    }
}

};




#endif