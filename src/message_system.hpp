#ifndef SKELETON__MESSAGE_SYSTEM_H
#define SKELETON__MESSAGE_SYSTEM_H

#include "stream_as_string.hpp"

/* The different logging subsystems. These are defined in this manner so that
    we can get an enum and a string representation while only providing the
    name in one place throughout the source code.

    General is a special subsystem and must always be present. */
#define MS_SUBSYSTEMS \
    MS_SUBSYSTEM(general), \
    MS_SUBSYSTEM(api), \
    MS_SUBSYSTEM(interface), \
    MS_SUBSYSTEM(event), \
    MS_SUBSYSTEM(lua)

/* Log message types. Fatal is a special type that must always be present. */
#define MS_TYPES \
    MS_TYPE(debug), \
    MS_TYPE(log), \
    MS_TYPE(warning), \
    MS_TYPE(error), \
    MS_TYPE(fatal)

/** Message wrapper class. Provides public enums and some static
    logging functions. */
class message_system {
public:
    enum message_type {
#define MS_TYPE(type) type
        MS_TYPES
#undef MS_TYPE
    };

    enum subsystem {
#define MS_SUBSYSTEM(system) system
        MS_SUBSYSTEMS
#undef MS_SUBSYSTEM
    };
private:
    static const char *s_type_names[];
    static const char *s_subsystem_names[];
private:
    static std::ofstream *s_logfile;
public:
    static void set_logfile(std::string path);
    static void close_logfile();
public:
    static void message(subsystem system, message_type type, std::string msg);
};

#define MSG(msg) \
    MSG3(general, log, msg)

#define MSG2(type, msg) \
    MSG3(general, type, msg)

#define MSG3(sys, type, msg) \
    ::message_system::message( ::message_system::sys, \
        ::message_system::type, ::stream_as_string() << msg)

#endif
