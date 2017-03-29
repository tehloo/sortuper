#include <stdlib.h>
#include <string.h>
#include <vector>
#include <map>

using namespace std;

namespace StateList {
    enum e_STATUS {
        _INVALID,
        Idle,
        Stopped,
        Seeding,
        Up_n_Down,
        Downloading,
    };
}

static map<string, int> m_STATUS;
static bool bInit_m_STATUS = false;

class TorItem {
private:
    int mId;
    int mProgress;
    unsigned long long mSize;
    int mStatus;
    string* mName;

public:
    TorItem() : mId(-1), mProgress(-1), mSize(0)
              , mStatus(-1), mName(NULL) {};
    TorItem(int _id, int _progress,
            unsigned long long _size,
            int _status, string _name);
    TorItem(TorItem& _tor);
    ~TorItem();

    string get_info();
    int get_id()        {   return mId;     };
    string* get_name()  {   return mName;   };
    
    static TorItem* parse_to_tor(string& _str);
    bool update(TorItem& tor);
    bool is_identical(TorItem& tor);
    bool is_same(TorItem& tor);

};

class TorAdaptor {
private:
    char* mCmdGetList;
    vector<TorItem*>* mvTors;

    int update_to_list(TorItem* tor);

public:
    TorAdaptor();
    ~TorAdaptor();
    void print_tors();
    int load_tors();
};
