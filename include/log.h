#pragma once

#include <stdio.h>
#include <iostream>

using namespace std;

#define INFO(msg)  cout << "info  --> " << msg << endl
#define DEBUG(msg) cout << "debug --> file : " << __FILE__ << " , " << "function : " << __FUNCTION__ << " , " << "line : " << __LINE__ << " , " << "reason : " << msg << endl
#define ERROR(msg) { cerr << "error --> file : " << __FILE__ << " , " << "function : " << __FUNCTION__ << " , " << "line : " << __LINE__ << " , " << "reason : " << msg << endl , _Exit(-1);} 
