#include "_public.h"

CSEM sem; // semaphore obj for shared memory

struct st_pid{
    int pid; // pid
    char name[51]; //process name
};

int main(int argc, char* argv[]){
    // id of shared memory
    int shmid;
    
    // Read or create shared memory, with keys 0x5005
    if( (shmid = shmget(0x5005,sizeof(struct st_pid), 0640|IPC_CREAT)) == -1){
        printf("shmget(0x5005) failed\n");
        return -1;
    }

    // If the semaphore has exist, get access to it, else create and init it to "value"
    if(sem.init(0x5005, 10)==false){ //semaphore key 0x5005 is different than share memory key 0x5005
        printf("sem.init(0x5005) failed\n");
        return -1;
    }

    // data structure in the shared memory segment
    struct st_pid *stpid = 0;

    // attach shared memory segment into a pointer in this current process
    if ((stpid = (struct st_pid*)shmat(shmid,0,0)) == (void*)-1){
        printf("shmat failed\n"); 
        return -1;
    }

    printf("before acquiring lock, time=%d, val=%d\n", time(0), sem.value());
    sem.P(-5); // acquire lock 
    printf("after acquiring lock, time=%d, val=%d\n", time(0), sem.value());

    printf("pid=%d, name=%s\n", stpid->pid, stpid->name);
    //write into shared memory
    stpid->pid = getpid();
    strcpy(stpid->name, argv[1]);
    printf("pid=%d, name=%s\n", stpid->pid, stpid->name);
    sleep(3);

    printf("before releasing lock, time=%d, val=%d\n", time(0), sem.value());
    sem.V(2);
    printf("after releasing lock, time=%d, val=%d\n", time(0), sem.value());

    //printf("before releasing lock 2, time=%d, val=%d\n", time(0), sem.value());
    sem.V(2);
    printf("after releasing lock 2, time=%d, val=%d\n", time(0), sem.value());

    //printf("before releasing lock 3, time=%d, val=%d\n", time(0), sem.value());
    sem.V(20);
    printf("after releasing lock 3, time=%d, val=%d\n", time(0), sem.value());

    // This process must not terminated, or our CSEM implementation will reset semaphore.
    sleep(100); // So keep this process alive and see will its message be shared to others. 
    // The choice of CSEM 's m_sem_flg SEM_UNDO is a thoughtful one
    // 如果信号量用于表示可用资源的数量（不变的），设置为SEM_UNDO更合适。
    // 如果信号量用于生产消费者模型，设置为0更合适。
    // 注意，网上查到的关于sem_flg的用法基本上是错的，一定要自己动手多测试。

    // detach shared memory from current process
    shmdt(stpid);
    
    
    
    return 0;
}