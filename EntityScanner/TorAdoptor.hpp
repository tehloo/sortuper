#include <stdlib.h>
#include <string.h>
#include <vector>

using namespace std;

enum STATUS {
    _INVALID,
    Downloading,
    Stopped,
};

class TorItem {
private:
    int mId;
    int mProgress;
    unsigned long long mSize;
    STATUS mStatus;
    string* mName;

public:
    TorItem() : mId(-1), mProgress(-1), mSize(0), mStatus(_INVALID), mName(NULL) {};
    TorItem(int _id, int _progress, unsigned long long _size, STATUS _status, string _name);

    string get_info();
    static TorItem* parse_to_tor(string& _str);

};

class TorAdaptor {
private:
    char* mCmdGetList;
    vector<TorItem*>* mvTors;

public:
    TorAdaptor();
    void print_tors();
    int load_tors();
};
