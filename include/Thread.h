#pragma once

#include <string>
#include <pthread.h>
#include <functional>

#include "log.h"

using namespace std;

class ThreadArgsData
{
public:
    using ThreadCallBack = std::function<void()>;
    ThreadArgsData(const ThreadCallBack&);
    ThreadArgsData(string name, const ThreadCallBack&);
    void RunThreadTask();
private:
    string _name;
    ThreadCallBack _callback_func;
};

ThreadArgsData::ThreadArgsData(const ThreadCallBack& cb)
    : _callback_func(cb)
{
    
}

ThreadArgsData::ThreadArgsData(string name, const ThreadCallBack& cb)
    : _name(name)
    , _callback_func(cb)
{
    
}

void ThreadArgsData::RunThreadTask()
{
    if(_callback_func)
    {
        cout << _name << " started." << endl;
        _callback_func();
    }
}


class Thread
{
public:
    using ThreadCallBack = std::function<void()>;
    Thread(const ThreadCallBack&);
    Thread();
    ~Thread();
    Thread(const Thread&) = delete;
    Thread& operator=(const Thread&) = delete;
    void StartRun();
    void JoinWait();
private:
    static void* ThreadFunc(void*);
private:
    pthread_t _pthid;
    ThreadCallBack _callback_func;
    bool _is_running;
};

Thread::Thread()
    : _is_running(false)
{

}

Thread::Thread(const ThreadCallBack& cb)
    : _callback_func(cb)
    , _is_running(false)
{

}

Thread::~Thread()
{
    if(_is_running)
    {
        pthread_detach(_pthid);
    }
}

void Thread::StartRun()
{
    ThreadArgsData* ptr = new ThreadArgsData("work thread", _callback_func);
    if(pthread_create(&_pthid, nullptr, ThreadFunc, ptr))
    {
        ERROR("pthread_create");
    }
    _is_running = true;
}

void Thread::JoinWait()
{
    if(_is_running)
    {
        pthread_join(_pthid, nullptr);
    }
    _is_running = false;
}

void* Thread::ThreadFunc(void* args)
{
    ThreadArgsData* ptr = (ThreadArgsData*)args;
    if(ptr)
    {
        ptr->RunThreadTask();
    }
    return nullptr;
}


