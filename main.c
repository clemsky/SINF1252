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

  int err;
  
  int n_threads = 1;
  FILE* binFile;
  binFile = (FILE*)malloc(sizeof(FILE));
  binFile = NULL;
  int option = VOWELS;

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
  if(binFile == NULL){
    printf("Pas de file en argument\n");
    exit(EXIT_FAILURE);
  }

  Stack* s;
  s = (Stack*)malloc(sizeof(Stack));
  create(s);

  pthread_t thread[n_threads];
  arg_thread *arg_t[n_threads];

  int numberHashes = 5;
  int numberHashesPerThread = numberHashes/n_threads;
  int rest = numberHashes % n_threads;

  err = pthread_mutex_init(&mutex_stack, NULL);
  if(err != 0){
    perror("mutex_init");
    exit(EXIT_FAILURE);
  }

  err = pthread_mutex_init(&mutex_file, NULL);
  if(err != 0){
    perror("mutex_init");
    exit(EXIT_FAILURE);
  }

  for (int i = 0; i < n_threads; i++){
    arg_t[i]=(arg_thread*)malloc(sizeof(arg_thread));
    if(arg_t[i] == NULL){
      exit(EXIT_FAILURE);
    }

    arg_t[i]->file = binFile;
    arg_t[i]->option = option;
    arg_t[i]->operation = numberHashesPerThread;
    arg_t[i]->stack = s;
    arg_t[i]->number = i;
    if (rest > 0){
      arg_t[i]->operation++;
      rest--;
    }
  }

  for (int i = 0; i < n_threads; i++){
    err = pthread_create(&(thread[i]), NULL, &computing, (void*)arg_t[i]);
    if(err != 0){
      perror("pthread_create");
      exit(EXIT_FAILURE);
    }
  }

  for (int i = 0; i < n_threads; i++){
    err = pthread_join(thread[i], NULL);
    if(err != 0){
      perror("pthread_join");
      exit(EXIT_FAILURE);
    }
  }

  err = pthread_mutex_destroy(&mutex_stack);
  if(err != 0){
    perror("mutex_destroy");
    exit(EXIT_FAILURE);
  }

  err = pthread_mutex_destroy(&mutex_file);
  if(err != 0){
    perror("mutex_destroy");
    exit(EXIT_FAILURE);
  }

  printf("Les mots de passe candidats sont :\n");
  printAll(s);
  fclose(binFile);
  return EXIT_SUCCESS;
}

void* computing(void* arg){
  arg_thread* arg_t = (arg_thread*)arg;
  size_t len = 16;

  for(int i = 0; i < arg_t->operation; i++){

    char buffer[32];
    uint8_t hash[32];
    char* res;
    res = (char*)malloc(len*(sizeof(char)));
    if(res == NULL){
      exit(1);
    }

    pthread_mutex_lock(&mutex_file);
    printf("Thread %i : begin fread\n",arg_t->number);
    fread(buffer,sizeof(buffer),1,arg_t->file);
    printf("Thread %i : end fread\n",arg_t->number);
    pthread_mutex_unlock(&mutex_file);

    /* Calling reversehash function */

    for (int i =0; i<32; i++){
      hash[i] = (uint8_t)buffer[i];
    }

    printf("Thread %i : begin revershash\n",arg_t->number);
    bool flag = reversehash(hash,res,len);
    printf("Thread %i : end reversehash\n",arg_t->number);

    if(flag){
      pthread_mutex_lock(&mutex_stack);
      place(arg_t->stack,res,arg_t->option);
      pthread_mutex_unlock(&mutex_stack);
    }
  }
  return NULL;
}
