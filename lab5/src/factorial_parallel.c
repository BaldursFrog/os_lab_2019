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

    int chunk_size = k / pnum;
    int remainder = k % pnum;

    for (int i = 0; i < pnum; i++) {
        thread_data[i].start = i * chunk_size + 1;
        thread_data[i].end = (i + 1) * chunk_size;
        thread_data[i].mod = mod;

        if (i == pnum - 1) {
            thread_data[i].end += remainder;
        }

        pthread_create(&threads[i], NULL, factorial_thread, &thread_data[i]);
    }

    for (int i = 0; i < pnum; i++) {
        pthread_join(threads[i], NULL);
    }

    printf("Factorial of %d modulo %d is %lld\n", k, mod, final_result);

    return 0;
}