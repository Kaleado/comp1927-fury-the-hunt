// queue.h ... interface to simple Queue of Strings
// Written by John Shepherd, September 2015

#ifndef QUEUE_H
#define QUEUE_H

#include "Places.h"
#include "Map.h"
#include "Globals.h"

typedef struct Node *Link;

typedef struct Node {
  LocationID val;
  Link  next;
} Node;
	
typedef struct QueueRep *Queue;

typedef struct QueueRep {
  Link  front;
  Link  back;
} QueueRep;


// Function signatures

Queue newQueue();
void disposeQueue(Queue);
void enterQueue(Queue, LocationID);
LocationID leaveQueue(Queue);
int  emptyQueue(Queue);
void showQueue(Queue q);
int lengthQueue(Queue q);
LocationID getAtIndexQueue(Queue q, int index);

#endif
