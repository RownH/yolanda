#include"../../lib/common.h"
#define MAXLINE 1024
int main(int argc,char **argv){
    if(argc!=3){
        printf("tcp_client <IPaddress> <port>");
        exit(0);
    }
    int port=atoi(argv[2]);

    int sock_fd=socket(AF_INET,SOCK_STREAM,0);
    sockaddr_in server;
    server.sin_family=AF_INET;
    server.sin_port=htons(port);
    inet_pton(AF_INET,argv[1],&server.sin_addr);
    int connect_rt=connect(sock_fd,(sockaddr *)&server,sizeof (server));

    if(connect_rt<-1){
      printf("connect failed");
      exit(0);
    }

    char recv_line[MAXLINE],send_line[MAXLINE];
    int n;

    fd_set readmask;
    fd_set allreads;

    FD_ZERO(&readmask);
    FD_SET(STDIN_FILENO,&allreads);//将输入窗口注册
    FD_SET(sock_fd,&allreads);

    for (;;) {
        readmask=allreads; //每次执行后 重置操作

        int rc=select(sock_fd+1,&readmask,nullptr,nullptr,nullptr); //检测读事件
        if(n<0){
            printf("select failed");
            exit(0);
        }
        if(FD_ISSET(sock_fd,&readmask)){
            n=read(sock_fd,recv_line,MAXLINE);   //如果当前套接字可读
            if(n<0){
                printf("read error !");
                exit(0);
            }
            else if (n==0) {
                printf("server closed \n");
                exit(0);
            }
            recv_line[n]=0;
            fputs(recv_line,stdout);
            fputs("\n",stdout);
        }
        if(FD_ISSET(STDIN_FILENO,&readmask)){
            if(fgets(send_line,MAXLINE,stdin)!=nullptr){
                int i=strlen(send_line);
                if(send_line[i-1]=='\n'){
                    send_line[i-1]=0;
                }
                if(strncmp(send_line,"quit",strlen(send_line))==0){
                    if(shutdown(sock_fd,1)){
                        printf("shutdown faild\n");
                    }
                }
                size_t rt=write(sock_fd,send_line,strlen(send_line));
                if(rt<0){
                    printf("write failed");
                    exit(0);
                }
            }
        }
    }


    return 0;
}
