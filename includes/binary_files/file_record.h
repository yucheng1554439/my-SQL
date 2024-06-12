#ifndef FILE_RECORD_H
#define FILE_RECORD_H

#include <iostream>
#include <cstring>
#include <string>
#include <iomanip>
#include <fstream>
#include <vector>

using namespace std;

//----------------------------------------------------------------------------
//                      C L A S S   R E C O R D
//----------------------------------------------------------------------------

class FileRecord{
public:
    //when you construct a Record, it's either empty or it contains a word
    FileRecord(){
        for(int i = 0; i < MAX; i++){
            for(int j = 0; j < MAX+1; j++){
                _record[i][j] = '\0';
            }
        }
    }

    FileRecord(vector<char*> str){
        for(int i = 0; i < MAX; i++){
            for(int j = 0; j < MAX+1; j++){
                _record[i][j] = '\0';
            }
        }
        for (int i = 0; i < MAX && i < str.size(); i++){
            strcpy(_record[i], str[i]); //, MAX
        }
    }

    FileRecord(vector<string> s){
        for(int i = 0; i < MAX; i++){
            for(int j = 0; j < MAX+1; j++){
                _record[i][j] = '\0';
            }
        }
        for(int i = 0; i < s.size(); i++){
            strcpy(_record[i], s[i].c_str()); // , MAX //NNEEEEEEEEEED TO BE FIXED------------
        }
    }

    //returns the record number
    long write(fstream& outs);

    //returns the number of bytes; read = MAX, or zero if read passed the end of file      
    long read(fstream& ins, long recno); 
    string readString(fstream& ins, long recno, int totalFieldNumber);   
    vector<string> readVector(fstream &ins, long recno, int totalFieldNumber);
    
    friend ostream& operator<<(ostream& outs, const FileRecord& r);

    static const int MAX = 100;
    char _record[MAX][MAX+1];

private:
    //static const int MAX = 100;
    int recno;
    //char _record[MAX][MAX+1];
};

long FileRecord::write(fstream &outs){
    //r.write(f); //take the Record r and write it into file f
    //and return the record number of this Record

    //write to the end of the file.
    outs.seekg(0, outs.end);

    //retrieves the current position of the file pointer
    long pos = outs.tellp();    

    for(int i = 0; i < MAX; i++){
        outs.write(_record[i], sizeof(_record[i]));
    }
    
    // std::cout << pos/sizeof(_record) << "RECORD";

    return pos/sizeof(_record);  //record number
}

long FileRecord::read(fstream &ins, long recno){
    //returns the number of bytes read.
    //    r.read(f, 6);
    //    cout<<r<<endl;
    long pos = recno * sizeof(_record);
    ins.seekg(pos, ios_base::beg);

    //ins.read(&_record[0], sizeof(_record))
    //don't you want to mark the end of  the cstring with null?
    //_record[] => [zero'\0'trash trash trash trash]
    //don't need the null character, but for those of us with OCD and PTSD:
   
    for(int i = 0; i < MAX; i++){
        ins.read(_record[i], sizeof(_record[i]));
        _record[i][ins.gcount()] = '\0';
    }

    return ins.gcount();
}

string FileRecord::readString(fstream &ins, long recno, int totalFieldNumber){
    //returns the number of bytes read.
    //    r.read(f, 6);
    //    cout<<r<<endl;
    long pos = recno * sizeof(_record);
    ins.seekg(pos, ios_base::beg);
    
    string a = "";
    //ins.read(&_record[0], sizeof(_record))
    //don't you want to mark the end of  the cstring with null?
    //_record[] => [zero'\0'trash trash trash trash]
    //don't need the null character, but for those of us with OCD and PTSD:
    
    for(int i = 0; i < totalFieldNumber; i++){
        ins.get(_record[i], sizeof(_record[i]));
        _record[i][ins.gcount()] = '\0';
        for(int j = 0; j < sizeof(_record[i]); j++){
            a += _record[i][j];
        }
    }

    return a;
}

vector<string> FileRecord::readVector(fstream &ins, long recno, int totalFieldNumber){
    //returns the number of bytes read.
    //    r.read(f, 6);
    //    cout<<r<<endl;
    long pos = recno * sizeof(_record);
    ins.seekg(pos, ios_base::beg);
    
    vector<string> recordVector;
    string temp_string = "";
    //ins.read(&_record[0], sizeof(_record))
    //don't you want to mark the end of  the cstring with null?
    //_record[] => [zero'\0'trash trash trash trash]
    //don't need the null character, but for those of us with OCD and PTSD:
    for(int i = 0; i < totalFieldNumber; i++){
        ins.read(_record[i], sizeof(_record[i]));
        _record[i][ins.gcount()] = '\0';

        temp_string = "";
        for(int j = 0; j < sizeof(_record[i])-1; j++){
            if(_record[i][j]!='\0'){
                temp_string += _record[i][j];
            }
            // std::cout << a;
        }       
        // strcpy(temp_string, _record[i]);
        // cout<<"\n-------temp_strin: "<<temp_string<<endl;
        recordVector.push_back(temp_string);
        // std::cout << a;
    }
    return recordVector;
}





ostream& operator<<(ostream& outs, const FileRecord& r){
    for(int i = 0; i < r.MAX; i++){
        outs << setw(11) << r._record[i];
    }
    return outs;
}



#endif