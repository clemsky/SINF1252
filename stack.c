#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>

int* max_vowels = 0;
int* max_consonants = 0;

typedef struct node{
  char* password;
  struct node *next;
} Node;

typedef struct stack {
    Node *top;
    int size;
} Stack;

void create(Stack *s) {
    s->size = 0;
    s->top = NULL;
}

void place(Stack *s, char*password){
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

  if(vowels >= max_vowels){
    push(s,password);
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

void printAll(Stack*s){
  while(s->top != NULL){
    printf("%s\n", pop(s));
  }
}
