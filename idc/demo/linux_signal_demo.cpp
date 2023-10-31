#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>

// If you don't set signal handler of your own, the system 
// will use default handler, which is usually teminating the process.
// That's not what we want.
void sigHandler(int sig){
    // echo signal handling 
    printf("接收到了%d信号。\n",sig);
}

void alarmHandler(int num){
    // echo signal handling 
    printf("接收到了时钟信号%d。\n",num);
    alarm(3);
}

void EXIT(int sig){
    // echo signal handling 
    printf("接收到了%d信号,程序将退出。\n",sig);
    //在这里编写关闭前善后的代码

    //
    exit(0);
}

int main(){

    //setting system signal handler
    for (int i=1; i<=64; i++){ // Total 64 types of signal in linux
        // but be adviced SIG=9 cannot be captured in any way
        signal(i, sigHandler);
    }
    // You can also overwrite previous handler settings
    signal(15,SIG_IGN);
    signal(13,SIG_DFL);
    // SIG=9 cannot be ignored either, the following code won't work
    signal(9,SIG_IGN);
    
    // 设置Ctrl+c 和killall的处理函数,注意这个覆盖了SIG=15
    signal(SIGINT, EXIT); 
    signal(SIGTERM,EXIT);

    //alram signal handler
    signal(14,SIG_DFL);
    signal(SIGALRM, alarmHandler);

    alarm(3);
    while(1){
        printf("执行了一次任务。\n");
        sleep(1);
    }
}