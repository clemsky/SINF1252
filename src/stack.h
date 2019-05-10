#ifndef _STACK_H
#define _STACK_H

/*Structure d'un noeud contenant un mot de passe
 */
typedef struct node{
  char* password;
  struct node *next;
} Node;

/*Structure d'une pile de mot de passe
 */
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
