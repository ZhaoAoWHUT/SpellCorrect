#pragma once

#include "Priority.h"
#include "TimeStamp.h"
#include "PThreads.h"

using namespace std;

namespace log {

class LoggingEvent
{
public:
    LoggingEvent() {}
    ~LoggingEvent() {}

    LoggingEvent(const string& category,
                 const string& message,
                 const string& ndc,
                 Priority::Value priority)
        : cateoryName(cateoryName),
          message(message),
          ndc(ndc),
          priority(priority),
          threadName(threading::getThreadId())
    {
    }

    const string cateoryName;
    const string message;
    const string ndc;
    Priority::Value priority;
    const string threadName;
    TimeStamp timeStamp;
private:

};

}
