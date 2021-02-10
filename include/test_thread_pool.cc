#include <iostream>
#include <unistd.h>
#include "ThreadPool.h"

using namespace std;

#define N 10

void TestThreadPool()
{
    ThreadPool pool(2);

    std::thread thd1([&pool]
                     {
                        for(int i = 0; i < N; i++)
                        {
                            auto thId = this_thread::get_id();
                            pool.AddTask([thId]
                                        {
                                            cout << "同步层线程1的线程ID:" << thId << endl; 
                                            sleep(1);
                                        });
                        }
                     }); 
    
    std::thread thd2([&pool]
                     {
                        for(int i = 0; i < N; i++)
                        {
                            auto thId = this_thread::get_id();
                            pool.AddTask([thId]
                                        {
                                            cout << "同步层线程2的线程ID:" << thId << endl; 
                                            sleep(1);
                                        });
                        }
                     }); 
    
    /* sleep(50); */
    sleep(5);
    pool.ThreadPoolRunStop();
}

int main()
{
    TestThreadPool();
    cout << "main thread in this" << endl;
    return 0;
}
