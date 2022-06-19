#include <iostream>
#include <string.h>


using std::cout;
using std::endl;

class String
{
public:
    String(){ cout << "String()" << endl; }
    String(const char *pstr);
    String(const String &rhs);
    String(String &&rhs);
    String &operator=(const String &rhs);
    String &operator=(String &&rhs);
    ~String(){ cout << "~String()" << endl; }
    void print();
      
private:
    char *_pstr;
};

String::String(const char *pstr)
:_pstr(new char[strlen(pstr) + 1]())
{
    cout << "String(const char*)" << endl;
    strcpy(_pstr, pstr);
}

String::String(const String &rhs)
:_pstr(new char[strlen(rhs._pstr) + 1]())
{
    cout << "String(const String &rhs)" << endl;
    strcpy(_pstr, rhs._pstr);
}

String::String(String &&rhs)
:_pstr(rhs._pstr)
{
    cout << "String(const &&rhs)" << endl;
    rhs._pstr = nullptr;
}

String &String::operator=(const String &rhs)
{
    cout << "String &operator=(const String &rhs)" << endl;
    if (this != &rhs)
    {
        delete [] _pstr;
        _pstr = new char[strlen(rhs._pstr) + 1]();
        strcpy(_pstr, rhs._pstr);
    }
    return *this;
}
String &String::operator=(String &&rhs)
{
    cout << "String &operator=(String &&rhs)" << endl;
    if (this != &rhs)
    {
        delete [] _pstr;
        _pstr = nullptr;
        _pstr = rhs._pstr;
        rhs._pstr = nullptr;
    }
    return *this;
}
void String::print()
{
    cout << _pstr << endl;
}

void test()
{
    String s1("hello");
    String s2("world");
    String s3(s1);//左值，拷贝控制语义
    s3.print();
    String s4(std::move(s1));//右值，移动语义
    s4.print();
    cout << endl;
    s3 = s2;
    s3.print();
    s4 = std::move(s2);
    s4.print();
}

int main()
{
    test();
    return 0;
}










