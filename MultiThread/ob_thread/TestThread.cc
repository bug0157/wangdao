#include "Thread.h"

#include <iostream>
#include <unistd.h>
#include <memory>

using std::cout;
using std::endl;
using std::unique_ptr;

class MyThread
:public Thread
{
public:
    void run() override
    {
        while (1) {
            cout << "Thread is running" << endl;
            sleep(1);
        }
    }
};

int main (int argc, char *argv[])
{
    unique_ptr<Thread> pthread(new MyThread());
    pthread->start();
    pthread->join();
    return 0;
}
