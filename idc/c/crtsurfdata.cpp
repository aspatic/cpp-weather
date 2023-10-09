#include "_public.h"


CLogFile logfile;

struct st_stcode{// 全国气象站点参数结构体。
    char provName[31]; // 省 
    char obtId[11]; // 站号
    char obtName[31]; // 站名
    double lat; // 纬度
    double lon; // 经度
    double height; // 高度
};

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

struct st_surfdata{// 全国气象站点分钟观测数据结构
    char obtid[11];      // 站点代码。
    char ddatetime[21];  // 数据时间：格式yyyymmddhh24miss
    int  t;              // 气温：单位，0.1摄氏度。
    int  p;              // 气压：0.1百帕。
    int  u;              // 相对湿度，0-100之间的值。
    int  wd;             // 风向，0-360之间的值。
    int  wf;             // 风速：单位0.1m/s
    int  r;              // 降雨量：0.1mm。
    int  vis;            // 能见度：0.1米。
};

vector<struct st_surfdata> vsurfdata;

void CrtSurfData(){
    // 模拟生成全国气象站点分钟观测数据，存放在vsurfdata容器中。
    srand(time(0));

    // 获取当前时间，当作观测时间。
    char strddatetime[21];
    memset(strddatetime,0,sizeof(strddatetime)); //填0初始化
    LocalTime(strddatetime,"yyyymmddhh24miss");
    
    for (int i=0;i<vstcode.size();i++){
        
        // 用随机数填充分钟观测数据的结构体
        struct st_surfdata stsurfdata;
        memset(&stsurfdata,0,sizeof(stsurfdata));
        strncpy(stsurfdata.obtid, vstcode[i].obtId, 10); // 站点代码。
        strncpy(stsurfdata.ddatetime,strddatetime,14); // 当前时间：格式yyyymmddhh24miss
        stsurfdata.t=rand()%351;       // 气温：单位，0.1摄氏度
        stsurfdata.p=rand()%265+10000; // 气压：0.1百帕
        stsurfdata.u=rand()%100+1;     // 相对湿度，0-100之间的值。
        stsurfdata.wd=rand()%360;      // 风向，0-360之间的值。
        stsurfdata.wf=rand()%150;      // 风速：单位0.1m/s
        stsurfdata.r=rand()%16;        // 降雨量：0.1mm
        stsurfdata.vis=rand()%5001+100000;  // 能见度：0.1米

        // 把观测数据的结构体放入vsurfdata容器。
        vsurfdata.push_back(stsurfdata);
    }
}

int main(int argc, char* argv[]){
    if (argc!=4){
        // 如果参数非法，给出帮助文档。

        printf("Using:/crtsurfdata inifile outpath logfile\n");
        printf("Example:/home/yche/cpp-weather/idc/bin/crtsurfdata /home/yche/cpp-weather/idc/ini/stcode.ini /tmp/surfdata /log/idc/crtsurfdata.log\n\n");
        printf("inifile 全国气象站点参数文件名。\n");
        printf("outpath 全国气象站点数据文件存放的目录。\n");
        printf("logfile 本程序运行的日志文件名。\n\n");

        return -1;
    }

    // 打开程序的日志文件。
    if (logfile.Open(argv[3])==false){
        printf("logfile.Open(%s) failed.\n",argv[3]); 
        return -1;
    }
    
    logfile.Write("crtsurfdata2 开始运行。\n");

    if(LoadSTCode(argv[1])==0){
        return -1;
    }

    CrtSurfData(); // 模拟生成全国气象站点分钟观测数据，存放在vsurfdata容器中。


    logfile.WriteEx("crtsurfdata2 运行结束。\n");

    return 0;
}