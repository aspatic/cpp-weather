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
    if(sem.init(0x5005)==false){ // semaphore key 0x5005 is different than share memory key 0x5005
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
    sem.P(); // acquire lock 
    printf("after acquiring lock, time=%d, val=%d\n", time(0), sem.value());

    printf("pid=%d, name=%s\n", stpid->pid, stpid->name);
    //write into shared memory
    stpid->pid = getpid();
    strcpy(stpid->name, argv[1]);
    printf("pid=%d, name=%s\n", stpid->pid, stpid->name);
    sleep(5);

    printf("before releasing lock, time=%d, val=%d\n", time(0), sem.value());
    sem.V();
    printf("after releasing lock, time=%d, val=%d\n", time(0), sem.value());


    // detach shared memory from current process
    shmdt(stpid);
    
    /////////////////////////////////////////////////////////////////
    // yche@DESKTOP-INS7591:~/cpp-weather/tools/demo$ g++ -g -o book share_mem_demo.cpp -I/home/yche/cpp-weather/public /home/yche/cpp-weather/public/_public.cpp
    // yche@DESKTOP-INS7591:~/cpp-weather/tools/demo$ ./book aaa
    // before acquiring lock, time=1697810413, val=1
    // after acquiring lock, time=1697810413, val=0
    // pid=4662, name=
    // pid=4713, name=aaa
    // before releasing lock, time=1697810418, val=0
    // after releasing lock, time=1697810418, val=1
    // yche@DESKTOP-INS7591:~/cpp-weather/tools/demo$ ipcs -s
    // 
    // ------ Semaphore Arrays --------
    // key        semid      owner      perms      nsems     
    // 0x00005005 0          yche       666        1         
    // 
    // yche@DESKTOP-INS7591:~/cpp-weather/tools/demo$ ipcrm sem 0
    // resource(s) deleted
    // yche@DESKTOP-INS7591:~/cpp-weather/tools/demo$ ipcs -s
    // 
    // ------ Semaphore Arrays --------
    // key        semid      owner      perms      nsems     
    // 
    /////////////////////////////////////////////////////////////////


    
    return 0;
}