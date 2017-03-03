//
//  EntryManager.hpp
//
//  Created by tehloo on 2017. 2. 16..
//  Copyright © 2017년 tehloo. All rights reserved.
//

#ifndef EntryManager_hpp
#define EntryManager_hpp

#define MAX_ENTRY_SIZE 100

#include "RawEntry.hpp"
#include <vector>

using namespace std;

class RawEntry;
class EntryFilter;

class EntryManager {
private:
    char* m_path;

    char* m_filter_ext;
    int m_filter_type;
    EntryFilter* m_filter;

    int m_entry_count;
    int m_entry_index;

    RawEntry* m_head;
    RawEntry* m_cur;
    RawEntry* entry_list[MAX_ENTRY_SIZE];

    void init_variables();
    void get_abs_path(char* path);
    void scan_dir(char* path, char* name);
    void add_entry(char* filename);
    void add_file(char* path, char* name);
    void add_dir(char* path, char* name);
    int get_dirs_in_dir(char* path, char* filename);

public:
    EntryManager();
    EntryManager(EntryManager* em);
//    EntryManager(char* path, char* ext, int type);
    EntryManager(char* path, EntryFilter* filter);

    void set_post(EntryManager* post);
    RawEntry* get_next_entry();
    void print_info();
    //  TODO: it should be on destructor and private
    void empty_list();
    char* get_path();
};

class EntryFilter {
private:
    int m_type;
    vector<char*>* m_ext_list;
    vector<char*>* parse_exts_to_vector(char* _exts);
public:
    EntryFilter(int _type, char* _exts);
    int get_type();
    bool match_ext(char* name);
};
#endif
