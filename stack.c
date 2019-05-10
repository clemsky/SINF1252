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

/*Initialisation d'une pile de mdp
 *avec comme arguments la taille de la pile(size),
 *le premier noeud de la pile(top),
 *le nmbr max de voyelle/consonne(max_vowels/max_consonants)
 */
void create(Stack *s) {
    s->size = 0;
    s->top = NULL;
    s->max_vowels = 0;
    s->max_consonants = 0;
}

/*IN  : Une pile *s, un mdp *pswrd, une option(Voyelle ou Consonne)
 *OUT : Modifie une pile.
 *      Si le mdp passé en argument contient autant de option(voyelle ou consonne)
 *      que les autres mdp de la pile, ajoute le mdp à la pile.
 *      Si le mdp passé en argument contient plus de option(voyelle ou consonne)
 *      que les autres mdp de la pile, nettoie la pile et place le mdp dedans.
 *      Si le mdp passé en argument contient moins de option(voyelle ou consonne)
 *      que les autres mdp de la pile, rien ne se passe.
 */
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

/*Rajoute le mdp(password) au dessus de la pile(s)
 */
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
/*Retourne l'élément top de la pile et le retire de la pile
 */
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

/*Vide la pile s et en recréé une
 */
void removeAll(Stack *s){
  while(s->top != NULL){
    pop(s);
  }
  create(s);
}

/*Print tout les éléments d'une pile s
 */
void printAll(Stack*s){
  while(s->top != NULL){
    printf("%s\n", pop(s));
  }
  free(s);
}

/*IN  : pile s, fichier (fileName)
 *OUT : écrit le contenu de la pile s dans le fichier fileName
 */
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
