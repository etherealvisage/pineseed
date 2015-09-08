#include <cstdlib>
#include <cstring>
#include <ctime>

#include <fstream>

#include "message_system.hpp"

std::ofstream *message_system::s_logfile;

const char *message_system::s_type_names[] = {
#define MS_TYPE(type) #type
    MS_TYPES
#undef MS_TYPE
};

const char *message_system::s_subsystem_names[] = {
#define MS_SUBSYSTEM(system) #system
    MS_SUBSYSTEMS
#undef MS_SUBSYSTEM
};

void message_system::set_logfile(std::string path) {
    std::string::size_type pos;
    if((pos = path.find("%d")) != std::string::npos) {
        int low = 0;
        int high = 10000;

        char buf[1000];
        std::string s;
        while(low != high) {
            int middle = (low+high)/2;

            std::sprintf(buf, "%05d", middle);
            s = path;
            s.replace(pos, 2, buf);

            std::ifstream input(s.c_str());
            if(input.is_open()) {
                if(low == middle) low = middle+1;
                else low = middle;
            }
            else {
                high = middle;
            }
        }
        std::sprintf(buf, "%05d", low);
        path.replace(pos, 2, buf);
    }

    s_logfile = new std::ofstream(path.c_str());
}

void message_system::close_logfile() {
    if(s_logfile) {
        delete s_logfile;
        s_logfile = NULL;
    }
}

void message_system::message(subsystem system, message_system::message_type type,
    std::string msg) {

    std::time_t t;
    std::time(&t);

    struct tm *lt = std::localtime(&t);

    char buffer[4096];
    /*if(Config::Tree::instance() &&
        Config::Tree::instance()->node("runtime.extended_log")->get<bool>(
            true)) {*/
    if(true) {

        std::snprintf(buffer, sizeof(buffer),
            "[%02i:%02i:%02i %-12s %-7s]         %s\n",
            lt->tm_hour, lt->tm_min, lt->tm_sec,
            s_subsystem_names[system], s_type_names[type],
            msg.c_str());
    }
    else {
        std::snprintf(buffer, sizeof(buffer), "%s\n", msg.c_str());
    }

    fprintf(stderr, "%s", buffer);
    if(s_logfile != NULL) {
        (*s_logfile) << buffer;
    }
    
    if(type == fatal) {
        close_logfile();
#ifdef DEBUG
        std::abort();
#else
        std::exit(1);
#endif
    }
}
