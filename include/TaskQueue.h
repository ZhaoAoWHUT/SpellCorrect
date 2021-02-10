#pragma once

#include "MutuxLock.h"
#include "Condition.h"
#include <functional>
#include <queue>

using namespace std;

class TaskQueue
{
public:
    using Task = std::function<void()>;
public:
    TaskQueue(unsigned int);
    
    bool ElemPushQueue(const Task&);
    Task ElemPopQueue();
    bool QueueIsEmpty() const;
    bool QueueIsFull() const;
    int  GetQueueSize() const;
    int  GetQueueMaxSize() const;

    void WakeUpALLConsumerthread();
private:
    bool _flag;
    unsigned int _max_size;
    queue<Task> _que;
    MutuxLock _mutex;
    Condition _que_not_empty;
    Condition _que_not_full;
};

TaskQueue::TaskQueue(unsigned int sz)
    : _flag(true)
    , _max_size(sz)
    , _mutex()
    , _que_not_empty(_mutex)
    , _que_not_full(_mutex)
{

}

/* 在生产者线程进行运行 */
inline bool TaskQueue::ElemPushQueue(const Task& task)
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

/*在消费者线程进行运行*/
inline TaskQueue::Task TaskQueue::ElemPopQueue()
{
    Task task;
    {
        MutuxLockGuard AutoLock(_mutex);
        while(_flag && QueueIsEmpty())
        {
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
    cout << "take task success" << endl;
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

void TaskQueue::WakeUpALLConsumerthread()
{
    _flag = false;
    _que_not_empty.NotifySleepInConditionAllThread();
}

