#ifndef __TASKQUEUQ_H__
#define __TASKQUEUQ_H__

#include "Mutexlock.h"
#include "Condition.h"
#include <queue>

class TaskQueue{
public:
    TaskQueue(size_t size);
    ~TaskQueue();
    bool isEmpty() const;
    bool isFull() const;
    void push(const int &value);
    int pop();
private:
    size_t _size;
    std::queue<int> _que;
    MutexLock _mutex;
    Condition _notEmpty;
    Condition _notFull;
};

#endif // !__TASKQUEUQ_H__
