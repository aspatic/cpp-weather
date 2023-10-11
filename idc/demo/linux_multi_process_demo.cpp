#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

#include <sys/types.h>
#include <sys/wait.h>

// 子进程是父进程的副本
// 子进程获得了一份父进程的数据空间，队和栈的副本，不是同一个文件的共享
// 父进程中打开的文件描述符也复制了一份给子进程，可能会导致之前的缓冲内容被两个进程各写入一遍
// 如果父进程先退出，子进程会成为孤儿进程，会导致被1号进程收养
// 如果子进程先退出，内核会给父进程发送SIGCHLD信号，如果父进程没有处理这个信号，子进程就会变成僵尸进程

// 如果子进程在父进程终止之前终止，内核会为这个自进程保留一个数据结构，包括进程编号，终止状态和使用CPU的时间等
// 父进程如果处理了这个子进程的退出信息，内核就会彻底释放掉这个数据结构
// 如果父进程没有处理子进程的退出信息，内核就不会释放该数据结构，子进程的进程编号也会一直被占用直到父进程也退出
// 由于系统总共可用的内存和进程编号都是有限的，如果大量产生僵尸进程，会耗尽系统的资源导致不能产生新的进程
// 这就是僵尸的危害

// 如果父进程在子进程终止前就退出，子进程就会成为孤儿进程，将被systemd即1号进程收养
// 由1号进程对他们进行状态收集工作 并不会有什么危害

///////////////////////////////
// 方法三的信号处理函数 注意有wait
void func(int SIG){
    int pid, status;
    pid = wait(&status);
}
///////////////////////////////

int main(){
    //////////////////////////////////////////////
    // 防止子进程变成僵尸的方法一：设置忽略SIGCHLD信号
    //signal(SIGCHLD,SIG_IGN);
    //////////////////////////////////////////////

    //////////////////////////////////////////////
    // 防止子进程变成僵尸的方法三：
    // 在信号SIGCHILD的处理函数中wait，因此在收到信号之前不需要阻塞父进程
    signal(SIGCHLD,func);
    //////////////////////////////////////////////
    int pid = fork();
    if (pid==0){
        printf("This is the child process #%d running.\n",getpid());
        sleep(5);
    }
    else if (pid > 0){
        printf("This is the parent process #%d running.\n",getpid());
        //////////////////////////////////////////////
        // 防止子进程变成僵尸的方法二：阻塞父进程，等得子进程退出
        //int status;
        //pid = wait(&status);
        //////////////////////////////////////////////

        sleep(10);// 如果采用方法三 这个睡眠睡不到10秒就会被SIGCHILD打断，（即提前结束）然后进入信号处理函数，
        sleep(10);// SIGCHILD属于软中断，只会打断上一句，这一句可以得到完整执行
    }
    else{
        printf("fork error\n");
    }
}