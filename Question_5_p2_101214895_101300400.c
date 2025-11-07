#include <stdio.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>

typedef struct{int multiple;int counter;} shared_t;
#define SHM_KEY 0x12345
#define SEM_KEY 0x54321
union semun{int val;struct semid_ds*buf;unsigned short*array;};
static void P(int id){struct sembuf op={0,-1,0};semop(id,&op,1);}
static void V(int id){struct sembuf op={0,+1,0};semop(id,&op,1);}

int main(void){
    int shmid=shmget(SHM_KEY,sizeof(shared_t),0666);
    if(shmid<0){perror("shmget");return 1;}
    shared_t* sh=(shared_t*)shmat(shmid,NULL,0);
    if(sh==(void*)-1){perror("shmat");return 1;}
    int semid=semget(SEM_KEY,1,0666);
    if(semid<0){perror("semget");return 1;}

    while(1){
        int c; P(semid); c=sh->counter; V(semid);
        if(c>100) break; usleep(80000);
    }

    printf("[P2] pid=%d\n",getpid());
    while(1){
        int c,m,done;
        P(semid);
        c=sh->counter; m=sh->multiple;
        if(m && c%m==0) printf("[P2] observed %d (multiple of %d)\n",c,m);
        done=(c>500);
        V(semid);
        if(done) break;
        usleep(80000);
    }
    shmdt(sh);
}
