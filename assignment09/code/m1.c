#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

#define SIZE 9

// Global variables to store the Sudoku puzzle and validation results
int sudoku[SIZE][SIZE];
int rowValidity = 0, colValidity = 0, subgridValidity[SIZE] = {0};

// Structure for passing data to threads
typedef struct {
    int row;
    int column;
} parameters;

/**
 * Function to check the validity of each column in the Sudoku grid.
 * Sets colValidity to 1 if all columns are valid, otherwise sets it to 0.
 */
void *checkColumn(void *param) {
    for (int col = 0; col < SIZE; col++) {
        int vis[SIZE] = {0};
        for (int i = 0; i < SIZE; i++) {
            int num = sudoku[i][col];
            if (vis[num - 1] || num < 1 || num > SIZE) {
                colValidity = 0;
                pthread_exit(0);
            }
            vis[num - 1] = 1;
        }
    }
    colValidity = 1;
    pthread_exit(0);
}

/**
 * Function to check the validity of each row in the Sudoku grid.
 * Sets rowValidity to 1 if all rows are valid, otherwise sets it to 0.
 */
void *checkRow(void *param) {
    for (int row = 0; row < SIZE; row++) {
        int vis[SIZE] = {0};
        for (int i = 0; i < SIZE; i++) {
            int num = sudoku[row][i];
            if (num < 1 || num > SIZE || vis[num - 1]) {
                rowValidity = 0;
                pthread_exit(0);
            }
            vis[num - 1] = 1;
        }
    }
    rowValidity = 1;
    pthread_exit(0);
}

/**
 * Function to check the validity of a 3x3 subgrid in the Sudoku grid.
 * Sets the corresponding entry in subgridValidity to 1 if the subgrid is valid.
 */
void *checkSubgrid(void *param) {
    parameters p1 = *((parameters *)param);
    int row = p1.row;
    int col = p1.column;
    int vis[SIZE] = {0};

    for (int i = row; i < row + 3; i++) {
        for (int j = col; j < col + 3; j++) {
            int num = sudoku[i][j];
            if (num < 1 || num > SIZE || vis[num - 1]) {
                pthread_exit(0);
            }
            vis[num - 1] = 1;
        }
    }

    subgridValidity[3 * (row / 3) + (col / 3)] = 1;
    pthread_exit(0);
}

int main() {
    // Open the input file for reading the Sudoku puzzle
    FILE *ptr = fopen("week10-ML2-input1.txt", "r");
    if (ptr == NULL) {
        printf("Failed to open the input file.\n");
        return 1;
    }

    // Read the Sudoku grid from the input file
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            fscanf(ptr, "%d", &sudoku[i][j]);
        }
    }
    fclose(ptr);

    // a. Create a thread to check all columns
    pthread_t thread_col;
    pthread_create(&thread_col, NULL, checkColumn, NULL);
    pthread_join(thread_col, NULL);

    // b. Create a thread to check all rows
    pthread_t thread_row;
    pthread_create(&thread_row, NULL, checkRow, NULL);
    pthread_join(thread_row, NULL);

    // c. Create threads to check each 3x3 subgrid
    pthread_t threads[SIZE];
    for (int i = 0; i < SIZE; i++) {
        parameters *p1 = malloc(sizeof(parameters));
        p1->row = (i / 3) * 3;
        p1->column = (i % 3) * 3;
        pthread_create(&threads[i], NULL, checkSubgrid, p1);
    }

    // Wait for all subgrid threads to finish
    for (int i = 0; i < SIZE; i++) {
        pthread_join(threads[i], NULL);
    }

    // Return results to the parent thread
    if (rowValidity != 1) {
        printf("Sudoku Puzzle is invalid: Row check failed.\n");
        return 0;
    }

    if (colValidity != 1) {
        printf("Sudoku Puzzle is invalid: Column check failed.\n");
        return 0;
    }

    for (int i = 0; i < SIZE; i++) {
        if (subgridValidity[i] != 1) {
            printf("Sudoku Puzzle is invalid: Subgrid %d check failed.\n", i + 1);
            return 0;
        }
    }

    printf("Sudoku Puzzle is valid!\n");
    return 0;
}
