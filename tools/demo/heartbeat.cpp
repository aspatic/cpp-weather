#include "_public.h"

// 进程心跳信息的结构
struct st_pinfo
{
    int pid; // 进程ID
    char pname[51]; // 进程名称，可以为空
    int timeout; // 预设的超时时间
    time_t atime; // 最近一次的心跳记录，是一个长整数
}

int main (int argc, char* agrv[]){
    if (argc < 2){
        printf("using: ./book procname\n");
        return 0;
    }
    while(1){
        sleep(10);
    }
    
    return 0;
}