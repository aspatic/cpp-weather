#include "_public.h"

void EXIT(int sig);

int main(int argc, char* argv[]){
    // print help messages
    if (argc != 4){

        printf("\nUsing:/home/yche/cpp-weather/tools/bin/gzipfiles pathname matchstr timeout\n\n");

        printf("Example:/home/yche/cpp-weather/tools/bin/gzipfiles /log/idc \"*.log.20*\" 0.02\n");
        printf("        /home/yche/cpp-weather/tools/bin/gzipfiles /tmp/surfdata \"*.xml,*.json\" 0.01\n");
        printf("        /home/yche/cpp-weather/tools/bin/procctl 300 /home/yche/cpp-weather/tools/bin/gzipfiles /log/idc \"*.log.20*\" 0.02\n");
        printf("        /home/yche/cpp-weather/tools/bin/procctl 300 /home/yche/cpp-weather/tools/bin/gzipfiles /tmp/surfdata \"*.xml,*.json\" 0.01\n\n\n");

        printf("这是一个工具程序，用于压缩历史的数据文件或日志文件。\n");
        printf("本程序把pathname目录及子目录中timeout天之前的匹配matchstr文件全部压缩，timeout可以是小数。\n");
        printf("本程序不写日志文件，也不会在控制台输出任何信息。\n");
        printf("本程序调用/usr/bin/gzip命令压缩文件。\n\n\n");
    }
    // ignore all IO and sys/signal

    // get file datetime

    // open dir

    // traverse all files in dir
    while(1){

    }
    
    return 0;
}


void EXIT(int sig)  
{
  printf("程序退出，sig=%d\n\n",sig);

  exit(0);
}
