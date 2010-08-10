// Generic stream logger.
// -------------------------------------------------------------------
// Copyright (C) 2007 OpenEngine.dk (See AUTHORS)
//
// This program is free software; It is covered by the GNU General
// Public License version 2 or any later version.
// See the GNU General Public License for more details (see LICENSE).
// --------------------------------------------------------------------

#ifndef _OE_WII_LOGGER_H_
#define _OE_WII_LOGGER_H_

#include <Logging/ILogger.h>

namespace OpenEngine {
namespace Logging {

/**
 * Generic stream logger.
 * @todo Add some usage code to the documentation.
 *
 * @class StreamLogger StreamLogger.h Logging/StreamLogger.h
 */
class WiiLogger : public ILogger {
private:
public:
    WiiLogger();
    virtual ~WiiLogger();
    void Write(LoggerType, string);
    std::string TypeToString(LoggerType);
};

} //NS Logger
} //NS OpenEngine

#endif // _STREAM_LOGGER_H_
