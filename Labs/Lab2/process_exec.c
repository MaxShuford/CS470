#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

#define NUM_CHILDREN 15

int main() {
    pid_t childPids[NUM_CHILDREN];
    int status;

    int normal_zero = 0;
    int normal_nonzero = 0;
    int signaled = 0;

    printf("Parent PID: %d\n\n", getpid());

    for (int i = 0; i < NUM_CHILDREN; i++) {
        pid_t pid = fork();

        if (pid < 0) {
            perror("fork");
            exit(1);
        }

        if (pid == 0) {
            // CHILD PROCESS
            printf("[Child %d] PID=%d ", i, getpid());

            // 2 children abort intentionally
            if (i == 11 || i == 12) {
                printf("-> abort()\n");
                abort();
            }

            // 2 children run invalid commands
            if (i == 9) {
                printf("-> invalid_command_1\n");
                char *args[] = {"invalid_command_1", NULL};
                execvp(args[0], args);
                perror("execvp");
                exit(127);
            }

            if (i == 10) {
                printf("-> invalid_command_2\n");
                char *args[] = {"invalid_command_2", NULL};
                execvp(args[0], args);
                perror("execvp");
                exit(127);
            }

            // Remaining valid commands
            char *commands[][4] = {
                {"ls", NULL},
                {"pwd", NULL},
                {"date", NULL},
                {"whoami", NULL},
                {"uname", "-a", NULL},
                {"ps", NULL},
                {"uptime", NULL},
                {"df", "-h", NULL},
                {"free", "-h", NULL},
                {"echo", "Hello Max", NULL},   // REQUIRED echo
                {"hostname", NULL},
                {"cal", NULL},
                {"env", NULL}
            };

            int cmd_index = i < 9 ? i : i - 2;

            printf("-> %s\n", commands[cmd_index][0]);
            execvp(commands[cmd_index][0], commands[cmd_index]);

            perror("execvp");
            exit(1);
        }

        // PARENT PROCESS
        childPids[i] = pid;
    }

    printf("\nParent waiting for children\n\n");

    for (int i = 0; i < NUM_CHILDREN; i++) {
        waitpid(childPids[i], &status, 0);

        if (WIFEXITED(status)) {
            int code = WEXITSTATUS(status);
            printf("Child %d (PID=%d) exited normally, exit code = %d\n",
                   i, childPids[i], code);

            if (code == 0)
                normal_zero++;
            else
                normal_nonzero++;
        } else if (WIFSIGNALED(status)) {
            int sig = WTERMSIG(status);
            printf("Child %d (PID=%d) terminated by signal %d\n",
                   i, childPids[i], sig);
            signaled++;
        }
    }

    printf("\n--- Summary ---\n");
    printf("Normal exit (code 0): %d\n", normal_zero);
    printf("Normal exit (non-zero): %d\n", normal_nonzero);
    printf("Terminated by signal: %d\n", signaled);

    return 0;
}
