#ifndef Linkedlist_H
#define Linkedlist_H

typedef struct node node;

struct node{
  float recorded_temperature;
  recorded_time timeWhenRecorded;
  struct node *next;
};

char insertFirst(node **first, node *el);

void freeMem(node **first);


node createNew(float temperature);
void removeLast(node **first);
void printList(node *n);



#endif