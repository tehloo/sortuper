//
//  RawEntry.hpp
//  EntityScanner
//
//  Created by tehloo on 2017. 1. 15..
//  Copyright © 2017년 tehloo. All rights reserved.
//

#ifndef RawEntry_hpp
#define RawEntry_hpp

#define ENTRY_INVALID -1
#define MAX_TOKEN_COUNT 100

#include <string>
#include "EntryManager.hpp"
using namespace std;

class EntryManager;

class RawEntry {
private:
    static const char separators[10];

    char* path;
    char* name;
    string* token[MAX_TOKEN_COUNT];
    int token_size;
    int token_index;
    RawEntry* pair;

    void parse_name(string fullname);
    void parse_to_tokens();
    void init_variables();
    void init_variables(char* path, char* name);

public:
    RawEntry();
    RawEntry(string fullname);
    RawEntry(char* path, char* name);
    ~RawEntry();

    string getName() { return name; }
    string get_full_path();
    string* get_token_info();

    int compare_with(RawEntry* entry);
    string* get_next_token();
    void init_token_index();
    RawEntry* select_post(EntryManager* em);
};


#endif /* RawEntry_hpp */
