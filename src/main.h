#ifndef _MAIN_H
#define _MAIN_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include <pthread.h>
#include <semaphore.h>

#include "stack.h"

#define VOWELS 0
#define CONSONANTS 1
#define OUT_STANDARD 0
#define OUT_FILE 1

pthread_mutex_t mutex_stack;
pthread_mutex_t mutex_file;

/*Structure permettant de mettre plusieurs argument dans les threads
 */
typedef struct arg_thread {
  int number;
  int* numberHashes;
  FILE *file;
  int option;
  Stack* stack;
} arg_thread;

int main(int argc, char *argv[]);

void* compute(void* arg);

#endif
