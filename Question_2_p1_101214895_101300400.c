#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

int main(void){
    pid_t pid=fork();
    if(pid<0){perror("fork");return 1;}
    if(pid==0){
        execlp("./q2_p2","./q2_p2",(char*)NULL);
        perror("exec");return 1;
    }
    long counter=0,cycle=0;
    printf("[P1] pid=%d, child=%d\n",getpid(),pid);
    while(1){
        if(counter%3==0) printf("Cycle number: %ld – %ld is a multiple of 3 [P1]\n",cycle,counter);
        else              printf("Cycle number: %ld\n",cycle);
        counter++;cycle++;usleep(120000);
    }
}
