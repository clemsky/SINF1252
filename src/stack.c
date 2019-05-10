#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include <pthread.h>
#include <semaphore.h>

#include "stack.h"
#include "main.h"

void create(Stack *s) {
    s->size = 0;
    s->top = NULL;
    s->max_vowels = 0;
    s->max_consonants = 0;
}

void place(Stack *s, char*password, int option){
  int vowels = 0;
  int consonants = 0;

  int i = 0;
  while(password[i] != '\0'){
    if (password[i] == 'a'|| password[i] == 'e' || password[i] == 'i' || password[i] =='o' || password[i] == 'u' || password[i] == 'y'){
      vowels++;
    }
    else{
      consonants++;
    }
    i++;
  }

  if (option == VOWELS){
    if(vowels == s->max_vowels){
      push(s,password);
    }
    else if(vowels > s->max_vowels){
      removeAll(s);
      push(s,password);
      s->max_vowels = vowels;
    }
  }

  if (option == CONSONANTS){
    if(consonants == s->max_consonants){
      push(s,password);
    }
    else if(consonants > s->max_consonants){
      removeAll(s);
      push(s,password);
      s->max_consonants = consonants;
    }
  }
}

void push(Stack *s, char* password) {
    Node *new;
    new = (Node *)malloc(sizeof(Node));

    if (new == NULL) {
        exit(1);
    }

    new->password = password;
    new->next = s->top;
    s->top = new;
    s->size++;
}

char* pop(Stack *s) {

    char* password;
    Node *rmv;

    if (s->top == NULL) {
        exit(1);
    }

    rmv = s->top;
    password = s->top->password;
    s->top = rmv->next;
    s->size--;
    free(rmv);

    return password;
}

void removeAll(Stack *s){
  while(s->top != NULL){
    pop(s);
  }
  create(s);
}

void printAll(Stack*s){
  while(s->top != NULL){
    printf("%s\n", pop(s));
  }
  free(s);
}

void printAllFile(Stack*s, char* fileName){
  FILE* file;
  file = fopen(fileName,"w");
  while(s->top != NULL){
    char* ptr = pop(s);
    fwrite(ptr,1,sizeof(ptr),file);
  }
  fclose(file);
  free(s);
}
