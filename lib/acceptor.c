#include <assert.h>
#include "acceptor.h"

struct acceptor *acceptor_init(int port) {
    struct acceptor *acceptor1 = malloc(sizeof(struct acceptor));  //连接对
    acceptor1->listen_port = port;  //端口
    acceptor1->listen_fd = socket(AF_INET, SOCK_STREAM, 0); //socket套接字

    make_nonblocking(acceptor1->listen_fd);  //设置为非阻塞模式  保证连接为轮旬方式

    struct sockaddr_in server_addr;
    bzero(&server_addr, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons(port);  //服务器初始化

    int on = 1;
    setsockopt(acceptor1->listen_fd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));   //设置acceptor使用地址可重用

    int rt1 = bind(acceptor1->listen_fd, (struct sockaddr *) &server_addr, sizeof(server_addr));//绑定服务器地址
    if (rt1 < 0) {
        error(1, errno, "bind failed ");
    }

    int rt2 = listen(acceptor1->listen_fd, LISTENQ);
    if (rt2 < 0) {
        error(1, errno, "listen failed ");
    }

//    signal(SIGPIPE, SIG_IGN);

    return acceptor1;//返回连接队
}
