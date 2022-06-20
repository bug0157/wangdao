#include "TaskQueue.h"
#include "MutexLock.h"

TaskQueue::TaskQueue(size_t queSize)
: _queSize(queSize)
, _que()
, _mutex()
, _notEmpty(_mutex)
, _notFull(_mutex)
, _flag(true)
{ }

TaskQueue::~TaskQueue() {}

bool TaskQueue::isEmpty() const
{
    return _que.size() == 0;
}

bool TaskQueue::isFull() const
{
    return _que.size() == _queSize;
}

void TaskQueue::push(const Elem &value)
{
    MutexLockGuard autoLock(_mutex);

    while (isFull()) {
        _notFull.wait();
    }
    _que.push(value);

    _notEmpty.notify();

}

Elem TaskQueue::pop()
{
    MutexLockGuard autoLock(_mutex);

    while (_flag && isEmpty()) {//线程未结束且任务队列为空才进行wait
        _notEmpty.wait();
    }

    if (_flag) {
        Elem temp = _que.front();
        _que.pop();
        _notFull.notify();
        return temp;
    } else {
        return nullptr;
    }
}

void TaskQueue::wakeup()//线程池要退出时将flag置位，并唤醒沉睡线程
{
    _flag = false;
    _notEmpty.notifyall();
}
