#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <sys/ipc.h> // inter-process communication
#include <sys/shm.h> // shared-memory

struct st_pid{
    int pid; // pid
    char name[51]; //process name
};

int main(int argc, char* argv[]){
    // id of shared memory
    int shmid;
    
    // Read or create shared memory, with keys 0x5005
    // int shmget(key_t key, size_t size, int shmflg);
    if( (shmid = shmget(0x5005,sizeof(struct st_pid), 0640|IPC_CREAT)) == -1){
        printf("shmget(0x5005) failed\n");
        return -1;
    }
    ///////////////////////////////////////////////////////////////////////////
    // $ipcs -m
    // ------ Shared Memory Segments --------
    // key        shmid      owner      perms      bytes      nattch     status      
    // 0x00005005 0          yche       640        416        0  
    // $ipcrm -m 0        
    // $ipcs -m       
    // ------ Shared Memory Segments --------
    // key        shmid      owner      perms      bytes      nattch     status 
    //
    ///////////////////////////////////////////////////////////////////////////
    
    // data structure in the shared memory segment
    struct st_pid *stpid = 0;

    // attach shared memory segment into a pointer in this current process
    // void *shmat(int shmid, const void *shmaddr, int shmflg);
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
    // int shmdt(const void *shmaddr);
    shmdt(stpid);
    

    // free shared memory back to system
    // // int shmctl(int shmid, int cmd, struct shmid_ds *buf);
    // if (shmctl(shmid, IPC_RMID, 0) == -1){
    //     printf("shmctl failed\n"); 
    //     return -1;
    // }
    
    return 0;
}