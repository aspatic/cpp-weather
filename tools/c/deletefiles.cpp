#include "_public.h"

void EXIT(int sig);

int main(int argc, char *argv[])
{
    // print help messages
    if (argc != 4)
    {

        printf("\nUsing:/home/yche/cpp-weather/tools/bin/deletefiles pathname matchstr timeout\n\n");

        printf("Example:/home/yche/cpp-weather/tools/bin/deletefiles /log/idc \"*.log.20*\" 0.02\n");
        printf("        /home/yche/cpp-weather/tools/bin/deletefiles /tmp/surfdata \"*.xml,*.json\" 0.01\n");
        printf("        /home/yche/cpp-weather/tools/bin/procctl 300 /home/yche/cpp-weather/tools/bin/deletefiles /log/idc \"*.log.20*\" 0.02\n");
        printf("        /home/yche/cpp-weather/tools/bin/procctl 300 /home/yche/cpp-weather/tools/bin/deletefiles /tmp/surfdata \"*.xml,*.json\" 0.01\n\n\n");

        printf("这是一个工具程序，用于删除历史的数据文件或日志文件。\n");
        printf("本程序把pathname目录及子目录中timeout(天)之前的匹配matchstr文件全部删除，timeout可以是小数。\n");
        printf("本程序不写日志文件，也不会在控制台输出任何信息。\n\n\n");

        return -1;
    }
    // ignore all IO and sys/signal
    // CloseIOAndSignal(true)
    signal(SIGINT, EXIT);
    signal(SIGTERM, EXIT);

    // get file datetime
    char strTimeOut[21];
    LocalTime(strTimeOut, "yyyy-mm-dd hh24:mi:ss", 0 - (int)(atof(argv[3]) * 24 * 60 * 60));

    // open dir
    CDir Dir;
    if (Dir.OpenDir(argv[1], argv[2], 10000, true) == false)
    {
        printf("Dir.OpenDir(%s) failed.\n", argv[1]);
        return -1;
    }

    // traverse all files in dir
    while (Dir.ReadDir())
    {
        // 判断文件的时间，如果在timeout参数指定的时间点之前，就删除它。
        if (strcmp(Dir.m_ModifyTime, strTimeOut) < 0)
        {
            if (REMOVE(Dir.m_FullFileName) == 0)
            {
                printf("REMOVE %s ok.\n", Dir.m_FullFileName);
            }
            else
            {
                printf("REMOVE %s failed.\n", Dir.m_FullFileName);
            }
        }
    }

    return 0;
}

void EXIT(int sig)
{
    printf("程序退出，sig=%d\n\n", sig);

    exit(0);
}
