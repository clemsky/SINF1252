#ifndef _MAIN_H
#define _MAIN_H

#define VOWELS 0
#define CONSONANTS 1

pthread_mutex_t mutex_stack;
pthread_mutex_t mutex_file;

typedef struct arg_thread {
  int operation;
  FILE *file;
  int option;
  Stack* stack;
} arg_thread;

int main(int argc, char *argv[]);

void *compute(void* arg);

#endif
