#include <iostream>
#include"../../lib/common.h"
#define FD_INIT_SIZE 128
#define MAX_LINE 1024
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
struct Buffer{
    int connect_fd;//套接字
    char buffer[MAX_LINE];
    size_t writeIndex;//缓冲写入
    size_t readIndex;//缓冲读取位置
    int readable;//是否可读
};
Buffer * alloc_Buffer(){
    Buffer * buffer=(Buffer*)malloc(sizeof (Buffer));
    if(!buffer){
        return NULL;
    }
    buffer->connect_fd=0;
    buffer->writeIndex=buffer->readIndex=buffer->readable=0;
    return buffer;
}
void free_Buffer(Buffer *buffer){
    free(buffer);
}

int onSocketRead(int fd,Buffer *buffer){
    char buf[1024];
    int i;
    ssize_t result;
    while (1) {
        result=recv(fd,buf,sizeof (buf),0);
        if(result==0){
            return 1;
        }
        else if(result<0){
            if(errno==EAGAIN){
                return 0;
            }
            return 1;
        }
        for (i=0;i<result;++i) {
            if(buffer->writeIndex< sizeof(buffer->buffer)){
                buffer->buffer[buffer->writeIndex++]=rot13_char(buf[i]);
            }
            if(buf[i]=='\n'){
                buffer->readable=1;
            }
        }
    }
    return 0;
}
int onSocketWrite(int fd,Buffer * buffer){
    while (buffer->readIndex<buffer->writeIndex) {
      size_t result=send(fd,buffer->buffer+buffer->readIndex,buffer->writeIndex,0);
      if(result<0){
          if(errno=EAGAIN){
              return 0;
          }
          return -1;
      }
      buffer->readIndex+=result;
    }
    if(buffer->readIndex==buffer->writeIndex){
        buffer->readIndex=buffer->writeIndex=0;
    }
    return 0;
}

int main(int argc,char **argv)
{
    int listen_fd;
    int i,maxfd;
    Buffer *buffer[FD_INIT_SIZE];
    for (i=0;i<FD_INIT_SIZE;i++) {
        buffer[i]=alloc_Buffer();
    }
    sockaddr_in server_addr;
    listen_fd=socket(AF_INET,SOCK_STREAM,0);
    server_addr.sin_addr.s_addr=htonl(INADDR_ANY);
    server_addr.sin_family=AF_INET;
    server_addr.sin_port=htons(SERV_PORT);

    fd_set readset,writeset,exset;
    FD_ZERO(&readset);
    FD_ZERO(&writeset);
    FD_ZERO(&exset);

    while (1) {
        maxfd=listen_fd;
        FD_ZERO(&readset);
        FD_ZERO(&writeset);
        FD_ZERO(&exset);

        FD_SET(listen_fd,&readset);

        for (i=0;i<FD_INIT_SIZE;++i) {
            if(buffer[i]->connect_fd>0){
                if(buffer[i]->connect_fd>maxfd){
                    maxfd=buffer[i]->connect_fd;
                }
                FD_SET(buffer[i]->connect_fd,&readset);
                if(buffer[i]->readable){
                    FD_SET(buffer[i]->connect_fd,&writeset);
                }
            }
        }
        if(select(maxfd+1,&readset,&writeset,&exset,NULL)<0){
            printf("select error");
        }
        if(FD_ISSET(listen_fd,&readset)){
            printf("listening socket readable ");
            sleep(5);
            sockaddr_storage ss;
            socklen_t slen=sizeof (ss);
            int fd=accept(listen_fd,(sockaddr *)&ss,&slen);
            if(fd<0){
                printf("accept failed");
                exit(0);
            }
             else if(fd>FD_INIT_SIZE){
                printf("too many connections");
                close(fd);
            }
             else{
             make_nonblocking(fd);
             if(buffer[fd]->connect_fd==0){
             buffer[fd]->connect_fd=fd;
        }else{
                    printf("too mant conntion");
                    exit(0);
            }
        }
        }
        for (i=0;i<maxfd+1;i++) {
            int r=0;
            if(i==listen_fd)continue;
            if(FD_ISSET(i,&readset)){
                r=onSocketRead(i,buffer[i]);
            }
            if(FD_ISSET(i,&writeset)){
                r=onSocketWrite(i,buffer[i]);
            }
                if(r){
                    buffer[i]->connect_fd=0;
                    close(i);
                }
            }


        }
    return 0;
}
