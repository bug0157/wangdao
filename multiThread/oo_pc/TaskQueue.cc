#include "TaskQueue.h"

TaskQueue::TaskQueue(size_t size)
: _size(size)
, _que()
, _mutex()
, _notEmpty(_mutex)
, _notFull(_mutex)
{

}
TaskQueue::~TaskQueue()
{

}
bool TaskQueue::isEmpty() const
{
    return _que.size() == 0;
}
bool TaskQueue::isFull() const
{
    return _que.size() == _size;
}
void TaskQueue::push(const int &value)
{
    MutexLockGuard autoLock(_mutex);
}
int TaskQueue::pop()
{

}

