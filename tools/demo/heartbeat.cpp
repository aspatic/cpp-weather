#include "_public.h"

#define MAXNUMP_ 1000   // 假定整个解决方案活动的进程不超过一千个
#define SHMKEYP_ 0x5095 // 创建共享内存时的key。

// 进程心跳信息的结构
struct st_pinfo
{
    int pid; // 进程ID
    char pname[51]; // 进程名称，可以为空
    int timeout; // 预设的超时时间
    time_t atime; // 最近一次的心跳记录，是一个长整数
};

int main (int argc, char* agrv[]){
    if (argc < 2){
        printf("using: ./book procname\n");
        return 0;
    }

    // 创建（或获取）共享内存，大小为n * sizeof(struct st_pinfo)
    
    int m_shmid = 0;
    if ((m_shmid = shmget(MAXNUMP_, SHMKEYP_ * sizeof(struct st_pinfo), 0640|IPC_CREAT)) == -1){
        printf("shmget(%x) failed\n", MAXNUMP_);
        return -1;
    }

    // 连接当前进程到共享内存
    struct st_pinfo *m_shm;
    m_shm = (struct st_pinfo*) shmat(m_shmid, 0, 0);

    // 创建，填写当前进程心跳信息数据结构
    struct st_pinfo stpinfo;
    memset(&stpinfo, 0, sizeof(struct st_pinfo) );

    stpinfo.pid = getpid();
    STRNCPY(stpinfo.pname, sizeof(stpinfo.pname), agrv[1], 50);
    stpinfo.timeout = 30;
    stpinfo.atime = time(0);

    // 在共享内存查找一个空位置，把当前进程心跳信息存入
    int m_pos_avail = -1;
    for (int i = 0; i < SHMKEYP_; i++ ){
        // if m_shm[i].pid == 0 
        if ( (m_shm+i)->pid == 0 ){
            // 找到了一个空位置
            m_pos_avail = i;
            break;
        }
    }
    if(m_pos_avail == -1){
        // 没找到空位置
        printf("共享内存空间已用完。\n");
        return -1;
    }
    memcpy(m_shm + m_pos_avail, &stpinfo, sizeof(struct st_pinfo));

    while(1){
        // “报平安” 更新共享内存中本进程的心跳


        sleep(10);
    }
    // 释放 当前进程占用的共享内存

    // 把共享内存从当前进程断开
    return 0;
}