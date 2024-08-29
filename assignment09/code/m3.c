#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

#define SIZE 9

// Global arrays to track the validity of rows, columns, and subgrids
int sudoku[SIZE][SIZE];
int rowValidity[SIZE] = {0}, columnValidity[SIZE] = {0}, subgridValidity[SIZE] = {0};

/* Structure for passing data to threads */
typedef struct {
    int row;
    int column;
} parameters;

/**
 * Function to check the validity of a specific column in the Sudoku grid.
 * Marks columnValidity[col] as 1 if the column is valid.
 */
void *checkColumn(void *param) {
    parameters p1 = *((parameters *)param);
    int col = p1.column;
    int vis[SIZE] = {0};

    for (int i = 0; i < SIZE; i++) {
        int num = sudoku[i][col];
        if (vis[num - 1] || num < 1 || num > SIZE) {
            pthread_exit(0);
        }
        vis[num - 1] = 1;
    }

    // Mark this column as valid
    columnValidity[col] = 1;
    pthread_exit(0);
}

/**
 * Function to check the validity of a specific row in the Sudoku grid.
 * Marks rowValidity[row] as 1 if the row is valid.
 */
void *checkRow(void *param) {
    parameters p1 = *((parameters *)param);
    int row = p1.row;
    int vis[SIZE] = {0};

    for (int i = 0; i < SIZE; i++) {
        int num = sudoku[row][i];
        if (num < 1 || num > SIZE || vis[num - 1]) {
            pthread_exit(0);
        }
        vis[num - 1] = 1;
    }

    // Mark this row as valid
    rowValidity[row] = 1;
    pthread_exit(0);
}

/**
 * Function to check the validity of a 3x3 subgrid in the Sudoku grid.
 * Marks subgridValidity[index] as 1 if the subgrid is valid.
 */
void *checkSubgrid(void *param) {
    parameters p1 = *((parameters *)param);
    int startRow = p1.row;
    int startCol = p1.column;
    int vis[SIZE] = {0};

    for (int i = startRow; i < startRow + 3; i++) {
        for (int j = startCol; j < startCol + 3; j++) {
            int num = sudoku[i][j];
            if (num < 1 || num > SIZE || vis[num - 1]) {
                pthread_exit(0);
            }
            vis[num - 1] = 1;
        }
    }

    // Mark this subgrid as valid
    subgridValidity[3 * (startRow / 3) + (startCol / 3)] = 1;
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

    // a. Create threads to check each column
    pthread_t threads_col[SIZE];
    for (int i = 0; i < SIZE; i++) {
        parameters *p1 = malloc(sizeof(parameters));
        p1->row = 0;
        p1->column = i;
        pthread_create(&threads_col[i], NULL, checkColumn, p1);
    }

    // Wait for all column threads to finish
    for (int i = 0; i < SIZE; i++) {
        pthread_join(threads_col[i], NULL);
    }

    // b. Create threads to check each row
    pthread_t threads_row[SIZE];
    for (int i = 0; i < SIZE; i++) {
        parameters *p1 = malloc(sizeof(parameters));
        p1->row = i;
        p1->column = 0;
        pthread_create(&threads_row[i], NULL, checkRow, p1);
    }

    // Wait for all row threads to finish
    for (int i = 0; i < SIZE; i++) {
        pthread_join(threads_row[i], NULL);
    }

    // c. Create threads to check each subgrid
    pthread_t threads_subgrid[SIZE];
    for (int i = 0; i < SIZE; i++) {
        parameters *p1 = malloc(sizeof(parameters));
        p1->row = (i / 3) * 3;
        p1->column = (i % 3) * 3;
        pthread_create(&threads_subgrid[i], NULL, checkSubgrid, p1);
    }

    // Wait for all subgrid threads to finish
    for (int i = 0; i < SIZE; i++) {
        pthread_join(threads_subgrid[i], NULL);
    }

    // Validate results and print the outcome
    for (int i = 0; i < SIZE; i++) {
        if (rowValidity[i] != 1) {
            printf("Sudoku Puzzle is invalid: Row %d is incorrect.\n", i + 1);
            return 0;
        }
    }

    for (int i = 0; i < SIZE; i++) {
        if (columnValidity[i] != 1) {
            printf("Sudoku Puzzle is invalid: Column %d is incorrect.\n", i + 1);
            return 0;
        }
    }

    for (int i = 0; i < SIZE; i++) {
        if (subgridValidity[i] != 1) {
            printf("Sudoku Puzzle is invalid: Subgrid %d is incorrect.\n", i + 1);
            return 0;
        }
    }

    // If all checks pass
    printf("Sudoku Puzzle is valid!\n");
    return 0;
}
