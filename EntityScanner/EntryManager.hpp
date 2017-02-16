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

using namespace std;

class EntryManager {
private:
    char* m_path;

    char* m_filter_ext;
    int m_filter_type;

    int m_entry_count;
    int m_entry_index;

    RawEntry* m_head;
    RawEntry* m_cur;
    RawEntry* entry_list[MAX_ENTRY_SIZE];

    void get_abs_path(char *path);
    void scan_dir();
    void add_entry(string filename);
    void empty_list();
    void print_info();

public:
    EntryManager();
    EntryManager(char* path, char* ext, int type);
};


#endif
