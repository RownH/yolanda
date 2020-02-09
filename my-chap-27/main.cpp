#include <iostream>
#include"../lib/acceptor.h"
#include"../lib/common.h"
#include"../lib/tcp_server.h"
#include"../lib/buffer.h"
using namespace std;
char rot13_char(char c){
    if ((c >= 'a' && c <= 'm') || (c >= 'A' && c <= 'M'))
        return c + 13;
    else if ((c >= 'n' && c <= 'z') || (c >= 'N' && c <= 'Z'))
        return c - 13; else return c;
}
//连接之后
int onConnectionCompleted(tcp_connection *tcpConnection){
    printf("connection completed \n");
    return 0;
};
int onMessage(buffer * input,tcp_connection *tcpConnection){
    printf("get message from tcp connection %s \n",tcpConnection->name);
    printf("%s",input->data);
    buffer *output=buffer_new();
    int size=buffer_readable_size(input);
    for(int i=0;i<size;i++){
        buffer_append_char(output,rot13_char(buffer_read_char(input)));
    }
    tcp_connection_send_buffer(tcpConnection,output);
    return 0;
}
int onWriteCompleted(tcp_connection *tcpConnection){
    printf("write completed \n");
    return 0;
}
int onConnectionClosed(tcp_connection *tcpConnection){
    printf("connection closed\n");
    return 0;
}
int main()
{
    event_loop *eventLoop=event_loop_init();
    acceptor *acceptor=acceptor_init(SERV_PORT);
    TCPserver *tcpServer=tcp_server_init(eventLoop,acceptor,onConnectionCompleted,onMessage,onWriteCompleted,onConnectionClosed,0);
    tcp_server_start(tcpServer);
    event_loop_run(eventLoop);

    return 0;
}
