#include <stdio.h>
#include <unistd.h>

int main(){
    while(1){
        printf("执行了一次任务。\n");
        sleep(1);
    }
}