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
#include <string.h>

const char RawEntry::separators[10] = {' ', '|', '_', '-', '.', 0,};

RawEntry::RawEntry() {
    init_variables();
}

RawEntry::RawEntry(string fullname) {
    init_variables();
    parse_name(fullname);
}

RawEntry::RawEntry(string path, string name) : path(path), name(name) {
    init_variables();
    parse_to_tokens();
}

RawEntry::~RawEntry() {
    // cout << "RawEntry for " << this->getName() << " destructed" << endl;
}

void RawEntry::init_variables()
{
    memset(token, 0x00, sizeof(token));
    this->init_token_index();
    pair = NULL;
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

string* RawEntry::get_token_info()
{
    ostringstream str;
    str << get_full_path();
    str << " | token size = " << token_size << "|";
    int i = 0;
    while (token[i] != NULL) {
        str << " " << i << ":\"" << *(token[i]) << "\" ";
        i++;
    }
    if (this->pair != NULL) {
        str << " -> " << pair->get_full_path();
    }
    str << endl;
    string* info = new string(str.str());
    return info;
}

RawEntry* RawEntry::select_post(EntryManager* em)
{
    EntryManager* entries = new EntryManager(em);
    if (pair != NULL) {
        //  TODO: learn operator overloading.
        cout << "select_post will reset pair " << pair->name;
    }
    pair = NULL;

    cout << "Entry " << this->name
         << " is selecting to post " << em->get_path() << endl;
    RawEntry *entry = NULL, *best = NULL;
    int score_board[MAX_ENTRY_SIZE] = {0,};
    int index = 0;
    int top_index = 0;
    while ((entry = entries->get_next_entry()) != NULL)
    {
        int score = compare_with(entry);
        if (score > score_board[top_index])
        {
            top_index = index;
            best = entry;
        }
        score_board[index++] = score;
    }
    if (best == NULL) {
        cout << " No matches at all..." << endl;
    } else {
        cout << " select_post found best entry " << best->getName() << endl;
        pair = best;
    }
    delete entries;

    return best;
}

void RawEntry::init_token_index()
{
    token_index = ENTRY_INVALID;
}

string* RawEntry::get_next_token()
{
    return token_index == token_size - 1 ? NULL : token[++token_index];
}

/**
 * compare_with
 *
 * 1.   `
 */
int RawEntry::compare_with(RawEntry* entry)
{
    int score = 0;
    cout << " ... compare " << name << " with " << entry->getName() << endl;
    string *str_a, *str_b;
    //  1. FIND ONE BY ONE
    this->init_token_index();
    while ((str_a = this->get_next_token()) != NULL)
    {
        entry->init_token_index();
        while ((str_b = entry->get_next_token()) != NULL)
        {
            if (str_a->compare(*str_b) == 0)
            {
                score++;
            }
        }
    }
    cout << " 1. score = " << score << endl;

    //  2. MIND SEQUENCE
    int order_score = 2;
    this->init_token_index();
    entry->init_token_index();
    while ((str_a = this->get_next_token()) != NULL)
    {
        while ((str_b = entry->get_next_token()) != NULL)
        {
            if (str_a->compare(*str_b) == 0)
            {
                score += order_score;
                order_score *= order_score;
                str_a = this->get_next_token();
                if (str_a == NULL) break;
            }
        }
    }
    cout << " 2. score = " << score << endl;
    return score;
}
