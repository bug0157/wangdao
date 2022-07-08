#ifndef EVENTLOOP_H_
#define EVENTLOOP_H_

#include <sys/epoll.h>
#include <vector>
#include <map>

#include "Acceptor.h"
#include "TcpConnection.h"

using TcpConnectionPtr = std::shared_ptr<TcpConnection>;
using TcpConnectionCallback = std::function<void(const TcpConnectionPtr &)>;

class EventLoop
{
public:
    EventLoop(Acceptor &acceptor);
    ~EventLoop();
    void loop();
    void unloop();

    void setConnectionCallback(TcpConnectionCallback &&cb);
    void setMessageCallback(TcpConnectionCallback &&cb);
    void setCloseCallback(TcpConnectionCallback &&cb);

private:
    int createEpollFd();
    void delEpollReadFd(int fd);
    void addEpollReadFd(int fd);
    void waitEpollFd();
    void handleNewConnection();
    void handleMessage(int fd);

private:
    int _epfd;//红黑树根节点
    Acceptor &_acceptor;
    bool _isLooping;
    std::vector<struct epoll_event> _vecList;
    std::map<int, std::shared_ptr<TcpConnection>> _conns;

    TcpConnectionCallback _onConnectionCb;
    TcpConnectionCallback _onMessageCb;
    TcpConnectionCallback _onCloseCb;

};

#endif // !EVENTLOOP_H_
