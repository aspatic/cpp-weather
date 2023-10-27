#include "_public.h"

#define MAXNUMP_ 1000   // 假定整个解决方案活动的进程不超过一千个
#define SHMKEYP_ 0x5095 // 创建共享内存时的key。
#define SEMKEYP_ 0x5095 // 创建01信号量时的key。

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
    if ((m_shmid = shmget(SHMKEYP_, MAXNUMP_ * sizeof(struct st_pinfo), 0640|IPC_CREAT)) == -1){
        printf("shmget(%x) failed\n", SHMKEYP_);
        return -1;
    }
    CSEM m_sem;
    if ( (m_sem.init(SEMKEYP_)) == false){
        printf("m_sem.init(%x) failed\n", SEMKEYP_);
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

    int m_pos_avail = -1;
    // 进程ID是循环分配的 如果之前有一个登记过心跳信息的进程异常退出，没有清理掉
    // 自己留在共享内存的心跳信息， 那么它将在共享内存残留一个过时的心跳信息
    // 如果在不巧的情况下我们这个新进程使用和之前异常退出的进程相同的ID，那就会匹配到共享内存的这条残留记录，导致被新进程守护进程误杀
    
    // 所以当务之急是扫描共享内存中是否有同名ID的残留记录，并尽快覆盖掉它。
    for (int i = 0; i < MAXNUMP_; i++ ){
        if (m_shm[i].pid = stpinfo.pid){
            m_pos_avail = i;
            break;
        }
    }
 
    m_sem.P(); // 加锁 获得共享内存操作权

    // 在共享内存查找一个空位置，把当前进程心跳信息存入
    if(m_pos_avail == -1){
        for (int i = 0; i < MAXNUMP_; i++ ){
            // if m_shm[i].pid == 0 
            if ( (m_shm+i)->pid == 0 ){
                // 找到了一个空位置
                m_pos_avail = i;
                break;
            }
        }
    }
    if(m_pos_avail == -1){
        // 没找到空位置
        m_sem.V(); // 解锁
        printf("共享内存空间已用完。\n");
        return -1;
    }
    memcpy(m_shm + m_pos_avail, &stpinfo, sizeof(struct st_pinfo));

    m_sem.V(); // 解锁

    while(1){
        // “报平安” 更新共享内存中本进程的心跳
        // 理论上讲这是独享的内存段，不会有竞争问题，应该也就不需要加锁
        if (m_pos == -1) return -1;
        m_shm[m_pos_avail].atime = time(0);

        sleep(10);
    }

    // 释放 当前进程占用的共享内存
    // 敷衍的写法是 m_shm[m_pos_avail].pid = 0;
    if(m_pos != -1) memset(m_shm + m_pos_avail, 0, sizeof(struct st_pinfo));

    // 把共享内存从当前进程断开
    if (m_shm != 0) shmdt(m_shm);

    return 0;
}