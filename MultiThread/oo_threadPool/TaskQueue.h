#ifndef __TASKQUEUQ_H__
#define __TASKQUEUQ_H__

#include "MutexLock.h"
#include "Condition.h"
#include "Task.h"
#include <queue>

using Elem = Task *;

class TaskQueue{
public:
    TaskQueue(size_t size);
    ~TaskQueue();
    bool isEmpty() const;
    bool isFull() const;
    void push(const Elem &elem);
    Elem pop();

    void wakeup();

private:
    size_t _queSize;
    std::queue<Elem> _que;
    MutexLock _mutex;
    Condition _notEmpty;
    Condition _notFull;
    bool _flag;
};

#endif // !__TASKQUEUQ_H__
