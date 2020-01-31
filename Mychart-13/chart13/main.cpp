#include <iostream>
#include "../../lib/common.h"
using namespace  std;
static int count;

static void recvfrom_int(int signo){
    cerr<<endl<<count<<" recevice datagrams"<<endl;
    exit(0);
}




int main(int argc,char *argv[])
{
    int socket_fd;
    socket_fd=socket(AF_INET,SOCK_DGRAM,0);
    sockaddr_in server_addr;
    bzero(&server_addr,sizeof (server_addr));
    server_addr.sin_family=AF_INET;
    server_addr.sin_addr.s_addr=htonl(INADDR_ANY);
    server_addr.sin_port=htons(SERV_PORT);

    bind(socket_fd,(sockaddr*)& server_addr,sizeof (server_addr));
    socklen_t client_len;
    char message[MAXLINE];
    message[0]=0;
    count=0;
    signal(SIGINT,recvfrom_int);
    sockaddr_in client_addr;
    int n=recvfrom(socket_fd,message,MAXLINE,0,(sockaddr*) &client_addr,&client_len);
    if(n<0){
        cerr<<"recvfrom failed"<<endl;
    }
    message[n]=0;
    cout<<"received"<<n<<"bytes:"<<message<<endl;
    if(connect(socket_fd,(sockaddr *)&client_addr,sizeof (client_addr))){
        cerr<<"connect failed"<<endl;
    }
    while (strncmp(message,"goodbyte",7)!=0) {
        char send_line[MAXLINE];
        sprintf(send_line,"Hi,%s",message);
        size_t rt=send(socket_fd,send_line,strlen(send_line),0);
        if(rt<0){
            cerr<<"send falied"<<endl;
        }
        printf("send bytes: %d \n",rt);
        size_t rc=recv(socket_fd,message,MAXLINE,0);
        if(rc<0){
            cerr<<"recv failed"<<endl;
        }
        count++;
    }


    return 0;
}
