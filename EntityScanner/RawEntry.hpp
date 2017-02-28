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
#define NO_DEBUG

#include <string>
#include <sstream>
#include "EntryManager.hpp"
using namespace std;

class EntryManager;

class RawEntry {
private:
    static const char separators[10];

    char* m_path;
    char* m_name;
    string* token[MAX_TOKEN_COUNT];
    int token_size;
    int token_index;
//    RawEntry* pair;

    void parse_name(string fullname);
    void parse_to_tokens();
    void init_variables();
    void init_variables(char* _path, char* _name);
    void init_token_list();

protected:
    ostringstream* get_entry_info();

public:
    RawEntry();
    RawEntry(string fullname);
    RawEntry(char* _path, char* _name);
    ~RawEntry();

    string get_name() { return m_name; }
    string get_full_path();

    int compare_with(RawEntry* entry);
    string* get_next_token();
    void init_token_index();
    RawEntry* select_post(EntryManager* em);
    virtual string get_info() = 0;
};


class DirEntry : public RawEntry {
private:
    char* m_root_path;
public:
    DirEntry(char* _path, char* _name);

    string get_info();
};

class FileEntry : public RawEntry {
private:
    DirEntry* pair;
public:
    FileEntry(char* _path, char* _name);

    DirEntry* select_post(EntryManager* em);
    string get_info();
};

#endif /* RawEntry_hpp */
