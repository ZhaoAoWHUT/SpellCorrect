#include "../include/Appender.h"

namespace log {

Appender::AppenderMap* Appender::_allAppenders = nullptr;

threading::Mutex Appender::_appenderMapMutex;

Appender::AppenderMap& Appender::_getAllAppenders()
{
    threading::ScopedLock Lock(_appenderMapMutex);
    if(_allAppenders == nullptr)
    {
        _allAppenders = new AppenderMap;
    }
    return *_allAppenders;
}
    
Appender* Appender::getAppend(const std::string& name)
{
    threading::ScopedLock Lock(_appenderMapMutex);
    AppenderMap& allAppender = _getAllAppenders();
    AppenderMap::iterator it = allAppender.find(name);
    return (it == allAppender.end()) ? nullptr : (*it).second;
}

void Appender::_addAppender(Appender* appender)
{
    threading::ScopedLock Lock(_appenderMapMutex);
    /* AppenderMap& allAppender = Appender::_getAllAppenders(); */
    /* allAppender[appender->_name] = appender; */
    _getAllAppenders()[appender->getName()] = appender;
}

void Appender::_removeAppender(Appender* appender)
{
    threading::ScopedLock Lock(_appenderMapMutex);
    _getAllAppenders().erase(appender->getName());
    if(_getAllAppenders().size() == 0)
    {
        delete _allAppenders;
        _allAppenders = nullptr;
    }
}

bool Appender::reopenall()
{
    threading::ScopedLock Lock(_appenderMapMutex);
    AppenderMap& appender = _getAllAppenders();
    bool result = true;
    for(auto& it : appender)
    {
        result = result && it.second->reopen();
    }
    return result;
}

void Appender::closeall()
{
    threading::ScopedLock Lock(_appenderMapMutex);
    for(auto& it : _getAllAppenders())
    {
        it.second->close();
    }
}

}
