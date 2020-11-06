#include "../include/AppenderSkeleton.h"

namespace log {


AppenderSkeleton::AppenderSkeleton(const std::string& name)
    : Appender(name),
      _threshold(Priority::NOTSET),
      _filter(nullptr)
{

}

AppenderSkeleton::~AppenderSkeleton()
{
    if(_filter)
        delete _filter;
}

bool AppenderSkeleton::reopen()
{
    return true;
}

void AppenderSkeleton::doAppend(const LoggingEvent& event)
{
    if((Priority::NOTSET == _threshold) || (event.priority <= _threshold))
    {
        if(!_filter || (_filter->decide(event) != Filter::DENY))
        {
            _append(event);
        }
    }
}

void AppenderSkeleton::setThreshold(Priority::Value priority)
{
    _threshold = priority;
}

Priority::Value AppenderSkeleton::getThreshold()
{
    return _threshold;
}

void AppenderSkeleton::setFilter(Filter* filter)
{
    if(_filter != filter)
    {
        if(_filter)
            delete _filter;
        _filter = filter;
    }
}

Filter* AppenderSkeleton::getFiler()
{
    return _filter;
}

}
