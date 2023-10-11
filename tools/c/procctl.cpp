#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main (){
    printf("aaa\n");

    //same as:
    //execl("/bin/ls", "sur/bin/ls", "-lt", "/tmp",(char*) 0);
    int status = execl("/bin/ls", "ls", "-lt", "/tmp/weather/",(char*) 0);
    
    // 之后的内容就执行不了了， 因为exec用它参数中指定的程序替换了当前进程的正文段，数据段，堆和栈
    // 之后就去执行别人的程序了
    printf("bbb, status=%d\n",status);//除非exec调用失败了，不然这一行代码是怎么都不会得到执行的

    return 0;
}

