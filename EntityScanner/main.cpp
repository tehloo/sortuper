//
//  main.cpp
//  EntityScanner
//
//  Created by tehloo on 2017. 1. 15..
//  Copyright © 2017년 tehloo. All rights reserved.
//

#include <iostream>
#include <dirent.h>
using namespace std;

bool find_files( char *dir_path );

int main(int argc, const char * argv[]) {
    // insert code here...
    std::cout << "Hello, World!\n";
    
    // confirm path
    string root_path = "/Users/tehloo/Downloads";
    char root_path_char[] = "/Users/tehloo/Downloads";
    cout << "root path is " << root_path;
    
    find_files(root_path_char);
    
    
    
    // print out all entities
    
    return 0;
}

bool find_files( char *dir_path )
{
    DIR *dir;
    struct dirent *ent;
    
    // make all file for path
    //  if path valid??
    if ((dir = opendir(dir_path)) != NULL)
    {
        while ((ent = readdir(dir)) != NULL){
            cout << " - " << ent->d_name << endl;
        }
    }

    return true;
}
