#include <iostream>
#include <functional>

using namespace std::placeholders;
using std::cout;
using std::endl;

void func(int x, int y)
{
    std::cout << "x = " << x <<  std::endl; 
    std::cout << "y = " << y <<  std::endl; 
}

std::function<void()> f1 = std::bind(&func, 1, 2);
std::function<void(int)> f2 = std::bind(&func, 1, _1);

int main()
{
    f1();
    f2(1);
}
