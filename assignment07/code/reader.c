#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>

void sem_wait(int sem_id) {
    struct sembuf sb = {0, -1, 0};
    if (semop(sem_id, &sb, 1) == -1) {
        perror("semop");
        exit(1);
    }
}

void sem_signal(int sem_id) {
    struct sembuf sb = {0, 1, 0};
    if (semop(sem_id, &sb, 1) == -1) {
        perror("semop");
        exit(1);
    }
}

int main() {
    key_t shm_key, sem_key, sem_key2, readcnt_key;
    int shm_id, rw_mutex, mutex, readcnt_id;
    union semun arg;
    char *data;
    int *readcount;

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

    if ((shm_key = ftok("repository.c", 'T')) == -1) {
        perror("ftok");
        exit(1);
    }

    if ((shm_id = shmget(shm_key, 1024, 0644 | IPC_CREAT)) == -1) {
        perror("shmget");
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

    printf("Attempting to acquire the lock...\n");

    sem_wait(mutex);
    (*readcount)++;
    if ((*readcount) == 1) {
        printf("First reader detected. Acquiring read-write lock...\n");
        sem_wait(rw_mutex);
    }
    sem_signal(mutex);

    printf("Lock acquired successfully.\n");
    printf("Reading shared memory data:\n%s\n", data);
    printf("Press Enter to release the lock...\n");
    getchar();

    sem_wait(mutex);
    (*readcount)--;
    if ((*readcount) == 0) {
        printf("Last reader leaving. Releasing read-write lock...\n");
        sem_signal(rw_mutex);
    }
    sem_signal(mutex);

    printf("Lock released successfully.\n");

    if (shmdt(data) == -1) {
        perror("shmdt");
        exit(1);
    }

    if (shmdt(readcount) == -1) {
        perror("shmdt");
        exit(1);
    }

    return 0;
}
