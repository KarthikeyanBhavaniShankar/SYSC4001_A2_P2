#include <stdio.h>
#include <unistd.h>

int main(void){
    long counter=0;
    while(counter>=-500){
        if(counter%3==0) printf("[P2] %ld is a multiple of 3\n",counter);
        counter--;usleep(80000);
    }
    printf("[P2] Reached %ld (< -500). Exiting.\n",counter);
    return 0;
}
