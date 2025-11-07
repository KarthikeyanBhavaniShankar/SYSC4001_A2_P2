#include <stdio.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/wait.h>

typedef struct{int multiple;int counter;} shared_t;
#define SHM_KEY 0x12345
#define SEM_KEY 0x54321
union semun{int val;struct semid_ds*buf;unsigned short*array;};
static void P(int id){struct sembuf op={0,-1,0};semop(id,&op,1);}
static void V(int id){struct sembuf op={0,+1,0};semop(id,&op,1);}

int main(void){
    int shmid=shmget(SHM_KEY,sizeof(shared_t),IPC_CREAT|0666);
    if(shmid<0){perror("shmget");return 1;}
    shared_t* sh=(shared_t*)shmat(shmid,NULL,0);
    if(sh==(void*)-1){perror("shmat");return 1;}

    int semid=semget(SEM_KEY,1,IPC_CREAT|0666);
    if(semid<0){perror("semget");return 1;}
    union semun u; u.val=1; semctl(semid,0,SETVAL,u);

    P(semid); sh->multiple=3; sh->counter=0; V(semid);

    pid_t pid=fork();
    if(pid<0){perror("fork");return 1;}
    if(pid==0){
        execlp("./q5_p2","./q5_p2",(char*)NULL);
        perror("exec");return 1;
    }

    printf("[P1] pid=%d; P2=%d\n",getpid(),pid);
    while(1){
        int c,m;
        P(semid);
        c=sh->counter; m=sh->multiple;
        if(m && c%m==0) printf("[P1] %d is a multiple of %d\n",c,m);
        sh->counter=c+1; c=sh->counter;
        V(semid);
        if(c>500) break;
        usleep(70000);
    }

    int st=0; waitpid(pid,&st,0);
    shmdt(sh); shmctl(shmid,IPC_RMID,NULL); semctl(semid,0,IPC_RMID);
    printf("[P1] Done. SHM and semaphore removed.\n");
}
