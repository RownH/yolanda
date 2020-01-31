#include <iostream>
#include "../../lib/common.h"
#include"messege.h"
static int count;
int main()
{
    int sleepingTime=50;
    int listenfd;
    listenfd=socket(AF_INET,SOCK_STREAM,0);
    sockaddr_in server_addr;
    bzero(&server_addr,sizeof (server_addr));
    server_addr.sin_family=AF_INET;
    inet_pton(AF_INET,"127.0.0.1",&server_addr.sin_addr);
    server_addr.sin_port=htons(8001);
    int rt1=bind(listenfd,(sockaddr *)&server_addr,sizeof (server_addr));
    if(rt1<0){
        printf("bind failed");
        exit(0);
    }
    int rt2=listen(listenfd,LISTENQ);
    if(rt2<0){
        printf("listen failed");
        exit(0);
    }
    int connfd;
    sockaddr_in client_addr;
    socklen_t client_len=sizeof (client_addr);
    if(connfd=accept(listenfd,(sockaddr *) &client_addr,&client_len))
    {
        printf("bind failed");
        exit(0);
    }
    messageObject message;
    count=0;
    for (;;) {
         int n=read(connfd,(char *)&message,sizeof (messageObject));
         if(n<0){
             printf("error read");
             exit(0);
         }
         else if(n==0){
             printf("client closed");
             exit(0);
         }
         printf("recive %d bytes\n",n);
         count++;
         switch (ntohl(message.type)) {
            case MSG_TYPE1:
                printf("process MSG_TYPE1 \n");
                break;
            case MSG_TYPE2:
                printf("process MSG_TYPE2 \n");
                break;
         case MSG_PING:
         {
             messageObject pong_message;
             pong_message.type=MSG_PONG;
             sleep(sleepingTime);
             size_t rc=send(connfd,(char *)&pong_message,sizeof(pong_message),0);

             if(rc<0){
                 printf("send failure");
             }
             break;
                }
         default:
             printf("unknow message type %d\n",ntohl(message.type));
         }
    }




}
