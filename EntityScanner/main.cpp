//
//  main.cpp
//  EntityScanner
//
//  Created by tehloo on 2017. 1. 15..
//  Copyright © 2017년 tehloo. All rights reserved.
//

#include <iostream>
#include <dirent.h>
#include "RawEntry.hpp"
using namespace std;

bool find_files(const char *dir_path );

int main(int argc, const char * argv[]) {
    // insert code here...
    std::cout << "Hello, World!\n";
    
    // confirm path
    string root_path = "/Volumes/tehloo mac HDD/downloads";
    const char* root_path_char = root_path.c_str();
    cout << "root path is " << root_path << endl;
    
    find_files(root_path_char);
    
    
    
    // print out all entities
    
    return 0;
}

bool find_files(const char *dir_path )
{
    DIR *dir;
    struct dirent *ent;
    RawEntry* entry_list[100] = {};
    
    // make all file for path
    //  if path valid??
    if ((dir = opendir(dir_path)) != NULL)
    {
        //  1. find specific files and let them respawn to custom objects.
        int i = 0;
        while ((ent = readdir(dir)) != NULL){
            string filename(ent->d_name);
            // FIXME: ext is located only on tail.
            if (filename.find(".smi") != string::npos) {
                cout << " + " << ent->d_name << endl;
                RawEntry *entry = new RawEntry(dir_path, filename);
                entry_list[i] = entry;
                i++;
            } else
                cout << " - " << ent->d_name << endl;
        }
        cout << " total " << i << " entries has made." << endl;
        
        while (i > 0) {
            i--;
            cout << entry_list[i]->get_full_path() << endl;
            free(entry_list[i]);
        }
    }

    return true;
}
