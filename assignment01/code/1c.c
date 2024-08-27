#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

/* execl for no arguments */
/* execle for 1 arguments */
/* execve for 10 arguments */

int main(int argc, char *argv[]) {
    pid_t pid, pid1;

    for (int i = 1; i <= 3; i++) {
        pid = fork();

        if (pid < 0) {
            fprintf(stderr, "Fork Failed");
            return 1;
        } else if (pid == 0) {
            pid1 = getpid();
            if (i == 1)
                execl("210123077-1a", "0", NULL, NULL);
            else if (i == 2)
                execle("210123077-1a", "0", argv[1], NULL, NULL);
            else
                execve("210123077-1a", argv, NULL);

            break;
        } else {
            int status;
            while (wait(&status) != -1);
        }
    }

    return 0;
}
