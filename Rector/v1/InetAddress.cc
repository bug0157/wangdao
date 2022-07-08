#include "InetAddress.h"

#include <arpa/inet.h>
#include <string.h>
#include <sys/socket.h>

//构造函数，传入ip地址值和端口号
InetAddress::InetAddress(const string &ip, unsigned short port)
{
    ::bzero(&_addr, sizeof(struct sockaddr_in));
    _addr.sin_family = AF_INET;
    _addr.sin_port = htons(port);
    // 将网络主机地址转换为二进制网络字节序
    _addr.sin_addr.s_addr = inet_addr(ip.c_str());
}

InetAddress::InetAddress(const struct sockaddr_in &addr)
: _addr(addr)
{

}

InetAddress::~InetAddress() {  }

string InetAddress::ip() const
{
    //将一个32位的网络字节序二进制IPv4地址转换成相应的点分十进制数串
    //由该函数的返回值所指向的字符串驻留在静态内存中,这意味着该函数是不可重入的
    return string(inet_ntoa(_addr.sin_addr));
}

unsigned short InetAddress::port() const
{
    //网络字节序转换成主机字节序 uint32_t
    return ntohs(_addr.sin_port);
}

const struct sockaddr_in *
InetAddress::getInetAddrPtr() const
{
    return &_addr;
}
