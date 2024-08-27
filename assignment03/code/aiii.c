#include <stdio.h>
#include <sys/types.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define BUFFER_SIZE 256
#define READ_END 0
#define WRITE_END 1

void fun(int n) {
    while(n != 1) {
        printf("%d ", n);
        if(n % 2) n = 3 * n + 1;
        else n /= 2;
    }
    printf("%d\n", n);
}

int main() {
    char* seq1 = "./compute_sequence_1";
    char* seq2 = "./compute_sequence_2";
    
    char command[BUFFER_SIZE];
    char *arguement[2];
	
    printf("Enter a command (or 'quit' to exit): ");
    fgets(command, sizeof(command), stdin);

    command[strlen(command) - 1] = '\0';
    if (strcmp(command, "quit") == 0 || strcmp(command, "quit") == 10) return 0; 

    char* parse = strtok(command, "|");
    int i = 0;
	
    while(parse) {
        arguement[i] = parse;
        i++;
        parse = strtok(NULL, "|");
    }
	
    int pipe_fd[2];
    if (pipe(pipe_fd) < 0) {
        fprintf(stderr, "Pipe failed");
        return 1;
    }

    pid_t pid = fork();

    if (pid == -1) {
        fprintf(stderr, "Pipe Failed"); 
        return 1;
    } 
    else if (pid == 0) { 
        // Child
        close(pipe_fd[WRITE_END]);

        dup2(pipe_fd[READ_END], STDIN_FILENO);
        close(pipe_fd[READ_END]);
		
        execlp(seq2, seq2, NULL);
		
    } 
    else { 
        // Parent
        close(pipe_fd[READ_END]);
			
        dup2(pipe_fd[WRITE_END], STDOUT_FILENO);
        close(pipe_fd[WRITE_END]);
		
        char *arguement2[2];
        i = 0;
        parse = strtok(command, " ");
        while(parse) {
            arguement2[i] = parse;
            i++;
            parse = strtok(NULL, " ");
        }	
        if(i == 1) arguement2[i] = "100";
        execlp(seq1, seq1, arguement2[1], NULL);
			
        wait(NULL);
        exit(1);
    }

    return 0;
}
