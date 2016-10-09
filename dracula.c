// dracula.c
// Implementation of your "Fury of Dracula" Dracula AI

#include <stdlib.h>
#include <stdio.h>
#include "Game.h"
#include "DracView.h"
#include <time.h>
#include "Map.h"

static void all_distance(Map g, DracView gameState, int numLocations, LocationID *Next_Place, int distance[][4]);
static int best_nextPlace(int distance[][4], int numAdjacent);

void decideDraculaMove(DracView gameState)
{
  //srand(time(NULL));
  int numAdjacent;
  LocationID* adjacent = whereCanIgo(gameState, &numAdjacent, 1, 1);
  Map M = newMap();

  //printf("numAdjacent: %d", numAdjacent);
  if(giveMeTheRound(gameState) == 0){
    registerBestPlay("AT","I want to be as far away from Geneva as possible!");
  }
  else {
    if(numAdjacent == 0){
      registerBestPlay("D1","");
    }
    else {
      int distance [numAdjacent][4];
      all_distance(M,gameState,numAdjacent,adjacent,distance);
      
      registerBestPlay(idToAbbrev(best_nextPlace(distance,numAdjacent)),"");
    }
  }
}


static void all_distance(Map g, DracView gameState, int numLocations, LocationID *Next_Place, int distance[][4]){
	int i = 0;
	LocationID Hunter_LG = whereIs(gameState, PLAYER_LORD_GODALMING);
	LocationID Hunter_DS = whereIs(gameState, PLAYER_DR_SEWARD);
	LocationID Hunter_VH = whereIs(gameState, PLAYER_VAN_HELSING);
	LocationID Hunter_MH = whereIs(gameState, PLAYER_MINA_HARKER);
	for(i=0; i<numLocations; i++){
		distance[i][0] = getlen(g,Next_Place[i],Hunter_LG);
		distance[i][1] = getlen(g,Next_Place[i],Hunter_DS);
		distance[i][2] = getlen(g,Next_Place[i],Hunter_VH);
		distance[i][3] = getlen(g,Next_Place[i],Hunter_MH);
	}
}

static int best_nextPlace(int distance[][4], int numAdjacent){
  int temp[numAdjacent];
  int i = 0;
  int j = 0;

  for(i=0; i<numAdjacent; i++)  temp[i] = distance[i][0];

  for(i=0; i<numAdjacent; i++){
    for(j=0; j<4; j++){
      if(distance[i][j] < temp[i])
        temp[i] = distance[i][j];
    }
  }

  int min = temp[0];
  for(i=0; i<numAdjacent; i++){
    if(temp[i] < min) min = temp[i];
  }

  return min;
}

