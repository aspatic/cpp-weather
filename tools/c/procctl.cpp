#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(int argc, char* argv[]){
    if (argc < 3){
        printf("./procctl timetvl program argv ...\n");
        printf("Example: /home/yche/cpp-weather/tools/c/procctl 5 /bin/tar -zcvf /tmp/weather/tmp.tgz /usr/include\n");

        printf("本程序是服务程序的调度程序，周期性地启动程序或shell脚本。\n");
        printf("timetvl 运行周期， 单位：秒 被调度的程序运行结束后在间隔timetvl秒后会被procctl重新启动。\n");
        printf("program 被调度的程序名，必须使用全路径。\n");
        printf("argv ... 被调度的程序的参数。\n");
        printf("注意，本程序不会被kill杀死，但可以被kill -9强行杀死。\n\n\n");

        return -1;
    }

    // 关闭信号和IO，本程序是后台调度，不希望被打扰
    for (int j=0; j<64; j++){
        signal(j, SIG_IGN);
        close(j);
    }
    // 生成子进程 父进程退出 让子进程运行在后台交由1号进程收养
    if (fork()!=0) exit(0); //好像在wsl中实际上是518号进程
    //启用SIGCHLD信号，让父进程可以通过wait取得子进程的退出状态
    signal(SIGCHLD,SIG_DFL);

    char* pargv[argc];
    for (int i = 2; i < argc; i++){
        pargv[i-2] = argv[i];
    }
    pargv[argc-2] = NULL; // 为什么要这么写呢，做个实验

    while (true){
        int pid = fork();
        if (pid == 0){ //child
            execv(argv[2], pargv); // 如果这一句调用失败了会怎样
            exit(0); // 需要补上这一句处理调用失败
        }
        else if(pid > 0){ //parent
            int status;
            wait(&status);
            sleep(atoi(argv[1]));
        }
        else{
            printf("fork error");
        }
    }

    return 0;
}

