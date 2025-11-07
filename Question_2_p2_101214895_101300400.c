#include <stdio.h>
#include <unistd.h>

int main(void){
    long counter=0,cycle=0;
    printf("[P2] pid=%d\n",getpid());
    while(1){
        if(counter%3==0) printf("Cycle number: %ld – %ld is a multiple of 3 [P2]\n",cycle,counter);
        else              printf("Cycle number: %ld [P2]\n",cycle);
        counter--;cycle++;usleep(150000);
    }
}
