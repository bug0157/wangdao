#ifndef __CONSUMER_H__

#include "Thread.h"
#include "TaskQueue.h"

#include <stdlib.h>
#include <unistd.h>
#include <iostream>

using std::cout;
using std::endl;

class Consumer
: public Thread
{
public:
    Consumer(TaskQueue &taskQue)
    :_taskQue(taskQue)
    {
        
    }

    void run() override
    {
        int cnt = 20;
        while (cnt-- > 0) {
            int num = _taskQue.pop();
            cout << "-->consume num = " << num << endl;
            sleep(2);
        }

    }

    ~Consumer()
    {
        
    }
private:
    TaskQueue &_taskQue;
};

#endif // !__CONSUMER_H__
