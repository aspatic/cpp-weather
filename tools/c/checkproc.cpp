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

        printf("Example: /home/yche/cpp-weather/tools/bin/procctl 10 /home/yche/cpp-weather/tools/bin/checkproc /tmp/log/checkproc.log\n\n");

        printf("本程序用于检查后台服务程序是否超时，如果已超时，就终止它。\n");
        printf("注意：\n");
        printf("  1）本程序由procctl即调度程序启动，运行周期建议为10秒。\n");
        printf("  2）为了避免被普通用户误杀，本程序应该用root用户启动。\n");
        printf("  3）如果要停止本程序，只能用killall -9 终止。\n\n\n");

        return 0;
    }
    // 忽略全部的信号和IO，不希望程序被干扰。
    CloseIOAndSignal(true);

    // 打开日志文件。
    if (logfile.Open(argv[1], "a+") == false)
    {
        printf("logfile.Open(%s) failed.\n", argv[1]);
        return -1;
    }
    // 创建/获取共享内存

    // 创建（或获取）共享内存，大小为 MAXNUMP * sizeof(struct st_procinfo)
    int shmid = 0;
    if ((shmid = shmget(SHMKEYP, MAXNUMP * sizeof(struct st_procinfo), 0666 | IPC_CREAT)) == -1)
    {
        logfile.Write("创建/获取共享内存(%x)失败。\n", SHMKEYP);
        return false;
    }

    // 将共享内存连接到当前进程的地址空间。
    struct st_procinfo *shm;
    shm = (struct st_procinfo *)shmat(shmid, 0, 0);

    // 遍历共享内存中全部的记录。
    for (int i = 0; i < MAXNUMP; i++)
    {
        // 如果记录的pid==0，表示空记录，continue;
        if (shm[i].pid == 0)
            continue;

        // 如果记录的pid!=0，表示是服务程序的心跳记录。
        logfile.Write("ii=%d,pid=%d,pname=%s,timeout=%d,atime=%d\n", i, shm[i].pid, shm[i].pname, shm[i].timeout, shm[i].atime);

        // 向进程发送信号0，判断它是否还存在，如果不存在，从共享内存中删除该记录，
        int iret = kill(shm[i].pid, 0);
        if (iret == -1)
        {
            logfile.Write("进程pid=%d(%s)已经不存在。\n", (shm + i)->pid, (shm + i)->pname);
            memset(shm + i, 0, sizeof(struct st_procinfo)); // 从共享内存中删除该记录。
            continue;
        }

        time_t now = time(0); // 取当前时间。
                              // 如果进程未超时，continue;
        if (now - shm[i].atime < shm[i].timeout)
            continue;
        // 如果已超时。
        logfile.Write("进程pid=%d(%s)已经超时。\n", (shm + i)->pid, (shm + i)->pname);
        // 发送信号15，尝试正常终止进程。
        kill(shm[i].pid, 15);
        // 每隔1秒判断一次进程是否存在，累计5秒，一般来说，5秒的时间足够让进程退出。
        for (int j = 0; j < 5; j++)
        {
            sleep(1);
            iret = kill(shm[i].pid, 0); // 向进程发送信号0，判断它是否还存在。
            if (iret == -1)
                break; // 进程已退出。
        }
        if (iret == -1)
            logfile.Write("进程pid=%d(%s)已经正常终止。\n", (shm + i)->pid, (shm + i)->pname);
        else // 如果进程仍存在，就发送信号9，强制终止它。
        {
            kill(shm[i].pid, 9);
            logfile.Write("进程pid=%d(%s)已经强制终止。\n", (shm + i)->pid, (shm + i)->pname);
        }
        // 从共享内存中删除已超时进程的心跳记录。
        memset(shm + i, 0, sizeof(struct st_procinfo));
    }

    // 把共享内存从当前进程中分离。
    shmdt(shm);
    return 0;
}