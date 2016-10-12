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
  PlayerID me = whoAmI(gameState);
  char* here = idToAbbrev(whereIs(gameState, me));
  LocationID currentPath[NUM_MAP_LOCATIONS];
  srand(time(NULL));
  int numAdjacent;
  int pathLength = 0;
  int i;
  int firstValid = -1;
  int nextMoveInPath = 0;
  LocationID* adjacent = whereCanIgo(gameState, &numAdjacent, 1, 0, 0);
  //printf("numAdjacent: %d", numAdjacent);
  if(giveMeTheRound(gameState) == 0){
    switch(me){
    case PLAYER_LORD_GODALMING:
      registerBestPlay("ST","Lord Godalming: Strasbourg");
      break;
    case PLAYER_DR_SEWARD:
      registerBestPlay("MA","Dr. Seward: Madrid");
      break;
    case PLAYER_VAN_HELSING:
      registerBestPlay("SZ","Van Helsing: Szeged");
      break;
    case PLAYER_MINA_HARKER:
      registerBestPlay("SO","Mina Harker: Sofia");
      break;
    }
  }
  else if(giveMeTheRound(gameState) % 8 == 1){
    //We conduct research to find where Dracula is.
    registerBestPlay(here, "researching");
  }
  else {
    //We update the path every three rounds.
    if(giveMeTheRound(gameState) % 1 == 0){
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
	nextMoveInPath = 1;
      }
    }
    if(nextMoveInPath < pathLength){
      registerBestPlay(idToAbbrev(currentPath[nextMoveInPath]),"moving to next move in path");
      nextMoveInPath++;
    }
    else {
      if(numAdjacent == 0){
	registerBestPlay(idToAbbrev(whereIs(gameState, me)), "waiting");
      }
      else {
	registerBestPlay(idToAbbrev(adjacent[rand() % numAdjacent]),"moving randomly");
      }
    }
  }
}
