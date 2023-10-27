#include "_public.h"

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