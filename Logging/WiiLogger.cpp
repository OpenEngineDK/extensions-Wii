// Generic stream logger.
// -------------------------------------------------------------------
// Copyright (C) 2007 OpenEngine.dk (See AUTHORS)
//
// This program is free software; It is covered by the GNU General
// Public License version 2 or any later version.
// See the GNU General Public License for more details (see LICENSE).
// --------------------------------------------------------------------

#include <Logging/WiiLogger.h>
#include <Meta/Time.h>
#include <Utils/DateTime.h>

#include <string.h>
#include <sstream>

extern "C" {
    int net_print_string( const char* file, int line, const char* format, ...);
}


namespace OpenEngine {
namespace Logging {

/**
 * Create a logger wrapping an output stream.
 *
 * @param stream Wii to use as log output.
 */
WiiLogger::WiiLogger() {

}

/**
 * Destruct the stream logger.
 * Flushes the stream.
 */
WiiLogger::~WiiLogger(){
}

/**
 * Write a log message.
 *
 * @param type Log message type.
 * @param msg Message to log.
 */
void WiiLogger::Write(LoggerType type, string msg) {
    std::ostringstream out;
    

    out << TypeToString(type) << " ";
    out << Utils::DateTime::GetCurrent() << ": ";
    out << msg << std::endl;   

    string s = out.str();

    net_print_string(NULL, 0, s.c_str());
}

/**
 * Get string representation for a log message type.
 *
 * @param type Log message type.
 * @return String representation of type.
 */
string WiiLogger::TypeToString(LoggerType type){
    string str;
    if (type == Error)
        str ="[EE]";
    else if (type == Warning)
        str = "[WW]";
    else if (type == Info)
        str = "[II]";
    else {
        str = "[";
        str += type;
        str += "]";
    }
    return str;
}

} //NS Logging
} //NS OpenEngine
