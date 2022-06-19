#include <iostream>

int number = 1;
void display(int number)
{
    std::cout << "全局display()" << std::endl;
}
namespace wd 
{
int number = 10;

namespace wh 
{
int number = 100;
void display(int number)
{
    std::cout << "wd::wh::display()" << std::endl;
}
}//end of namespace wh

#if 1
void display(int number)
{
    std::cout << "number = " << number << std::endl;
    std::cout << "wd::number = " << wd::number << std::endl;
    std::cout << "wd::wh::number = " << wd::wh::number << std::endl;
}
#endif
}// end of namespace wd
 //
int main(void)
{
    display(number);
    return 0;
}
