// dracula.c
// Implementation of your "Fury of Dracula" Dracula AI

#include <stdlib.h>
#include <stdio.h>
#include "Game.h"
#include "DracView.h"
#include <time.h>


void decideDraculaMove(DracView gameState)
{
  srand(time(NULL));
  int numAdjacent;
  LocationID* adjacent = whereCanIgo(gameState, &numAdjacent, 1, 1);
  //printf("numAdjacent: %d", numAdjacent);
  if(giveMeTheRound(gameState) == 0){
    registerBestPlay("AT","I want to be as far away from Geneva as possible!");
  }
  else {
    registerBestPlay(idToAbbrev(adjacent[rand() % numAdjacent]),"");
  }
}
