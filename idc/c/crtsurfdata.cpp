#include "_public.h"

CLogFile logfile;
// 获取当前时间，当作观测时间。
char strddatetime[21];

struct st_stcode{// 全国气象站点参数结构体。
    char provName[31]; // 省 
    char obtId[11]; // 站号
    char obtName[31]; // 站名
    double lat; // 纬度
    double lon; // 经度
    double height; // 高度
};
std::vector<struct st_stcode> vstcode;
bool LoadSTCode(const char* inifile);

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
void CrtSurfData();

bool CrtSurfFile(const char* outpath, const char* datafmt);

int main(int argc, char* argv[]){
    if (argc!=5){
        // 如果参数非法，给出帮助文档。

        printf("Using:/crtsurfdata inifile outpath logfile datafmt\n");
        printf("Example:/home/yche/cpp-weather/idc/bin/crtsurfdata /home/yche/cpp-weather/idc/ini/stcode.ini /tmp/surfdata /log/idc/crtsurfdata.log xml,json,csv\n\n");
        printf("inifile 全国气象站点参数文件名。\n");
        printf("outpath 全国气象站点数据文件存放的目录。\n");
        printf("logfile 本程序运行的日志文件名。\n\n");
        printf("datafmt 生成数据文件的保存格式,支持xml,json,csv三种格式,中间用逗号分隔。\n\n");
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

    // 如果给出文件格式就生成这种文件，如果三种同时给出，则三份不同格式文件都生成
    if (strstr(argv[4], "xml")){ CrtSurfFile(argv[2],"xml");}
    if (strstr(argv[4], "json")){ CrtSurfFile(argv[2],"json");}
    if (strstr(argv[4], "csv")){ CrtSurfFile(argv[2],"csv");}



    logfile.WriteEx("crtsurfdata 运行结束。\n");

    return 0;
}

bool CrtSurfFile(const char* outpath, const char* datafmt){
    // 拼接输出文件名，如 SURF_ZH_20210718093354_2222.csv
    char strFileName[301];
    sprintf(strFileName, 
            "%s/SURF_ZH_%s_%d.%s", outpath, strddatetime, getpid(),datafmt); //加进程id保证无重复

    CFile File;
    // 打开写入目标文件 (创建临时文件)
    if (File.OpenForRename(strFileName,"w") == 0){
        logfile.Write("File.OpenForRename(%s) failed\n", strFileName);
        return false;
    }
    // 写入标题
    if (strcmp(datafmt, "csv")==0){
        File.Fprintf("站点代码,数据时间,气温,气压,相对湿度,风向,风速,降雨量,能见度\n");
    }
    if (strcmp(datafmt, "csv")==0){
        File.Fprintf("<data>\n");
    }

    // 写入每条数据
    for (int i = 0; i < vsurfdata.size(); i++){
        if (strcmp(datafmt, "csv")==0){
            // csv format
            File.Fprintf("%s,%s,%.1f,%.1f,%d,%d,%.1f,%.1f,%.1f\n",\
            vsurfdata[i].obtid,vsurfdata[i].ddatetime,vsurfdata[i].t/10.0,vsurfdata[i].p/10.0,\
            vsurfdata[i].u,vsurfdata[i].wd,vsurfdata[i].wf/10.0,vsurfdata[i].r/10.0,vsurfdata[i].vis/10.0);
        }
        if (strcmp(datafmt, "xml")==0){
            // cxml format
            File.Fprintf("<obtid>%s</obtid>"\
                        "<ddatetime>%s</ddatetime>"\
                        "<t>%.1f</t>"\
                        "<p>%.1f</p>"\
                        "<u>%d</u>"\
                        "<wd>%d</wd>"\
                        "<wf>%.1f</wf>"\
                        "<r>%.1f</r>"\
                        "<vis>%.1f</vis>"\
                        "<endl/>\n",
            vsurfdata[i].obtid,vsurfdata[i].ddatetime,vsurfdata[i].t/10.0,vsurfdata[i].p/10.0,\
            vsurfdata[i].u,vsurfdata[i].wd,vsurfdata[i].wf/10.0,vsurfdata[i].r/10.0,vsurfdata[i].vis/10.0);
        }
    }
    // 文件的结束标签
    if (strcmp(datafmt,"xml")==0) File.Fprintf("</data>\n");

    //sleep(50);
    // 关闭文件。(结束写入，并将临时文件复制到目标文件)
    File.CloseAndRename();
    
    logfile.Write("生成数据文件%s成功，数据时间%s，记录数%d。\n",strFileName,strddatetime,vsurfdata.size());
    return true;
}

void CrtSurfData(){
    // 模拟生成全国气象站点分钟观测数据，存放在vsurfdata容器中。
    srand(time(0));


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
