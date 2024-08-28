#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>

int main() {
    key_t shm_key, sem_key, sem_key2, readcnt_key;
    int shm_id, rw_mutex, mutex, readcnt_id;
    char *data;
    int *readcount;
    union semun arg;

    if ((shm_key = ftok("repository.c", 'T')) == -1) {
        perror("ftok");
        exit(1);
    }

    if ((shm_id = shmget(shm_key, 1024, 0644 | IPC_CREAT)) == -1) {
        perror("shmget1");
        exit(1);
    }

    data = shmat(shm_id, (void *)0, 0);
    if (data == (char *)(-1)) {
        perror("shmat");
        exit(1);
    }

    if ((readcnt_key = ftok("repository.c", 'S')) == -1) {
        perror("ftok");
        exit(1);
    }

    if ((readcnt_id = shmget(readcnt_key, 128, 0644 | IPC_CREAT)) == -1) {
        perror("shmget");
        exit(1);
    }

    readcount = shmat(readcnt_id, (void *)0, 0);
    if (readcount == (int *)(-1)) {
        perror("shmat");
        exit(1);
    }

    *readcount = 0;

    if ((sem_key = ftok("repository.c", 'J')) == -1) {
        perror("ftok");
        exit(1);
    }

    if ((rw_mutex = semget(sem_key, 1, 0666 | IPC_CREAT)) == -1) {
        perror("semget");
        exit(1);
    }

    if ((sem_key2 = ftok("repository.c", 'L')) == -1) {
        perror("ftok");
        exit(1);
    }

    if ((mutex = semget(sem_key2, 1, 0666 | IPC_CREAT)) == -1) {
        perror("semget");
        exit(1);
    }

    arg.val = 1;
    if (semctl(rw_mutex, 0, SETVAL, arg) == -1) {
        perror("semctl");
        exit(1);
    }

    arg.val = 1;
    if (semctl(mutex, 0, SETVAL, arg) == -1) {
        perror("semctl");
        exit(1);
    }

    char code[] = "#include<stdio.h>\nint main()\n{\nprintf(\"Hello World!\\n\");\n}";
    strcpy(data, code);

    printf("Press enter to exit repository");
    getchar();

    if (shmdt(data) == -1) {
        perror("shmdt");
        exit(1);
    }

    if (shmdt(readcount) == -1) {
        perror("shmdt");
        exit(1);
    }

    if (semctl(mutex, 0, IPC_RMID, arg) == -1) {
        perror("semctl");
        exit(1);
    }

    if (semctl(rw_mutex, 0, IPC_RMID, arg) == -1) {
        perror("semctl");
        exit(1);
    }

    if (shmctl(shm_id, IPC_RMID, NULL) == -1) {
        perror("shmctl");
        exit(1);
    }

    if (shmctl(readcnt_id, IPC_RMID, NULL) == -1) {
        perror("shmctl");
        exit(1);
    }

    return 0;
}
