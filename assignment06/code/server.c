#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <unistd.h>
#include <sys/wait.h>

#define SHM_SIZE 25
#define NUM_REQUESTS 100

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

    if ((shm_key = ftok("server.c", 'R')) == -1) {
        perror("ftok");
        exit(1);
    }
    if ((shm_id = shmget(shm_key, SHM_SIZE, 0644 | IPC_CREAT)) == -1) {
        perror("shmget");
        exit(1);
    }

    if ((sem_key = ftok("server.c", 'J')) == -1) {
        perror("ftok");
        exit(1);
    }

    if ((sem_id = semget(sem_key, 1, 0666 | IPC_CREAT)) == -1) {
        perror("semget");
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

    for (int i = 0; i < 25; i++) {
        *(data + i) = 0;
    }

    for (int i = 0; i < 2; i++) {
        pid_t child_pid = fork();
        if (child_pid == 0) {
            char *queue_ptr = shmat(shm_id, (void *)0, 0);
            if (queue_ptr == (char *)(-1)) {
                perror("shmat");
                exit(1);
            }

            char *count = queue_ptr + 23;
            char *front = queue_ptr + 24;

            while ((*count) < 100) {
                sem_wait(sem_id);

                if (*(queue_ptr + *front) != 0) {
                    printf("Child %d :%d\n", i + 1, *(queue_ptr + *front));
                    *(queue_ptr + *front) = 0;
                    (*front)++;
                    (*front) %= 23;
                    (*count)++;
                }

                sem_signal(sem_id);
                usleep(100000); // Sleep for 100,000 microseconds (0.1 seconds)
            }

            if (shmdt(queue_ptr) == -1) {
                perror("shmdt");
                exit(1);
            }

            exit(0);
        }
    }

    wait(NULL);
    wait(NULL);

    if (semctl(sem_id, 0, IPC_RMID, arg) == -1) {
        perror("semctl");
        exit(1);
    }

    if (shmdt(data) == -1) {
        perror("shmdt");
        exit(1);
    }

    shmctl(shm_id, IPC_RMID, NULL);

    return 0;
}
