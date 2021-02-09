#include <iostream>

#include "Condition.h"
#include "Thread.h"

#define N 10000

class Task* workqueue;

MutuxLock _mutex;
Condition _cond(_mutex);

class Task
{
public:
    Task(string info);
public:
    Task* _next;
    string _info;
};


Task::Task(string info)
    : _info(info)
{

}

void task_enqueue(Task* task)
{
    _mutex.Lock();
    task->_next = workqueue;
    workqueue = task;
    _mutex.Unlock();
    _cond.NotifySleepInConditionOneThread();
}

Task* task_outqueue()
{
    Task* ptr;
    _mutex.Lock();
    while(workqueue == nullptr)
    {
        _cond.SleepInConditon();
    }
    ptr = workqueue;
    workqueue = ptr->_next;
    _mutex.Unlock();
    return ptr;
}

void init_task()
{
    for(int i = 0; i < N; i++)
    {
        string str = "task " + to_string(i);
        Task* ptask = new Task(str);
        task_enqueue(ptask);
    }
}

void process_task()
{
    for(;;)
    {
        Task* ptr = task_outqueue();
        INFO("process task.");
        cout << ptr->_info << endl;
        free(ptr);
    }
}

int main()
{
    _mutex.InitLock();
    Thread thread1(init_task);
    Thread thread2(process_task);

    thread1.StartRun();
    thread2.StartRun();

    thread1.JoinWait();
    thread2.JoinWait();
    
    return 0;
}

