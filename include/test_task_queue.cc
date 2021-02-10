#include <iostream>
#include <unistd.h>
#include "TaskQueue.h"
#include "Thread.h"

#define N 1000

using namespace std;

TaskQueue _task_queue(100);

void task()
{
    cout << "I am process func, do task" << endl;
    /* sleep(2); */
}


void push_task_enqueue()
{
    for(int i = 0; i < N; i++)
    {
        _task_queue.TaskPushQueue(task);
        cout << "push task success" << endl;
        sleep(1);
    }
}

void pop_task_outqueue()
{
    for(int i = 0; i < N; i++)
    {   
        _task_queue.TaskPopQueue();
        cout << "take task success" << endl;
        /*process task need some time*/
        sleep(2);
    }
}

int main()
{
    Thread produce(push_task_enqueue);
    Thread consumer(pop_task_outqueue);
    
    produce.StartRun();
    consumer.StartRun();

    produce.JoinWait();
    consumer.JoinWait();
    
    return 0;
}
