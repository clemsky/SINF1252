#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>

typedef struct{
  Node *head;
} CircularLinkedList;

typedef struct {
  int n_vowels;
  int n_consonants;
  Node *next;
  Node *previous;
} Node;

int add(CircularLinkedList *list, int vowels, int consonants){
  Node* new = (Node*)malloc(sizeof(Node));
  if(new == NULL){
    return 1;
  }

  new->n_vowels = vowels;
  new->n_consonants = consonants;

  if(list->head == NULL){
    list->head = new;
    new->next = new;
    new->previous = new;
  }
  else {
    Node* oldHead = list->head;
    new->next = oldHead;
    new->previous = oldHead->previous;
    oldHead->previous = new;
    if (oldHead->next == oldHead){
      oldHead->next = new;
    }
    list->head = new;
  }
  return 0;
}

void remove(CircularLinkedList* list, Node* rmv){
  if(rmv == list->head){
    if(rmv->next = rmv){
      free(rmv);
      list->head = NULL;
    }
    else{
      list->head = rmv->next;
      list->head->previous = rmv->previous;
      rmv->previous->next = list->head;
      free(rmv);
    }
  }
  else {
    rmv->previous->next = rmv->next;
    rmv->next->previous = rmv->previous;
    free(rmv);
  }
}

void printAll(CircularLinkedList* list){
  while(list->head != NULL){
    printf("%s\n", );
  }
}
