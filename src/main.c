#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include <pthread.h>
#include <semaphore.h>
#include <time.h>

#include "main.h"
#include "stack.h"
#include "reverse.h"

int main(int argc, char* argv[]){

  time_t begin = time(NULL);

  int err;

  int n_threads = 1;
  int option = VOWELS;
  int numberFiles = 0;
  int output = OUT_STANDARD;
  char* outFile;

  for (int i = 0; i < argc; i++){
    const char* str = argv[i];
    if (strstr(str,".bin") != NULL){
      numberFiles++;
    }
  }

  if(numberFiles == 0){
    printf("Pas de fichier en argument\n");
    exit(EXIT_FAILURE);
  }

  FILE* binFile[numberFiles];
  for (int i = 0; i < numberFiles; i++){
    binFile[i] = (FILE*)malloc(sizeof(FILE));
  }
  char* fileName[numberFiles];
  for (int i = 0; i < numberFiles; i++){
    fileName[i] = (char*)malloc(30*sizeof(char));
  }

  int j = 0;
  for (int i = 0; i < argc; i++){
    const char* str = argv[i];
    if (strcmp(argv[i],"-t") == 0){
      n_threads = atoi(argv[i+1]);
    }
    if (strcmp(argv[i],"-c") == 0){
      option = CONSONANTS;
    }
    if (strcmp(argv[i],"-o") ==  0){
      output = OUT_FILE;
      outFile = argv[i+1];
    }
    if (strstr(str,".bin") != NULL){
      if (strlen(str) > 30){
        printf("Nom de fichier trop long");
        exit(EXIT_FAILURE);
      }
      strcpy(fileName[j],str);
      j++;
    }
  }


  Stack* s;
  s = (Stack*)malloc(sizeof(Stack));
  create(s);

  pthread_t thread[n_threads];
  arg_thread *arg_t[n_threads];

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
    arg_t[i]->option = option;
    arg_t[i]->number = i+1;
    arg_t[i]->stack = s;
  }

  for (int a = 0; a < numberFiles; a++){

    printf("Analyse du fichier %i\n",(a+1));
    binFile[a] = fopen(fileName[a],"rb");
    if(binFile[a] == NULL){
      printf("Erreur ouverture fichier\n");
      exit(EXIT_FAILURE);
    }

    fseek(binFile[a], 0L, SEEK_END);
    int size = ftell(binFile[a]);
    rewind(binFile[a]);

    if (size % 32 != 0){
      printf("Mauvais fichier en entrée");
      exit(EXIT_FAILURE);
    }

    int *numberHashes;
    numberHashes = (int*)malloc(sizeof(int));
    *numberHashes= size/32;

    for (int i = 0; i < n_threads; i++){
      arg_t[i]->file = binFile[a];
      arg_t[i]->numberHashes = numberHashes;
    }

    for (int i = 0; i < n_threads; i++){
      err = pthread_create(&(thread[i]), NULL, &compute, (void*)arg_t[i]);
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
    fclose(binFile[a]);
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

  for (int i = 0; i < numberFiles; i++){
    free(binFile[i]);
  }

  for (int i = 0; i < numberFiles; i++){
    free(fileName[i]);
  }

  if (output == OUT_FILE){
    printf("Les mots de passe candidats sont imprimés sur le fichier %s\n",outFile);
    printAllFile(s,outFile);
  }
  else {
    printf("Les mots de passe candidats sont :\n");
    printAll(s);
  }

  time_t end = time(NULL);
  printf("Temps d'exécution pour %i threads : %ld secondes\n",n_threads,(end-begin));
  return EXIT_SUCCESS;
}

void* compute(void* arg){
  arg_thread* arg_t = (arg_thread*)arg;
  size_t len = 16;

  while(true){

    bool flag1 = false;
    char buffer[32];
    uint8_t hash[32];
    char* res;
    res = (char*)malloc(len*(sizeof(char)));
    if(res == NULL){
      exit(1);
    }

    pthread_mutex_lock(&mutex_file);
    printf("Nombre de hash restant : %i\n",*(arg_t->numberHashes));
    if(*(arg_t->numberHashes) > 0){
      *(arg_t->numberHashes) = *(arg_t->numberHashes)-1;
      fread(buffer,sizeof(buffer),1,arg_t->file);
      flag1 = true;
    }
    pthread_mutex_unlock(&mutex_file);

    if(!flag1){
      break;
    }

    for (int i =0; i<32; i++){
      hash[i] = (uint8_t)buffer[i];
    }

    //printf("Thread %i : begin revershash\n",arg_t->number);
    bool flag2 = reversehash(hash,res,len);
    //printf("Thread %i : end reversehash\n",arg_t->number);

    if(flag2){
      pthread_mutex_lock(&mutex_stack);
      place(arg_t->stack,res,arg_t->option);
      pthread_mutex_unlock(&mutex_stack);
    }
  }
  return NULL;
}
