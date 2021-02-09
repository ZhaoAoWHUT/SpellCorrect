#pragma once

#include "log.h"
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

class MutuxLock
{
public:
    MutuxLock();
    ~MutuxLock();
    void InitLock();
    void InitLock(int mattr);
    void Lock();
    void Unlock();
    pthread_mutex_t* GetMutexLockPtr() {   return &_lock; }
    pthread_mutex_t& GetMutexLock() {   return _lock; }
    MutuxLock(const MutuxLock &) = delete;
    MutuxLock& operator=(const MutuxLock &) = delete;
private:
    pthread_mutex_t _lock;
    pthread_mutexattr_t _mattr;
};

MutuxLock::MutuxLock()
{
    if(pthread_mutex_init(&_lock, nullptr))
    {
        ERROR("pthread_mutex_init");
    }
    /* cout << "init in MutuxLock" << endl; */
}

void MutuxLock::InitLock()
{
    if(pthread_mutex_init(&_lock, nullptr))
    {
        ERROR("pthread_mutex_init");
    }
    /* cout << "init in InitLock" << endl; */
}

void MutuxLock::InitLock(int param)
{
    pthread_mutexattr_init(&_mattr);
    pthread_mutexattr_setpshared(&_mattr, param);
    if(pthread_mutex_init(&_lock, &_mattr))
    {
        ERROR("pthread_mutex_init");
    }

}

MutuxLock::~MutuxLock()
{
    if(pthread_mutex_destroy(&_lock))
    {
        ERROR("pthread_mutex_destroy");
    }
}

void MutuxLock::Lock()
{
    if(pthread_mutex_lock(&_lock))
    {
        ERROR("pthread_mutex_lock");
    }
}

void MutuxLock::Unlock()
{
    if(pthread_mutex_unlock(&_lock))
    {
        ERROR("pthread_mutex_unlock");
    }
}

class MutuxLockGuard
{
public:
    MutuxLockGuard(MutuxLock& m)
        : _mutex(m)
    {
        _mutex.Lock();
    }
    ~MutuxLockGuard()
    {
        _mutex.Unlock();
    }
private:
    MutuxLock& _mutex;
};



