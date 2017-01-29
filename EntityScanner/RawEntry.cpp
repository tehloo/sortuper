//
//  RawEntry.cpp
//  EntityScanner
//
//  Created by tehloo on 2017. 1. 15..
//  Copyright © 2017년 tehloo. All rights reserved.
//

#include "RawEntry.hpp"
#include <iostream>
#include <sstream>


RawEntry::RawEntry() {
    token_size = 0;
}

RawEntry::RawEntry(string fullname) {
    
    parse_name(fullname);
}

RawEntry::~RawEntry() {
    // cout << "RawEntry for " << this->getName() << " destructed" << endl;
}

void RawEntry::parse_name(string fullname) {
    cout << "parse fullname for " << fullname << endl;
}

string RawEntry::get_full_path() {
    string rtn(path);
    rtn.append("/");
    rtn.append(name);
    return rtn;
}

void RawEntry::parse_to_tokens() {
    string target = name;
    size_t min_pos = 0;
    
    while (target.size() > min_pos) {
        int i_sep = 0;
        min_pos = 0;
        
        char spt;
        while ((spt = separators[i_sep]) != 0) {
            size_t pos = target.find_first_of(spt);
            if (min_pos == 0 || min_pos > pos) {
                min_pos = pos;
//                cout << "  found seperate '" << spt << "' on " << pos << endl;
            }
            i_sep++;
        }
        if (min_pos != target.npos) {
            token[token_size] = new string(target.substr(0, min_pos).c_str());
             
//            cout << i_sep << " > " << token[token_size] << " from " << target << endl;
            if (target.size() <= min_pos) break;
            target = target.substr(min_pos+1);
            token_size++;
        }
    }
}

string* RawEntry::get_token_info() {
    ostringstream str;
    str << get_full_path() << endl;
    str << "token size = " << token_size << endl;
    int i = 0;
    while (token[i] != NULL) {
        str << " " << *(token[i]);
        i++;
    }
    str << endl;
    string* info = new string(str.str());
    return info;
}
