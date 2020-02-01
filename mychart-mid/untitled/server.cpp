#include"../../lib/common.h"
static int count; //总数

static void sig_int(int signo){
    printf("\n received %d datagrams \n",count);//程序退出时 打印信息
}
char *run_cmd(char *cmd){
    char *data=(char*)malloc(10384);
    bzero(data,sizeof (data));
    FILE *fdp;  //创建文件
    const int max_buffer=256;
    char buffer[max_buffer];
    fdp=popen(cmd,"r"); //打开终端
    char * data_index=data;
    if(fdp){
        while (!feof(fdp)) {
            if(fgets(buffer,max_buffer,fdp)!=nullptr){
                int len=strlen(buffer);
                memcpy(data_index,buffer,sizeof(buffer));
                data_index+=len;
            }
        }
        pclose(fdp);
    }
    return data;
}
int main(int argc,char **argv){
    int listenfd;
    listenfd=socket(AF_INET,SOCK_STREAM,0);
    sockaddr_in server_in;
    bzero(&server_in,sizeof (server_in));
    server_in.sin_family=AF_INET;
    server_in.sin_addr.s_addr=htonl(INADDR_ANY);
    server_in.sin_port=htons(SERV_PORT);

    int on=1;
    setsockopt(listenfd,SOL_SOCKET,SO_REUSEADDR,&on,sizeof (on));
    int rt1=bind(listenfd,(sockaddr *)&server_in,sizeof (server_in));
    if(rt1<0){
        printf("bind faild");
        exit(0);
    }
    signal(SIGPIPE,SIG_IGN);//默认处理函数
    int connfd;
    sockaddr_in client_addr;
    socklen_t client_len=sizeof (client_addr);
    listen(listenfd,5);
    char buf[256];
    count=0;
    while (1) {
        if((connfd=accept(listenfd,(sockaddr *)&client_addr,&client_len))<0){
               printf("bind failed");
               exit(0);
        }
            while (1) {
                bzero(buf,sizeof (buf));
                int n =read(connfd,buf,sizeof (buf));
                if(n<0){
                    printf("client closed");
                    exit(0);
                }
                else if(n==0){
                    printf("client closed");
                    close(connfd);
                    break;
                }
                count++;
                buf[n]=0;
                if(strncmp(buf,"ls",n)==0){
                    char *result=run_cmd("ls");
                    if(send(connfd,result,sizeof (result),0)){
                        return 1;
                    }
                    free(result);
                }
                else if(strncmp(buf,"pwd",n)==0){
                    char *result=getcwd(buf,256);
                    if(send(connfd,result,strlen(result),0)<0)
                        return 1;
                }
                else if(strncmp(buf,"cd ",3)){
                    char target[256];
                    bzero(target,sizeof (target));
                    memcpy(target,buf+3,strlen(buf)-3);
                    if(chdir(target)==-1){
                        printf("change dir failed,%s \n",target);
                    }
                }
                else{
                    char *error="error:unkown input type";
                    if(send(connfd,error,strlen(error),0)<0)
                        return 1;
                }
            }
    }
    exit(0);
}
