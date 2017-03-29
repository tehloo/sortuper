#include <iostream>
#include <sstream>
#include <fstream>
#include "TorAdoptor.hpp"

#define LOAD_FROM_FILE 1

using namespace std;


TorItem::TorItem(int _id, int _progress, unsigned long long _size, int _status, string _name)
        : mId(_id), mProgress(_progress), mSize(_size), mStatus(_status) {
    mName = new string(_name);
}

TorItem::TorItem(TorItem& _tor) : mId(_tor.mId),
                             mProgress(_tor.mProgress),
                             mSize(_tor.mSize),
                             mStatus(_tor.mStatus) {
    this->mName = new string(*(_tor.mName));
}

TorItem::~TorItem() {
    cout << "deleting " << *mName << endl;
    delete mName;
}

string TorItem::get_info() {
    ostringstream* ost = new ostringstream();
    *ost << mId << ". " << *mName << "/ size=" << mSize << "/ status=" << mStatus;
    return ost->str();        
}

bool TorItem::is_identical(TorItem& tor) {
    if (this->get_id() == tor.get_id()
        && this->get_name() != NULL
        && this->get_name()->compare(*(tor.get_name())) == 0) {
        return true;
    }
    return false;
}

bool TorItem::is_same(TorItem& tor) {
    if (is_identical(tor)
        && this->mProgress == tor.mProgress
        && this->mSize == tor.mSize
        && this->mStatus == tor.mStatus)
        return true;
    return false;
}

bool TorItem::update(TorItem& tor) {
    if (!this->is_identical(tor))
        return false;

    TorItem prev(*this);

    this->mProgress = tor.mProgress;
    this->mSize = tor.mSize;
    this->mStatus = tor.mStatus;

    return !(this->is_same(prev));
}


TorAdaptor::TorAdaptor() {
    if (!bInit_m_STATUS) {
        m_STATUS["Idle"] = StateList::Idle;
        m_STATUS["Stopped"] = StateList::Stopped;
        m_STATUS["Seeding"] = StateList::Seeding;
        m_STATUS["Up & Down"] = StateList::Up_n_Down;
        m_STATUS["Downloading"] = StateList::Downloading;

        bInit_m_STATUS = true;
    }
    this->mvTors = new vector<TorItem*>();
}

TorAdaptor::~TorAdaptor() {
    cout << endl << " deleting TorAdaptor" << endl;
    for(vector<TorItem*>::iterator it = this->mvTors->begin();
        it < mvTors->end(); it++) {
        delete (*it);
    }
    delete this->mvTors;
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
    unsigned int i = 0;
    while (i < sizeof(*buf)) {
        value += (10 ^ len--) * buf[i++];
    }
    return value;
}

TorItem* TorItem::parse_to_tor(string& _str) {
    int length = _str.length();
    if (length == 0) return NULL;
    TorItem* tor = new TorItem();
    int field_count = 0;

    for (int i = 0; i < length; i++) {
        const char* str = _str.c_str() + i;
        if (*str == ' ') {
            continue;
        } else if (tor->mId == -1) {
            //  ID
            field_count = 1;
            tor->mId = atoi(str);
            i = 4;
        } else if (tor->mProgress == -1) {
            //  ID
            field_count = 2;
            tor->mProgress = atoi(str);
            i = 55;     //  goes to Status
/*            
        } else if (tor->mSize == 0) {
            field_count = 3;
            tor->mSize = atof(str);
            if (*(_str.c_str() + 21) == 'G'
                && *(_str.c_str() + 23) == 'B') tor->mSize *= 1024* 1024* 1024;

            i = 57;
*/
        } else if (tor->mStatus == -1) {
            char status[32] = {0,};
            unsigned int i_status = 0;
            while (i_status < sizeof(status) / sizeof(char)) {
                if (str[i_status] == ' ' && str[i_status+1] == ' ') break;
                else status[i_status] = str[i_status];
                i_status++;
            }
            i += i_status < 32 ? i_status : 0 ;
            int t_status = m_STATUS[status];
            tor->mStatus = (t_status > StateList::_INVALID
                            && t_status <= StateList::Downloading)
                            ? m_STATUS[status] : StateList::_INVALID;
        } else if (tor->mName == NULL) {
            tor->mName = new string(str);
            break;
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
        count_item = this->update_to_list(tor) > 0 ? count_item + 1 : count_item;
    }
    return -1;
    
}

/**
 *  update_to_list
 *
 *  return item index if it is added.
 *  return -1 if item is alread exist or not added.
 */
int TorAdaptor::update_to_list(TorItem* tor) {
    if (tor == NULL) return -1;

    for (vector<TorItem*>::iterator it = mvTors->begin();
         it < mvTors->end(); it++) {
        if ((*it)->is_identical(*tor)) {
            return (*it)->update(*tor) ? (*it)->get_id() : -1;
        }
    }
    mvTors->push_back(tor);
    return tor->get_id();
}
