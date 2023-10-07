#include "_public.h"

CLogFile logfile;

int main(int argc, char* argv[]){
    if (argc!=4){
        printf("Using:/crtsurfdata inifile outpath logfile\n");
        printf("Example:/home/yche/cpp-weather/idc/bin/crtsurfdata /home/yche/cpp-weather/idc/ini/stcode.ini /tmp/surfdata /log/idc/crtsurfdata.log\n\n");
        printf("inifile 全国气象站点参数文件名。\n");
        printf("outpath 全国气象站点数据文件存放的目录。\n");
        printf("logfile 本程序运行的日志文件名。\n\n");

        return -1;
    }
    
    if (logfile.Open(argv[3])==false){
        printf("logfile.Open(%s) failed.\n",argv[3]); 
        return -1;
    }
    
    logfile.Write("crtsurfdata1 开始运行。\n");

    logfile.WriteEx("crtsurfdata1 运行结束。\n");

    return 0;
}