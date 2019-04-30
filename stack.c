#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>


typedef struct node {
    const u_int8_t* hash;
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

void push(Stack *s, const u_int8_t hash) {
    Node *new;

    new = (Node *)malloc(sizeof(Node));

    if (new == NULL) {
        exit(1);
    }

    new->hash = hash;
    new->next = s->top;
    s->top = new;

    s->size++;
}

const u_int8_t pop(Stack *s) {

    const u_int8_t hash;
    Node *rmv;

    //if (s->top == NULL) {
        //exit(1);
    //}

    rmv = s->top;
    hash = s->top->hash;
    s->top = rmv->next;
    s->size--;
    free(rmv);

    return hash; // return the value that was popped off the stack
}
