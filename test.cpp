#include <algorithm>
#include <functional>
#include <iostream>
#include <string>
#include <vector>

using std::vector;
using std::less;
using std::ends;
using std::endl;

template<typename T>
struct myless
{
    bool operator()(const T &lhs, const T &rhs) const
    {
        return lhs < rhs;
    }

};

int main(){
    vector<int> vec;
    myless<int> less;
    std::remove_if(vec.begin(), vec.end(), std::bind1st(less, 4));
}
