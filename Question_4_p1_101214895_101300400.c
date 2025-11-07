#include <stdio.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>

typedef struct{int multiple;int counter;} shared_t;
#define SHM_KEY 0x12345

int main(void){
    int shmid=shmget(SHM_KEY,sizeof(shared_t),IPC_CREAT|0666);
    if(shmid<0){perror("shmget");return 1;}
    shared_t* sh=(shared_t*)shmat(shmid,NULL,0);
    if(sh==(void*)-1){perror("shmat");return 1;}
    sh->multiple=3; sh->counter=0;

    pid_t pid=fork();
    if(pid<0){perror("fork");return 1;}
    if(pid==0){
        execlp("./q4_p2","./q4_p2",(char*)NULL);
        perror("exec");return 1;
    }

    printf("[P1] pid=%d; P2=%d; multiple=%d\n",getpid(),pid,sh->multiple);
    while(1){
        int c=sh->counter,m=sh->multiple;
        if(m && c%m==0) printf("[P1] %d is a multiple of %d\n",c,m);
        sh->counter=c+1;
        if(sh->counter>500) break;
        usleep(70000);
    }
    int st=0; waitpid(pid,&st,0);
    shmdt(sh); shmctl(shmid,IPC_RMID,NULL);
    printf("[P1] Done. SHM removed.\n");
}
