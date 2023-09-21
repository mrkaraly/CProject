#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MAX_INPUT_LENGTH 1024

int main() {
    char input[MAX_INPUT_LENGTH];
    ssize_t input_length;

    while (1) {
        /* Display the prompt */
        write(STDOUT_FILENO, "simple_shell> ", 14);

        /* Read user input */
        input_length = read(STDIN_FILENO, input, sizeof(input));
        if (input_length == -1) {
            /* Handle EOF (Ctrl+D) */
            write(STDOUT_FILENO, "\nExiting simple_shell.\n", 24);
            break;
        }

        /* Null-terminate the input string */
        input[input_length] = '\0';

        /* Remove the newline character */
        int newline_pos = -1;
        int i;
        for (i = 0; i < input_length; i++) {
            if (input[i] == '\n') {
                newline_pos = i;
                break;
            }
        }
        if (newline_pos != -1) {
            input[newline_pos] = '\0';
        }

        /* Fork a child process */
        pid_t pid;
        pid = fork();

        if (pid == -1) {
            perror("fork");
            exit(EXIT_FAILURE);
        }

        if (pid == 0) {
            /* Child process */
            char *args[] = {input, NULL};
            execve(input, args, NULL);
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
    }

    return 0;
}
