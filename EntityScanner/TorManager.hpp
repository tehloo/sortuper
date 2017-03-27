#include <iostream>
#include <thread>
#include <map>

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

    static bool sb_run_thread;
    static void monitor_on_thread();

    void init_variables();
    void init_cmd();
    int cmd_receiver(char* cmd);
    void get_start_monitor();
    void stop_monitor();

public:
    TorManager();
    void run();
};
