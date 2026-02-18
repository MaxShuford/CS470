// CS470 - LAB2 - myshell.c 
// Max Shuford 2/16/2026

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>

#define MAX_LINE 1024  
 
int main() {
    
    char line[MAX_LINE];
    while (1) { 
        printf("myshell> ");
        fflush(stdout);      
        if (!fgets(line, sizeof(line), stdin))
            break;   
 
        line[strcspn(line, "\n")] = 0;

        if (strlen(line) == 0)
            continue;

        // PARSE INPUT 
        char *args[100];
        int arg_count = 0;

        char *input_file = NULL;
        char *output_file = NULL;
        int append = 0;

        char *p = line;

        // Tokenize the input line
        while (*p) {

            while (*p == ' ' || *p == '\t')
                p++;

            if (*p == '\0')
                break;

            // Handle quoted strings
            if (*p == '"') {
                p++;
                args[arg_count++] = p;

                while (*p && *p != '"')
                    p++;

                if (*p == '"')
                    *p++ = '\0';
            }
            // Handle redirection >>
            else if (strncmp(p, ">>", 2) == 0) {
                p += 2;
                while (*p == ' ') p++;
                output_file = p;
                append = 1;

                while (*p && *p != ' ')
                    p++;
                if (*p)
                    *p++ = '\0';
            }
            // Handle redirection >
            else if (*p == '>') {
                p++;
                while (*p == ' ') p++;
                output_file = p;
                append = 0;

                while (*p && *p != ' ')
                    p++;
                if (*p)
                    *p++ = '\0';
            }
            // Handle input redirection <
            else if (*p == '<') {
                p++;
                while (*p == ' ') p++;
                input_file = p;

                while (*p && *p != ' ')
                    p++;
                if (*p)
                    *p++ = '\0';
            }
            // Normal argument
            else {
                args[arg_count++] = p;
                while (*p && *p != ' ')
                    p++;
                if (*p)
                    *p++ = '\0';
            }
        }

        args[arg_count] = NULL;

        if (args[0] == NULL)
            continue;

        // COMMANDS
        if (strcmp(args[0], "exit") == 0) {
            break;
        }

        if (strcmp(args[0], "cd") == 0) {
            if (args[1] == NULL) {
                char *home = getenv("HOME");
                if (home)
                    chdir(home);
            } else {
                if (chdir(args[1]) != 0)
                    perror("cd error");
            }
            continue;
        }

        // FORK
        pid_t pid = fork();

        if (pid < 0) {
            perror("fork error");
            continue;
        }

        if (pid == 0) {
            // CHILD PROCESS

            // Input redirection
            if (input_file != NULL) {
                int fd = open(input_file, O_RDONLY);
                if (fd < 0) {
                    perror("input file error");
                    exit(1);
                }
                dup2(fd, STDIN_FILENO);
                close(fd);
            }

            // Output redirection
            if (output_file != NULL) {
                int fd;
                if (append)
                    fd = open(output_file, O_WRONLY | O_CREAT | O_APPEND, 0644);
                else
                    fd = open(output_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);

                if (fd < 0) {
                    perror("output file error");
                    exit(1);
                }

                dup2(fd, STDOUT_FILENO);
                close(fd);
            }

            execvp(args[0], args);
            perror("command not found");
            exit(1);
        }
        else {
            // PARENT PROCESS
            waitpid(pid, NULL, 0);
        }
    }

    printf("\nGoodbye!\n");
    return 0;
}