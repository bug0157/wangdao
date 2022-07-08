#include "Socket.h"
#include <sys/socket.h>
#include <stdio.h>
#include <unistd.h>

//利用RAII的思想，对象在其构造时获取资源，析构时释放资源
Socket::Socket()
{
    //创建socket
    _fd = ::socket(AF_INET, SOCK_STREAM, 0);
    if (_fd < 0) {
        perror("socket");
    }
}

Socket::Socket(int fd)
: _fd(fd)
{

}

Socket::~Socket()
{
    close(_fd);
}

int Socket::fd()
{
    return _fd;
}
