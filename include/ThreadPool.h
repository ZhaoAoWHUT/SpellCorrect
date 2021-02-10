#pragma once

#include <list>
#include <atomic>
#include <memory>
#include <thread>
#include "Thread.h"
#include "TaskQueue.h"

using namespace std;

static const int CPUNumber = std::thread::hardware_concurrency();

class ThreadPool
{
public:
    using Task = std::function<void()>;
public:
    ThreadPool();
    ThreadPool(int);
    ~ThreadPool();

    void ThreadPoolRunStart(int);
    void ThreadPoolRunStop();
    void RunInThread();
    Task GetTask();
    void AddTask(Task&& task);
    void AddTask(const Task& task);
private:
    list<shared_ptr<Thread>> _thread_group;
    TaskQueue _task_queue;
    atomic_bool _running;
};

ThreadPool::ThreadPool()
    : _task_queue()
{
    ThreadPoolRunStart(CPUNumber);
}

ThreadPool::ThreadPool(int num)
    : _task_queue()
{
    ThreadPoolRunStart(num);
}

ThreadPool::~ThreadPool()
{
    /* before destory threadpool, if threadpool is running, call stop func, clear resource*/
    if(_running)
    {
        ThreadPoolRunStop();
    }
}

void ThreadPool::ThreadPoolRunStart(int threadNum)
{
    _running = true;
    //create thread group
    for(int i = 0; i < threadNum; i++)
    {
        _thread_group.push_back(make_shared<Thread>(std::bind(&ThreadPool::RunInThread, this), to_string(i+1)));
    }
    cout << ">>> init thread pool, thread number : " << threadNum << endl;
    for(auto & _thread : _thread_group)
    {
        _thread->StartRun();
    }
}

void ThreadPool::ThreadPoolRunStop()
{
    if(_running)
    {
        if(!_task_queue.QueueIsEmpty())
        {
            cout << ">>> remain task number : " << _task_queue.GetQueueSize() << endl;
        }
        
        _running = false;
        _task_queue.WakeUpALLWorkThread();

        cout << ">>> stop task run in thread" << endl;

        for(auto& _thread : _thread_group)
        {
           _thread->JoinWait(); 
        }

        _thread_group.clear();

        cout << ">>> thread pool run stop" << endl;
    }
}

void ThreadPool::RunInThread()
{
    while(_running)
    {
       Task task = GetTask();
       if(task != nullptr)
       {
            task();
       }
    }
}

inline ThreadPool::Task ThreadPool::GetTask()
{
    return _task_queue.TaskPopQueue();
}

inline void ThreadPool::AddTask(Task&& task)
{
    _task_queue.TaskPushQueue(std::forward<Task>(task));
}

inline void ThreadPool::AddTask(const Task& task)
{
    _task_queue.TaskPushQueue(task);
}

