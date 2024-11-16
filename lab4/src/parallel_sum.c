#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <getopt.h>
#include "sum.h"

void GenerateArray(int *array, unsigned int array_size, unsigned int seed) {
  srand(seed);
  for (int i = 0; i < array_size; i++) {
    array[i] = rand();
  }
}

void *ThreadSum(void *args) {
  struct SumArgs *sum_args = (struct SumArgs *)args;
  return (void *)(size_t)Sum(sum_args);
}

int main(int argc, char **argv) {
  uint32_t threads_num = 0;
  uint32_t array_size = 0;
  uint32_t seed = 0;

  // Парсинг аргументов командной строки
  while (1) {
    static struct option options[] = {
        {"threads_num", required_argument, 0, 0},
        {"seed", required_argument, 0, 0},
        {"array_size", required_argument, 0, 0},
        {0, 0, 0, 0}};

    int option_index = 0;
    int c = getopt_long(argc, argv, "", options, &option_index);

    if (c == -1) break;

    switch (option_index) {
      case 0:
        threads_num = atoi(optarg);
        break;
      case 1:
        seed = atoi(optarg);
        break;
      case 2:
        array_size = atoi(optarg);
        break;
      default:
        printf("Usage: %s --threads_num \"num\" --seed \"num\" --array_size \"num\"\n", argv[0]);
        return 1;
    }
  }

  if (threads_num == 0 || seed == 0 || array_size == 0) {
    printf("Usage: %s --threads_num \"num\" --seed \"num\" --array_size \"num\"\n", argv[0]);
    return 1;
  }

  pthread_t threads[threads_num];
  int *array = malloc(sizeof(int) * array_size);

  // Генерация массива
  GenerateArray(array, array_size, seed);

  struct SumArgs args[threads_num];
  int chunk_size = array_size / threads_num;

  // Замер времени
  clock_t start_time = clock();

  for (uint32_t i = 0; i < threads_num; i++) {
    args[i].array = array;
    args[i].begin = i * chunk_size;
    args[i].end = (i == threads_num - 1) ? array_size : (i + 1) * chunk_size;

    if (pthread_create(&threads[i], NULL, ThreadSum, (void *)&args[i])) {
      printf("Error: pthread_create failed!\n");
      return 1;
    }
  }

  int total_sum = 0;
  for (uint32_t i = 0; i < threads_num; i++) {
    int sum = 0;
    pthread_join(threads[i], (void **)&sum);
    total_sum += sum;
  }

  clock_t end_time = clock();
  double elapsed_time = (double)(end_time - start_time) / CLOCKS_PER_SEC;

  free(array);
  printf("Total: %d\n", total_sum);
  printf("Elapsed time: %f seconds\n", elapsed_time);

  return 0;
}








// #include <stdint.h>
// #include <stdio.h>
// #include <stdlib.h>

// #include <pthread.h>

// struct SumArgs {
//   int *array;
//   int begin;
//   int end;
// };

// int Sum(const struct SumArgs *args) {
//   int sum = 0;
//   // TODO: your code here 
//   return sum;
// }

// void *ThreadSum(void *args) {
//   struct SumArgs *sum_args = (struct SumArgs *)args;
//   return (void *)(size_t)Sum(sum_args);
// }

// int main(int argc, char **argv) {
//   /*
//    *  TODO:
//    *  threads_num by command line arguments
//    *  array_size by command line arguments
//    *	seed by command line arguments
//    */

//   uint32_t threads_num = 0;
//   uint32_t array_size = 0;
//   uint32_t seed = 0;
//   pthread_t threads[threads_num];

//   /*
//    * TODO:
//    * your code here
//    * Generate array here
//    */

//   int *array = malloc(sizeof(int) * array_size);

//   struct SumArgs args[threads_num];
//   for (uint32_t i = 0; i < threads_num; i++) {
//     if (pthread_create(&threads[i], NULL, ThreadSum, (void *)&args)) {
//       printf("Error: pthread_create failed!\n");
//       return 1;
//     }
//   }

//   int total_sum = 0;
//   for (uint32_t i = 0; i < threads_num; i++) {
//     int sum = 0;
//     pthread_join(threads[i], (void **)&sum);
//     total_sum += sum;
//   }

//   free(array);
//   printf("Total: %d\n", total_sum);
//   return 0;
// }
