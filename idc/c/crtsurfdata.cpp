#include "_public.h"



struct st_stcode{
    char provName[31]; // 省 
    char obtId[11]; // 站号
    char obtName[31]; // 站名
    double lat; // 纬度
    double lon; // 经度
    double height; // 高度
};

CLogFile logfile;
std::vector<struct st_stcode> vstcode;

bool LoadSTCode(const char* inifile){
    
    // 打开站点参数文件
    CFile File;
    if (File.Open(inifile,"r")==0){
        logfile.Write("File.Open(%s) failed.\n",inifile);
        return false;
    }

    char strBuffer[301];
    struct st_stcode stcode;
    CCmdStr CmdStr;
    while (true){
        // read one line at a time
        if(File.Fgets(strBuffer,300,true)==0) break;

        CmdStr.SplitToCmd(strBuffer,",",true);
        // drop table header line
        if (CmdStr.CmdCount()!= 6) continue;

        CmdStr.GetValue(0, stcode.provName,30);// 省 
        CmdStr.GetValue(1, stcode.obtId,10);// 站号
        CmdStr.GetValue(2, stcode.obtName,30);// 站名
        CmdStr.GetValue(3, &stcode.lat);// 纬度
        CmdStr.GetValue(4, &stcode.lon);// 经度
        CmdStr.GetValue(5, &stcode.height);// 高度
        
        vstcode.push_back(stcode);
    }
    /*
    for (int i=0; i<vstcode.size(); ++i){
        logfile.Write("provName=%s,obtId=%s,obtName=%s,lat=%.2f,lon=%.2f,height=%.2f\n", vstcode[i].provName, vstcode[i].obtId, vstcode[i].obtName, vstcode[i].lat, vstcode[i].lon, vstcode[i].height);
    }
    */
    return true;
}


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
    
    logfile.Write("crtsurfdata2 开始运行。\n");

    if(LoadSTCode(argv[1])==0){
        return -1;
    }

    logfile.WriteEx("crtsurfdata2 运行结束。\n");

    return 0;
}