#include "chatserver.hpp"
#include "chatservice.hpp"
#include <iostream>
#include <signal.h>
using namespace std; 

// 处理服务器ctrl+c结束后，重置user的状态信息的
void resetHandler (int)
{
    ChatService::instance()->reset();
    exit(0);
}

int main ()
{
    signal(SIGINT, resetHandler);

    EventLoop loop;
    InetAddress addr("127.0.0.1", 6000); // 先写死 后面配置
    ChatServer server(&loop, addr, "ChatServer");

    server.start();
    loop.loop();

    return 0;    
}
