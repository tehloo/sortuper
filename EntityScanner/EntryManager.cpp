//
//  EntryManager.cpp
//  EntityScanner
//
//  Created by tehloo on 2017. 2. 16..
//  Copyright © 2017년 tehloo. All rights reserved.
//

#include "EntryManager.hpp"
#include <iostream>
#include <fstream>
#include <dirent.h>
#include <stdlib.h>


using namespace std;

EntryManager::EntryManager() : 
            m_path(NULL),
            m_filter_ext(NULL),
            m_filter_type(DT_UNKNOWN),
            m_entry_count(ENTRY_INVALID),
            m_entry_index(ENTRY_INVALID),
            m_head(NULL),
            m_cur(NULL) {};

EntryManager::EntryManager(char* path, char* ext, int type)
{
    EntryManager();
    m_filter_ext = ext;
    m_filter_type = type;
    get_abs_path(path);

    //  Do we need to make sure every parameter has been set.
    scan_dir();
    print_info();
    empty_list();
}

void EntryManager::get_abs_path(char* path)
{
    cout << endl << "EntryManager updating path " << path;
    m_path = realpath(path, NULL);
    cout << " as " << m_path << endl;
}

void EntryManager::add_entry(string filename)
{
    entry_list[m_entry_count++] = new RawEntry(m_path, filename);
}

void EntryManager::scan_dir() 
{
    DIR* dir = opendir(m_path);
    struct dirent *ent;
    cout << endl << "\tfind_files requested to " << m_path << endl;
    if (dir == NULL)
    {
        cout << "dir is not opened" << endl;
        return;
    }

    //  1. find specific files and let them respawn to custom objects.
    while ((ent = readdir(dir)) != NULL) {
        //  if type is specified, other type will be ignored.
        if (m_filter_type != NULL && ent->d_type != m_filter_type) {
            continue;
        }
        string filename(ent->d_name);
        // FIXME: ext is located only on tail.
        // TODO: we'd better to make entry manager for each purpose.
        if (m_filter_ext != NULL && filename.find(m_filter_ext) != string::npos) {
            add_entry(filename);
        } else if (m_filter_ext == NULL) {
            add_entry(filename);
        } else {
            // cout << " - " << (int)ent->d_type << ":" << ent->d_name << endl;
        }
    }
    cout << "\t\ttotal " << m_entry_count << " entries has made." << endl << endl << endl;
}

void EntryManager::print_info()
{
    cout << " *** ENTRY MANGER for " << m_path << endl;
    cout << "\t * ext = " << (m_filter_ext == NULL ? "NULL" : m_filter_ext)
         << " | * type " << m_filter_type << endl;
    cout << "\t * entry size = " << m_entry_count << endl;

    int i = 0;
    while (i < m_entry_count) {
        cout << "   " << i << ". "  << *(entry_list[i++]->get_token_info());
    }
}

void EntryManager::empty_list() 
{
    int i = 0;
    //  IT WILL BE REMOVED. it deletes all entries. only for checking.
    while (i < m_entry_count) {
//        cout << " deleting...  " << *(entry_list[i]->get_token_info()) << endl;
        delete entry_list[i++];
    }
}
