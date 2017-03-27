#include <unistd.h>
#include <cctype>

#include "TorManager.hpp"

#define SLEEP_DURATION 1
#define MAX_CMD_LENGTH 256

TorManager::TorManager() {
    std::cout << std::endl << " initialing TorManager ...." << std::endl;
    this->init_variables();
    this->init_cmd();
}

bool TorManager::sb_run_thread = false;
void TorManager::init_variables() {
    this->mt_monitor = NULL;
}

void TorManager::init_cmd() {
    if (b_init_m_cmd)
        return;

    m_CMD["run"] = CmdList::RUN;
    m_CMD["stop"] = CmdList::STOP;
    m_CMD["info"] = CmdList::INFO;
    m_CMD["quit"] = CmdList::QUIT;
    b_init_m_cmd = true;
}

const std::string get_cur_time() {
    time_t t = time(0);
    struct tm *timeinfo = localtime(&t);
    char buf[80];

    strftime(buf, sizeof(buf), "%Y/%m/%d %X", timeinfo);
    return buf;
}

void TorManager::run() {
    char cmd[MAX_CMD_LENGTH];
    std::cout << std::endl << " running TorManager ...." << std::endl;
    while (true) {
        std::cin.getline(cmd, MAX_CMD_LENGTH);
        if (cmd_receiver(cmd) == CmdList::QUIT)
            break;


//        sleep(SLEEP_DURATION);
    }
}

void cmd_to_lower(char* cmd) {
    int i = 0;
    while (cmd[i]) {
        cmd[i] = tolower(cmd[i]);
        i++;
    }
}

void TorManager::get_start_monitor() {
    if (this->mt_monitor != NULL || sb_run_thread) {
        std::cout << "get_start_monitor can not start thread" << std::endl;
        return;
    }
    TorManager::sb_run_thread = true;
    this->mt_monitor = new std::thread(TorManager::monitor_on_thread);
}

void TorManager::stop_monitor() {
    if (this->mt_monitor == NULL || !TorManager::sb_run_thread) {
        std::cout << "monitor is not running..." << std::endl;
        return;
    }
    TorManager::sb_run_thread = false;
    this->mt_monitor->join();
    std::cout << " monitor thread finished." << std::endl;
}

void TorManager::monitor_on_thread() {
    while(TorManager::sb_run_thread) {
        std::cout << get_cur_time << std::endl;
        sleep(SLEEP_DURATION);
    }
    std::cout << std::endl << "finishing monitor_on_thread..." << std::endl;
}

int TorManager::cmd_receiver(char* cmd) {
//    std::cout << get_cur_time() << " > " << cmd << std::endl;
    cmd_to_lower(cmd);
    std::map<std::string, int>::iterator it = m_CMD.find(cmd);
    if (it == m_CMD.end()) {
        std::cout << "unknown command." << std::endl;
        return CmdList::_INVALID;
    }

    switch(it->second) {
    case CmdList::RUN:
        get_start_monitor();
        break;
    case CmdList::STOP:
        stop_monitor();
        break;

    case CmdList::INFO:
        std::cout << "not ready yet" << std::endl;
        break;

    case CmdList::QUIT:
        std::cout << "quit TorManager" << std::endl;
        break;
    default:
        std::cout << "quit any how..." << std::endl;
        break;
    }

    return it->second;
}

