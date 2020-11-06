#ifndef _APPENDERSKELETON_H_
#define _APPENDERSKELETON_H_

#include "Appender.h"
#include "Filter.h"

namespace log {

class AppenderSkeleton : public Appender {

    public:

        virtual ~AppenderSkeleton();

        virtual void doAppend(const LoggingEvent& event);

        virtual bool reopen();

        virtual void close() = 0;

        virtual bool requiresLayout() const = 0;

        virtual void setLayout(Layout* layout) = 0;

        virtual void setThreshold(Priority::Value priority);

        virtual Priority::Value getThreshold();

        virtual void setFilter(Filter* filter);

        virtual Filter* getFiler();

    protected:

        AppenderSkeleton(const std::string& name);

        virtual void _append(const LoggingEvent& event) = 0;

    private:

        Priority::Value _threshold;

        Filter* _filter;
};

}


#endif
