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

  /*Calcule le nombre de fichier(numberFiles) à traiter
   */
  int i;
  for (i = 0; i < argc; i++){
    const char* str = argv[i];
    if (strstr(str,".bin") != NULL){
      numberFiles++;
    }
  }

  if(numberFiles == 0){
    printf("Pas de fichier en argument\n");
    exit(EXIT_FAILURE);
  }

  /*Alloue de la mémoire pour les fichiers à traiter
   */
  FILE* binFile[numberFiles];
  for (i = 0; i < numberFiles; i++){
    binFile[i] = (FILE*)malloc(sizeof(FILE));
  }
  /*Alloue de la mémoire pour les noms des fichiers
   */
  char* fileName[numberFiles];
  for (i = 0; i < numberFiles; i++){
    fileName[i] = (char*)malloc(30*sizeof(char));
  }

  /*Lis les valeurs passées en argument
   */
  int j = 0;
  for (i = 0; i < argc; i++){
    const char* str = argv[i];

    /*Vérifie le nombre de threads
     */
    if (strcmp(argv[i],"-t") == 0){
      n_threads = atoi(argv[i+1]);
    }
    /*Vérifie le mode voyelle/consonne
     */
    if (strcmp(argv[i],"-c") == 0){
      option = CONSONANTS;
    }
    /*Vérifie la sortie du programme
     */
    if (strcmp(argv[i],"-o") ==  0){
      output = OUT_FILE;
      outFile = argv[i+1];
    }
    /*Vérifie les noms des fichiers et les stocke dans
     *fileName
     */
    if (strstr(str,".bin") != NULL){
      if (strlen(str) > 30){
        printf("Nom de fichier trop long");
        exit(EXIT_FAILURE);
      }
      strcpy(fileName[j],str);
      j++;
    }
  }

  /*Initialisation de la pile
   */
  Stack* s;
  s = (Stack*)malloc(sizeof(Stack));
  create(s);

  pthread_t thread[n_threads];
  arg_thread *arg_t[n_threads];

  err = pthread_mutex_init(&mutex_stack, NULL);
  if(err != 0){
    printf("Erreur initialisation mutex");
    exit(EXIT_FAILURE);
  }

  err = pthread_mutex_init(&mutex_file, NULL);
  if(err != 0){
    printf("Erreur initialisation mutex");
    exit(EXIT_FAILURE);
  }

  /*Allocation de mémoire et initialisation pour
   *les arguments des threads
   */
  for (i = 0; i < n_threads; i++){
    arg_t[i]=(arg_thread*)malloc(sizeof(arg_thread));
    if(arg_t[i] == NULL){
      exit(EXIT_FAILURE);
    }
    arg_t[i]->option = option;
    arg_t[i]->number = i+1;
    arg_t[i]->stack = s;
  }

  /*Analyse des différents fichiers
   */
  int a;
  for (a = 0; a < numberFiles; a++){

    /*Ouverture du fichier à l'indice a
     */
    printf("Analyse du fichier %i\n",(a+1));
    binFile[a] = fopen(fileName[a],"rb");
    if(binFile[a] == NULL){
      printf("Erreur ouverture fichier\n");
      exit(EXIT_FAILURE);
    }

    /*Vérification de la validité du fichier
     */
    fseek(binFile[a], 0L, SEEK_END);
    int size = ftell(binFile[a]);
    rewind(binFile[a]);

    if (size % 32 != 0){
      printf("Mauvais fichier en entrée");
      exit(EXIT_FAILURE);
    }

    /*Allocation de la mémoire pour le nombre de
     *mot de passe à traiter
     */
    int *numberHashes;
    numberHashes = (int*)malloc(sizeof(int));
    *numberHashes= size/32;

    /*Initialisation du reste des valeurs des
     *arguments des thread
     */
    for (i = 0; i < n_threads; i++){
      arg_t[i]->file = binFile[a];
      arg_t[i]->numberHashes = numberHashes;
    }

    /*Lancement de compute pour chaque thread
     */
    for (i = 0; i < n_threads; i++){
      err = pthread_create(&(thread[i]), NULL, &compute, (void*)arg_t[i]);
      if(err != 0){
        printf("Erreur initialisation thread");
        exit(EXIT_FAILURE);
      }
    }

    /*Jonction des threads
     */
    for (i = 0; i < n_threads; i++){
      err = pthread_join(thread[i], NULL);
      if(err != 0){
        printf("Erreur jonction thread");
        exit(EXIT_FAILURE);
      }
    }
    /*Fermeture du fichier
     */
    fclose(binFile[a]);
  }

  err = pthread_mutex_destroy(&mutex_stack);
  if(err != 0){
    printf("Erreur destruction mutex");
    exit(EXIT_FAILURE);
  }

  err = pthread_mutex_destroy(&mutex_file);
  if(err != 0){
    printf("Erreur destruction mutex");
    exit(EXIT_FAILURE);
  }

  for (i = 0; i < numberFiles; i++){
    free(binFile[i]);
  }

  for (i = 0; i < numberFiles; i++){
    free(fileName[i]);
  }

  if (output == OUT_FILE){
    printf("Les mots de passe candidats sont imprimés sur le fichier %s\n",outFile);
    printAllFile(s,outFile);
  }

  /*Pour le terminal
   */
  else {
    printf("Les mots de passe candidats sont :\n");
    printAll(s);
  }

  /*Calcul du temps d'exécution
   */
  time_t end = time(NULL);
  printf("Temps d'exécution pour %i threads : %ld secondes\n",n_threads,(end-begin));
  return EXIT_SUCCESS;
}

/*Exécute le traitement des mots de passes contenus
 *dans les fichiers passés en arguments et garde les candidats
 *stockés dans la pile s
 */
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

    // Section critique
    pthread_mutex_lock(&mutex_file);
    printf("Thread %i : Nombre de hash restant : %i\n",arg_t->number,*(arg_t->numberHashes));
    if(*(arg_t->numberHashes) > 0){
      *(arg_t->numberHashes) = *(arg_t->numberHashes)-1;
      fread(buffer,sizeof(buffer),1,arg_t->file);
      flag1 = true;
    }
    pthread_mutex_unlock(&mutex_file);
    // Fin de la section critique

    if(!flag1){
      break;
    }

    int i;
    for (i =0; i<32; i++){
      hash[i] = (uint8_t)buffer[i];
    }

    bool flag2 = reversehash(hash,res,len);

    if(flag2){
      // Section critique
      pthread_mutex_lock(&mutex_stack);
      place(arg_t->stack,res,arg_t->option);
      pthread_mutex_unlock(&mutex_stack);
      // Fin de la section critique
    }
  }
  return NULL;
}
