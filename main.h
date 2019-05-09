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

pthread_mutex_t mutex_stack;
pthread_mutex_t mutex_file;

typedef struct arg_thread {
  int operation;
  int number;
  FILE *file;
  int option;
  Stack* stack;
} arg_thread;

int main(int argc, char *argv[]);

void* computing(void* arg);

#endif
