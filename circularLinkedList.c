#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>

#include "circularLinkedList.h"

typedef struct circularLinkedList{
  Node *head;
} circularLinkedList;

typedef struct node{
  char* password;
  int n_vowels;
  int n_consonants;
  struct node *next;
  struct node *previous;
} Node;

void create(circularLinkedList* new){
  new->head = NULL;
}

int add(circularLinkedList *list, char* s){

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

  Node* new = (Node*)malloc(sizeof(Node));
  if(new == NULL){
    return 1;
  }

  new->password = s;
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

void remove(circularLinkedList* list, Node* rmv){
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

void printAll(circularLinkedList* list){
  while(list->head != NULL){
    printf("%s\n",list->head->password);
    remove(list, list->head);
  }
}
