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
#include <stdlib.h>

const char RawEntry::separators[10] = {' ', '|', '_', '-', '.', '/', 0,};

RawEntry::RawEntry() {
    init_variables();
}

RawEntry::RawEntry(string fullname) {
    init_variables();
    parse_name(fullname);
}

RawEntry::RawEntry(char* _path, char* _name) {
    init_variables(_path, _name);
    parse_to_tokens();
//    cout << "\tentry created " << m_name << endl;;
}

RawEntry::~RawEntry() {
    // cout << "RawEntry for " << this->getName() << " destructed" << endl;
}

void RawEntry::init_variables() {
    this->m_path = NULL;
    this->m_name = NULL;
    this->init_token_list();
    this->init_token_index();
    //this->pair = NULL;
}

char* alloc_and_copy(char* str) {
    return strcpy((char*)calloc(strlen(str) + 1, sizeof(char)), str);
}

void RawEntry::init_variables(char* _path, char* _name) {
    this->init_variables();
    this->m_path = alloc_and_copy(_path);
    this->m_name = alloc_and_copy(_name);
}

void RawEntry::parse_name(string fullname) {
    cout << "parse fullname for " << fullname << endl;
}

string RawEntry::get_full_path() {
    string rtn(m_path);
    rtn.append("/");
    rtn.append(m_name);
    return rtn;
}

void RawEntry::init_token_list() {
    this->token_size = 0;
    memset(this->token, 0x00, sizeof(token));
}

void RawEntry::parse_to_tokens() {
    this->init_token_list();

    char* sentance = (char*)calloc(strlen(m_path) + strlen(m_name) + 2, sizeof(char));
    strcpy(sentance, m_path);
    strcat(sentance, "/");
    strcat(sentance, m_name);

//    cout << "\tparse_to_token " << sentance << "\n\t";

    char* idx_back = sentance;
    char* idx_fore = sentance;

    while (1) {
        const char* sep = separators;
        size_t token_len = 0;
        //  find serators to make a token.
        while (*idx_fore != 0 && *idx_fore != *sep) {
            if (*(sep++) == 0) {
                idx_fore++;
                token_len++;
                sep = separators;
            }
        }
        if (token > 0 && idx_back != idx_fore) {
            token[token_size++] = new string(idx_back, token_len);
#ifndef NO_DEBUG
            cout << " #" << token_size << "\"" << *(token[token_size - 1]) << "\"";
#endif
        }

        //  it's end
        if (*idx_fore == 0) break;
        idx_fore++;

        //  skip separators, if it is continued.
        sep = separators;
        do {
            if (*sep == *idx_fore) {
                idx_fore++;
                sep = separators;
                continue;
            }
        } while (*(++sep) == 0);
        idx_back = idx_fore;
    }
    free(sentance);
    cout << endl;
}

ostringstream* RawEntry::get_entry_info() {
    ostringstream* str = new ostringstream();
    *str << m_name << "\n\t* path=" << this->get_name();
#ifndef NO_DEBUG
    *str << "\n\t* token size = " << token_size << "|";
    int i = 0;
    while (token[i] != NULL) {
        *str << " " << i << ":\"" << *(token[i]) << "\" ";
        i++;
    }
#endif
    return str;
}

void RawEntry::init_token_index() {
    token_index = ENTRY_INVALID;
}

string* RawEntry::get_next_token() {
    return token_index == token_size - 1 ? NULL : token[++token_index];
}


#define BASE_SCORE_FOR_SEQUENCE 2

int RawEntry::compare_with(RawEntry* entry) {
    int score = 0;
    string *str_a, *str_b;

    //  1. FIND ONE BY ONE
    this->init_token_index();
    while ((str_a = this->get_next_token()) != NULL) {
        entry->init_token_index();
        while ((str_b = entry->get_next_token()) != NULL) {
            if (str_a->compare(*str_b) == 0) {
                score++;
            }
        }
    }

    //  2. MIND SEQUENCE
    int order_score = BASE_SCORE_FOR_SEQUENCE;
    this->init_token_index();
    while ((str_a = this->get_next_token()) != NULL) {
        entry->init_token_index();
        while ((str_b = entry->get_next_token()) != NULL) {
            if (str_a->compare(*str_b) == 0) {
                order_score *= BASE_SCORE_FOR_SEQUENCE;
                score += order_score;
                str_a = this->get_next_token();
                if (str_a == NULL) break;
            }
        }
    }
#ifndef NO_DEBUG
    if (score > 0) {
        cout << " ... compare with " << entry->get_name() << endl;
        cout << "\t score = " << score;
        if (order_score > BASE_SCORE_FOR_SEQUENCE)
            cout << "(+" << order_score << ")";
        cout << endl;
    }
#endif
    return score;
}



DirEntry::DirEntry(char* _path, char* _name) : RawEntry(_path, _name) {
//    cout << " DirEntry created " << this->get_name() << endl;
}

FileEntry::FileEntry(char* _path, char* _name) : RawEntry(_path, _name) {
//    cout << " FileEntry created " << this->get_name() << endl;
}

DirEntry* FileEntry::select_post(EntryManager* em) {
    EntryManager* entries = new EntryManager(em);
    if (this->pair != NULL) {
        //  TODO: learn operator overloading.
        cout << "select_post will reset pair " << pair->get_name();
    }
    pair = NULL;
#ifndef NO_DEBUG
    cout << "Entry " << this->get_name()
         << " is selecting to post " << em->get_path() << endl;
#endif
    DirEntry *entry = NULL, *best = NULL;
    int score_board[MAX_ENTRY_SIZE] = {0,};
    int index = 0;
    int top_index = 0;
    while ((entry = (DirEntry*)entries->get_next_entry()) != NULL) {
        int score = compare_with(entry);
        if (score > score_board[top_index]) {
            top_index = index;
            best = entry;
        }
        score_board[index++] = score;
    }
#ifndef NO_DEBUG
    if (best == NULL) {
        cout << " No matches at all..." << endl;
    } else {
        cout << " select_post found best entry " << best->get_name() << endl;
    }
#endif
    pair = best;
    delete entries;

    return best;
}

string FileEntry::get_info() {
    ostringstream* str = this->get_entry_info();
    if (this->pair != NULL) {
        *str << "\n      -> paired with " << pair->get_full_path();
    }
    return str->str();
}

string DirEntry::get_info() {
    return this->get_entry_info()->str();
}
