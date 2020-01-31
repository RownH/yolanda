#include <iostream>
#include "../lib/common.h"
#define MAXLINE 4096
using namespace  std;
static int count;
int main(int argc,char *argv[])
{
    int socket_fd;
    socket_fd=socket(AF_INET,SOCK_DGRAM,0);
    sockaddr_in server_addr;
    bzero(&server_addr,sizeof (server_addr));

    server_addr.sin_family=AF_INET;
    server_addr.sin_port=htons(SERV_PORT);
    inet_pton(AF_INET,argv[1],&server_addr.sin_addr);
    socklen_t server_len=sizeof (server_addr);
    if(connect(socket_fd,(sockaddr *)&server_addr,server_len)){
        cerr<<"connect failed"<<endl;
    }
    char send_line[MAXLINE],recv_line[MAXLINE+1];
    int n;
    while(fgets(send_line,MAXLINE,stdin)!=NULL){
        int i=strlen(send_line);
        if(send_line[i-1]=='\n'){
            send_line[i-1]=0;
        }
        cout<<"now sending: "<<send_line<<endl;
        size_t rt=send(socket_fd,send_line,strlen(send_line),0);
        if(rt<0){
            cerr<<"send failed"<<endl;
        }
        recv_line[0]=0;
        n=recv(socket_fd,recv_line,MAXLINE,0);
        if(n<0){
            cerr<<"recv failed"<<endl;
        }
        recv_line[n]=0;
        fputs(recv_line,stdout);
        fputs("\n",stdout);
    }
    return 0;
}
