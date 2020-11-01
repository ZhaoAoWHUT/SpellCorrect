#include <iostream>
#include "../include/Priority.h"

using namespace std;
using namespace log;

int main(int argc,char **argv)
{
    Priority pr;
    cout << pr.getPriorityName(2) << endl;
    cout << pr.getPriorityValue("INFO") << endl;
    cout << pr.getPriorityValue("defefwefwe") << endl;
    return 0;
}

