#ifndef TABLE_CPP
#define TABLE_CPP

#include "table.h"
#include "../binary_files/file_record.h"
#include "../binary_files/utilities.h"
using namespace std;

int Table::sequenceNumber = 0;

Table::Table()
:numOfRec(0)
{

}
Table::Table(string a)
:title(a), numOfRec(0)
{
    // std::cout << "Title:" << title << "\n";
    fstream f;

    f.open((a + ".txt").c_str());
    if (f)
    {
        // printing the success message
        string s;
        // cout<<"filename: "<<a<<endl;
        while(getline(f, s)){
            //Set the field name equal to the things in the txt
            // cout<<"string in getline: "<<s<<endl;
            field_names.push_back(s);
        }
        // while(f>>s){
        //     field_names.push_back(s);
        // }
        f.close();
        for(int i = 0; i < field_names.size(); i++){
            MMap<string, long> temp;
            temp.clear();
            _indices_recno.push_back(temp);
        }
        //Then get the value from the binary file
        fstream file;
        open_fileRW(file, (a + ".bin").c_str());
        FileRecord record;
        long recordNumbers = 0; // recordNumbers to loop through while reading thru the bin file
        int fieldLength = field_names.size();
        for(int i = 0; i < fieldLength; i++){
            index.insert(field_names[i], i);
        }
        //loop thru the bin file
        while(record.read(file, recordNumbers) != 0){
            vectorstr b = record.readVector(file, recordNumbers, fieldLength);
            long recon = recordNumbers;
            for(int j = 0; j < b.size(); j++){
                _indices_recno[index[field_names[j]]].insert(b[j], recon);
            }
            recnoVec.push_back(recon);
            numOfRec++;
            recordNumbers++;
        }

        // cout << "_indices_recno\n" << _indices_recno << endl;


        file.close();
    }else{
        open_fileW(f, (a + ".txt").c_str());
        //Set the field name equal to the things in the txt
        f.close();
        // printing the error message
        cout << "File does not exists" << endl;
        for(int i = 0; i < field_names.size(); i++){
            MMap<string, long> temp;
            temp.clear();
            _indices_recno.push_back(temp);
        }
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


Table::Table(string a, vectorstr b)
:title(a), field_names(b), numOfRec(0)
{
    //insert the empty Multimaps into the vector
    for(int i = 0; i < field_names.size(); i++){
        MMap<string, long> temp;
        temp.clear();
        _indices_recno.push_back(temp);
    }


    ofstream f;
    f.open((a + ".txt").c_str(), ios::trunc);
    int length = b.size();
    // cout<<"length: "<<length<<"\n"
    for(int i = 0; i < length; i++){
        // cout<<"field names being written: "<<b[i]<<endl;
        f << b[i] << "\n";
    }
    f.close();


    for(int i = 0; i < b.size(); i++){
        index.insert(b[i], i);
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

void Table::insert_into(vectorstr b){

    FileRecord record(b);
    fstream file;
    open_fileRW(file, (title+".bin").c_str());
    long recon = record.write(file);
    for(int i = 0; i < b.size(); i++){
        _indices_recno[i].insert(b[i], recon);
    }
    // cout << "_indices_recno" << _indices_recno << endl;
    recnoVec.push_back(recon);
    numOfRec++;
    file.close();
}


Table Table::select(vectorstr fieldnames, Queue<Token*> queue_of_compar){
    //----------My main select-----------
    Table temp(title+"_"+to_string(sequenceNumber), fieldnames);
    sequenceNumber++;
    vector<long> recVectr;
    Stack<Token*> _stack;
    Stack<Token*> _result_stack;
    fstream file;
    bool falseInput = false;
    // int numOfField = field_names.size();
    selected_recnos.clear();
    //if the queue is not empty
    while(!queue_of_compar.empty()){
        //----- < <= >= > = ----- //PROBLEM, if there is then delet the resultstack replace with stack
        // cout<<"-----queue of compar in select is not empty"<<endl;
        if(queue_of_compar.front()->type_string() == "RELATIONAL"){
            // cout<<"RELATIONAL"<<endl;
            Token* rhs = _stack.pop();
            Token* lhs = _stack.pop();
            // cout<<"LHS: "<<lhs->getString()<<endl;
            // cout<<"RHS: "<<rhs->getString()<<endl;
            // cout<<"LHS: "<<lhs->getString() << "||index[lhs->getString():" << index[lhs->getString()] << endl;

            if(field_names[index[lhs->getString()]] != lhs->getString()){
                falseInput = true;
                break;
            }

            recVectr = queue_of_compar.pop()->evaluate(lhs, rhs, _indices_recno, field_names, index[lhs->getString()]); 

            // std::cout << "INSIDE" <<  recVectr;
            ResultSet* result = new ResultSet(recVectr);
            // cout<<"---------result in rpn -----"<<endl<<result->evaluate()<<endl;
            _result_stack.push(result);
        //------ and or -----
        }else if(queue_of_compar.front()->type_string() == "LOGICAL"){
            // cout<<"LOGICAL"<<endl;
            Token* rhs = _result_stack.pop();
            Token* lhs = _result_stack.pop();
            recVectr = queue_of_compar.pop()->evaluate(lhs, rhs);  
            ResultSet* result = new ResultSet(recVectr);
            _result_stack.push(result);
        }else{
            _stack.push(queue_of_compar.pop());
        }
    }

    //update of the vector of numbers to the last one in the stack after the operation
    // recVectr = _result_stack.pop()->evaluate();
    if(falseInput){
        recVectr.clear();
    }else{
        recVectr = _result_stack.pop()->evaluate();
    }
    //We have the record number list, now we just push the record into the new table
    selected_recnos = recVectr;
    int sizeOfRecVectr = recVectr.size();
    int numOfField = fieldnames.size();

    open_fileRW(file, (title+".bin").c_str());
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
    // cout<<"sizeOfRecVectr in select(vectorstr fieldnames, Queue<Token*> queue_of_compar): "<<sizeOfRecVectr<<endl;
    // std::cout << "temp.numOfRec:" << temp.numOfRec << endl;
    // temp.numOfRec = sizeOfRecVectr;
    // std::cout << "temp.numOfRecAfter:" << temp.numOfRec << endl;
    // temp.numOfRec = sizeOfRecVectr;

    // cout<<"Printing temp in select(vectorstr fieldnames, Queue<Token*> queue_of_compar)"<<endl;
    // cout<<temp<<endl;
    file.close();
    return temp;
}


Table Table::select(vectorstr fieldnames){
    
    Table temp(title+"_"+to_string(sequenceNumber), fieldnames);
    sequenceNumber++;
    fstream file;
    // int numOfField = field_names.size();
    open_fileRW(file, (title+".bin").c_str());
    selected_recnos.clear();

    FileRecord record;
    vector<long> recVectr;
    long recordNumbers = 0;
    int fieldLength = fieldnames.size();

    while(record.read(file, recordNumbers) != 0){
        vectorstr b = record.readVector(file, recordNumbers, fieldLength);
        long recon = recordNumbers;
        recVectr.push_back(recon);
        recordNumbers++;
    }
    file.close();

    fstream file1;
    open_fileRW(file1, (title+".bin").c_str());

    // std::cout << "recVectr" << recVectr << endl;
    selected_recnos = recVectr;
    int sizeOfRecVectr = recVectr.size();
    int numOfField = fieldnames.size();

    for(int j = 0; j < sizeOfRecVectr; j++){
        FileRecord record1;
        vectorstr newRecordValue = record1.readVector(file1, recVectr[j], field_names.size());
        vectorstr inOrderRec;
        for(int i = 0; i < numOfField; i++){
            //we are pushing back in the order of field
            inOrderRec.push_back(newRecordValue[index[fieldnames[i]]]);
        }
        // std::cout << "inOrderRec" << inOrderRec << endl;
        temp.insert_into(inOrderRec);
    }
    // std::cout << "temp.numOfRec:" << temp.numOfRec << endl;
    // temp.numOfRec = sizeOfRecVectr;
    // std::cout << "temp.numOfRecAfter:" << temp.numOfRec << endl;
    file1.close();
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
            while(tempStack.top()->type_string() != "LPAREN"){
                postOrderQueue.push(tempStack.pop());
            }
            tempStack.pop();
        }else if(string_of_compar[i] == "or"){
            Logical* temp = new Logical(string_of_compar[i]);
            // Queue<Token*> tempForPop;
            //sink down if the thing that ur sitting above is bigger and equal than you
            while(!tempStack.empty() && (tempStack.top()->type_string() == "RELATIONAL"|| tempStack.top()->getString() == "and")){ //|| tempStack.top()->type_string() == "LOGICAL"
                // tempForPop.push(tempStack.pop());
                postOrderQueue.push(tempStack.pop());
            }
            tempStack.push(temp);
            // while(!tempForPop.empty()){
            //     tempStack.push(tempForPop.pop());
            // }
        }else if(string_of_compar[i] == "and"){
            Logical* temp = new Logical(string_of_compar[i]);
            // Queue<Token*> tempForPop;
            //sink down if the thing that ur sitting above is bigger and equal than you
            while(!tempStack.empty() && (tempStack.top()->type_string() == "RELATIONAL")){ //|| tempStack.top()->type_string() == "LOGICAL"
                // tempForPop.push(tempStack.pop());
                postOrderQueue.push(tempStack.pop());
            }
            tempStack.push(temp);
            // while(!tempForPop.empty()){
            //     tempStack.push(tempForPop.pop());
            // }
        }else if(string_of_compar[i] == ">" || string_of_compar[i] == "<" || string_of_compar[i] == ">=" || string_of_compar[i] == "<=" || string_of_compar[i] == "="){
            // Relational* temp = new Relational(string_of_compar[i]);
            // tempStack.push(temp);
            Relational* temp = new Relational(string_of_compar[i]);
            // Queue<Token*> tempForPop;
            //sink down if the thing that ur sitting above is bigger and equal than you
            while(!tempStack.empty() && (tempStack.top()->type_string() == "RELATIONAL")){ //|| tempStack.top()->type_string() == "LOGICAL"
                // tempForPop.push(tempStack.pop());
                postOrderQueue.push(tempStack.pop());
            }
            tempStack.push(temp);
            // while(!tempForPop.empty()){
            //     tempStack.push(tempForPop.pop());
            // }
        }else{
            TokenStr* temp = new TokenStr(string_of_compar[i]);
            postOrderQueue.push(temp);
        }
    }
    while(!tempStack.empty()){
        postOrderQueue.push(tempStack.pop());
    }
    //printing the post order experission
    // Queue<Token*> printingtest = postOrderQueue;
    // while(!printingtest.empty()){
    //     std::cout << printingtest.pop()->getString() << " ";
    // }
    // std::cout << endl;
    //Debugging
    // cout<<"-------Printing select directly from select(fieldname, string compar)--------"<<endl;
    // cout<<select(fieldnames, postOrderQueue)<<endl;
    Table temp = select(fieldnames, postOrderQueue);
    //
    //Now we offcialy done with Shawting yard!!! We have a postOrderQueue
    return temp;
}


Table Table::select(vectorstr fieldnames, string field_searching, string operatr, string value_searching){
    Table temp(value_searching, fieldnames);
    fstream file;
    int numOfField = field_names.size();
    open_fileRW(file, (title+".bin").c_str());
    selected_recnos.clear();

    //for operator =
    if(operatr == "="){

        for(int i = 0; i < numOfField; i++){
            if(field_names[i]==field_searching){
                
                int sizeOfRecVectr = _indices_recno[i].get(value_searching).size();
                vector<long> recVectr = _indices_recno[i].get(value_searching);
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
        }       

    }
    //for operator >
    if(operatr == ">"){

        for(int i = 0; i < numOfField; i++){
            //find the subtree i that has the value we are searching for
            if(field_names[i]==field_searching){
                //get the upperbound of the valueSearching in the tree
                MMap<string, long>::Iterator itertrUpper = _indices_recno[i].upper_bound(value_searching);
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
        }
        
    }
    //for operator <
    if(operatr == "<"){

        for(int i = 0; i < numOfField; i++){
            //find the subtree i that has the value we are searching for
            if(field_names[i]==field_searching){
                //get the lowerBound of the valueSearching in the tree
                MMap<string, long>::Iterator itertrLower = _indices_recno[i].lower_bound(value_searching);
                vector<long> recVectr;

                for(MMap<string, long>::Iterator temp = _indices_recno[i].begin(); temp != itertrLower; temp++){
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
        }
    }
    //for operator <=
    if(operatr == "<="){

        for(int i = 0; i < numOfField; i++){
            //find the subtree i that has the value we are searching for
            if(field_names[i]==field_searching){
                //get the upperBound of the valueSearching in the tree
                MMap<string, long>::Iterator itertrLower = _indices_recno[i].upper_bound(value_searching);
                vector<long> recVectr;

                for(MMap<string, long>::Iterator temp = _indices_recno[i].begin(); temp != itertrLower; temp++){
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
        }
    }
    //for operator >=
    if(operatr == ">="){

        for(int i = 0; i < numOfField; i++){
            //find the subtree i that has the value we are searching for
            if(field_names[i]==field_searching){
                //get the lowerBound of the valueSearching in the tree
                MMap<string, long>::Iterator itertrUpper = _indices_recno[i].lower_bound(value_searching);
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
        }
    }

    file.close();
    return temp;
}
Table Table::select_all(){
    Table temp(title);
    selected_recnos = recnoVec;
    // int vecSize = recnoVec.size();
    // int numOfField = field_names.size();
    // FileRecord record;
    // fstream file;
    // open_fileRW(file, (title+".bin").c_str());
    // for(int recnooo = 0; recnooo < vecSize; recnooo++){

    //     vectorstr recorVector = record.readVector(file, recnooo, numOfField);
    //     // std::cout << "NMSL" << recon;
    //     std::cout << recorVector;
    //     for(int i = 0; i < recorVector.size(); i++){
    //         // std::cout << b[i];
    //         temp._indices_recno[i].insert(recorVector[i], recnooo);
    //     }
    //     // std::cout << b << endl;
    //     temp.recnoVec.push_back(recnooo);
    //     temp.numOfRec++;
        
    // }
    // file.close();

    //old version
    // temp = vector_table(recnoVec, title, field_names.size());
    // temp.field_names = field_names;
    // temp._indices_recno = _indices_recno;
    // temp.index = index;
    // temp.recnoVec = recnoVec;
    // temp.numOfRec = numOfRec;
    return temp;
}

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
} //return the record numbers

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
    
    // for(int i = 0; i < print_me.field_names.size(); i++){
    //     std::cout << "          " << print_me.field_names[i];
    // }   
    // for(int i = 0; i < print_me.numOfRec; i ++){
    //     FileRecord record(print_me.field_names);
    //     // fstream f;
    //     open_fileRW(print_me.file, print_me.title.c_str());
    //     std::cout << "          " << record.read(print_me.file, i);
    // } 
    std::cout << "Table Name: " << setw(10)<< print_me.title << "\tNumbers of Record:" << print_me.numOfRec << endl;
    FileRecord record;
    std::cout << "Record";
    for(int i = 0; i < print_me.field_names.size(); i++){
        std::cout << setw(10) << print_me.field_names[i];
    }
    std::cout << std::endl;
    fstream f;
    open_fileRW(f, (print_me.title+".bin").c_str());
    // cout<<"!!!!!!!print_me.numOfRec"<<print_me.numOfRec<<endl;
    for(int i = 0; i < print_me.numOfRec; i ++){
        // open_fileRW(file, title.c_str());
        // int i=0;
        // while(record.read(f, i)!=0){   
            record.read(f, i);
            std::cout  << setw(10)<< i << record << endl;
            // i++;
        // }
    }
    f.close();
    return outs;
}

vectorstr Table::get_field_names(){
    return field_names;
}

#endif