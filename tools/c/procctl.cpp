#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(int argc, char* argv[]){

    // 用fork创建一个子进程，在子进程中调用execl执行新的程序
    // 只有子进程会被新程序替换，父进程只需要等待子进程结束
    // 这样实现程序调用的功能
    
    // Sample of function calls:
    // ./procctl 5 /bin/ls -lt /tmp/weather/

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

