#include <iostream>
#include "../../lib/common.h"
#include"messege.h"

#define MAXLINE 4096
#define KEEP_ALIVE_TIME 10
#define KEEP_ALIVE_INTERVAL 3
#define KEEP_ALIVE_PROBETIMES 3

int main()
{
    int socket_fd;
    socket_fd=socket(AF_INET,SOCK_STREAM,0);
    sockaddr_in server_addr;
    bzero(&server_addr,sizeof(server_addr));
    server_addr.sin_family=AF_INET;
    server_addr.sin_port=htons(8001);
    inet_pton(AF_INET,"127.0.0.1",&server_addr.sin_addr);


    socklen_t server_len=sizeof(server_addr);
    int connect_rt=connect(socket_fd,(sockaddr *)&server_addr,server_len);
    if(connect_rt<0){
        printf("connection failed :%d",errno);
        exit(0);
    }
    char recv_line[MAXLINE+1];
    int n;
    fd_set readmask;
    fd_set allreads;
    timeval tv;
    int heartbeats=0;

    tv.tv_sec=KEEP_ALIVE_TIME;
    tv.tv_usec=0;

    messageObject messageObject;
    FD_ZERO(&allreads);//初始化
    FD_SET(socket_fd,&allreads);
    for(;;){
        readmask=allreads;
        int rc=select(socket_fd+1,&readmask,nullptr,nullptr,&tv);
        if(rc<0){
            printf("select error\n");
            exit(0);
        }
        if(0==rc){
            if(++heartbeats>KEEP_ALIVE_PROBETIMES){
                printf("connection dead\n");
                exit(0);
            }
            printf("sending heartbeat #%d\n",heartbeats);
            messageObject.type=htonl(MSG_PING);
            rc=send(socket_fd,(char*)&messageObject,sizeof(messageObject),0);
            if(rc<0){
                printf("send failtrue");
            }
            tv.tv_sec=KEEP_ALIVE_TIME;
            continue;
        }
        if(FD_ISSET(socket_fd,&readmask)){
            n=read(socket_fd,recv_line,MAXLINE);
            if(n<0){
                    printf("read error");
            }
            else if(n==0){
                printf("server terminated \n");
                exit(0);
            }
            printf("received heartbeat ,make heartbeats to 0\n");
            heartbeats=0;
            tv.tv_sec=KEEP_ALIVE_TIME;
        }
    }
}
