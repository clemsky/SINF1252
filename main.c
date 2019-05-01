#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include <pthread.h>
#include <semaphore.h>

#include "main.h"
#include "stack.c"

#define N_THREADS 4

Stack* s;
FILE *binFile;
int* numberHashes

pthread_mutex_t mutex_stack;
pthread_mutex_t max_vowels;
pthread_mutex_t max_consonants;
pthread_mutex_t mutex_file;

int main(int argc, char* argv[]){

  pthread_t thread[N_THREADS];
  int err;

  create(s);

  binFile = fopen("test.bin","rb");
  fseek(binFile, 0, SEEK_END);
  int size = ftell(f);
  fseek(binFile, 0, SEEK_SET);

  *numberHashes = size/32;

  err = pthread_mutex_init(&mutex_stack, NULL);
  if(err != 0){
    error(err,"mutex_init")
  }

  err = pthread_mutex_init(&max_vowels, NULL);
  if(err != 0){
    error(err,"mutex_init")
  }

  err = pthread_mutex_init(&max_consonants, NULL);
  if(err != 0){
    error(err,"mutex_init")
  }

  err = pthread_mutex_init(&mutex_file, NULL);
  if(err != 0){
    error(err,"mutex_init")
  }

  // Creating the threads

  for (int i = 0; i < N_THREADS; i++){
    err = pthread_create(&(thread[i]), NULL, &compute, (void*)binFile);
    if(err != 0){
      error(err,"pthread_create")
    }
  }

  for (int i = 0; i < N_THREADS; i++){
    err = pthread_join(thread[i], NULL)
    if(err != 0){
      error(err,"pthread_join")
    }
  }

  err = pthread_mutex_destroy(&mutex_stack);
  if(err != 0){
    error(err,"mutex_destroy")
  }

  err = pthread_mutex_destroy(&max_vowels);
  if(err != 0){
    error(err,"mutex_destroy")
  }

  err = pthread_mutex_destroy(&max_consonants);
  if(err != 0){
    error(err,"mutex_destroy")
  }

  printAll(s);

}

void *compute(void* binFileVoid){
  FILE* binFile = (FILE*)binFileVoid;

  while (true) {
    char buffer[32];
    pthread_mutex_lock(&mutex_file);
    fread(buffer,sizeof(buffer),1,binFile);
    *numberHashes--;
    pthread_mutex_unlock(&mutex_file);
  }

}
