#include "_public.h"
void EXIT(int sig)
{
    printf("sig=%d\n", sig);
    exit(0);
}

CPActive Active;

int main(int argc, char *argv[])
{
    if (argc < 3)
    {
        printf("using: ./book procname timeout\n");
        return 0;
    }
    // signal handle for 2 and 15
    signal(2, EXIT);
    signal(15, EXIT);
    Active.AddPInfo(atoi(argv[2]), argv[1]);
    while (1)
    {
        Active.UptATime();
        sleep(10);
    }
    return 0;
}
