#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main() {
    pid_t pid = fork();

    if (pid < 0) {
        perror("fork");
        return 1;
    }

    if (pid == 0) {
        printf("Child running\n");
        exit(42);
    } else {
        int status;

        wait(&status);

        if (WIFEXITED(status)) {
            printf("Child exited with %d\n",
                   WEXITSTATUS(status));
        }
    }

    return 0;
}
