#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

typedef struct {
    int start;
    int end;
    int mod;
    long long result;
} ThreadData;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
long long final_result = 1;

void* factorial_thread(void* arg) {
    ThreadData* data = (ThreadData*)arg;
    long long local_result = 1;

    for (int i = data->start; i <= data->end; i++) {
        local_result = (local_result * i) % data->mod;
    }

    pthread_mutex_lock(&mutex);
    final_result = (final_result * local_result) % data->mod;
    pthread_mutex_unlock(&mutex);

    return NULL;
}

int main(int argc, char* argv[]) {
    if (argc != 5) {
        fprintf(stderr, "Usage: %s -k <k> --pnum=<pnum> --mod=<mod>\n", argv[0]);
        return 1;
    }

    int k = 0, pnum = 0, mod = 0;

    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-k") == 0) {
            k = atoi(argv[++i]);
        } else if (strncmp(argv[i], "--pnum=", 7) == 0) {
            pnum = atoi(argv[i] + 7);
        } else if (strncmp(argv[i], "--mod=", 6) == 0) {
            mod = atoi(argv[i] + 6);
        }
    }

    if (k <= 0 || pnum <= 0 || mod <= 0) {
        fprintf(stderr, "Invalid arguments\n");
        return 1;
    }

    pthread_t threads[pnum];
    ThreadData thread_data[pnum];

    int chunk_size = k / pnum; // Размер части факториала, которую будет вычислять каждый поток
    int remainder = k % pnum; // Этот остаток добавляется к последнему потоку, чтобы учесть неравномерное деление.

    for (int i = 0; i < pnum; i++) {
        thread_data[i].start = i * chunk_size + 1; // начало диапазора вычесления факториала в потоке
        thread_data[i].end = (i + 1) * chunk_size; // конец
        thread_data[i].mod = mod; // Модуль, по которому будет вычисляться факториал

        if (i == pnum - 1) { // remainder: Остаток от деления k на pnum. Этот остаток добавляется к последнему потоку, чтобы учесть неравномерное деление.
            thread_data[i].end += remainder;
        }

        pthread_create(&threads[i], NULL, factorial_thread, &thread_data[i]); // создает потоки для параллельного вычисления факториала.
    }
    // &threads[i]: Указатель на переменную типа pthread_t, которая будет хранить идентификатор созданного потока
    // NULL: Указатель на структуру pthread_attr_t, которая определяет атрибуты потока. В данном случае NULL означает использование атрибутов по умолчанию.
    // factorial_thread: Указатель на функцию, которую будет выполнять поток. В данном случае это функция factorial_thread, которая вычисляет часть факториала.
    // &thread_data[i]: Указатель на аргумент, который будет передан функции потока. В данном случае это указатель на структуру ThreadData, содержащую информацию о том, какую часть факториала должен вычислять поток.
    
    for (int i = 0; i < pnum; i++) { // 
        pthread_join(threads[i], NULL);
    }

    printf("Factorial of %d modulo %d is %lld\n", k, mod, final_result);

    return 0;
}