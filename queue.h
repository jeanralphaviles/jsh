#ifndef QUEUE_H
#define QUEUE_H

struct q_node {
  struct q_node *prev;
  struct q_node *next;
  void *data;
};

// Assumes ownership of elements enqueued
void enqueue(struct q_node*, void*);
void dequeue(struct q_node*);
// Retains ownership of elements returned
void* front(struct q_node*);
int size(struct q_node*);

#endif
