#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include <pthread.h>
#include <semaphore.h>

#define N 50

pthread_mutex_t  mutex_list;
sem_t  list_empty;
sem_t  list_full;

pthread_mutex_init (&mutex , NULL);
sem_init (&empty , 0 , N);
sem_init (&full , 0 , 0);
