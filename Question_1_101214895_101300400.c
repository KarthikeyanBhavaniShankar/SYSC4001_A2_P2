#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

int main(void){
    pid_t pid=fork();
    if(pid<0){perror("fork");return 1;}
    if(pid==0){
        long c=0;
        printf("[P2] pid=%d, ppid=%d\n",getpid(),getppid());
        while(1){printf("[P2 %d] %ld\n",getpid(),c++);sleep(1);}
    }else{
        long c=0;
        printf("[P1] pid=%d, child(P2)=%d\n",getpid(),pid);
        while(1){printf("[P1 %d] %ld\n",getpid(),c++);sleep(1);}
    }
}
