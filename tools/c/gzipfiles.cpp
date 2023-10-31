#include "_public.h"

void EXIT(int sig);

int main(int argc, char* argv[]){
    
    return 0;
}


void EXIT(int sig)  
{
  printf("程序退出，sig=%d\n\n",sig);

  exit(0);
}
