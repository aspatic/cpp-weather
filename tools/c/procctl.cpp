#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(int argc, char* argv[]){
    if (argc < 3){
        printf("Using: ./procctl timetvl program argv ...\n");
        printf("Example: ./procctl 5 /bin/ls -lt /tmp/weather/\n");

        printf("本程序是服务程序的调度程序，周期性地启动程序或shell脚本。\n");
        printf("timetvl 运行周期， 单位：秒 被调度的程序运行结束后在间隔timetvl秒后会被procctl重新启动。\n");
        printf("program 被调度的程序名，必须使用全路径。\n");
        printf("argv ... 被调度的程序的参数。\n");
        printf("注意，本程序不会被kill杀死，但可以被kill -9强行杀死。\n\n\n");
    }

    // 关闭信号和IO，本程序是后台调度，不希望被打扰
    for (int j=0; j<64; j++){
        signal(j, SIG_IGN);
        close(j);
    }

    char* pargv[argc];
    for (int i = 2; i < argc; i++){
        pargv[i-2] = argv[i];
    }
    pargv[argc-2] = NULL; // 为什么要这么写呢，做个实验

    while (true){
        if (fork()==0){
            // for example:
            //int status = execl("/bin/ls", "ls", "-lt", "/tmp/weather/",(char*) 0);
            // if (argc == 3) execl(argv[2], argv[2], (char*) 0);    
            // if (argc == 4) execl(argv[2], argv[2], argv[3], (char*) 0);
            // if (argc == 5) execl(argv[2], argv[2], argv[3], argv[4],(char*) 0);
            
            // 为了更加优雅地实现这个程序， 我们用另外一种exec程序来实现
            execv(argv[2], pargv);


        }
        else if (fork > 0){
            int status;
            wait(&status);
            sleep(atoi(argv[1]));
        }
    }

    return 0;
}

