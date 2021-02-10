#include <iostream>
#include "log.h"
#include "func.h"
#include "MutuxLock.h"
using namespace std;
/* #define N 20000000 */
#define N 20000

class TestData
{
public:
    inline MutuxLock& GetMutexLock(){return __lock;}
    inline void InitValue() { val = 0;  }
    inline void ProcessFunc() { val++;  }
    inline int GetValue() { return val;  }
    void enter_critical_section(int id);
    void leave_critical_section(int id);
public:
    volatile int flag[2] = { 0 };
    volatile int turn;
private:
    volatile int val;
    MutuxLock __lock;
};


void TestData::enter_critical_section(int id)
{
    flag[id] = true;
    turn = 1 - id;
    __sync_synchronize();
    while(flag[1 - id] && turn == 1 - id);
}

void TestData::leave_critical_section(int id)
{
    flag[id] = false;
}

int main()
{
    int shmid = shmget(1000, 1<<20, IPC_CREAT | 0600);
    if(shmid == -1)
    {
        cout << "shmget occur error" << endl;

    }
    TestData* ptr = static_cast<TestData*>(shmat(shmid, nullptr, 0));
    ptr->InitValue();
    ptr->GetMutexLock().InitLock(PTHREAD_PROCESS_SHARED);
    struct timeval start, end;
    gettimeofday(&start, nullptr);
    pid_t pid;
    pid = fork();
    if(pid < 0)
    {
        cout << "fork occur error" << endl;

    }
    else if(pid == 0)
    {
        cout << "I am child process, my process id is " << getpid() << endl;
        for(int i = 0; i < N; i++)
        {
            /* ptr->GetMutexLock().Lock(); */
            ptr->enter_critical_section(0);
            ptr->ProcessFunc();
            /* num++; */
            /* cout << "0 ----> " << ptr->GetValue() << endl; */
            ptr->leave_critical_section(0);
            /* ptr->GetMutexLock().Unlock(); */
        }

    }
    else
    {
        cout << "I am parent process, my process id is " << getpid() << endl;
        for(int i = 0; i < N; i++)
        {
            /* ptr->GetMutexLock().Lock(); */
            ptr->enter_critical_section(1);
            ptr->ProcessFunc();
            /* num++; */
            /* cout << "1 ----> " << ptr->GetValue() << endl; */
            ptr->leave_critical_section(1);
            /* ptr->GetMutexLock().Unlock(); */
        }

        wait(nullptr);
        gettimeofday(&end, nullptr);
        cout << "process result = " << ptr->GetValue() << ", use time = " 
            << (end.tv_sec - start.tv_sec) * 1000000 + end.tv_usec - start.tv_usec << endl;

    }
    return 0;

}
