#include <iostream>
#include <string>

using std::cout;
using std::endl; 
using std::string;

template<typename T, size_t capacity = 10>
class Queue
{
public:
    Queue():_data(new T[capacity]()),_size(0),_front(0),_rear(0){}
    void enQueue(const T &val);
    void deQueue();
    bool isEmpty() const;
    bool isFull() const;
    T getFront() const;
    T getRear() const;
    int getSize() const{ return _size; };
    ~Queue();
private:
    int _front;
    int _rear;
    int _size;
    T *_data;
};

template<typename T, size_t capacity>
Queue<T, capacity>::~Queue()
{
    if (_data)
    {
        delete [] _data;
        _data = nullptr;
    }
}



template<typename T, size_t capacity>
bool Queue<T, capacity>::isFull() const
{
    return (_rear + 1)%capacity == _front;
}

template<typename T, size_t capacity>
bool Queue<T, capacity>::isEmpty() const
{
    return _rear == _front;
}

template<typename T, size_t capacity>
void Queue<T, capacity>::enQueue(const T &val)
{
    if (!isFull()) {
        _data[_rear] = val;
        _rear = (_rear + 1)%capacity;
        ++_size;
    }else {
        cout << "Queue is full" << endl;
        return;
    }
}

template<typename T, size_t capacity>
void Queue<T, capacity>::deQueue()
{
    _front = (_front + 1)%capacity;
}

template<typename T, size_t capacity>
T Queue<T, capacity>::getFront() const
{
    if (!isEmpty()) {
        return _data[_front];
    }
}

template<typename T, size_t capacity>
T Queue<T, capacity>::getRear() const
{
    if (!isEmpty()) {
        return _data[_front];
    }
}

void test0()//测试用例1 整型数据
{
    Queue<int> q1;
    q1.enQueue(1);
    q1.enQueue(2);
    q1.enQueue(3);
    q1.enQueue(4);
    q1.enQueue(5);
    q1.enQueue(6);
    q1.enQueue(7);
    q1.enQueue(8);
    q1.enQueue(9);
    q1.deQueue();
    q1.deQueue();
    q1.enQueue(10);
    q1.enQueue(11);

    while(!q1.isEmpty())
    {
        cout << "front: " << q1.getFront() << endl
             << "rear: " << q1.getRear() << endl
             << "size: " << q1.getSize() << endl;
        q1.deQueue();
    }
}

class Student
{
public:
    Student(){}
    Student(string name, int age)
    :_name(name)
    ,_age(age)
    {}

    ~Student(){}

    string _name;
    int _age;
};

void test1()//测试用例2 自定义类型
{

    Student s1("abc", 12);
    Student s2("bcd", 14);
    Student s3("abt", 12);
    Student s4("abv", 22);
    Student s5("abd", 32);
    Student s6("abs", 19);

    Queue<Student> q2;
    q2.enQueue(s1);
    q2.enQueue(s2);
    q2.enQueue(s3);
    q2.enQueue(s4);
    q2.deQueue();
    q2.deQueue();
    q2.enQueue(s5);
    q2.enQueue(s6);

    while(!q2.isEmpty())
    {
        cout << "name: " << q2.getFront()._name << " "
             << "age: " << q2.getFront()._age << endl
             << "size: " << q2.getSize() << endl;
        q2.deQueue();
    }
}

int main()
{
    test0();
    test1();
    return 0;
}
