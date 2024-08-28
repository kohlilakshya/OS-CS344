#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>

#define SHM_SIZE 25

// Function to perform semaphore wait
void sem_wait(int sem_id) {
    struct sembuf sb = {0, -1, 0};
    if (semop(sem_id, &sb, 1) == -1) {
        perror("semop");
        exit(1);
    }
}

// Function to perform semaphore signal
void sem_signal(int sem_id) {
    struct sembuf sb = {0, 1, 0};
    if (semop(sem_id, &sb, 1) == -1) {
        perror("semop");
        exit(1);
    }
}

int main() {
    key_t shm_key, sem_key;
    int shm_id, sem_id;
    union semun arg;
    char *data;
    int semid = -1;

    if ((sem_key = ftok("server.c", 'J')) == -1) {
        perror("ftok");
        exit(1);
    }

    if ((sem_id = semget(sem_key, 1, 0666 | IPC_CREAT)) == -1) {
        perror("semget");
        exit(1);
    }

    if ((shm_key = ftok("server.c", 'R')) == -1) {
        perror("ftok");
        exit(1);
    }

    if ((shm_id = shmget(shm_key, SHM_SIZE, 0644 | IPC_CREAT)) == -1) {
        perror("shmget");
        exit(1);
    }

    arg.val = 1;
    if (semctl(sem_id, 0, SETVAL, arg) == -1) {
        perror("semctl");
        exit(1);
    }

    data = shmat(shm_id, (void *)0, 0);
    if (data == (char *)(-1)) {
        perror("shmat");
        exit(1);
    }

    int rear = 0;
    int count = 0;

    while (count < 100) {
        sem_wait(sem_id);
        if (*(data + rear) == 0) {
            count++;
            *(data + rear) = count;
            printf("Queued %d\n", count);
            rear++;
            rear %= 23;
        }
        sem_signal(sem_id);
    }

    if (semctl(sem_id, 0, IPC_RMID, arg) == -1) {
        perror("semctl");
    }

    if (shmdt(data) == -1) {
        perror("shmdt");
        exit(1);
    }

    return 0;
}
