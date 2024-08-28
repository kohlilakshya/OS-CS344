# CS344: Operating Systems Lab

Welcome to my repository for the CS344: Operating Systems Lab course! This repository contains various labs and assignments that I have completed as part of the course. The repository provide hands-on experience with essential operating systems concepts, including process management, memory management, file systems, Inter-Process Communication (IPC), and more.

## Repository Structure

The repository is organized into directories for each lab, with each directory containing relevant code and documentation for that particular lab.

## Assignments

### Assignment 01:

- **Description**: This assignment focuses on process management. The lab involves understanding and implementing key system calls related to process management.

- **Objectives**:
  - Implement and demonstrate the usage of system calls: `fork()`, `wait()`, `exit()`, `getenv()`, `execle()`, `execlp()`, and `execl()`.
  - Understand the process creation, execution, and termination.

- **Files**:
  - `code/`: Contains the source code for Assignment 01, demonstrating the use of system calls.
  - `documentation/`: Includes the lab instructions.

- **Instructions**:
  After cloning the repository. Change the directory to assignment01 and then you can directly run the following scripts to compile and execute the code:

  ```bash
  ./compile-1a-and-execute.sh
  ./compile-1b-and-execute.sh
  ./compile-1c-and-execute.sh
  ./compile-1d-and-execute.sh
# Assignment 02

- **Description**: This assignment focuses on daemon processes and process management. The lab involves creating and managing a daemon process that generates and logs a sequence of numbers based on the process ID.

- **Objectives**:
  - Implement a daemon process in C that performs specific tasks including process creation, logging, and sequence generation.
  - Understand and use system calls: `fork()`, `umask()`, `setsid()`, `chdir()`, `openlog()`, `syslog()`, `wait()`, `exit()`, `kill()`, and `sleep()`.
  - Implement functions to start and stop the daemon process, ensuring proper resource management and logging.

- **Files**:
  - `code/`: Contains the source code for Assignment 02, including the implementation of the daemon process.
  - `documentation/`: Includes the lab instructions.

<!-- - **Instructions**:
  After cloning the repository. Change the directory to assignment02 and then follow these steps to compile and execute the code:

  ```bash
  # Compile the source code
  gcc -o run_daemon code/1.c

   -->

  - **Instructions**:
    After cloning the repository, follow these steps:

      ```bash
      cd assignment02
      gcc -o run_daemon code/1.c
      sudo ./run_daemon start
      sudo ./run_daemon stop
      ```


# Assignment 03:
- **Description**: This lab focuses on interprocess communication and process management using system calls in C. The lab involves creating multiple C programs to demonstrate the use of pipes and other system calls to implement command processing and sequence generation.

- **Objectives**:
  - Implement interprocess communication using pipes.
  - Understand and use system calls: `pipe()`, `dup2()`, `fork()`, `exec()`, `write()`, and `read()`.
  - Develop C programs to compute sequences and handle commands with pipes.

- **Files**:
  - `code/`: Contains the source code for Assignment 03, including the implementation of the required programs.
  - `documentation/`: Includes the lab instructions.

- **Instructions**:
  After cloning the repository. Change the directory to assignment03 and then follow these steps to compile and execute the code:

### Compilation
  Compile each program using the following commands:
  ```bash
  # Compile the first program to compute sequences
  gcc -o compute_sequence_1 code/ai.c

  # Compile the second program to read integers and compute sequences
  gcc -o compute_sequence_2 code/aii.c
  ```


Note: Before executing the next command, ensure that compute_sequence_1 and compute_sequence_2 are compiled first.
```Bash
# Compile the third program for handling command input and processing with pipes
gcc -o process_commands code/aiii.c

# Compile the fourth program for advanced pipe management and execution
gcc -o aiv code/aiv.c
```

# Assignment 05:

- **Description**: This lab focuses on implementing interprocess communication using message passing in C. The assignment involves performing edge detection on a grayscale image using multiple processes and message queues.

- **Objectives**:
  - Implement message passing between processes using System V message queues.
  - Understand and use system calls: `ftok()`, `msgget()`, `msgsnd()`, `msgrcv()`, `msgctl()`, and `fork()`.
  - Develop C programs to obtain keys, create message queues, send and receive messages, and perform computations in child processes.
   - Ensure synchronization and orderly output of the computed edge values.


- **Files**:
  - `code/`: Contains the source code for Assignment 05, including two programs for message passing and edge detection.
  - `documentation/`: Includes the lab instructions and the edge-detection.pdf file.


# Assignment 06:

- **Description**: This lab focuses on process synchronization using semaphores and shared memory. The assignment involves implementing a web server simulation using System V semaphores and shared memory, requiring an understanding of various system calls and process synchronization techniques.

- **Objectives**:
  - Implement semaphore operations using `semop()` and `sembuf`.
  - Create and manage shared memory segments.
  - Develop a web server that processes HTTP requests using child processes and semaphore synchronization.
  - Develop a client program that simulates generating HTTP requests and interacts with the web server.

- **Files**:
   - `code/`: Contains the source code for Assignment 06, including two programs:
      - `server.c`: Contains the implementation of the web server simulation.
      - `client.c`: Contains the implementation of the client program that generates HTTP requests.
   - `documentation/`: Includes the lab instructions and additional resources.

- **Instructions**:
  After cloning the repository, follow these steps:
     ```bash
     cd assignment06
     gcc code/server.c -o server
     ./server
     ```

  In a separate terminal, compile and execute the client program:
     ```bash
     gcc code/client.c -o client
     ./client
     ```

