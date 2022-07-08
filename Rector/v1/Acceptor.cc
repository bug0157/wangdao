#include "Acceptor.h"

Acceptor::Acceptor(const string &ip, unsigned short port)
: _listenSock()
, _servAddr(ip, port)
{

}

Acceptor::~Acceptor()
{

}

void Acceptor::ready()
{
    setReuseAddr();
    setReusePort();
    bind();
    listen();
}

void Acceptor::setReuseAddr()
{
    int on = 1;
    int ret = setsockopt(_listenSock.fd(), 
                        SOL_SOCKET, SO_REUSEADDR, 
                        &on, sizeof(on));
    if (-1 == ret) {
        perror("setsockopt");
        return;
    }
}

void Acceptor::setReusePort()
{
    int on = 1;
    int ret = setsockopt(_listenSock.fd(), 
                        SOL_SOCKET, SO_REUSEPORT, 
                        &on, sizeof(on));
    if (-1 == ret) {
        perror("setsockopt");
        return;
    }
}

void Acceptor::bind()
{
    int ret = ::bind(_listenSock.fd(), 
                    (const struct sockaddr *)_servAddr.getInetAddrPtr(), 
                    sizeof(struct sockaddr));
    if (-1 == ret) {
        perror("bind");
    }
}

void Acceptor::listen()
{
    int ret = ::listen(_listenSock.fd(), 128);
    if (-1 == ret) {
        perror("listen");
    }
}

int Acceptor::accept()
{
    int connfd = ::accept(_listenSock.fd(), nullptr, nullptr);
    if (-1 == connfd) {
        perror("accept");
        return -1;
    }
    return connfd;
}

int Acceptor::fd()
{
    return _listenSock.fd();
}




















