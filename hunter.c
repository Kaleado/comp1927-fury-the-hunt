// hunter.c
// Implementation of your "Fury of Dracula" hunter AI

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "assert.h"
#include "Game.h"
#include "Map.h"
#include "HunterView.h"

void decideHunterMove(HunterView gameState){
  LocationID currentPath[NUM_MAP_LOCATIONS];
  srand(time(NULL));
  int numAdjacent;
  int pathLength;
  int i;
  int firstValid = -1;
  int nextMoveInPath = 0;
  LocationID* adjacent = whereCanIgo(gameState, &numAdjacent, 1, 1, 1);
  //printf("numAdjacent: %d", numAdjacent);
  if(giveMeTheRound(gameState) == 0){
      registerBestPlay("AT","I want to be as far away from Geneva as possible!");
  }
  else {
    //We update the path every three rounds.
    if(giveMeTheRound(gameState) % 3 == 0){
      LocationID trail[TRAIL_SIZE];
      giveMeTheTrail(gameState, PLAYER_DRACULA, trail);
      for(i = 0; trail[i] != -1; i++){
	if(trail[i] >= MIN_MAP_LOCATION && trail[i] <= MAX_MAP_LOCATION){
	  firstValid = trail[i];
	  break;
	}
      }
      if(firstValid != -1){
	pathLength = findMeAPath(gameState, firstValid, currentPath);
	nextMoveInPath = 0;
      }
    }
    if(nextMoveInPath < pathLength){
      registerBestPlay(idToAbbrev(currentPath[nextMoveInPath]),"moving to next move in path");
      nextMoveInPath++;
    }
    else {
      registerBestPlay(idToAbbrev(adjacent[rand() % numAdjacent]),"");
    }
  }
}
