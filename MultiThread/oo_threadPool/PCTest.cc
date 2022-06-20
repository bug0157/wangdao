#include "Producer.h"
#include "Consumer.h"

#include <iostream>
#include <memory>

using std::unique_ptr;

void test1()
{
    TaskQueue taskQue(10);
    unique_ptr<Producer> producer(new Producer(taskQue));
    unique_ptr<Consumer> consumer(new Consumer(taskQue));

    producer->start();
    consumer->start();

    producer->join();
    consumer->join();
}

int main (int argc, char *argv[])
{
    test1();
    return 0;
}
