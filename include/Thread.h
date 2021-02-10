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
    Thread(const ThreadCallBack&, int);
    Thread(const ThreadCallBack&, string);
    Thread();
    ~Thread();
    Thread(const Thread&) = delete;
    Thread& operator=(const Thread&) = delete;
    void StartRun();
    void JoinWait();
    pthread_t GetThreadId() const;
private:
    static void* ThreadFunc(void*);
private:
    pthread_t _pthid;
    ThreadCallBack _callback_func;
    bool _is_running;
    string _idx;
};

Thread::Thread()
    : _is_running(false)
{

}

Thread::Thread(const ThreadCallBack& cb)
    : _callback_func(cb)
    , _is_running(false)
    , _idx()
{

}

Thread::Thread(const ThreadCallBack& cb, int idx)
    : _callback_func(cb)
    , _is_running(false)
    , _idx(to_string(idx))
{

}

Thread::Thread(const ThreadCallBack& cb, string idx)
    : _callback_func(cb)
    , _is_running(false)
    , _idx(idx)
{

}

Thread::~Thread()
{
    if(_is_running)
    {
        pthread_detach(_pthid);
    }
}

pthread_t Thread::GetThreadId() const
{
    return _pthid;
}

void Thread::StartRun()
{
    string name = "work thread " + _idx;
    ThreadArgsData* ptr = new ThreadArgsData(name, _callback_func);
    if(pthread_create(&_pthid, nullptr, ThreadFunc, ptr))
    {
        ERROR("pthread_create");
    }
    _is_running = true;
}

/* 当线程退出的时候均不会释放其占用的资源等等，包括相应的描述符等等，
 * 只有通过调用pthread_join函数来堵塞当前线程直到目标进程结束，
 * 最后会把相应的资源回收，否则如果没有调用join那么线程的状态就类似于线程的僵死状态。*/
void Thread::JoinWait()
{
    if(_is_running)
    {
        pthread_join(_pthid, nullptr);
    }
    cout << ">>> join thread id : " << _pthid << endl;
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


