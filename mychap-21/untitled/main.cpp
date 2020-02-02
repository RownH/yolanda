#include"../../lib/common.h"

#define INIT_SIZE 128

int main(int argc,char **argv)
{
    int listen_fd,connected_fd;
    int ready_number;
    ssize_t n;
    char buf[MAXLINE];
    sockaddr_in client_addr;

    sockaddr_in server_in;
    server_in.sin_addr.s_addr=htonl(INADDR_ANY);
    server_in.sin_family=AF_INET;
    server_in.sin_port=htons(SERV_PORT);
    listen_fd=socket(AF_INET,SOCK_STREAM,0);
    size_t rt=bind(listen_fd,(sockaddr *)&server_in,sizeof (listen_fd));
    if(rt<0){
        printf("bind falied");
        exit(0);
    }
    listen(listen_fd,5);
    pollfd event_set[INIT_SIZE];
    event_set[0].fd=listen_fd;
    event_set[0].events=POLLRDNORM;

    int i;
    for (i=1;i<INIT_SIZE;i++) {
        event_set[i].fd=-1;
        //跳过检测
    }
    for (;;) {
        if((ready_number=poll(event_set,INIT_SIZE,-1))<0){
            printf("poll failed");
            exit(0);
        }
        if(event_set[0].revents &POLLRDNORM){
            socklen_t client_len=sizeof (client_addr);
            connected_fd=accept(listen_fd,(sockaddr *)&client_addr,&client_len);
            for (i=1;i<INIT_SIZE;i++) {
                if(event_set[i].fd<0){
                    event_set[i].fd=connected_fd;
                    event_set[i].events=POLLRDNORM;
                    break;
                }
            }
            if(i==INIT_SIZE){
                printf("can not hold so many client");
            }
            if(--ready_number<=0)continue;
        }
        for (i=1;i<INIT_SIZE;i++) {
            int socket_fd;
            if((socket_fd=event_set[i].fd)<0){
                continue;
            }
            if(event_set[i].revents &(POLLRDNORM | POLLERR)){
                if((n=read(socket_fd,buf,MAXLINE))>0){
                    if((write(socket_fd,buf,n))<0){
                        printf("write error ");
                        exit(0);
                    }
                }
                else if(n==0 || errno==ECONNRESET){
                    close(connected_fd);
                    event_set[i].fd=-1;
                }
                else{
                    printf("read error");
                    exit(0);
                }
                if(--ready_number<=0){
                    break;
                }
            }
        }
    }
}
