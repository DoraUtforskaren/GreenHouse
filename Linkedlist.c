#include "my_lib.c"


char insertFirst(node **first, node *el) {
  //llsize++;
  if (el != NULL){
    node *temp;
    temp = *first;
    if(temp == NULL){ //If list is empty
      (*first) = el;
      el->next = NULL;
    }
    else{
      el->next = (*first);
      (*first)= el;
    }
  }
  else{
    return 0; //No memory left
  }
  return 1;
}

void freeMem(node **first) {
  if (*first == NULL) { // list is empty
    printf("Memory is already empty");
  } else {
    node *temp;
    while (*first != NULL) {
      temp = *first;
      *first = (*first)->next;
      free(temp); // freeing is converting the value of temp to void
    }
  }
}

void freeMem_stats(stats *first) {
  if (first == NULL) { // list is empty
    printf("Memory is already empty");
  } else {
    stats *temp;
    while (first != NULL) {
      temp = first;
      first = (first)->next;
      free(temp); // freeing is converting the value of temp to void
    }
  }
}

void removeLast(node **first){
  //llsize--;
  node *previous; 
  node *current;
  current = *first;
  if (*first == NULL){
    return;
  }
  while(current->next != NULL){
    previous = current;
    current = current->next;
  }
  previous->next = NULL;
  freeMem(&current);
}

void printList(node *n) {
  while (n != NULL) {

    printf("Temp: %f \n", n->recorded_temperature);
    printf("Minute: %d \n", n->timeWhenRecorded.minute);
    printf("Day: %d \n", n->timeWhenRecorded.day);
    n = n->next;
  }
  printf("\n");
  return;
}
