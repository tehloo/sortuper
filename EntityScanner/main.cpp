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
#include "EntryManager.hpp"
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
    //  cout << "get_paths : root_path=" << root_to_post << "/ target_path=" << target_path << endl;
    
    //  we will make some managers
    EntryManager *post_manager = new EntryManager(root_to_post, NULL, DT_DIR);
    EntryManager *target_manager = new EntryManager(target_path, "smi", DT_UNKNOWN);
    
    free(root_to_post); root_to_post = NULL;
    free(target_path); target_path = NULL;
    
    delete post_manager;
    delete target_manager;

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
