#include <iostream>
#include "log.h"



void Func1()
{
    INFO("process Func1");
}


void Func2()
{
    DEBUG("process Func2");
}

void Func3()
{
    ERROR("process Func3");
}

int main()
{
    Func1();
    Func2();
    Func3();
    cout << "hello world" << endl;
    return 0;
}

