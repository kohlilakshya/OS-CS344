#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include <sys/wait.h>

/* execl for no arguments */
/* execle for 1 arguments */
/* execve for 10 arguments */

int main(int argc, char* argv[]){
    pid_t pid,pid1;

    for(int i=1;i<=3;i++){
        pid=fork();

        if(pid<0){
            fprintf(stderr,"Fork Failed");
            return 1;
        } else if(pid==0){
            pid1=getpid();
            char *value = getenv("n");
            if(i==1) execl("210123077-1b", "0", NULL, "n");
            if(i==2) execle("210123077-1b", "0", argv[1], NULL, "n");
            if(i==3) execve("210123077-1b", argv,"n");
            break;
        } else if (i==3) {
            int status;
            while(wait(&status)!=-1);            
        }
    }

    return 0;
}
