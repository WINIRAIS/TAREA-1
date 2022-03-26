#include "list.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct Node Node;

struct Node {
  void *data;
  Node *next;
  Node *prev;
};

struct List {
  Node *head;
  Node *tail;
  Node *current;
};

typedef List List;

Node *createNode(void *data) {
  Node *new = (Node *)malloc(sizeof(Node));
  assert(new != NULL);
  new->data = data;
  new->prev = NULL;
  new->next = NULL;
  return new;
}

List *createList() {
  List *lista = (List *)malloc(sizeof(lista));

  lista->head = NULL;
  lista->tail = NULL;
  lista->current = NULL;

  return lista;
}

void *firstList(List *list) {
  if ((list->current != NULL) && (list->head != NULL)) {
    list->current = list->head;
    return list->head->data;
  }
  return NULL;
}

void *nextList(List *list) {
  if ((list->current != NULL) && (list->current->next != NULL)) {
    list->current = list->current->next;
    return list->current->data;
  }
  return NULL;
}

void *lastList(List *list) {
  if ((list->current != NULL) && (list->tail != NULL)) {
    list->current = list->tail;
    return list->tail->data;
  }
  return NULL;
}

void *prevList(List *list) {
  if ((list->current != NULL) && (list->current->prev != NULL)) {
    list->current = list->current->prev;
    return list->current->data;
  }
  return NULL;
}

void pushFront(List *list, void *data) {
  Node *nodo = createNode(data);
  nodo->next = list->head;
  if (list->head != NULL) {
    list->head->prev = nodo;
  }else{
    list->head = nodo;
    list->tail = nodo;
  }
  list->head = nodo;
  nodo->prev = NULL;
}

void pushBack(List *list, void *data) {
  list->current = list->tail;
  pushCurrent(list, data);
}

void pushCurrent(List *list, void *data) {
  Node *nodo1 = createNode(data);
  if(list->current != NULL){
    list->current->next = nodo1;
  }
  list->tail = nodo1;
  nodo1->prev = list->current;
}

void *popFront(List *list) {
  list->current = list->head;
  return popCurrent(list);
}

void *popBack(List *list) {
  list->current = list->tail;
  return popCurrent(list);
}

void *popCurrent(List *list) { 
  if(list->current != NULL){
    if(list->current->prev != NULL){
      list->current->prev->next = list->current->next;
    }else{ 
        list->head = list->current->next;
      }
    if(list->current->next != NULL){
      list->current->next->prev = list->current->prev;
    }else{ 
      list->tail = list->current->prev;
      list->tail->next = NULL;
    }
  }
  return list->current->data; 
  }

void cleanList(List *list) {
  while (list->head != NULL) {
    popFront(list);
  }
}