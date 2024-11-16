#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
    pid_t child_pid = fork();

    if (child_pid == 0) {
        // Дочерний процесс
        printf("Дочерний процесс с PID %d завершается...\n", getpid());
        exit(0);
    } else if (child_pid > 0) {
        // Родительский процесс
        printf("Родительский процесс с PID %d продолжает выполнение...\n", getpid());

        // Родительский процесс не вызывает wait(), что приводит к созданию зомби-процесса
        sleep(10);

        // Теперь родительский процесс вызывает wait(), чтобы удалить зомби-процесс
        int status;
        wait(&status);
        printf("Родительский процесс удалил зомби-процесс.\n");
    } else {
        // Ошибка при вызове fork()
        perror("fork");
        exit(1);
    }

    return 0;
}