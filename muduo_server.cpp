// //
// muduo网络库给用户提供了两个主要的类
//     TcpServer：用于编写服务器程序的
//     TcpClient：用于编写客户端程序的

#include <muduo/net/TcpServer.h>
#include <muduo/net/EventLoop.h>
#include <iostream>
#include <functional>
#include <string>
using namespace std;
using namespace muduo;
using namespace muduo::net;
using namespace std::placeholders;
// using namespace placeholders;

/*基于muduo网络库开发服务器程序
1. 组合TcpServer对象
2. 创建EventLoop事件循环对象的指针
3. 明确TcpServer构造函数需要什么参数，输出ChatServer的构造函数
4. 在当前服务器类的构造函数当中，注册处理连接的回调函数和处理读写事件的回调函数
5. 设置合适的服务端线程数量，muduo库会自己分配I/O线程和worker线程
*/    

// gcc -I头文件的路径 -L库文件的路径 -l库文件名 -o 输出文件名 源文件名
//muduo_base 头文件路径 /usr/local/include/muduo/base
//muduo 库文件路径 /usr/local/lib
//muduo_net 库文件路径 /usr/local/lib
//muduo_base 库文件路径 /usr/local/lib
//muduo 库文件路径 /usr/local/lib
//muduo_net 库文件路径 /usr/local/lib
//muduo_base 库文件路径 /usr/local/lib
//主要在connection handle 和 message handle 中实现

class ChatServer
{
public:
    ChatServer(EventLoop *loop,               // 事件循环
               const InetAddress &listenAddr, // IP+Port
               const string &nameArg)         // 服务器的名字
        : _server(loop, listenAddr, nameArg), _loop(loop)
    {
        // 给服务器注册用户连接的创建和断开回调
        _server.setConnectionCallback(std::bind(&ChatServer::onConnection, this, _1));

        // 给服务器注册用户读写事件的回调
        _server.setMessageCallback(std::bind(&ChatServer::onMessage, this, _1, _2, _3));

        //设置服务器端的线程数 1个 I/O线程 3个worker线程
        _server.setThreadNum(4);
    }

    //开始时间循环
    void start()
    {
        _server.start();
    }

private:
    // 专门处理用户的连接和断开 epoll
    void onConnection(const TcpConnectionPtr &conn)
    {
        // cout << conn->peerAddress().toIpPort() << " -> " << 
        // conn->localAddress().toIpPort() << 
        // " state: " << (conn->connected() ? "UP" : "DOWN") << endl;

        if(conn->connected())
        {
            cout<<conn->peerAddress().toIpPort()<<"->"<<conn->localAddress().toIpPort()<<" state: online"<<endl;
        }
        else
        {
            cout<<conn->peerAddress().toIpPort()<<"->"<<conn->localAddress().toIpPort()<<" state: offline"<<endl;
            conn->shutdown(); //close(fd)
            // _loop->quit();
        }
    }

    // 专门处理用户的读写事件
    void onMessage(const TcpConnectionPtr &conn, // 连接
                   Buffer *buffer,               // 缓冲区
                   Timestamp timestamp)            // 缓冲区接受数据的时间信息
    {
        string buf = buffer->retrieveAllAsString();
        cout<<"recv data: "<<buf<<" time: "<<timestamp.toString()<<endl;
        conn->send(buf);
    }
    TcpServer _server; // #1
    EventLoop *_loop;  // #2 epoll
};

// g++ -o server muduo_server.cpp  -lmuduo_net -lmuduo_base -lpthread

//需要改的就类的名称和端口号
int main()
{
    EventLoop loop; //epoll + 多线程I/O
    InetAddress addr("127.0.0.1", 6000);
    ChatServer server(&loop, addr, "ChatServer");
    server.start(); //启动服务器  listenfd  epoll_ctl => epoll
    loop.loop(); //epoll_wait()阻塞的方式等待新用户连接，已连接用户的读写事件等
    return 0;
}
