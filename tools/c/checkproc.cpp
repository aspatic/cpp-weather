#include "_public.h"

// 程序运行的日志。
CLogFile logfile;

int main(int argc, char *argv[])
{
    // 程序的帮助。
    if (argc != 2)
    {
        printf("\n");
        printf("Using:./checkproc logfilename\n");

        printf("Example: /home/yche/cpp-weather/tools/bin/procctl 10 //home/yche/cpp-weather/tools/bin/checkproc /tmp/log/checkproc.log\n\n");

        printf("本程序用于检查后台服务程序是否超时，如果已超时，就终止它。\n");
        printf("注意：\n");
        printf("  1）本程序由procctl即调度程序启动，运行周期建议为10秒。\n");
        printf("  2）为了避免被普通用户误杀，本程序应该用root用户启动。\n");
        printf("  3）如果要停止本程序，只能用killall -9 终止。\n\n\n");

        return 0;
    }

    // 打开日志文件。
    if (logfile.Open(argv[1], "a+") == false)
    {
        printf("logfile.Open(%s) failed.\n", argv[1]);
        return -1;
    }
    // 创建/获取共享内存

    // 将共享内存连接到当前进程的地址空间。

    // 遍历共享内存中全部的记录。
    for (int ii=0;ii<MAXNUMP;ii++){
        // 如果记录的pid==0，表示空记录，continue;
        // 如果记录的pid!=0，表示是服务程序的心跳记录。

        // 向进程发送信号0，判断它是否还存在，如果不存在，从共享内存中删除该记录，

        // 如果进程未超时，continue;

        // 如果已超时。
        // 发送信号15，尝试正常终止进程。
        // 每隔1秒判断一次进程是否存在，累计5秒，一般来说，5秒的时间足够让进程退出。
        // 如果进程仍存在，就发送信号9，强制终止它。
        // 从共享内存中删除已超时进程的心跳记录。
    }

    // 把共享内存从当前进程中分离。

    return 0;
}