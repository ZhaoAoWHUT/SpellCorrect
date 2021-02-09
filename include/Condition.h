#pragma once

#include <pthread.h>
#include "MutuxLock.h"

class Condition
{
public:
    /*constructor for 'Condition' must explicitly initialize the reference member '_mutex'*/
    /* Condition() {} is error */  
    Condition(MutuxLock&);
    ~Condition();

    Condition(const Condition&) = delete;
    Condition& operator=(const Condition&) = delete;

    void SleepInConditon();

    void NotifySleepInConditionOneThread();

    void NotifySleepInConditionAllThread();

private:
    pthread_cond_t _cond;
    MutuxLock& _mutex;     /* must use reference, forbid copy and assign for mutexlock*/
};

Condition::Condition(MutuxLock& __mutex)
    : _mutex(__mutex)
{
    if(pthread_cond_init(&_cond, nullptr))
    {
        ERROR("pthread_cond_init");
    }
}

Condition::~Condition()
{
    if(pthread_cond_destroy(&_cond))
    {
        ERROR("pthread_cond_destroy");
    }
}

void Condition::SleepInConditon()
{
    if(pthread_cond_wait(&_cond, _mutex.GetMutexLockPtr()))
    {
        ERROR("pthread_cond_wait");
    }
}

void Condition::NotifySleepInConditionOneThread()
{
    if(pthread_cond_signal(&_cond))
    {
        ERROR("pthread_cond_signal");
    }
}

void Condition::NotifySleepInConditionAllThread()
{
    if(pthread_cond_broadcast(&_cond))
    {
        ERROR("pthread_cond_broadcast");
    }
}
