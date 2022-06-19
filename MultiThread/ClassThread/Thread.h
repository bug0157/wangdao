#ifndef __THREAD_H__
#define __THREAD_H__

#include <pthread.h>

class Thread
{
public:
    Thread ();
    virtual ~Thread ();
    void start();
    void join();

private:
    static void *threadFunc(void *arg);
    virtual void run() = 0;
private:
    pthread_t _tid;
    bool _isRunning;
};

class WorkerThread
: public Thread
{
public:
    WorkerThread();
    ~WorkerThread();
private:
    void run() override;
private:
    pthread_t _workerTid;
    bool _isRunning;
};

#endif
