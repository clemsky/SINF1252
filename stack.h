#ifndef _STACK_H
#define _STACK_H

typedef struct node{
  char* password;
  struct node *next;
} Node;

typedef struct stack {
    Node *top;
    int size;
    int max_vowels;
    int max_consonants;
} Stack;

void create(Stack *s);

void place(Stack *s, char*password, int option);

void push(Stack *s, char* password);

char* pop(Stack *s);

void removeAll(Stack *s);

void printAll(Stack* s);

void printAllFile(Stack* s, char* fileName);

#endif
