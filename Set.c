// set.c ... simple, inefficient Set of Strings
// Written by John Shepherd, September 2015

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "Set.h"

#define strEQ(s,t) (s == t)
#define strLT(s,t) (s < t)

typedef struct Node *Link;

typedef struct Node {
	LocationID val;
	Link  next;
} Node;
	
typedef struct SetRep {
	int   nelems;
	Link  elems;
} SetRep;

// Function signatures

Set newSet();
void disposeSet(Set);
void insertInto(Set,LocationID);
void dropFrom(Set,LocationID);
int  isElem(Set,LocationID);
int  nElems(Set);
LocationID* toArray(Set, int*);
Set intersection(Set, Set);

static Link newNode(LocationID);
static void disposeNode(Link);
static int  findNode(Link,LocationID,Link *,Link *);


// newSet()
// - create an initially empty Set
Set newSet()
{
	Set new = malloc(sizeof(SetRep));
	assert(new != NULL);
	new->nelems = 0;
	new->elems = NULL;
	return new;
}

// disposeSet(Set)
// - clean up memory associated with Set
void disposeSet(Set s)
{
	if (s == NULL) return;
	Link next, curr = s->elems;
	while (curr != NULL) {
		next = curr->next;
		disposeNode(curr);	
		curr = next;
	}
}

// insertInto(Set,Str)
// - ensure that Str is in Set
void insertInto(Set s, LocationID str)
{
	assert(s != NULL);
	Link curr, prev;
	int found = findNode(s->elems,str,&curr,&prev);
	if (found) return; // already in Set
	Link new = newNode(str);
	s->nelems++;
	if (prev == NULL) {
		// add at start of list of elems
		new->next = s->elems;
		s->elems = new;
	}
	else {
		// add into list of elems
		new->next = prev->next;
		prev->next = new;
	}
}

// dropFrom(Set,Str)
// - ensure that Str is not in Set
void dropFrom(Set s, LocationID str)
{
	assert(s != NULL);
	Link curr, prev;
	int found = findNode(s->elems,str,&curr,&prev);
	if (!found) return;
	s->nelems--;
	if (prev == NULL)
		s->elems = curr->next;
	else
		prev->next = curr->next;
	disposeNode(curr);
}

// isElem(Set,Str)
// - check whether Str is contained in Set
int isElem(Set s, LocationID str)
{
	assert(s != NULL);
	Link curr, prev;
	return findNode(s->elems,str,&curr,&prev);
}

// nElems(Set)
// - return # elements in Set
int  nElems(Set s)
{
	assert(s != NULL);
	return s->nelems;
}

//Converts a set to an array.
LocationID* toArray(Set s, int* size){
  if(s->nelems == 0){
    if(size != NULL){
      *size = 0;
    }
    return NULL;//nothing to do here
  }
  LocationID* arr = malloc(s->nelems*sizeof(LocationID));
  Link curr;
  int i = 0;
  for(curr = s->elems; curr != NULL; curr = curr->next){
    arr[i] = curr->val;
    i++;
  }
  if(size != NULL){
    *size = i;
  }
  return arr;
}

//Find the intersection between two sets.
Set intersection(Set a, Set b){
  Set c = newSet();
  int sizeOfA, i;
  LocationID* elementsOfA = toArray(a, &sizeOfA);
  for(i = 0; i < sizeOfA; i++){
    if(isElem(b, elementsOfA[i]) == 1){
      insertInto(c, elementsOfA[i]);
    }
  }
  free( (void*)elementsOfA );
  return c;
}

// showSet(Set)
// - display Set (for debugging)
void showSet(Set s)
{
	Link curr;
	if (s->nelems == 0)
		printf("Set is empty\n");
	else {
		printf("Set has %d elements:\n",s->nelems);
		int id = 0;
		curr = s->elems;
		while (curr != NULL) {
		  printf("[%03d] %d\n", id, curr->val);
		  //printf("[%03d] %s\n", id, idToAbbrev(curr->val));
			id++;
			curr = curr->next;
		}
	}
}

// Helper functions

static Link newNode(LocationID str)
{
	Link new = malloc(sizeof(Node));
	assert(new != NULL);
	new->val = str;
	new->next = NULL;
	return new;
}

static void disposeNode(Link curr)
{
	assert(curr != NULL);
	free(curr);
}

// findNode(L,Str)
// - finds where Str could be added into L
// - if already in L, curr->val == Str
// - if not already in L, curr and prev indicate where to insert
// - return value indicates whether Str found or not
static int findNode(Link list, LocationID str, Link *cur, Link *pre)
{
	Link curr = list, prev = NULL;
	while (curr != NULL && strLT(curr->val,str)) {
		prev = curr;
		curr = curr->next;
	}
	*cur = curr; *pre = prev;
	return (curr != NULL && strEQ(str,curr->val));
}
