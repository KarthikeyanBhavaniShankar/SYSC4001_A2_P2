#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

int main(void){
    pid_t pid=fork();
    if(pid<0){perror("fork");return 1;}
    if(pid==0){
        execlp("./q3_p2","./q3_p2",(char*)NULL);
        perror("exec");return 1;
    }
    printf("[P1] waiting for P2 %d...\n",pid);
    int st=0; waitpid(pid,&st,0);
    printf("[P1] done.\n");
}
