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
            }
        }
    }



}
