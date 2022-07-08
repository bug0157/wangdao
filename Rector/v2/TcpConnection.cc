#include <sys/socket.h>

#include <iostream>
#include <sstream>
#include <utility>

#include "TcpConnection.h"

TcpConnection::TcpConnection(int fd)
: _sock(fd)
, _sockIO(fd)
, _localAddr(getLocalAddr())
, _peerAddr(getPeerAddr())
{

}

TcpConnection::~TcpConnection()
{

}

//将数据发送回客户端
void TcpConnection::send(const string &msg)
{
    _sockIO.writen(msg.c_str(), msg.size());
}

//接收一行数据
string TcpConnection::receive()
{
    char buff[65535] = {};
    _sockIO.readLine(buff, sizeof(buff));
    return string(buff);
}

string TcpConnection::toString()
{
    std::ostringstream oss;
    oss << _localAddr.ip() << ":"
        << _localAddr.port() << "---->"
        << _peerAddr.ip() << ":"
        << _peerAddr.port();

    return oss.str();
}

//获取本地网络信息
InetAddress TcpConnection::getLocalAddr()
{
    struct sockaddr_in addr;
    socklen_t len = sizeof(struct sockaddr);
    int ret = getsockname(_sock.fd(), (struct sockaddr*)&addr, &len);
    if (-1 == ret) {
        perror("getsockname");
    }
    return InetAddress(addr);
}

InetAddress TcpConnection::getPeerAddr()
{
    struct sockaddr_in addr;
    socklen_t len = sizeof(struct sockaddr);
    int ret = getpeername(_sock.fd(), (struct sockaddr*)&addr, &len);
    if (-1 == ret) {
        perror("getpeername");
    }
    return InetAddress(addr);
}

//注册三种类型的回调函数
void TcpConnection::setConnectionCallback(const TcpConnectionCallback &cb){
    _onConnectionCb = std::move(cb);
}

void TcpConnection::setMessageCallback(const TcpConnectionCallback &cb){
    _onMessageCb = std::move(cb);
}

void TcpConnection::setCloseCallback(const TcpConnectionCallback &cb){
    _onCloseCb = std::move(cb);
}

//回调函数的具体执行
void TcpConnection::handleConnectionCallback(){
    if (_onConnectionCb) {
        _onConnectionCb(shared_from_this()); 
    }
}

void TcpConnection::handleMessageCallback(){
    if (_onMessageCb) {
        _onMessageCb(shared_from_this()); 
    }
}

void TcpConnection::handleCloseCallback(){
    if (_onCloseCb) {
        _onCloseCb(shared_from_this()); 
    }
}

