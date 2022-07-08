#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <sys/socket.h>

#include "SocketIO.h"

SocketIO::SocketIO(int fd)
: _fd(fd)
{

}

SocketIO::~SocketIO()
{

}

int SocketIO::readn(char *buf, int len)
{
    int nleft = len;//剩余未读量
    char *pstr = buf;
    int ret = 0;
    while (nleft > 0) {
        ret = read(_fd, pstr, nleft);
        if (-1 == ret && errno == EINTR) {
            continue;
        }
        else if (-1 == ret) {
            perror("read error -1");
            return -1;
        }
        else if (0 == ret) {
            break;
        }
        else {
            pstr += ret;
            nleft -= ret;
        }
    }
    return len - nleft;
}

int SocketIO::readLine(char *buf, int len)
{
    int nleft = len - 1;
    char *pstr = buf;
    int ret = 0, total = 0;
    while (nleft > 0) {
        //MSG_PEEK不会将缓冲区的内容清空
        ret = recv(_fd, pstr, nleft, MSG_PEEK);
        if (-1 == ret && errno == EINTR) {
            continue;
        }
        else if (-1 == ret){
            perror("readLine error -1"); 
            return -1;
        }
        else if (0 == ret) {
            break; 
        }
        else {
            for (int idx = 0; idx < ret; ++idx) {
                if ('\n' == pstr[idx]) {
                    int sz = idx + 1;//读到换行符，多读一位放\0
                    readn(pstr, sz);
                    pstr += sz;
                    *pstr = '\0';

                    return total + sz;
                }
            }
            readn(pstr, ret);//内核态拷贝到用户态
            total += ret;
            pstr += ret;
            nleft -= ret;
        }
    }
    *pstr = '\0';
    return total - nleft;
}

int SocketIO::writen(const char *buf, int len)
{
    int nleft = len;
    const char *pstr = buf;
    int ret = 0;

    while (nleft > 0) {
        ret = write(_fd, pstr, nleft);
        if (-1 == ret && errno == EINTR) {
            continue; 
        }
        else if (-1 == ret) {
            perror("write error -1");
            return -1;
        }
        else if (ret == 0) {
            break;
        }
        else {
            pstr += ret;
            nleft -= ret;
        }
    }
    return len - nleft;
}
