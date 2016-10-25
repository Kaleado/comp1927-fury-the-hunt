// set.h ... interface to simple Set of Strings
// Written by John Shepherd, September 2015

#ifndef SET_H
#define SET_H

#include "Places.h"

typedef struct SetRep *Set;

// Function signatures

Set newSet();
void disposeSet(Set);
void insertInto(Set,LocationID);
void dropFrom(Set,LocationID);
int  isElem(Set,LocationID);
int  nElems(Set);
void showSet(Set);
Set intersection(Set, Set);
LocationID* toArray(Set, int*);

#endif
