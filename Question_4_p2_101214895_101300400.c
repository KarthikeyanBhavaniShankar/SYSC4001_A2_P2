#include <stdio.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>

typedef struct{int multiple;int counter;} shared_t;
#define SHM_KEY 0x12345

int main(void){
    int shmid=shmget(SHM_KEY,sizeof(shared_t),0666);
    if(shmid<0){perror("shmget");return 1;}
    shared_t* sh=(shared_t*)shmat(shmid,NULL,0);
    if(sh==(void*)-1){perror("shmat");return 1;}

    while(sh->counter<=100) usleep(80000);
    printf("[P2] pid=%d; starting. multiple=%d\n",getpid(),sh->multiple);

    while(1){
        int c=sh->counter,m=sh->multiple;
        if(m && c%m==0) printf("[P2] observed %d (multiple of %d)\n",c,m);
        if(c>500) break;
        usleep(80000);
    }
    shmdt(sh);
}
