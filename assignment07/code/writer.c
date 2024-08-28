#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>

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
    int shm_id, rw_mutex;
    union semun arg;
    char *data;

    if ((sem_key = ftok("repository.c", 'J')) == -1) {
        perror("ftok");
        exit(1);
    }
    if ((rw_mutex = semget(sem_key, 1, 0666 | IPC_CREAT)) == -1) {
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
    if ((data = shmat(shm_id, (void *)0, 0)) == (char *)(-1)) {
        perror("shmat");
        exit(1);
    }
    
    printf("Trying to acquire lock...\n");
    sem_wait(rw_mutex);
    int ind = 0;
    while (data[ind] != '}') ind++;
    sprintf(data + ind, "printf(\"Hello written by PID: %d\\n\");\n}", getpid());
    printf("Writing done. Press Enter to release the lock...\n");
    getchar();
    sem_signal(rw_mutex);
    printf("Lock released.\n");
    
    if (shmdt(data) == -1) {
        perror("shmdt");
        exit(1);
    }
    return 0;
}
