#include "Thread.h"

#include <pthread.h>
#include <stdio.h>

Thread::Thread():_tid(0),_isRunning(false) { }

Thread::~Thread()
{
    if (_isRunning) {
        pthread_detach(_tid);
    }
}

void Thread::start()
{
    int ret = pthread_create(&_tid, nullptr, threadFunc, this);
    if (ret) {
        perror("pthread_create");
        return;
    }
    _isRunning = true;
}

void Thread::join()
{
    if (_isRunning) {
        pthread_join(_tid, nullptr);
        _isRunning = false;
    }
}

void *Thread::threadFunc(void *arg)
{
    Thread *pth = static_cast<Thread *>(arg);
    if (pth) {
        pth->run();
    }
    return nullptr;
}
