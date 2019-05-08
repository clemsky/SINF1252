#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include <pthread.h>
#include <semaphore.h>
#include <errno.h>

#include "main.h"
#include "stack.h"
#include "reverse.h"

int main(int argc, char* argv[]){

  int n_threads = 1;
  int option = VOWELS;
  FILE *binFile = NULL;
  Stack* s;

  s = (Stack*)malloc(sizeof(Stack));
  if (s == NULL){
    perror("malloc failed");
    exit(1);
  }

  for (int i = 0; i < argc; i++){
    const char* str = argv[i];
    if (strcmp(argv[i],"-t") == 0){
      n_threads = atoi(argv[i+1]);
    }
    if (strcmp(argv[i],"-c") == 0){
      option = CONSONANTS;
    }
    if (strstr(str,".bin") != NULL){
      binFile = fopen(str,"rb");
    }
  }

  if (binFile == NULL){
    perror("Error : Opening file");
    exit(1);
  }

  pthread_t thread[n_threads];
  arg_thread *arg_t[n_threads];
  int err;

  create(s);

  fseek(binFile, 0, SEEK_END);
  int size = ftell(binFile);
  fseek(binFile, 0, SEEK_SET);

  int numberHashes = size/32;
  int numberHashesPerThread = numberHashes/n_threads;
  int rest = numberHashes % n_threads;

  /* Initialising mutex */

  err = pthread_mutex_init(&mutex_stack, NULL);
  if(err != 0){
    perror("mutex_init");
    exit(0);
  }

  err = pthread_mutex_init(&mutex_file, NULL);
  if(err != 0){
    perror("mutex_init");
    exit(1);
  }

  /* Setting arguments for threads */

  for (int i = 0; i < n_threads; i++){

    arg_t[i]->file = binFile;
    arg_t[i]->option = option;
    arg_t[i]->operation = numberHashesPerThread;
    arg_t[i]->stack = s;
    if (rest > 0){
      arg_t[i]->operation++;
      rest--;
    }
  }

  /* Creating the threads */

  for (int i = 0; i < n_threads; i++){
    err = pthread_create(&(thread[i]), NULL, &compute, (void*)arg_t[i]);
    if(err != 0){
      perror("pthread_create");
      exit(1);
    }
  }

  /* Joining the threads */

  for (int i = 0; i < n_threads; i++){
    err = pthread_join(thread[i], NULL);
    if(err != 0){
      perror("pthread_join");
      exit(1);
    }
  }

  /* Destroying mutex */

  err = pthread_mutex_destroy(&mutex_stack);
  if(err != 0){
    perror("mutex_destroy");
    exit(1);
  }

  err = pthread_mutex_destroy(&mutex_file);
  if(err != 0){
    perror("mutex_destroy");
    exit(1);
  }

  fclose(binFile);
  printAll(s);
  return EXIT_SUCCESS;
}

/*
 * Function that each thread does until the number of
 * operation is null
 *
 */

void *compute(void* arg){

  arg_thread* arg_t = (arg_thread*)arg;
  size_t len = 16;

  for(int i = 0; i < arg_t->operation; i++){

    char buffer[32];
    const uint8_t hash[32];
    char* res;
    res = (char*)malloc(len*(sizeof(char)));
    if(res == NULL){
      exit(1);
    }


    pthread_mutex_lock(&mutex_file);
    fread(buffer,sizeof(buffer),1,arg_t->file);
    pthread_mutex_unlock(&mutex_file);

    /* Calling reversehash function */

    bool flag = reversehash(hash,res,len);

    if(flag){
      pthread_mutex_lock(&mutex_stack);
      place(arg_t->stack,res,arg_t->option);
      pthread_mutex_unlock(&mutex_stack);
    }
  }
  free(arg_t);
  free(arg);
  return NULL;
}
