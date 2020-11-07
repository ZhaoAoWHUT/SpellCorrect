#ifndef _BASICLAYOUT_H_
#define _BASICLAYOUT_H_

#include "Layout.h"

namespace log {

class BasicLayout : public Layout 
{
    public:
        BasicLayout();
        virtual ~BasicLayout();
        virtual string format(const LoggingEvent& event);
};

}

#endif
