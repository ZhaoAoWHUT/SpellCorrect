#pragma once

#include "MutuxLock.h"
#include "Condition.h"
#include <functional>
#include <queue>

using namespace std;

static const int MaxTaskCount = 1000;

class TaskQueue
{
public:
    using Task = std::function<void()>;
public:
    TaskQueue();
    TaskQueue(unsigned int);
    
    bool TaskPushQueue(const Task&);
    bool TaskPushQueue(Task&&);
    Task TaskPopQueue();
    bool QueueIsEmpty() const;
    bool QueueIsFull() const;
    int  GetQueueSize() const;
    int  GetQueueMaxSize() const;

    void WakeUpALLWorkThread();
private:
    bool _flag;
    unsigned int _max_size;
    queue<Task> _que;
    MutuxLock _mutex;
    Condition _que_not_empty;
    Condition _que_not_full;
};

TaskQueue::TaskQueue()
    : _flag(true)
    , _max_size(MaxTaskCount)
    , _mutex()
    , _que_not_empty(_mutex)
    , _que_not_full(_mutex)
{

}

TaskQueue::TaskQueue(unsigned int sz)
    : _flag(true)
    , _max_size(sz)
    , _mutex()
    , _que_not_empty(_mutex)
    , _que_not_full(_mutex)
{

}

/* 在生产者线程进行运行 */
inline bool TaskQueue::TaskPushQueue(const Task& task)
{
    {
        MutuxLockGuard AutoLock(_mutex);
        while(QueueIsFull())
        {
            _que_not_full.SleepInConditon();
        }
        _que.push(task);
    }
    /* cout << "push task success" << endl; */
    _que_not_empty.NotifySleepInConditionOneThread();
    return true;
}

inline bool TaskQueue::TaskPushQueue(Task&& task)
{
    {
        MutuxLockGuard AutoLock(_mutex);
        while(QueueIsFull())
        {
            cout << ">>> work thread id : " << pthread_self()  << " sleep in conditon, task queue is full, need to wait..." << endl;
            _que_not_full.SleepInConditon();
        }
        _que.push(std::forward<Task>(task));
    }
    /* cout << "push task success" << endl; */
    _que_not_empty.NotifySleepInConditionOneThread();
    return true;
}    

/*在消费者线程进行运行*/
inline TaskQueue::Task TaskQueue::TaskPopQueue()
{
    Task task;
    {
        MutuxLockGuard AutoLock(_mutex);
        while(_flag && QueueIsEmpty())
        {
            cout << ">>> work thread id : " << pthread_self() << " sleep in conditon, task queue is empty, need to wait..." << endl;
            _que_not_empty.SleepInConditon();
        }

        if(_flag)
        {
            task = _que.front();
            _que.pop();
        }
        else
        {
            return nullptr;
        }

    }
    _que_not_empty.NotifySleepInConditionOneThread();
    return task;
}

inline bool TaskQueue::QueueIsEmpty() const
{
    return _que.empty();
}

inline bool TaskQueue::QueueIsFull() const
{
    return _que.size() == _max_size;
}

inline int TaskQueue::GetQueueSize() const
{
    return _que.size();
}

inline int TaskQueue::GetQueueMaxSize() const
{
    return _max_size;
}

void TaskQueue::WakeUpALLWorkThread()
{
    _flag = false;
    _que_not_empty.NotifySleepInConditionAllThread();
}

