#include <iostream>
#include <sstream>
#include <fstream>
#include "TorAdoptor.hpp"

#define LOAD_FROM_FILE 1

using namespace std;


TorItem::TorItem(int _id, int _progress, unsigned long long _size, STATUS _status, string _name)
        : mId(_id), mProgress(_progress), mSize(_size), mStatus(_status) {
    mName = new string(_name);
}

string TorItem::get_info() {
    ostringstream* ost = new ostringstream();
    *ost << mId << ". " << mName << "/ size=" << mSize << "/ status=" << mStatus;
    return ost->str();        
}


TorAdaptor::TorAdaptor() {
    this->mvTors = new vector<TorItem*>();
}

void TorAdaptor::print_tors() {
    for (vector<TorItem*>::iterator it = mvTors->begin();
         it < mvTors->end(); it++) {
        cout << "    " << (*it)->get_info() << endl;
    }
}

int get_char_to_int(char* buf) {
    int len = sizeof(*buf) - 1;
    int value = 0;
    int i = 0;
    while (i < sizeof(*buf)) {
        value += (10 ^ len--) * buf[i++];
    }
    return value;
}

TorItem* TorItem::parse_to_tor(string& _str) {
    int length = _str.length();
    TorItem* tor = new TorItem();
    int field_count = 0;

    for (int i = 0; i < length; i++) {
        const char* str = _str.c_str() + i;
        if (*str != ' ') {
            continue;
        } else if (tor->mId == -1) {
            //  ID
            field_count = 1;
            tor->mId = atoi(str);
        } else if (tor->mProgress == -1) {
            //  ID
            field_count = 2;
            tor->mProgress = atoi(str);
        } else if (tor->mSize == 0) {
            field_count = 3;
            tor->mSize = atof(str);
            if (*(_str.c_str() + 21) == 'G'
                && *(_str.c_str() + 23) == 'B') tor->mSize *= 1024* 1024* 1024;

            i = 57;
        } else if (tor->mStatus == _INVALID) {
            if (strcmp((str + 1), "Stopped") == 0) {
                tor->mStatus = Stopped;
            }
            i = 70;
        } else if (tor->mName == NULL) {
            const char* name = str + 1;
            tor->mName = new string(name);
        }
    }
    return tor;
}

int TorAdaptor::load_tors() {
#ifdef LOAD_FROM_FILE
    ifstream file("./transmission.result");
    string str;
#endif
    bool startLoading = false;
    int count_item = 0;
    while (getline(file, str)) {
        if (str.find("ID") == 0) {
            startLoading = true;
            continue;
        } else if (str.find("Sum:") == 0) {
            startLoading = false;
            return count_item;
        } else if (!startLoading) {
            continue;
        }
        TorItem* tor = TorItem::parse_to_tor(str);
        if (tor != NULL) {
            this->mvTors->push_back(tor);
            count_item++;
        }
    }
    return -1;
    
}
