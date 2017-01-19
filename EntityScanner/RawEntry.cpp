//
//  RawEntry.cpp
//  EntityScanner
//
//  Created by tehloo on 2017. 1. 15..
//  Copyright © 2017년 tehloo. All rights reserved.
//

#include "RawEntry.hpp"
#include <iostream>


RawEntry::RawEntry(string fullname) {
    parse_name(fullname);
}

void RawEntry::parse_name(string fullname) {
    cout << "parse fullname for " << fullname << endl;
}

string RawEntry::get_full_path() {
    string rtn(path);
    rtn.append(name);
    return rtn;
}
