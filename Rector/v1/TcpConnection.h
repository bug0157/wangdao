#ifndef TCPCONNECTION_H_
#define TCPCONNECTION_H_

#include "Socket.h"
#include "SocketIO.h"
#include "InetAddress.h"

#include <memory>
#include <functional>


class TcpConnection
{
    using TcpConnectionPtr = std::shared_ptr<TcpConnection>;
    using Callback = std::function<void(TcpConnectionPtr)>;

public:
    TcpConnection(int fd);
    ~TcpConnection();
    void send(const string &msg);
    string receive();
    string toString();

    void setConnectionCallback(const Callback &cb);
    void setMessageCallback(const Callback &cb);
    void setCloseCallback(const Callback &cb);

    void handleConnectionCallback();
    void handleMessageCallback();
    void handleCloseCallback();

private:
    InetAddress getLocalAddr();
    InetAddress getPeerAddr();

private:
    Socket _sock;
    SocketIO _sockIO;
    InetAddress _localAddr;
    InetAddress _peerAddr;

    Callback _onConnectionCb;
    Callback _onMessageCb;
    Callback _onCloseCb;
};








#endif // !TCPCONNECTION_H_
