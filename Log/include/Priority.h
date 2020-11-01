# ifndef _LOG_PRIORITY_H_
# define _LOG_PRIORITY_H_

#include <string>
#include <stdexcept>

namespace log {

    class Priority
    {
        public:
            Priority() {}
            ~Priority() {}
            
            static const int MESSAGE_SIZE;

            typedef enum {
                EMERG = 0,
                FATAL  = 0,
                ALERT  = 100,
                CRIT   = 200,
                ERROR  = 300,
                WARN   = 400,
                NOTICE = 500,
                INFO   = 600,
                DEBUG  = 700,
                NOTSET = 800
            } PriorityLevel;

            typedef int Value;

            static const std::string& getPriorityName(int priority) throw();
            static Value getPriorityValue(const std::string& priorityName) throw();
        private:

    };
}
#endif
