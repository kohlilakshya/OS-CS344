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
  After cloning the repository, you can directly run the following scripts to compile and execute the code:

  ```bash
  ./compile-1a-and-execute.sh
  ./compile-1b-and-execute.sh
  ./compile-1c-and-execute.sh
  ./compile-1d-and-execute.sh

# Assignment 02

## Description
This assignment focuses on daemon processes and process management. The lab involves creating and managing a daemon process that generates and logs a sequence of numbers based on the process ID.

## Objectives
- Implement a daemon process in C that performs specific tasks including process creation, logging, and sequence generation.
- Understand and use system calls: `fork()`, `umask()`, `setsid()`, `chdir()`, `openlog()`, `syslog()`, `wait()`, `exit()`, `kill()`, and `sleep()`.
- Implement functions to start and stop the daemon process, ensuring proper resource management and logging.

## Files
- `code/`: Contains the source code for Assignment 02, including the implementation of the daemon process.
- `documentation/`: Includes the lab instructions.

## Instructions
After cloning the repository, follow these steps to compile and execute the code:

```bash
# Compile the source code
gcc -o run_daemon code/1.c

# Start the daemon process with root privileges
sudo ./run_daemon start

# Stop the daemon process with root privileges
sudo ./run_daemon stop
