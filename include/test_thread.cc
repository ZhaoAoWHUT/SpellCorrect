#include <iostream>
#include "log.h"
#include "func.h"
#include "Thread.h"
#include "MutuxLock.h"


void process()
{
    cout << "thread process task" << endl;
    for(int i = 0; i < 1000; i++)
    {
        cout << i << endl;
    }
}

int main()
{
    Thread thread(process);
    thread.StartRun();
    thread.JoinWait();
    return 0;
}
