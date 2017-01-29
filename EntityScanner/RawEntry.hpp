//
//  RawEntry.hpp
//  EntityScanner
//
//  Created by tehloo on 2017. 1. 15..
//  Copyright © 2017년 tehloo. All rights reserved.
//

#ifndef RawEntry_hpp
#define RawEntry_hpp

#include <string>
using namespace std;

class RawEntry {
private:
    string path;
    string name;
    string* token[100];
    int token_size;
    char separators[10] = {' ', '|', '_', '-', '.', 0,};
    
    void parse_name(string fullname);
    void parse_to_tokens();
    
public:
    RawEntry();
    RawEntry(string fullname);
    RawEntry(string path, string name) : path(path), name(name) {RawEntry(); parse_to_tokens();}
    ~RawEntry();
    
    string getName() { return name; }
    string get_full_path();
    string* get_token_info();
};


#endif /* RawEntry_hpp */
