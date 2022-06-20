#ifndef __THREADPOOL_H__
#define __THREADPOOL_H__

#include "Thread.h"
#include "TaskQueue.h"

#include <vector>
#include <memory>

using std::vector;
using std::unique_ptr;

class ThreadPool{
public:
    ThreadPool(size_t threadNum, size_t queSize);
    ~ThreadPool();

    void addTask(Task *);
    Task *getTask();

    void start();
    void stop();

private:
    void threadFunc();

private:
    vector<unique_ptr<Thread>> _threads;
    TaskQueue _taskQue;
    size_t _threadNum;
    size_t _queSize;
    bool _isExit;//用来表示线程池是否退出
};

#endif // !__THREADPOOL_H__ 
