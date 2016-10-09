// dracula.c
// Implementation of your "Fury of Dracula" Dracula AI

#include <stdlib.h>
#include <stdio.h>
#include "Game.h"
#include "DracView.h"
#include <time.h>
#include "Map.h"
#include "Places.h"
 
static void all_distance(Map g, DracView gameState, int numLocations, LocationID *Next_Place, int distance[][4]);
static int best_nextPlace(int distance[][4], int numAdjacent, LocationID trail[TRAIL_SIZE], LocationID* adjacent);

void decideDraculaMove(DracView gameState)
{
   //srand(time(NULL));
   int numAdjacent;
   LocationID trail[TRAIL_SIZE];
   LocationID* adjacent = whereCanIgo(gameState, &numAdjacent, 1, 1);
   Map M = newMap();
   giveMeTheTrail(gameState, PLAYER_DRACULA, trail);

   //printf("numAdjacent: %d", numAdjacent);
   if(giveMeTheRound(gameState) == 0){
      registerBestPlay("AT","I want to be as far away from Geneva as possible!");
   } else {
      if(numAdjacent == 0){
         registerBestPlay("TP","Oh no");
      } else {
         int distance [numAdjacent][4];
         all_distance(M,gameState,numAdjacent,adjacent,distance);
         int nextID = best_nextPlace(distance,numAdjacent,trail,adjacent);
         if (nextID == NOWHERE) {
            registerBestPlay("TP","Oh no");
         } else {
            registerBestPlay(idToAbbrev(nextID),"far awayyyy");
         }
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

static int best_nextPlace(int distance[][4], int numAdjacent, LocationID trail[TRAIL_SIZE], LocationID* adjacent){
   int temp[numAdjacent];
   int min[numAdjacent];
   int i, j, t, flag;
//   int hide = 0, db = 0;
/*
   for (i=0; i<TRAIL_SIZE; i++) {
      if (trail[i] == HIDE) hide = 1;
      if (trail[i]<=DOUBLE_BACK_5 && trail[i]>=DOUBLE_BACK_1) db = 1;
   }
*/
   for(i=0; i<numAdjacent; i++){
      temp[i] = distance[i][0];
      for(j=1; j<4; j++){
         if(distance[i][j] < temp[i]) temp[i] = distance[i][j];
      }
      min[i] = i;
   }

   for (i=numAdjacent-1; i>0; i--) {
      for (j=0; j<i; j++) {
         if(temp[j] < temp[j+1]) {
            t = temp[j];
            temp[j] = temp[j+1];
            temp[j+1] = t;
            t = min[j];
            min[j] = min[j+1];
            min[j+1] = t;
         }
      }
   }
   
   for (i=0; i<numAdjacent; i++) {
      flag = 1;
      for (j=0; j<TRAIL_SIZE; j++) {
         if (trail[j] == adjacent[min[i]]) {
            flag = 0;
         }
         //printf ("\nflag = %d, trail at %d, adjacent at %d, %d\n", flag, trail[j], temp[i], numAdjacent);
      }
      if (flag) return adjacent[min[i]];
   }

   return NOWHERE;
}

