#include <iostream>
#include <list>
#include <string>
#include <memory>

using std::list;
using std::string;
using std::unique_ptr;

class Observer;

class Subject
{
public:
    virtual void attach(Observer *pObs) = 0;
    virtual void detach(Observer *pObs) = 0;
    virtual void notify() = 0;
    virtual ~Subject() {}
};


class ConcreteSubject
: public Subject
{
public:
    void attach(Observer *pObs) override;
    void detach(Observer *pObs) override;
    void notify() override;

    void setStatus(int status) { _status = status; }
    int getStatus() const { return _status; }
private:
    list<Observer*> _obList;
    int _status;
};

class Observer
{
public:
    virtual void update(int value) = 0;
    virtual ~Observer() {}
};

class ConcreteObserber1
: public Observer
{
public:
    ConcreteObserber1(const string &name)
    :_name(name) {}

    void update(int value)
    {
        std::cout << "ConcreteObserber1:" << _name << " value:" << value << std::endl;
    }

private:

    string _name;
};


class ConcreteObserber2
: public Observer
{
public:
    ConcreteObserber2(const string &name)
    :_name(name) {}

    void update(int value)
    {
        std::cout << "ConcreteObserber2:" << _name << " value:" << value << std::endl;
    }

private:
    string _name;
};

void ConcreteSubject::attach(Observer *pObs)
{
    _obList.push_back(pObs);
}

void ConcreteSubject::detach(Observer *pObs)
{
    for (auto it = _obList.begin(); it != _obList.end(); ++it) {
        if (*it == pObs) {
            _obList.remove(pObs);
            break;
        }
    }
}

void ConcreteSubject::notify()
{
    for (auto &ob  : _obList) {
        ob->update(_status);
    }
}

int main (int argc, char *argv[])
{
    unique_ptr<ConcreteSubject> pSubject(new ConcreteSubject());
    unique_ptr<Observer> pObserver1(new ConcreteObserber1("naroto"));
    unique_ptr<Observer> pObserver2(new ConcreteObserber2("sukura"));

    pSubject->setStatus(2);
    pSubject->attach(pObserver1.get());
    pSubject->attach(pObserver2.get());

    pSubject->notify();
    pSubject->detach(pObserver2.get());
    pSubject->setStatus(3);

    pSubject->notify();

    return 0;
}



















