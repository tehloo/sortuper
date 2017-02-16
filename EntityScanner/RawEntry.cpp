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

RawEntry::RawEntry(string path, string name) : path(path), name(name) {
    RawEntry();
    parse_to_tokens();
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
        int i_sep = 0;  //  index for separators
        min_pos = 0;    //  earliest position between separators
        
        char spt;
        //  searching separators on target string 1 by 1, them choose earliest one 
        //  to make it as a token.
        while ((spt = separators[i_sep]) != 0) {
            size_t pos = target.find_first_of(spt);
            if (min_pos == 0 || min_pos > pos) {
                min_pos = pos;
//                cout << "  found seperate '" << spt << "' on " << pos << endl;
            }
            i_sep++;
        }
        //  it is not the end of target string.
        if (min_pos != string::npos) {
            token[token_size] = new string(target.substr(0, min_pos).c_str());
//            cout << i_sep << " > " << token[token_size] << " from " << target << endl;
            
            //  really??
            if (target.size() <= min_pos) break;
            target = target.substr(min_pos+1);
        } else {
            token[token_size] = new string(target);
        }
        token_size++;
    }
}

string* RawEntry::get_token_info() {
    ostringstream str;
    str << get_full_path();
    str << " | token size = " << token_size << "|";
    int i = 0;
    while (token[i] != NULL) {
        str << " " << i << ":\"" << *(token[i]) << "\" ";
        i++;
    }
    str << endl;
    string* info = new string(str.str());
    return info;
}
