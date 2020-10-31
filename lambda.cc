#include <iostream>
#include <functional>
#include <vector>

using namespace std;

class Func
{
    void func(int x, int y)
    {
        //auto f1 = []{ return a; }  //error: 'this' cannot be implicitly captured in this context 
        auto f2 = [=] {return a+x+y;};
        auto f3 = [=] {return a+x+y;};
        auto f4 = [this] {return a;};
        //auto f5 = [this] {return a+x+y;};   //error: cannot capture x and y 
        auto f6 = [this,x,y] {return a+x+y;};
        auto f7 = [this]{return a++;};
    }
private:
    int a = 0;
};

class CountEven
{
    int& count_;
public:
    CountEven(int& count)
        : count_(count)
    {  }

    void operator()(int val)
    {
        if(!(val & 1))
        {
            count_++;
        }
    }
};

int main(int argc,char **argv)
{
    {
        int x = 0, y = 1;
        //auto f1 = [] {return x;};     //error, No capture x
        auto f2 = [&] {return x++;};
        auto f3 = [=] {return x;};
        //auto f4 = [=] {return x++};     //error, cannot assign to a variable captured by copy in a non-mutable lambda
        /* auto f5 = [x] {return x+y;};    //error, y not capture */
        auto f6 = [x, &y] {return x+(y++);};
        auto f7 = [=, &y] {return x+(y++);};
        auto f8 = [=]() mutable {return x++;};
    }

    {
        std::function<int(int)> c1 = [](int a){return a;};
        std::function<int(void)> c2 = std::bind([](int a){return a;}, 123);
    }

    {
        int n = 0;
        using func_t = int(*)(int);
        func_t c4 = [](int a){return a;};
        /* func_t c5 = [n](int a){return a;};   error, 没有捕获变量的lambda表达式可以直接转换为函数指针，而捕获变量的lambda表达式则不能转换函数指针 */
        
        typedef void(*ptr)(int*);
        ptr p = [](int* p) {delete p;};
        /* ptr p1 = [&](int* p) {delete p;}; */
    }
    
    {
        vector<int> v = {1,2,3,4,5,6};
        int even_count = 0;
        for_each(v.begin(), v.end(), CountEven(even_count));
        cout << "the number of even is " << even_count << endl;
    }

    {
        vector<int> v = {1,2,3,4,5,6};
        int even_count = 0;
        for_each(v.begin(), v.end(), [&even_count](int val){ if(!(val & 1)){++even_count;} });
        cout << "the number of even is " << even_count << endl;
    }

    return 0;
}

