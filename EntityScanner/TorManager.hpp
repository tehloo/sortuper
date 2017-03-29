#include <iostream>
#include <thread>
#include <map>
#include "TorAdoptor.hpp"

namespace CmdList {
    enum e_CMD {
        _INVALID,
        RUN,
        STOP,
        INFO,
        QUIT,
    };
}

static std::map<std::string, int> m_CMD;
static bool b_init_m_cmd = false;

class TorManager {
private:
    std::thread* mt_monitor;
    TorAdaptor* tor;

    static void monitor_on_thread();

    void init_variables();
    void init_cmd();
    int cmd_receiver(char* cmd);
    void get_start_monitor();
    void stop_monitor();
    void get_torAdaptor();
    void show_tor_info();

public:
    static bool sb_run_thread;
    TorManager();
    void run();
};
