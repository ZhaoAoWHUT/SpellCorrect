#ifndef  _LAYOUT_H_
#define _LAYOUT_H_

#include "LoggingEvent.h"

namespace log {

class Layout {
public:
    virtual ~Layout() {  };
    virtual std::string format(const LoggingEvent& event ) = 0;
};

}

#endif
