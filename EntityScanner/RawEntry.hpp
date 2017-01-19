//
//  RawEntry.hpp
//  EntityScanner
//
//  Created by tehloo on 2017. 1. 15..
//  Copyright © 2017년 tehloo. All rights reserved.
//

#ifndef RawEntry_hpp
#define RawEntry_hpp

#include <string>
using namespace std;

class RawEntry {
private:
    string path;
    string name;
    
    void parse_name(string fullname);
    
public:
    RawEntry(string fullname);
    RawEntry(string path, string name) : path(path), name(name) {}
    
    string getName() { return name; }
    string get_full_path();
};


#endif /* RawEntry_hpp */
