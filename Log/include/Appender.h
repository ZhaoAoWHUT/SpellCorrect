#ifndef  _APPENDER_H_
#define _APPENDER_H_

#include <string>
#include <map>
#include <set>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "Priority.h"
#include "LoggingEvent.h"
#include "PThreads.h"
#include "Layout.h"

namespace log {

    class Appender
    {
        public:
            static Appender* getAppend(const std::string& name);
            static bool reopenall();
            static void closeall();
            virtual ~Appender();
            virtual void doAppend(const LoggingEvent& event) = 0;
            virtual bool reopen() = 0;
            virtual void close() = 0;
            virtual bool requiresLayout() const = 0;
            virtual void setLayout(Layout* layout) = 0;
            inline const std::string& getName() const { return _name;  };
            virtual void setThreshold(Priority::Value priority) = 0;
            virtual Priority::Value getThreshold() = 0;
        protected:
            Appender(const std::string& name);
        private:
            typedef std::map<std::string, Appender*> AppenderMap;
            static AppenderMap* _allAppenders;
            static threading::Mutex _appenderMapMutex;
            static AppenderMap& _getAllAppenders();
            static void _deleteAllAppenders();
            static void _addAppender(Appender* appender);
            static void _removeAppender(Appender* appender);
            const std::string _name;
    };
    
    typedef std::set<Appender*> AppenderSet;
}


#endif



