#include <iostream>
#include"../../lib/common.h"
#define MAX_LINE 1024
#define MAXEVENTS 128
char rot13_char(char c){
    if((c>='a' && c<='m' || (c>='A' && c<='M'))){
        return c+13;
    }
    else if((c>='n' && c<='z') || (c>'N' &&c<='Z' ))
    {
        return c-13;
    }
    else{
        return c;
    }
}
int main()
{
   int listen_fd,socket_fd;
   int n,i;
   int efd;
   epoll_event event;
   epoll_event *events;
   listen_fd=socket(AF_INET,SOCK_STREAM,0);
   sockaddr_in server_addr;
   server_addr.sin_addr.s_addr=htonl(INADDR_ANY);
   server_addr.sin_family=htons(SERV_PORT);
   server_addr.sin_family=AF_INET;
   bind(listen_fd,(sockaddr *)&server_addr,sizeof (server_addr));
   listen(listen_fd,5);
   efd=epoll_create1(0);
   if(efd==-1){
       printf("epoll create failed");
       exit(0);
   }
   event.data.fd=listen_fd;
   event.events=EPOLLIN | EPOLLET;// ET模式
   if(epoll_ctl(efd,EPOLL_CTL_ADD,listen_fd,&event)==-1){
       printf("epoll_ctl add listen fd failed");
       exit(0);
   }
   events=(epoll_event *)calloc(MAXEVENTS,sizeof (event));
   while (1) {
        n=epoll_wait(efd,events,MAXEVENTS,-1);//阻塞模式
        printf("epoll_wait wakeup\n");
        for (i=0;i<n;i++) {
            if((events[i].events &EPOLLERR) || (events[i].events &EPOLLHUP) ||(!(events[i].events &EPOLLIN))){
                fprintf(stderr,"epoll error\n");
                close(events[i].data.fd);
                continue;
            }
            else if(listen_fd==events[i].data.fd){
                sockaddr_storage ss;
                socklen_t slen=sizeof (ss);
                int fd=accept(listen_fd,(sockaddr *)&ss,&slen);
                if(fd<0){
                    printf("accept failed");
                }
                else{
                    make_nonblocking(fd);
                    event.data.fd=fd;
                    event.events =EPOLLIN | EPOLLET;
                    if(epoll_ctl(efd,EPOLL_CTL_ADD,fd,&event)==-1){
                        printf("epoll_ctl add connection fd failed");
                        exit(0);
                    }
                }
                continue;
            }
            else{
                socket_fd=events[i].data.fd;
                printf("get event on socket fd==%d \n",socket_fd);
                while (1) {
                    char buf[512];
                    if((n=read(socket_fd,buf,sizeof (buf)))<0){
                        if(errno!=EAGAIN){
                            printf("read error");
                            close(socket_fd);
                        }
                        break;
                    }
                    else if(n==0){
                        close(socket_fd);
                        break;
                    }
                    else {
                        for (i=0;i<n;i++) {
                            buf[i]=rot13_char(buf[i]);
                        }
                        if(write(socket_fd,buf,n)<0){
                            printf("write error");
                            exit(0);
                        }
                    }
                }
                }
            }
        }
   free(events);
   close(listen_fd);
}
