#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include <pthread.h>
#include <semaphore.h>

#include "main.h"
#include "stack.h"
#include "reverse.h"



int main(int argc, char* argv[]){

  int n_threads = 1;
  int option = VOWELS;
  FILE *binFile;
  Stack* s;

  for (int i = 0; i < argc; i++){
    const char* str = argv[i];
    if (argv[i] == "-t"){
      n_threads = atoi(argv[i+1]);
    }
    if (argv[i] == "-c"){
      option = CONSONANTS;
    }
    if (strstr(str,".bin") != NULL){
      binFile = fopen(str,"rb");
    }
  }

  pthread_t thread[n_threads];
  arg_thread arg_t[n_threads];
  int err;

  create(s);

  fseek(binFile, 0, SEEK_END);
  int size = ftell(f);
  fseek(binFile, 0, SEEK_SET);

  int numberHashes = size/32;
  int numberHashesPerThread = numberHashes/n_threads
  int rest = numberHashes % n_threads

  /* Initialising mutex */

  err = pthread_mutex_init(&mutex_stack, NULL);
  if(err != 0){
    error(err,"mutex_init")
  }

  err = pthread_mutex_init(&mutex_file, NULL);
  if(err != 0){
    error(err,"mutex_init")
  }

  /* Setting arguments for threads */

  for (int i = 0; i < n_threads; i++){
    arg_t[i]->file = binFile;
    arg_t[i]->option = option;
    arg_t[i]->operation = numberHashesPerThread;
    arg_t[i]->stack = s;
    if (rest > 0){
      arg_t[i]->operation++
      rest--;
    }
  }

  /* Creating the threads */

  for (int i = 0; i < n_threads; i++){
    err = pthread_create(&(thread[i]), NULL, &compute, (void*)arg_t[i]);
    if(err != 0){
      error(err,"pthread_create")
    }
  }

  /* Joining the threads */

  for (int i = 0; i < n_threads; i++){
    err = pthread_join(thread[i], NULL)
    if(err != 0){
      error(err,"pthread_join")
    }
  }

  /* Destroying mutex */

  err = pthread_mutex_destroy(&mutex_stack);
  if(err != 0){
    error(err,"mutex_destroy")
  }

  err = pthread_mutex_destroy(&mutex_file);
  if(err != 0){
    error(err,"mutex_destroy")
  }

  fclose(binFile)
  printAll(s);
}

/*
 * Function that each thread does until the number of
 * operation is null
 *
 */

void *compute(void* arg){

  arg_thread* arg_t = (arg_thread*)arg;

  for(int i = 0; i < arg_t->operation; i++){

    char buffer[32];
    const uint8_t* hash[32];
    char* res;
    res = (char*)malloc(len*(sizeof(char)))
    if(res == NULL){
      exit(0);
    }
    size_t len = 16;

    pthread_mutex_lock(&mutex_file);
    fread(buffer,sizeof(buffer),1,arg_t->file);
    pthread_mutex_unlock(&mutex_file);
    hash = (uint8_t*)buffer;

    /* Calling reversehash function */

    bool flag = reversehash(hash,res,len);

    if(flag){
      pthread_mutex_lock(&mutex_stack);
      place(arg_t->stack,res,arg_t->option);
      pthread_mutex_unlock(&mutex_stack);
    }
    free(hash);
    free(buffer);
  }
  free(arg_t);
  free(arg);
}
