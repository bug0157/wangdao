#ifndef __ACCEPTOR_H__
#define __ACCEPTOR_H__

#include "InetAddress.h"
#include "Socket.h"
#include <string>

using std::string;

class Acceptor
{
public:
    Acceptor(const string &addr, unsigned short port);
    ~Acceptor();
    void ready();
    int accept();
    int fd();

private:
    void setReuseAddr();
    void setReusePort();
    void bind();
    void listen();

private:
    Socket _listenSock;//监听套接字，设置ip和端口复用
    InetAddress _servAddr;//设置的服务器地址
};


#endif // !__ACCEPTOR_H__
