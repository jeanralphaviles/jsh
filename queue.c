#include "queue.h"

#include <stdlib.h>

void enqueue(struct q_node *q_head, void *data) {
  struct q_node *curr_node = q_head;

  while(curr_node->next != NULL) { 
    curr_node = curr_node->next;
  }
  struct q_node new_node;
  new_node.prev = curr_node;
  new_node.next = NULL;
  new_node.data = data;

  curr_node->next = &new_node;
}

void dequeue(struct q_node *q_head) {
  q_head->next->prev = NULL;
  free(q_head->data);
  free(q_head);
}

void* front(struct q_node *q_head) {
  return q_head->data;
}

int size(struct q_node *q_head) {
  int size = 1;
  while((q_head = q_head->next) != NULL) {
    ++size;
  }
  return size;
}
