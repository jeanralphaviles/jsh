#ifndef QUEUE_H
#define QUEUE_H

struct q_node {
  struct q_node *prev;
  struct q_node *next;
  void *data;
};

struct queue {
  struct q_node* head;
};

// --Queue Functions--
void enqueue(struct queue*, void*);
void dequeue(struct queue*);
void* front(struct queue*);
int size(struct queue*);
struct queue* createQueue();

// --Queue Node Functions--
void enqueueQnode(struct q_node*, void*); // Assumes ownership of elements enqueued
struct q_node* dequeueQnode(struct q_node*); // Returns new head
void* frontQnode(struct q_node*); // Retains ownership of elements returned
int sizeQnode(struct q_node*);
struct q_node* createQueueNode(void*); // Retains ownership of arugment

#endif
