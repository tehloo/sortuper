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
    cout << "\n\nHello, SORTUPER!\n\n";
    char* root_to_post = NULL;
    char* target_path = NULL;
    get_paths(&root_to_post, &target_path);
    //  cout << "get_paths : root_path=" << root_to_post << "/ target_path=" << target_path << endl;
    if (root_to_post == NULL || target_path == NULL) {
        cout << "Please check preferences..."
                << " root_to_post = " << (root_to_post == NULL ? "NULL" : root_to_post)
                << " / target_path = " << (target_path == NULL ? "NULL" : target_path)
                << endl;
        if (root_to_post != NULL) free(root_to_post);
        if (target_path != NULL) free(target_path);
        return 0;
    }
    
    //  filter needed...
    EntryFilter* filter_post = new EntryFilter(DT_DIR, NULL);
    EntryFilter* filter_target = new EntryFilter(DT_UNKNOWN, "mkv|avi|mp3|mp4");
    //  we will make some managers
    EntryManager* post = new EntryManager(root_to_post, filter_post);
    EntryManager* target = new EntryManager(target_path, filter_target);
    
    free(root_to_post); root_to_post = NULL;
    free(target_path); target_path = NULL;
#ifndef NO_DEBUG
    post->print_info();
    target->print_info();
#endif
    target->set_post(post);
    target->print_info();

    //  TODO: Those need to go to destructor/
    post->empty_list();
    target->empty_list();
    
    delete post;
    delete target;
    delete filter_post;
    delete filter_target;

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
            p_temp = (char*)calloc(path_length + 1, sizeof(char)) ;
            str.copy(p_temp, path_length, path_start_at);
            *root_path = p_temp;
        }
        else if (index = str.find("target_dir") != string::npos)
        {
            size_t path_start_at = index + 11;
            size_t path_end_at = str.length() - 1;
            size_t path_length = path_end_at - path_start_at;
            //  TODO: should I add 1 char for null?
            *target_path = (char*)calloc(path_length + 1, sizeof(char)) ;
            str.copy(*target_path, path_length, path_start_at);
        }
    }

    return false;
}
