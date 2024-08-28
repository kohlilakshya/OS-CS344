#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/wait.h>

#define NUM_TABLES 5
#define NUM_SEMA 5

// Function to perform semaphore wait
void sem_wait(int sem_id, int sem_num) {
    struct sembuf sb = {sem_num, -1, 0};
    if (semop(sem_id, &sb, 1) == -1) {
        perror("semop wait");
        exit(EXIT_FAILURE);
    }
}

// Function to perform semaphore signal
void sem_signal(int sem_id, int sem_num) {
    struct sembuf sb = {sem_num, 1, 0};
    if (semop(sem_id, &sb, 1) == -1) {
        perror("semop signal");
        exit(EXIT_FAILURE);
    }
}

// Structure to represent a database table
struct table {
    char name[50];
    int pid[10];
};

int main() {
    key_t sem_key, shm_key;
    int shm_id, sem_id;
    union semun arg[NUM_SEMA];
    struct table *tables;

    // Obtain a key for semaphore
    if ((sem_key = ftok("1.c", 'J')) == -1) {
        perror("ftok (semaphore)");
        exit(EXIT_FAILURE);
    }

    // Create a semaphore set with NUM_SEMA semaphores
    if ((sem_id = semget(sem_key, NUM_SEMA, 0666 | IPC_CREAT)) == -1) {
        perror("semget");
        exit(EXIT_FAILURE);
    }

    // Initialize semaphores
    for (int i = 0; i < NUM_SEMA; i++) {
        arg[i].val = 1;
        if (semctl(sem_id, i, SETVAL, arg[i]) == -1) {
            perror("semctl (init)");
            exit(EXIT_FAILURE);
        }
    }

    // Obtain a key for shared memory
    if ((shm_key = ftok("1.c", 'R')) == -1) {
        perror("ftok (shared memory)");
        exit(EXIT_FAILURE);
    }

    // Create a shared memory segment
    if ((shm_id = shmget(shm_key, sizeof(struct table) * NUM_TABLES, 0644 | IPC_CREAT)) == -1) {
        perror("shmget");
        exit(EXIT_FAILURE);
    }

    // Attach the shared memory segment
    tables = (struct table *)shmat(shm_id, NULL, 0);
    if (tables == (struct table *)(-1)) {
        perror("shmat");
        exit(EXIT_FAILURE);
    }

    // Initialize each table with a table number
    for (int i = 0; i < NUM_TABLES; i++) {
        snprintf(tables[i].name, sizeof(tables[i].name), "table%d", i);
    }

    for (int i = 0; i < NUM_TABLES; i++) {
        printf("Round Number %d\n", i + 1);
        // Create five child processes (philosophers)
        for (int j = 0; j < NUM_TABLES; j++) {
            pid_t pid = fork();
            if (pid < 0) {
                perror("fork");
                exit(EXIT_FAILURE);
            } else if (pid == 0) {
                // Child process (philosopher) picks two adjacent database tables
                int left = (j + i) % NUM_TABLES;
                int right = (j + i + 1) % NUM_TABLES;

                // Wait on both semaphores
                sem_wait(sem_id, left);
                sem_wait(sem_id, right);

                // Perform operations on the tables
                printf("Transaction %d holding the semaphore\n", getpid());
                printf("Transaction %d: operating on Table %d - Table %d\n\n", getpid(), left, right);

                tables[left].pid[2 * i] = getpid();
                tables[right].pid[2 * i + 1] = getpid();

                // Release both semaphores
                sleep(2);
                sem_signal(sem_id, left);
                sem_signal(sem_id, right);
                exit(EXIT_SUCCESS);
            } else {
                wait(NULL); // Parent waits for the child to finish
            }
        }
    }

    // Print the content of each table
    for (int i = 0; i < NUM_TABLES; i++) {
        printf("Content of table %d:\n", i + 1);
        for (int j = 0; j < 10; j++) {
            printf("%d ", tables[i].pid[j]);
        }
        printf("\n\n");
    }

    // Remove the semaphore set
    if (semctl(sem_id, NUM_SEMA, IPC_RMID, arg) == -1) {
        perror("semctl (remove)");
        exit(EXIT_FAILURE);
    }

    // Detach the shared memory segment
    if (shmdt(tables) == -1) {
        perror("shmdt");
        exit(EXIT_FAILURE);
    }

    // Remove the shared memory segment
    if (shmctl(shm_id, IPC_RMID, NULL) == -1) {
        perror("shmctl");
        exit(EXIT_FAILURE);
    }

    return EXIT_SUCCESS;
}
