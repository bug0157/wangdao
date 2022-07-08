#include <unistd.h>

#include <iostream>

#include "Acceptor.h"
#include "TcpConnection.h"

void test0()
{    
    Acceptor acceptor("127.0.0.1", 8888);
    acceptor.ready();

    TcpConnection con(acceptor.accept());

    std::cout << con.toString() << "has connected" << std::endl;
}

int main (int argc, char *argv[])
{
    test0();
    return 0;
}

