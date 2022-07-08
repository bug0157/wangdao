#include <unistd.h>

#include <iostream>

#include "EventLoop.h"

EventLoop::EventLoop(Acceptor &acceptor)
    : _epfd(createEpollFd())
    , _acceptor(acceptor)
    , _isLooping(false)
    , _vecList(1024){
        addEpollReadFd(acceptor.fd());
}

EventLoop::~EventLoop(){
    if (_epfd) {
        close(_epfd);
    }
}

void EventLoop::loop(){
    _isLooping = true;
    while (_isLooping) {
        waitEpollFd();
    }
}

void EventLoop::unloop(){
    _isLooping = false;
}

//封装epoll_wait
void EventLoop::waitEpollFd(){
    int nready = -1;
    do {
        nready = ::epoll_wait(_epfd, &*_vecList.begin(), _vecList.size(), 5000);
    }while (-1 == nready && errno == EINTR);

    if (-1 == nready) {
        perror("nready = -1");
        return;
    }
    else if (0 == nready) {
        std::cout << ">>epoll_wait timeout" << std::endl;  
    }
    else {
        //达到监听上限后扩容
        if (nready == (int)_vecList.size()) {
            _vecList.resize(2 * nready);
        }

        for (int idx = 0; idx < nready; ++idx) {
            int fd = _vecList[idx].data.fd;
            //有新连接
            if (fd == _acceptor.fd()) {
                if (_vecList[idx].events & EPOLLIN) {
                    handleNewConnection();
                }
            }
            //有数据传输
            else {
                if (_vecList[idx].events & EPOLLIN) {
                    handleMessage(_epfd);
                }
            }
        }
    }
}

void EventLoop::handleNewConnection(){
    //返回正确，则三次握手成功，建立连接
    int peerfd = _acceptor.accept();
    //将peerfd加入红黑树监听
    addEpollReadFd(peerfd);

    TcpConnectionPtr con(new TcpConnection(peerfd));

    //连接创建完成，注册三个事件
    con->setConnectionCallback(_onConnectionCb);
    con->setMessageCallback(_onMessageCb);
    con->setCloseCallback(_onCloseCb);

    _conns.insert(std::make_pair(peerfd, con));
    con->handleConnectionCallback();
}

void EventLoop::handleMessage(int fd){
    auto iter = _conns.find(fd);
    if (iter != _conns.end()) {
        
    }
}















