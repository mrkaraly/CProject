#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "main.h"

int execute_command(char *command) {
    /* Fork a child process */
    pid_t pid;
    pid = fork();

    if (pid == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    if (pid == 0) {
        /* Child process */
        char *args[] = {command, NULL};
        execve(command, args, NULL);
        perror("execve");
        _exit(EXIT_FAILURE);
    } else {
        /* Parent process */
        int status;
        if (waitpid(pid, &status, 0) == -1) {
            perror("waitpid");
            exit(EXIT_FAILURE);
        }
    }

    return 0;
}
