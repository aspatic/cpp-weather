#include "_public.h"

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

    // data structure in the shared memory segment
    struct st_pid *stpid = 0;

    // attach shared memory segment into a pointer in this current process
    if ((stpid = (struct st_pid*)shmat(shmid,0,0)) == (void*)-1){
        printf("shmat failed\n"); 
        return -1;
    }

    printf("pid=%d, name=%s\n", stpid->pid, stpid->name);
    //write into shared memory
    stpid->pid = getpid();
    strcpy(stpid->name, argv[1]);
    printf("pid=%d, name=%s\n", stpid->pid, stpid->name);

    // detach shared memory from current process
    shmdt(stpid);
    


    
    return 0;
}