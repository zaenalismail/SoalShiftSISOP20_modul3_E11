#include <stdio.h> 
#include <stdlib.h>  
#include <unistd.h> 

void cmd1(){
    execlp("ls","ls",NULL); 
}

void cmd2(){
    execlp("wc","wc", "-l", NULL); 
} 


int main(){ 
    int pipes[2]; 

    pipe(pipes); 
  
    if(!fork()){ 
        close(1); 
        dup(pipes[1]); 
        close(pipes[0]);
        cmd1();   
    }else{ 
        close(0);
        dup(pipes[0]);
        close(pipes[1]);
        printf("Jumlah file dan folder dalam direktori : \n");
        cmd2();
    }
}