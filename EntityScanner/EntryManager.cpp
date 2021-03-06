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
#include <string.h>


using namespace std;

EntryManager::EntryManager() {
    this->init_variables();
}

EntryManager::EntryManager(EntryManager* em)
{
    if (em->m_path != NULL) {
        m_path = (char*)malloc(strlen(em->m_path) + 1);
        memcpy(m_path, em->m_path, strlen(em->m_path) + 1);
    } else {m_path = NULL;}
    if (em->m_filter_ext != NULL) {
        m_filter_ext = (char*)malloc(strlen(em->m_filter_ext) + 1);
        memcpy(m_filter_ext, em->m_filter_ext, strlen(em->m_filter_ext) + 1);
    } else {m_filter_ext = NULL;}

    m_filter_type = em->m_filter_type;
    m_entry_count = em->m_entry_count;
    m_entry_index = 0;
    m_head = NULL;
    m_cur = NULL;
    memcpy(entry_list, em->entry_list, sizeof(em->entry_list));
}

EntryManager::EntryManager(char* path, EntryFilter* filter) {
    this->init_variables();
    this->m_filter = filter;
    this->get_abs_path(path);
    this->scan_dir(m_path, NULL);
}

void EntryManager::init_variables() {
    this->m_path = NULL;
    this->m_filter = NULL;
    this->m_filter_ext = NULL;
    this->m_filter_type = DT_UNKNOWN;
    this->m_entry_count = ENTRY_INVALID;
    this->m_entry_index = ENTRY_INVALID;
    this->m_head = NULL;
    this->m_cur = NULL;
    memset(this->entry_list, 0, sizeof(this->entry_list));
}

//  TODO: we need free heap all we got

void EntryManager::get_abs_path(char* path)
{
    cout << endl << "\tEntryManager updating path " << path;
    m_path = realpath(path, NULL);
    cout << " as " << m_path << endl;
}

void EntryManager::add_file(char* path, char* filename) {
    entry_list[m_entry_count++] = new FileEntry(path, filename);
//    cout << "\t- " << entry_list[m_entry_count - 1]->get_full_path() << endl;
}

void EntryManager::add_dir(char* path, char* name) {
    entry_list[m_entry_count++] = new DirEntry(path, name);
//    cout << "\t- " << entry_list[m_entry_count - 1]->get_full_path() << endl;
}

char* get_file_path(char* path, char* name) {
    if (name == NULL)
        return path;
    size_t len_path = strlen(path) + strlen(name) + 1;
    char *curpath = (char*)malloc(len_path * sizeof(char) + 1);
    memset(curpath, 0x00, len_path * sizeof(char) + 1);
    return strcat(strcat(strcat(curpath, path), "/"), name);
}

void EntryManager::scan_dir(char* path, char* name) {
    char* curpath = get_file_path(path, name);
    DIR* dir = opendir(curpath);
    if (dir == NULL) {
        cout << "dir is not opened" << endl;
        return;
    }
    if (name != NULL) add_dir(path, name);

    struct dirent *ent;
    //  1. find specific files and let them respawn to custom objects.
    while ((ent = readdir(dir)) != NULL) {
        if (ent->d_type == DT_DIR
            && ((strcmp(ent->d_name, ".") == 0
                 || strcmp(ent->d_name, "..") == 0))) {
            continue;
        }
        //  skip hidden files
        if (ent->d_name[0] == '.') {
            cout << "\t\tskip hidden file or dir - " << ent->d_name << endl;
            continue;
        }
        //  if type is specified, other type will be ignored.
        if (m_filter_type != DT_UNKNOWN && ent->d_type != m_filter_type) {
            continue;
        }
        // FIXME: ext is located only on tail.
        // TODO: we'd better to make entry manager for each purpose.
        // TODO: Do we need to use string for this???
        if (this->m_filter->match_ext(ent->d_name)) {
            add_file(curpath, ent->d_name);
        } else if (this->m_filter->get_type() == DT_DIR && ent->d_type == DT_DIR) {
            scan_dir(curpath, ent->d_name);
        }
    }
    cout << "\t\ttotal " << m_entry_count << " entries has made."
         << endl << endl << endl;
}

void EntryManager::print_info()
{
    cout << " *** ENTRY MANGER for " << m_path << endl;
    cout << "\t * ext = " << (m_filter_ext == NULL ? "NULL" : m_filter_ext)
         << " | * type " << m_filter_type << endl;
    cout << "\t * entry size = " << m_entry_count << endl;

    int i = 0;
    while (i < m_entry_count) {
        cout << "   " << i << ". "  << entry_list[i++]->get_info() << endl;
    }
}

void EntryManager::empty_list()
{
    int i = 0;
    //  IT WILL BE REMOVED. it deletes all entries. only for checking.
    while (i < m_entry_count) {
        delete entry_list[i++];
    }
}

void EntryManager::set_post(EntryManager* post)
{
    //  loop for each files to post
    int i = 0;
    while (i < m_entry_count) {
        FileEntry* entry = (FileEntry*)entry_list[i++];
        //  select most similar one.
        //  TODO: it can be addte to member for target specified class.
        DirEntry* selected = entry->select_post(post);
        //  new each entry will have their own 'pair'
    }
}

RawEntry* EntryManager::get_next_entry()
{
    if (m_entry_index == m_entry_count - 1) {
        return NULL;
    }
    return entry_list[++m_entry_index];
}

char* EntryManager::get_path()
{
    return m_path;
}



EntryFilter::EntryFilter(int _type, char* _exts) {
    this->m_type = _type;
    this->m_ext_list = this->parse_exts_to_vector(_exts);
}

vector<char*>* EntryFilter::parse_exts_to_vector(char* _exts) {
    if (_exts == NULL) return NULL;
    vector<char*>* v = new vector<char*>;

    int i_fore = 0;
    int i_back = 0;
    int num_exts = 0;

    while (_exts[i_fore] != 0) {
        while (_exts[i_back] != '|' && _exts[i_back] != 0) {
            i_back++;
        }

        int len = i_back - i_fore;
        if (len > 0) {
            char* ext = (char*)calloc(len + 1, sizeof(char));
            strncpy(ext, (_exts + i_fore), len);
            v->push_back(ext);
            if (_exts[i_back] == 0) break;
            i_fore = ++i_back;
        }
    }
#ifndef NO_DEBUG
    cout << v->size() << " exts has been updated...";
    vector<char*>::iterator it = v->begin();
    while (it != v->end()) {
        cout << " " << *it;
        it++;
    }
    cout << endl;
#endif
    return v;
}

int EntryFilter::get_type() {
    return this->m_type;
}

char* get_ext_ptr(char* name) {
    char* ptr = name;
    char* ext = NULL;

    while (*ptr != 0) {
        if (*ptr == '.' && *(ptr+1) != 0) {
            ext = ptr + 1;
        }
        ptr++;
    }
    return ext;
}

bool EntryFilter::match_ext(char* name) {
    vector<char*>* v = this->m_ext_list;
    if (v == NULL) return false;
    char* ext_name = get_ext_ptr(name);
    if (ext_name == NULL) return false;
//    for (vector<char*>::iterator it = v->begin(); it != v->end(); it++) {
 //       char* ext = *(it);
    for(int i = 0; i < v->size(); i++) {
        char* ext = (*v)[i];
        if (strcmp(ext_name, ext) == 0) return true;
    }
    return false;
}
