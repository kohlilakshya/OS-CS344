#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define MATRIX_SIZE 400
#define NUM_THREADS 4

// Global matrices for multiplication
int mat1[MATRIX_SIZE][MATRIX_SIZE];
int mat2[MATRIX_SIZE][MATRIX_SIZE];
int mat3[MATRIX_SIZE][MATRIX_SIZE] = {0};

// Mutex for synchronized access to mat3
pthread_mutex_t mutexsum;

// Structure to hold the range of rows for each thread
typedef struct
{
    int row_start;
    int row_end;
} parameters;

// Function to print a matrix
void print_matrix(int matrix[MATRIX_SIZE][MATRIX_SIZE], const char* matrix_name) 
{
    printf("Matrix %s:\n", matrix_name);
    for (int i = 0; i < MATRIX_SIZE; i++) 
    {
        for (int j = 0; j < MATRIX_SIZE; j++) 
        {
            printf("%d\t", matrix[i][j]);
        }
        printf("\n");
    }
    printf("%s printed successfully.\n", matrix_name);
}

// Thread function to multiply the matrices
void* multiply(void* param) 
{
    parameters p1 = *((parameters*)param);

    // Perform multiplication for the assigned rows
    for (int i = p1.row_start; i <= p1.row_end; i++) 
    {
        for (int j = 0; j < MATRIX_SIZE; j++) 
        {
            int temp_sum = 0;
            for (int k = 0; k < MATRIX_SIZE; k++) 
            {
                temp_sum += mat1[i][k] * mat2[k][j];
            }

            // Use mutex to protect access to shared resource
            pthread_mutex_lock(&mutexsum);
            mat3[i][j] += temp_sum;
            pthread_mutex_unlock(&mutexsum);
        }
    }

    pthread_exit(NULL);
}

int main() 
{
    // Initialize matrices with random values
    for (int i = 0; i < MATRIX_SIZE; i++) 
    {
        for (int j = 0; j < MATRIX_SIZE; j++) 
        {
            mat1[i][j] = rand() % 100;
            mat2[i][j] = rand() % 100;
            mat3[i][j] = 0;
        }
    }

    // Print the initial matrices
    print_matrix(mat1, "mat1");
    print_matrix(mat2, "mat2");

    // Initialize the mutex
    pthread_mutex_init(&mutexsum, NULL);

    // Create thread attributes
    pthread_t threads[NUM_THREADS];
    pthread_attr_t attr;
    pthread_attr_init(&attr);

    int rows_per_thread = MATRIX_SIZE / NUM_THREADS;

    // Create threads for matrix multiplication
    for (int i = 0; i < NUM_THREADS; i++) 
    {
        parameters* p1 = malloc(sizeof(parameters));
        p1->row_start = i * rows_per_thread;
        p1->row_end = (i + 1) * rows_per_thread - 1;

        pthread_create(&threads[i], &attr, multiply, p1);
    }

    // Wait for all threads to complete
    for (int i = 0; i < NUM_THREADS; i++) 
    {
        pthread_join(threads[i], NULL);
    }

    // Destroy the mutex
    pthread_mutex_destroy(&mutexsum);

    // Print the result matrix
    printf("Matrix multiplication completed successfully.\n");
    print_matrix(mat3, "mat3");

    // Save the result matrix to a file
    FILE* output_file = fopen("result_matrix.txt", "w");
    if (output_file == NULL) 
    {
        perror("Error opening file");
        return 1;
    }

    for (int i = 0; i < MATRIX_SIZE; i++) 
    {
        for (int j = 0; j < MATRIX_SIZE; j++) 
        {
            fprintf(output_file, "%d\t", mat3[i][j]);
        }
        fprintf(output_file, "\n");
    }

    fclose(output_file);
    printf("Matrix multiplication completed successfully. Result saved in 'result_matrix.txt'.\n");

    return 0;
}
