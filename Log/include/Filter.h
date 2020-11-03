#ifndef  _FILTER_H_
#define _FILTER_H_

#include "LoggingEvent.h"

namespace log {

class Filter
{
public:
    typedef enum {
        DENY = -1,
        NEUTRAL = 0,
        ACCEPT = 1
    } Decision;

    Filter();
    virtual ~Filter();
    virtual void setChainedFilter(Filter* filter);
    virtual Filter* getChainedFilter();
    virtual Filter* getEndofChain();
    virtual void appendChainedFilter(Filter* filter);
    virtual Decision decide(const LoggingEvent& event);
protected:
    virtual Decision _decide(const LoggingEvent& event) = 0;
private:
    Filter* _chainedFilter;
};

}
#endif

