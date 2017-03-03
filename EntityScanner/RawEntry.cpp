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

bool RawEntry::set_token_index(int index) {
    if (this->token_size <= index) return false;
    this->token_index = index;
}

void RawEntry::strip_common_tokens(RawEntry* entry1, RawEntry* entry2) {
    entry1->init_token_index();
    entry2->init_token_index();

    string* token1 = NULL;
    string* token2 = NULL;
    while ((token1 = entry1->get_next_token()) != NULL
           && (token2 = entry2->get_next_token()) != NULL) {
        if (token1->compare(*token2) != 0)
            break;
    }
}

string* RawEntry::get_next_token() {
    return token_index == token_size - 1 ? NULL : token[++token_index];
}

int get_similar_score(const char* str1, const char* str2) {
    int score = 0;
    while (*str1 != 0) {
        while (*str2 != 0) {
            if (*str1 == *str2) {
                score++;
                str1++;
            }
            if (*str1 == 0)
                return score;
            str2++;
        }
        str1++;
    }
    return score;
}

#define BASE_SCORE_FOR_SEQUENCE 2

int RawEntry::compare_with(RawEntry* entry) {
    int score = 0;
    string *str_a, *str_b;
    strip_common_tokens(this, entry);
    int entry_stripped_index = entry->get_token_index();

    //  1. FIND ONE BY ONE
    int init_score = 0;
    while ((str_a = this->get_next_token()) != NULL) {
        entry->set_token_index(entry_stripped_index);
        while ((str_b = entry->get_next_token()) != NULL) {
            if (str_a->compare(*str_b) == 0 && str_a->length() > 1) {
                init_score += str_b->length() * 10;
            }
        }
    }

    //  2. MIND SEQUENCE
    strip_common_tokens(this, entry);
    int last_seq_a = 0;
    int last_seq_b = 0;

    int order_score = 0;
    while ((str_a = this->get_next_token()) != NULL) {
        int order_match = 0;
        entry->set_token_index(entry_stripped_index);
        while ((str_b = entry->get_next_token()) != NULL) {
            if (str_a->compare(*str_b) == 0) {
                cout << " " << *str_a;
                order_match++;
                order_score += (order_match * str_a->length() * 100);
                //  keep last token index which was sequentially same.
                last_seq_a = this->get_token_index();
                last_seq_b = entry->get_token_index();
                str_a = this->get_next_token();
                if (str_a == NULL) break;
            }
        }
    }

    //  3. INDEPTH COMPARE
    int depth_score = 0;
    this->set_token_index(last_seq_a);
    entry->set_token_index(last_seq_b);
    while ((str_a = this->get_next_token()) != NULL) {
        entry->set_token_index(last_seq_b);
        while ((str_b = entry->get_next_token()) != NULL) {
            depth_score += get_similar_score(str_a->c_str(), str_b->c_str());
        }
    }

    score = init_score == 0 ? 0 : init_score + order_score + depth_score;

#ifndef NO_DEBUG
    if (score > 0) {
        cout << " ... compare with " << entry->get_name() << endl;
        cout << "\t score = " << score;
        cout << "( = " << init_score << " + " << order_score << " + " << depth_score << ")";
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
    cout << "\n Entry " << this->get_name()
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
        } else if (score == score_board[top_index]) {
            best = NULL;
//            cout << " we have same score. ignore this score as best" << endl;
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
