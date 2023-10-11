#include <stdio.h>
#include <unistd.h>
#include <signal.h>

// If you don't set signal handler of your own, the system 
// will use default handler, which is usually teminating the process.
// That's not what we want,=
void sigHandler(int sig){
    // echo signal handling 
    printf("接收到了%d信号。\n",sig);
}

int main(){

    //setting system signal handler
    for (int i=1; i<=64; i++){ // Total 64 types of signal in linux
        signal(i, sigHandler);
    }
    while(1){
        printf("执行了一次任务。\n");
        sleep(1);
    }
}