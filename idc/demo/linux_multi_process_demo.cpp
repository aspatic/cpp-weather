// 子进程是父进程的副本
// 子进程获得了一份父进程的数据空间，队和栈的副本，不是同一个文件的共享
// 父进程中打开的文件描述符也复制了一份给子进程，可能会导致之前的缓冲内容被两个进程各写入一遍
// 如果父进程先退出，子进程会成为孤儿进程，会导致被1号进程收养
// 如果子进程先退出，内核会给父进程发送SIGCHLD信号，如果父进程没有处理这个信号，子进程就会变成僵尸进程

int main(){
    int pid = fork();
    if (pid==0){
        printf("This is the child process #%d running.\n",getpid());
        sleep(5);
    }
    if (pid > 0){
        printf("This is the parent process #%d running.\n",getpid());
        sleep(10);
    }
    else{
        printf("fork error\n");
    }
}