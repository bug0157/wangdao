#ifndef __SOCKET_H__
#define __SOCKET_H__

#include "NonCopyable.h"

class Socket
: public NonCopyable
{
public:
    Socket();
    explicit Socket(int fd);
    ~Socket();
    int fd();

private:
    int _fd;
};



#endif // !__SOCKET_H__
