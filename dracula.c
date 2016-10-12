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
   giveMeTheTrail(gameState, PLAYER_DRACULA, trail);

   // random version
   /*
   int hide = 0, db = 0,i;

   for (i=0; i<TRAIL_SIZE; i++) {
      if (trail[i] == HIDE) hide = 1;
      if (trail[i]<=DOUBLE_BACK_5 && trail[i]>=DOUBLE_BACK_1) db = 1;
   }

   
   if(giveMeTheRound(gameState) == 0){
      registerBestPlay("SA","I want to be as far away from Geneva as possible!");
   } else {
      if (numAdjacent) {
         registerBestPlay(idToAbbrev(adjacent[(int)rand() % numAdjacent]),"moving randomly");
      } else {
         if (hide) {
            if (db) {
               registerBestPlay("TP","tp");
            } else {
               registerBestPlay("D1","db");
            }
         } else {
            registerBestPlay("HI","hide");
         }
      }
   }
   return;
   */

   // Logically moving version
   Map M = newMap();

   //printf("numAdjacent: %d", numAdjacent);
   if(giveMeTheRound(gameState) == 0){
      registerBestPlay("DC","I want to be as far away from Geneva as possible!");
   } else {
      int distance [numAdjacent][4];
      all_distance(M,gameState,numAdjacent,adjacent,distance);
      /*int i;
      for (i = 0; i < numAdjacent; i++) {
         printf ("ab = %d, %s\n", adjacent[i], idToAbbrev(adjacent[i]));
      }*/
      registerBestPlay(idToAbbrev(best_nextPlace(distance,numAdjacent,trail,adjacent)),"far awayyyy");
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
   int hide = 0, db = 0;
   for (i=0; i<TRAIL_SIZE-1; i++) {
      if (trail[i] == HIDE) hide = 1;
      if (trail[i]<=DOUBLE_BACK_5 && trail[i]>=DOUBLE_BACK_1) db = 1;
   }
   // find shortest distance for each place
   for(i=0; i<numAdjacent; i++){
      temp[i] = distance[i][0];
      for(j=1; j<4; j++){
         if(distance[i][j] < temp[i]) temp[i] = distance[i][j];
      }
      min[i] = i;
   }
   // sort places by distance
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
   // find normal places
   for (i=0; i<numAdjacent; i++) {
      flag = 1;
      for (j=0; j<TRAIL_SIZE-1; j++) {
         if (trail[j] == adjacent[min[i]]) {
            flag = 0;
         }
      }
      if (flag) return adjacent[min[i]];
   }
   // find special places
   for (i=0; i<numAdjacent; i++) {
      flag = 1;
      for (j=0; j<TRAIL_SIZE-1; j++) {
         if (trail[j] == adjacent[min[i]]) {
            if (j) {
               if (!db) {
                  switch (j) {
                     case 1: return abbrevToID("D2");
                     case 2: return abbrevToID("D3");
                     case 3: return abbrevToID("D4");
                     case 4: return abbrevToID("D5");
                  }
               }
            } else {
               if ((!hide) && isLand(trail[j])) {
                  return abbrevToID("HI");
               } else {
                  if (!db) {
                     return abbrevToID("D1");
                  }
               }
            }
         }

      }
      if (flag) return adjacent[min[i]];
   }


   return NOWHERE;
}
