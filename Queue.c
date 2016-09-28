// queue.c ... simple Queue of Strings
// Written by John Shepherd, September 2015

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "Queue.h"


// Function signatures

Queue newQueue();
void disposeQueue(Queue);
void enterQueue(Queue, LocationID);
LocationID leaveQueue(Queue);
int  emptyQueue(Queue);
void showQueue(Queue q);
int lengthQueue(Queue q);
LocationID getAtIndexQueue(Queue q, int index);

static Link newNode(LocationID);
static void disposeNode(Link);


// newQueue()
// - create an initially empty Queue
Queue newQueue()
{
  Queue new = malloc(sizeof(QueueRep));
  assert(new != NULL);
  new->front = NULL;
  new->back = NULL;
  return new;
}

// disposeQueue(Queue)
// - clean up memory associated with Queue
void disposeQueue(Queue q)
{
  if (q == NULL) return;
  Link next, curr = q->front;
  while (curr != NULL) {
    next = curr->next;
    disposeNode(curr);	
    curr = next;
  }
}

// enterQueue(Queue,Str)
// - add Str to back of Queue
void enterQueue(Queue q, LocationID loc)
{
  Link new = newNode(loc);
  if (q->front == NULL){
    q->back = new;
    q->front = new;
  }
  else {
    // add into list of elems
    q->back->next = new;
    q->back = new;
  }
}

// leaveQueue(Queue)
// - return string at front of Queue
LocationID leaveQueue(Queue q)
{
  assert (q->front != NULL);
  LocationID loc = q->front->val;
  Link old = q->front;
  q->front = old->next;
  if (q->front == NULL)
    q->back = NULL;
  free(old);
  return loc;
}

int lengthQueue(Queue q){
  int len;
  Link curr = q->front;
  
  for(len = 0; curr != NULL; curr = curr->next){  
    len++;
  }
  return len;
}

// emptyQueue(Queue)
// - check whether Queue is empty
int emptyQueue(Queue q)
{
  return (q->front == NULL);
}

// showQueue(Queue)
// - display Queue (for debugging)
void showQueue(Queue q)
{
  Link curr;
  if (q->front == NULL)
    printf("Queue is empty\n");
  else {
    printf("Queue (front-to-back):\n");
    int id = 0;
    curr = q->front;
    while (curr != NULL) {
      printf("[%03d] %d\n", id, curr->val);
      id++;
      curr = curr->next;
    }
  }
}

LocationID getAtIndexQueue(Queue q, int index){
  int i = 0;
  Link curr = NULL;
  for(i = 0; i < index; i++){
    curr = curr->next;
  }
  return curr->val;
}

// Helper functions

static Link newNode(LocationID loc)
{
  Link new = malloc(sizeof(Node));
  assert(new != NULL);
  new->val = loc;
  new->next = NULL;
  return new;
}

static void disposeNode(Link curr)
{
  assert(curr != NULL);
  free(curr);
}

