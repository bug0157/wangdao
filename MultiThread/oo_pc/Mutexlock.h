#ifndef __MUTEXLOCK_H__
#define __MUTEXLOCK_H__

#include <pthread.h>

class MutexLock {
public:
    MutexLock();
    ~MutexLock();
    void lock();
    void unlock();
    pthread_mutex_t *getMutexLockPtr(){
        return &_mutex;
    }
private:
    pthread_mutex_t _mutex;
};

#endif // !__MUTEXLOCK_H__
