#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char **argv) {
    if (argc != 3) {
        printf("Usage: %s seed array_size\n", argv[0]);
        return 1;
    }


    pid_t pid = fork();
    if (pid < 0) {
        perror("Fork failed");
        return 1;
    }

    if (pid == 0) {
        // Дочерний процесс
        execl("./sequential_min_max", "./sequential_min_max", argv[1], argv[2], (char *)NULL);
        // Если execl возвращает, значит произошла ошибка
        perror("execl failed");
        return 1;
    } else {
        // Родительский процесс
        int status;
        waitpid(pid, &status, 0);
        if (WIFEXITED(status)) {
            printf("Child exited with status %d\n", WEXITSTATUS(status));
        } else {
            printf("Child terminated abnormally\n");
        }
    }

    return 0;
}