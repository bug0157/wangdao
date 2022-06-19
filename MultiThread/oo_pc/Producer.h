#ifndef __PRODUCER_H__

#include "Thread.h"
#include "TaskQueue.h"

#include <stdlib.h>
#include <unistd.h>
#include <iostream>

using std::cout;
using std::endl;

class Producer
: public Thread
{
public:
    Producer(TaskQueue &taskQue)
    :_taskQue(taskQue)
    {
        
    }

    void run() override
    {
        int cnt = 20;
        ::srand(clock());
        while (cnt-- > 0) {
            int num = ::rand()%100;
            _taskQue.push(num);
            cout << "-->produce num = " << num << endl;
            sleep(2);
        }

    }

    ~Producer()
    {
        
    }
private:
    TaskQueue &_taskQue;
};

#endif // !__PRODUCER_HH__
