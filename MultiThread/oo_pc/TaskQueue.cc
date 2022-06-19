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
    _mutex.lock();

    if (isFull()) {
        _notFull.wait();
    }
    _que.push(value);

    _notEmpty.notify();

    _mutex.unlock();
}
int TaskQueue::pop()
{
    _mutex.lock();

    if (isEmpty()) {
        _notEmpty.wait();
    }

    int temp = _que.front();
    _que.pop();

    _notFull.notify();

    _mutex.unlock();

    return temp;
}

