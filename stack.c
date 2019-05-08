#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>

#include "stack.h"

void create(Stack *s) {
    s->size = 0;
    s->top = NULL;
    s->max_vowels = 0;
    s->max_consonants = 0;
}

void place(Stack *s, char*password, int option){
  int i = 0;
  int vowels = 0;
  int consonants = 0;

  while(s[i] != '\0'){
    if (s[c] == 'a'|| s[c] == 'e' || s[c] == 'i' || s[c] =='o' || s[c] == 'u' || s[c] == 'y'){
      vowels++;
    }
    else{
      consonants++;
    }
  }

  if (option == VOWELS){
    if(vowels == max_vowels){
      push(s,password);
    }
    else if(vowels < max_vowels){
      free(password);
    }
    else if(vowels > max_vowels){
      removeAll(s);
      push(s,password);
    }
  }

  if (option == CONSONANTS){
    if(consonants == max_consonants){
      push(s,password);
    }
    else if(consonants < max_consonants){
      free(password);
    }
    else if(consonants > max_consonants){
      removeAll(s);
      push(s,password);
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
        exit(0);
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
}

void printAll(Stack*s){
  while(s->top != NULL){
    printf("%s\n", pop(s));
  }
}
