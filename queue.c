#include "queue.h"

#include <stdio.h>
#include <stdlib.h>

// Queue Functions
void enqueue(struct queue* q, void* data) {
  if (q->head == NULL) {
    q->head = createQueueNode(data);
  } else {
    enqueueQnode(q->head, data);
  }
}

void dequeue(struct queue* q) {
  if (q->head == NULL) {
    fprintf(stderr, "Dequeue from Empty Queue\n");
  } else {
    q->head = dequeueQnode(q->head);
  }
}

void pop_back(struct queue* q) {
  if (size(q) == 0) {
    fprintf(stderr, "Popback from empty queue\n");
  } else if (size(q) == 1) {
    dequeue(q);
  } else {
    removeLastNode(q->head);
  }
}

void* front(struct queue* q) {
  if (q->head == NULL) {
    fprintf(stderr, "Getting element from Empty Queue");
    return NULL;
  } else {
    return frontQnode(q->head);
  }
}

void* back(struct queue* q) {
  if (q->head == NULL) {
    fprintf(stderr, "Getting element from Empty Queue");
    return NULL;
  } else {
    return lastQnode(q->head);
  }
}

int size(struct queue* q) {
  if (q->head == NULL) {
    return 0;
  } else {
    return sizeQnode(q->head);
  }
}

struct queue* createQueue() {
  struct queue* q = (struct queue*)malloc(sizeof(struct queue*));
  q->head = NULL;
  return q;
}

// --Queue Node Functions--
void enqueueQnode(struct q_node *q_head, void *data) {
  struct q_node *curr_node = q_head;

  while(curr_node->next != NULL) { 
    curr_node = curr_node->next;
  }
  struct q_node* new_node = (struct q_node*)malloc(sizeof(struct q_node));
  new_node->prev = curr_node;
  new_node->next = NULL;
  new_node->data = data;

  curr_node->next = new_node;
}

struct q_node* dequeueQnode(struct q_node *q_head) {
  struct q_node* newHead = q_head->next;
  /*free(q_head->data);*/
  free(q_head);
  return newHead;
}

void removeLastNode(struct q_node* q_head) {
  while(q_head->next != NULL) {
    q_head = q_head->next;
  }
  q_head = q_head->prev;
  free(q_head->next);
  q_head->next = NULL;
}

void* frontQnode(struct q_node *q_head) {
  return q_head->data;
}

void* lastQnode(struct q_node *q_head) {
  while (q_head->next != NULL) {
    q_head = q_head->next;
  }
  return q_head->data;
}

int sizeQnode(struct q_node *q_head) {
  int size = 1;
  while((q_head = q_head->next) != NULL) {
    ++size;
  }
  return size;
}

struct q_node* createQueueNode(void* data) {
  struct q_node* newNode = (struct q_node*)malloc(sizeof(struct q_node));
  newNode->data = data;
  newNode->prev = NULL;
  newNode->next = NULL;
  return newNode;
}
