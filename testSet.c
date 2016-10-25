// hunter.c
// Implementation of your "Fury of Dracula" hunter AI

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include "assert.h"
#include "Game.h"
#include "Map.h"
#include "Set.h"

void printAsArray(Set a){
  int sA, i;
  LocationID* arrA = malloc(nElems(a)*sizeof(LocationID));
  arrA = toArray(a, &sA);
  printf("Printing set A as an array: [");
  for(i = 0; i < sA; i++){
    printf("%d, ", arrA[i]);
  }
  printf("]\n");
  assert(sA == nElems(a));
  free(arrA);
  return;
}

int main(){
  int i;

  //We test the toArray function.
  Set a = newSet();
  printAsArray(a);
  insertInto(a, 0);
  showSet(a);
  printAsArray(a);
  printf("Inserting some more elements...\n");
  for(i = 0; i < 16; i++){
    insertInto(a, i);
  }
  printAsArray(a);
  disposeSet(a);
  
  //We test the intersection function.
  a = newSet();
  Set b = newSet();
  insertInto(a, 0);
  insertInto(b, 0);
  Set c = intersection(a, b);
  showSet(c);
  assert(nElems(c) == 1);
  insertInto(b,1);
  disposeSet(c);
  c = intersection(a,b);
  assert(nElems(c) == 1);
	    
  return 0;
}
