//
//  main.cpp
//  EntityScanner
//
//  Created by tehloo on 2017. 1. 15..
//  Copyright © 2017년 tehloo. All rights reserved.
//

#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <dirent.h>
#include "RawEntry.hpp"
using namespace std;

bool get_paths(char** root_path, char** target_path);
bool scan_dirs(const char *dir_path);
bool find_files(const char *dir_path);
bool find_entries(const char *dir_path, char *ext, int type);

int main(int argc, const char * argv[]) {
    // insert code here...
    std::cout << "Hello, World!\n";
    char* root_to_post = NULL;
    char* target_path = NULL;
    get_paths(&root_to_post, &target_path);
//    cout << "get_paths : root_path=" << root_to_post << "/ target_path=" << target_path << endl;
    
    // confirm path
    string default_root_path = "/Volumes/tehloo mac HDD/downloads";
    string default_post_path = "/Volumes/tehloo mac HDD/videos";
    char* abs_post = realpath(root_to_post == NULL ? default_post_path.c_str() : root_to_post, NULL);
    char* abs_target = realpath(target_path == NULL ? default_root_path.c_str() : target_path, NULL);
    free(root_to_post); root_to_post = NULL;
    free(target_path); target_path = NULL;

    cout << "root path is " << abs_post << endl;
    cout << "target path is " << abs_target << endl;
    
    
    //  scan paths of directories
    scan_dirs(abs_post);

    //  find files to copy
    find_files(abs_target);

    return 0;
}

bool get_paths(char** root_path, char** target_path) {
    ifstream file("preference.ini");
    string str;
    string path;
    size_t index = 0;
    char* p_temp = NULL;
    while (getline(file, str))
    {
        if (index = str.find("root_dirs_to_post") != string::npos)
        {
            size_t path_start_at = index + 18;
            size_t path_end_at = str.length() - 1;
            size_t path_length = path_end_at - path_start_at;
            //  TODO: should I add 1 char for null?
            p_temp = (char*)calloc(path_length, sizeof(char)) ;
            str.copy(p_temp, path_length, path_start_at);
            *root_path = p_temp;
        }
        else if (index = str.find("target_dir") != string::npos)
        {
            size_t path_start_at = index + 11;
            size_t path_end_at = str.length() - 1;
            size_t path_length = path_end_at - path_start_at;
            //  TODO: should I add 1 char for null?
            *target_path = (char*)calloc(path_length, sizeof(char)) ;
            str.copy(*target_path, path_length, path_start_at);
        }
    }

    return false;
}

bool scan_dirs(const char *dir_path) {
    return find_entries(dir_path, NULL, DT_DIR);
}

bool find_files(const char *dir_path) {
    return find_entries(dir_path, ".smi", DT_UNKNOWN);
}

bool find_entries(const char *dir_path, char *ext, int type)
{
    DIR *dir;
    struct dirent *ent;
    RawEntry* entry_list[100] = {};
    
    cout << endl << "find_files requested to " << dir_path << endl;
    // make all file for path
    //  if path valid??
    if ((dir = opendir(dir_path)) != NULL)
    {
        //  1. find specific files and let them respawn to custom objects.
        int i = 0;
        while ((ent = readdir(dir)) != NULL){
            if (type != NULL && ent->d_type != type) {
                continue;
            }
            string filename(ent->d_name);
            // FIXME: ext is located only on tail.
            if (ext != NULL && filename.find(ext) != string::npos) {
                cout << " + " << ent->d_type << ":" << ent->d_name << endl;
                RawEntry *entry = new RawEntry(dir_path, filename);
                entry_list[i] = entry;
                i++;
            } else {
                cout << " - " << (int)ent->d_type << ":" << ent->d_name << endl;
            }
        }
        cout << " total " << i << " entries has made." << endl << endl << endl;
        
        while (i > 0) {
            i--;
            cout << "   " << *(entry_list[i]->get_token_info()) << endl;
            delete entry_list[i];
        }
    }

    return true;
}
