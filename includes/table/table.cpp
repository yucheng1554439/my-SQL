#ifndef TABLE_CPP
#define TABLE_CPP

#include "table.h"
#include "../binary_files/file_record.h"
#include "../binary_files/utilities.h"
using namespace std;
//serie number for new tables
int Table::sequenceNumber = 0;

Table::Table()
:numOfRec(0)
{
    field_names.clear();
    string title = "";
    _indices_recno.clear();
    index.clear();
    recnoVec.clear();
    selected_recnos.clear();
}
//constructor with one parameter where open and make a copy of the table with the table_name
Table::Table(string tableName)
:title(tableName), numOfRec(0)
{
    if(!file_exists((tableName + ".bin").c_str())){return;}
    fstream file;

    file.open((tableName + ".txt").c_str());
    if (file)
    {
        
        string fields;
        //geting the field names from the txt file
        while(getline(file, fields)){
            field_names.push_back(fields);
        }
        file.close();
        //initialize the record number map with empty MMAP
        for(int i = 0; i < field_names.size(); i++){
            MMap<string, long> temp;
            temp.clear();
            _indices_recno.push_back(temp);
        }
        //Then get the value from the binary file
        fstream file2;
        
            open_fileRW(file2, (tableName + ".bin").c_str());
            FileRecord record;
            // recordNumbers to loop through while reading thru the bin file
            long recordNumbers = 0; 
            int fieldLength = field_names.size();
            for(int i = 0; i < fieldLength; i++){
                //insert the MAP index with the field name and corresponded index number
                index.insert(field_names[i], i);
            }
            //loop thru the bin file and get the information to our table
            while(record.read(file2, recordNumbers) != 0){
                vectorstr b = record.readVector(file2, recordNumbers, fieldLength);
                long recon = recordNumbers;
                for(int j = 0; j < b.size(); j++){
                    _indices_recno[index[field_names[j]]].insert(b[j], recon);
                }
                recnoVec.push_back(recon);
                numOfRec++;
                recordNumbers++;
            }
        file2.close();
    }else{
        file.close();
        // printing the error message
        cout << "File does not exists" << endl;
        // for(int i = 0; i < field_names.size(); i++){
        //     MMap<string, long> temp;
        //     temp.clear();
        //     _indices_recno.push_back(temp);
        // }
    }
    
}

// Table::Table(const Table& a){
//     field_names = a.field_names;
//     title = a.title;
//     _indices_recno = a._indices_recno;
//     index = a.index;
//     recnoVec = a.recnoVec;
//     selected_recnos = a.selected_recnos;
//     numOfRec = a.numOfRec;
// }


Table::Table(string tableName, vectorstr fields)
:title(tableName), field_names(fields), numOfRec(0)
{
    //insert the empty Multimaps into the vector
    for(int i = 0; i < field_names.size(); i++){
        MMap<string, long> temp;
        temp.clear();
        _indices_recno.push_back(temp);
    }

    ofstream f;
    //deleting content before inputing
    f.open((tableName + ".txt").c_str());
    int length = fields.size();
    //input the field names into the txt file
    for(int i = 0; i < length; i++){
        f << fields[i] << "\n";
    }
    f.close();


    for(int i = 0; i < fields.size(); i++){
        //insert the MAP index with the field name and corresponded index number
        index.insert(fields[i], i);
    }


    fstream file;
    open_fileW(file, (title+".bin").c_str());
    file.close();
}


// Table::~Table(){
//     fstream file;
//     file.open(title.c_str());
//     file.clear();
//     file.close();
// }

void Table::insert_into(vectorstr recordString){
    if(!file_exists((title+".bin").c_str()) || field_names.empty()){return;}
    FileRecord record(recordString);
    fstream file;
    //inserting the record number into the corresponded index of _indices_recno
    open_fileRW(file, (title+".bin").c_str());
    long recon = record.write(file);
    for(int i = 0; i < recordString.size(); i++){
        _indices_recno[i].insert(recordString[i], recon);
    }
    // std::cout << "_indices_recno" << _indices_recno << endl;
    recnoVec.push_back(recon);
    numOfRec++;
    file.close();

}


Table Table::select(vectorstr fieldnames, Queue<Token*> queue_of_compar){
    //----------My main select using a postorder of input-----------
    //create a child table named after mother table
    if(!file_exists((title+".bin").c_str())){return Table();}
    Table temp(title+"_"+to_string(sequenceNumber), fieldnames);
    sequenceNumber++;
    vector<long> recVectr;
    Stack<Token*> _stack;
    // Stack<Token*> _result_stack;
    fstream file;
    bool falseInput = false;
    selected_recnos.clear(); //Commented

    //if the queue is not empty
    while(!queue_of_compar.empty()){
        //----- < <= >= > = ----- 
        if(queue_of_compar.front()->type_string() == "RELATIONAL"){
            Token* rhs = _stack.pop();
            Token* lhs = _stack.pop();
            //if the field name(lhs) is not in the table then we mark that there is false input
            if(field_names[index[lhs->getString()]] != lhs->getString()){
                falseInput = true;
                break;
            }
            //get the record vector after evaluating with that equation
            recVectr = queue_of_compar.pop()->evaluate(lhs, rhs, _indices_recno, field_names, index[lhs->getString()]); 
            ResultSet* result = new ResultSet(recVectr);
            _stack.push(result);
        //------ and or -----
        }else if(queue_of_compar.front()->type_string() == "LOGICAL"){
            Token* rhs = _stack.pop();
            Token* lhs = _stack.pop();
            //get the record vector after evaluating with that equation and push back into the stack
            recVectr = queue_of_compar.pop()->evaluate(lhs, rhs);  
            ResultSet* result = new ResultSet(recVectr);
            _stack.push(result);
        }else{
            _stack.push(queue_of_compar.pop());
        }
    }

    //update of the vector of numbers to the last Token in the stack after the operation
    //if there is a false input we just make the record vector empty
    if(falseInput){
        recVectr.clear();    //commented
    }else{
        recVectr = _stack.pop()->evaluate();
    }
    //We have the record number list, now we just push the record into the new table
    //store the selected record numbers into mother's private member var
    selected_recnos = recVectr;
    int sizeOfRecVectr = recVectr.size();
    int numOfField = fieldnames.size();
    //open the mother bin file and store the data of the record numbers into the child table
    open_fileRW(file, (title+".bin").c_str());
    for(int j = 0; j < sizeOfRecVectr; j++){
        FileRecord record;
        vectorstr newRecordValue = record.readVector(file, recVectr[j], field_names.size());
        vectorstr inOrderRec;
        //change the record string we have into the order of the fields
        for(int i = 0; i < numOfField; i++){
            //we are pushing back in the order of field
            inOrderRec.push_back(newRecordValue[index[fieldnames[i]]]);
        }
        temp.insert_into(inOrderRec);
    }
    file.close();
    return temp;
}


Table Table::select(vectorstr fieldnames){
    //----------Select which gives you only certain fields----------
    if(!file_exists((title+".bin").c_str())){return Table();}
    Table temp(title+"_"+to_string(sequenceNumber), fieldnames); //child table
    sequenceNumber++;
    fstream file;
    FileRecord record;
    long recordNumbers = 0;
    selected_recnos.clear(); //Commented
    
    open_fileRW(file, (title+".bin").c_str());
    
    //selected record numbers are the same as all the mother record numbers
    selected_recnos = recnoVec;
    int sizeOfRecVectr = recnoVec.size();
    int numOfField = fieldnames.size();
    //insert the record string with certain field into the child table
    for(int j = 0; j < sizeOfRecVectr; j++){
        FileRecord record1;
        vectorstr newRecordValue = record1.readVector(file, recnoVec[j], field_names.size());
        vectorstr inOrderRec;
        for(int i = 0; i < numOfField; i++){
            //we are pushing back in the order of field
            inOrderRec.push_back(newRecordValue[index[fieldnames[i]]]);
        }
        temp.insert_into(inOrderRec);
    }
    file.close();
    return temp;
}


Table Table::select(vectorstr fieldnames, vector<string> string_of_compar){
    int vecSize = string_of_compar.size();
    Queue<Token*> postOrderQueue;
    Stack<Token*> tempStack;
    //loop thru the input expression
    for(int i = 0; i < vecSize; i++){
        if(string_of_compar[i] == "("){
            LParen* temp = new LParen(string_of_compar[i]);
            tempStack.push(temp);
        }else if(string_of_compar[i] == ")"){
            //pop out all the operator from the stack above ( to the queue
            RParen* temp = new RParen(string_of_compar[i]);
            bool containLParen = false;

            //there is a Left Parenthesis in the stack
            for(Stack<Token*>::Iterator it = tempStack.begin(); it != tempStack.end(); it++){
                if((*it)->type_string() == "LPAREN"){
                    containLParen = true;
                    std::cout << "containLParen" << containLParen << endl;
                }
            }
            //if the temporary copy of the stack doesnt have a LParen 
            if(!containLParen){
                std::cout << "(Extra Right Parenthesis)\n";
            }else{
                while(tempStack.top()->type_string() != "LPAREN"){
                    postOrderQueue.push(tempStack.pop());
                }
                if(!tempStack.empty()){
                    tempStack.pop();
                }
            }
            
        }else if(string_of_compar[i] == "or"){
            Logical* temp = new Logical(string_of_compar[i]);
            //poping the stack if the thing that ur sitting above is bigger and equal than you
            while(!tempStack.empty() && (tempStack.top()->type_string() == "RELATIONAL"|| tempStack.top()->getString() == "and")){
                postOrderQueue.push(tempStack.pop());
            }
            tempStack.push(temp);
        }else if(string_of_compar[i] == "and"){
            Logical* temp = new Logical(string_of_compar[i]);
            //poping the stack if the thing that ur sitting above is bigger and equal than you
            while(!tempStack.empty() && (tempStack.top()->type_string() == "RELATIONAL")){ 
                postOrderQueue.push(tempStack.pop());
            }
            tempStack.push(temp);
        }else if(string_of_compar[i] == ">" || string_of_compar[i] == "<" || string_of_compar[i] == ">=" || string_of_compar[i] == "<=" || string_of_compar[i] == "="){
            Relational* temp = new Relational(string_of_compar[i]);
            //poping the stack if the thing that ur sitting above is bigger and equal than you
            while(!tempStack.empty() && (tempStack.top()->type_string() == "RELATIONAL")){ 
                postOrderQueue.push(tempStack.pop());
            }
            tempStack.push(temp);
        }else{
            TokenStr* temp = new TokenStr(string_of_compar[i]);
            postOrderQueue.push(temp);
        }
    }

    while(!tempStack.empty()){
        if(tempStack.top()->type_string() == "LPAREN"){
            std::cout << "(Extra Left Parenthesis)\n";
            tempStack.pop();
        }else{
            postOrderQueue.push(tempStack.pop());
        }
    }
    // while(!tempStack.empty()){
    //     postOrderQueue.push(tempStack.pop());
    // }
    // ---------- Debugging ----------
    // printing the post order experission
    // Queue<Token*> printingtest = postOrderQueue;
    // while(!printingtest.empty()){
    //     std::cout << "|" <<printingtest.pop()->getString() << "| ";
    // }
    // std::cout << endl;
    
    //calling the select with the postorder input
    return select(fieldnames, postOrderQueue);
}


Table Table::select(vectorstr fieldnames, string field_searching, string operatr, string value_searching){
    if(!file_exists((title+".bin").c_str())){return Table();}
    Table temp(value_searching, fieldnames);
    fstream file;
    int numOfField = field_names.size();
    
    open_fileRW(file, (title+".bin").c_str());
    selected_recnos.clear(); //Commented

    int indexx = index[field_searching];

    //for operator =
    if(operatr == "="){
        int sizeOfRecVectr = _indices_recno[indexx].get(value_searching).size();
        vector<long> recVectr = _indices_recno[indexx].get(value_searching);
        selected_recnos = recVectr;
        for(int j = 0; j < sizeOfRecVectr; j++){
            
            FileRecord record;
            vectorstr newRecordValue = record.readVector(file, recVectr[j], field_names.size());
            vectorstr inOrderRec;
            for(int i = 0; i < numOfField; i++){
                //we are pushing back in the order of field
                inOrderRec.push_back(newRecordValue[index[fieldnames[i]]]);
            }
            temp.insert_into(inOrderRec);
        }
    }
    //for operator >
    if(operatr == ">"){

        //get the upperbound of the valueSearching in the tree
        MMap<string, long>::Iterator itertrUpper = _indices_recno[indexx].upper_bound(value_searching);
        vector<long> recVectr;

        for(itertrUpper; itertrUpper != MMap<string, long>::Iterator(nullptr); itertrUpper++){
            for(int i = 0; i < ((*itertrUpper).value_list).size(); i++){
                //get all the record numbers
                recVectr.push_back(((*itertrUpper).value_list)[i]);
            }
        }

        int sizeOfRecVectr = recVectr.size();
        selected_recnos = recVectr;
        for(int j = 0; j < sizeOfRecVectr; j++){
            
            FileRecord record;
            vectorstr newRecordValue = record.readVector(file, recVectr[j], field_names.size());
            vectorstr inOrderRec;
            for(int i = 0; i < numOfField; i++){
                //we are pushing back in the order of field
                inOrderRec.push_back(newRecordValue[index[fieldnames[i]]]);
            }
            temp.insert_into(inOrderRec);
        }
    }
    //for operator <
    if(operatr == "<"){

        //get the lowerBound of the valueSearching in the tree
        MMap<string, long>::Iterator itertrLower = _indices_recno[indexx].lower_bound(value_searching);
        vector<long> recVectr;

        for(MMap<string, long>::Iterator temp = _indices_recno[indexx].begin(); temp != itertrLower; temp++){
            for(int i = 0; i < ((*temp).value_list).size(); i++){
                //get all the record numbers
                recVectr.push_back(((*temp).value_list)[i]);
            }
        }

        int sizeOfRecVectr = recVectr.size();
        selected_recnos = recVectr;
        for(int j = 0; j < sizeOfRecVectr; j++){
            
            FileRecord record;
            vectorstr newRecordValue = record.readVector(file, recVectr[j], field_names.size());
            vectorstr inOrderRec;
            for(int i = 0; i < numOfField; i++){
                //we are pushing back in the order of field
                inOrderRec.push_back(newRecordValue[index[fieldnames[i]]]);
            }
            temp.insert_into(inOrderRec);
        }
    }
    //for operator <=
    if(operatr == "<="){

        //get the upperBound of the valueSearching in the tree
        MMap<string, long>::Iterator itertrLower = _indices_recno[indexx].upper_bound(value_searching);
        vector<long> recVectr;

        for(MMap<string, long>::Iterator temp = _indices_recno[indexx].begin(); temp != itertrLower; temp++){
            for(int i = 0; i < ((*temp).value_list).size(); i++){
                //get all the record numbers
                recVectr.push_back(((*temp).value_list)[i]);
            }
        }

        int sizeOfRecVectr = recVectr.size();
        selected_recnos = recVectr;
        for(int j = 0; j < sizeOfRecVectr; j++){
            
            FileRecord record;
            vectorstr newRecordValue = record.readVector(file, recVectr[j], field_names.size());
            vectorstr inOrderRec;
            for(int i = 0; i < numOfField; i++){
                //we are pushing back in the order of field
                inOrderRec.push_back(newRecordValue[index[fieldnames[i]]]);
            }
            temp.insert_into(inOrderRec);
        }
    }
    //for operator >=
    if(operatr == ">="){

        //get the lowerBound of the valueSearching in the tree
        MMap<string, long>::Iterator itertrUpper = _indices_recno[indexx].lower_bound(value_searching);
        vector<long> recVectr;

        for(itertrUpper; itertrUpper != MMap<string, long>::Iterator(nullptr); itertrUpper++){
            for(int i = 0; i < ((*itertrUpper).value_list).size(); i++){
                //get all the record numbers
                recVectr.push_back(((*itertrUpper).value_list)[i]);
            }
        }

        int sizeOfRecVectr = recVectr.size();
        selected_recnos = recVectr;
        for(int j = 0; j < sizeOfRecVectr; j++){
            
            FileRecord record;
            vectorstr newRecordValue = record.readVector(file, recVectr[j], field_names.size());
            vectorstr inOrderRec;
            for(int i = 0; i < numOfField; i++){
                //we are pushing back in the order of field
                inOrderRec.push_back(newRecordValue[index[fieldnames[i]]]);
            }
            temp.insert_into(inOrderRec);
        }
    }

    file.close();
    return temp;
}
Table Table::select_all(){
    if(!file_exists((title+".bin").c_str())){return Table();} //COMMENTED
    Table temp(title+"_"+to_string(sequenceNumber), field_names);
    sequenceNumber++;
    fstream file;
    selected_recnos = recnoVec;
    // temp.recnoVec = recnoVec;
    // temp.selected_recnos = recnoVec;

    long recordNumbers = 0;
    int sizeOfRecVectr = recnoVec.size();
    int numOfField = field_names.size();
    
    open_fileRW(file, (title+".bin").c_str());
    FileRecord record1;
    for(int j = 0; j < sizeOfRecVectr; j++){
        vectorstr newRecordValue = record1.readVector(file, recnoVec[j], numOfField);
        // vectorstr inOrderRec;
        // for(int i = 0; i < numOfField; i++){
        //     //we are pushing back in the order of field
        //     inOrderRec.push_back(newRecordValue[index[field_names[i]]]);
        // }
        temp.insert_into(newRecordValue); //inOrderRec
    }
    file.close();
    return temp;
}
//----Unused----
Table Table::vector_table(vector<long> vec, string fileName, int fieldNameLength){
    Table temp;
    temp.title = fileName;
    FileRecord record;
    fstream file;
    int vecLength = vec.size();
    open_fileRW(file, (fileName+".bin").c_str());

    for(int i = 0; i < vecLength; i++){
        temp.insert_into(record.readVector(file, vec[i], fieldNameLength));
    }
    return temp;
}

vector<long> Table::select_recnos(){
    return selected_recnos;
}

void Table::set_selected_recnos(vector<long> vector){
    selected_recnos = vector;
}


// Table& Table::operator =(const Table& RHS){
//     field_names = RHS.field_names;
//     title = RHS.title;
//     _indices_recno = RHS._indices_recno;
//     index = RHS.index;
//     recnoVec = RHS.recnoVec;
//     numOfRec = RHS.numOfRec;
//     return *this;
// }


ostream& operator <<(ostream& outs, const Table& print_me){
    if(!file_exists((print_me.title+".bin").c_str())){return outs;}
    if(print_me.numOfRec != 0 && print_me.title != ""){
        std::cout << "Table Name: " << setw(10)<< print_me.title << "\tNumbers of Record:" << print_me.numOfRec << endl;
        FileRecord record;
        std::cout << "     Record";
        for(int i = 0; i < print_me.field_names.size(); i++){
            std::cout << setw(10) << print_me.field_names[i];
        }
        std::cout << std::endl;
        fstream f;
        
        open_fileRW(f, (print_me.title+".bin").c_str());
        for(int i = 0; i < print_me.numOfRec; i ++){
                record.read(f, i);
                std::cout << "\n" << setw(8)<< i << setw(8) << record ;
        }
        f.close();
    }else if(!print_me.field_names.empty() && print_me._indices_recno.size() == 0){
        std::cout << "MESSAGE: (Table Created)\n";
    }else if(print_me.title == ""){
        std::cout << "MESSAGE: (Record not Found)\n";
    }
    return outs;
}

vectorstr Table::get_field_names(){
    return field_names;
}

#endif