// hunter.c
// Implementation of your "Fury of Dracula" hunter AI

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include "assert.h"
#include "Game.h"
#include "Map.h"
#include "HunterView.h"
#include "Set.h"
#include "Places.h"
#include "GuessLocation.h"

int isKnown(LocationID loc){
  return (loc != CITY_UNKNOWN &&
	  loc != SEA_UNKNOWN &&
	  loc != UNKNOWN_LOCATION
	  );
}

LocationID getRandomMove(HunterView gameState){
  int player = whoAmI(gameState);
  int round = giveMeTheRound(gameState);
  int numAdjacent;
  LocationID* adjacent = whereCanIgo(gameState, &numAdjacent, 1, 0, 1);  
  return adjacent[(round + player) % numAdjacent];
}

LocationID getCoastalTown(PlayerID player){
  switch(player){
  case PLAYER_LORD_GODALMING:
    return VENICE;
  case PLAYER_DR_SEWARD:
    return NANTES;
  case PLAYER_VAN_HELSING:
    return BORDEAUX;
  case PLAYER_MINA_HARKER:
    return BARCELONA;
  }
  return -1;
}

LocationID here(HunterView gameState){
  return whereIs(gameState, whoAmI(gameState));
}

void decideHunterMove(HunterView gameState){
  PlayerID me = whoAmI(gameState);
  LocationID currentPath[NUM_MAP_LOCATIONS];
  srand(time(NULL));
  LocationID draculasWhereabouts = whereIs(gameState, PLAYER_DRACULA);
  Set possibleTrail[TRAIL_SIZE];
  char msg[50];
  LocationID trail[TRAIL_SIZE];
  int pathLength = 0;
  int round = giveMeTheRound(gameState);
  //int j;
  int i;
  giveMeTheTrail(gameState, me, trail);
  //for (j=0; j<TRAIL_SIZE; j++) printf ("*%s* ", idToAbbrev(trail[j]));
  printf ("hp = %d\n", howHealthyIs(gameState, me));
  //We must handle when there is a double back.
  while(draculasWhereabouts >= DOUBLE_BACK_1 && draculasWhereabouts <= DOUBLE_BACK_5){
    giveMeTheTrail(gameState, PLAYER_DRACULA, trail);
    draculasWhereabouts = trail[draculasWhereabouts - DOUBLE_BACK_1 + 1];
    printf("dracswherabouts: %d", draculasWhereabouts);
  }
  
  //We set the hunters' initial locations.
  if(giveMeTheRound(gameState) == 0){
    switch(me){
    case PLAYER_LORD_GODALMING:
      registerBestPlay("ST","Lord Godalming: Strasbourg");
      break;
    case PLAYER_DR_SEWARD:
      registerBestPlay("MA","Dr. Seward: Madrid");
      break;
    case PLAYER_VAN_HELSING:
      registerBestPlay("AM","Van Helsing: Manchester");
      break;
    case PLAYER_MINA_HARKER:
      registerBestPlay("CD","Mina Harker: Castle Dracula");
      break;
    }
  }
  //Our top priority is to pursue Dracula.
  else if(isKnown(draculasWhereabouts)){
    pathLength = findMeAPath(gameState, draculasWhereabouts, currentPath);
    if(pathLength <= 1){
      //If we are on top of the place we are going, wait.
      printf("On top of destination -- waiting: %d", here(gameState));
      sprintf(msg, "On top of destination -- waiting: %s", idToAbbrev(here(gameState)));
      registerBestPlay(idToAbbrev(here(gameState)), msg);
    }
    else{
      printf("Move to next move in path of length %d: %d", pathLength, currentPath[1]);
      sprintf(msg, "Move to next move in path: %s", idToAbbrev(currentPath[1]));
      registerBestPlay(idToAbbrev(currentPath[1]), msg);
    }
  }
  //We conduct research every six rounds so we have an idea of where Dracula is (granted we don't already do).
  /*else if(giveMeTheRound(gameState) % 6 == 0){
    strcpy(msg, "Waiting and conducting research.");
    registerBestPlay(idToAbbrev(here(gameState)), msg);
    }*/
  //If we don't know where Dracula is...
  else if(!isKnown(draculasWhereabouts)){
    giveMeTheTrail(gameState, PLAYER_DRACULA, trail);
    guessDraculasLocation(trail, possibleTrail);
    //We now have some guesses as to Dracula's location.
    //Now, we must step through each possible location for Dracula and send each hunter to one.
    for(i = 0; i < TRAIL_SIZE; i++){
      LocationID* potentialDestinations;
      int numberOfPotentialDestinations;
      //debugging
      int ikl = 0;
      for(ikl = 0; ikl < TRAIL_SIZE; ikl++){
	printf("trail[%d]: %d\n", ikl, trail[ikl]);
	showSet(possibleTrail[ikl]);
      }
      //enddebugging
      potentialDestinations = toArray(possibleTrail[i], &numberOfPotentialDestinations);
      if(numberOfPotentialDestinations != 0){
	pathLength = findMeAPath(gameState, potentialDestinations[(me + round)%numberOfPotentialDestinations], currentPath);
	if(pathLength > 1){
	  sprintf(msg, "Made guess for Dracula's trail[%d] -- moving towards: %s", i, idToAbbrev(currentPath[1]));
	  registerBestPlay(idToAbbrev(currentPath[1]), msg);
	  break;
	}
	else {
	  //This should be changed.
	  //continue;
	  sprintf(msg, "Made guess for Dracula's trail[%d] -- waiting: %s", i, idToAbbrev(here(gameState)));
	  registerBestPlay(idToAbbrev(here(gameState)), msg);
	  break;
	}	
      }
    }
    /*
    //...but we know he's in a city.
    if(draculasWhereabouts == CITY_UNKNOWN){
      registerBestPlay(idToAbbrev(getRandomMove(gameState)), "Moving randomly because Dracula is in an unknown city.");      
    }
    //...but we know he's at sea.
    else if(draculasWhereabouts == SEA_UNKNOWN){
      //Then we go to coastal towns and wait for him to get back.
      LocationID coastalTown = getCoastalTown(me);
      pathLength = findMeAPath(gameState, coastalTown, currentPath);
      if(pathLength <= 1){
	//If we are on top of the place we are going, wait.
	printf("On top of coastal town -- waiting: %d", here(gameState));
	sprintf(msg, "On top of coastal town -- waiting: %s", idToAbbrev(here(gameState)));
	registerBestPlay(idToAbbrev(here(gameState)), msg);
      }
      else{
	//Otherwise, we continue on our way to the coastal town.
	printf("Move to next move in path of length %d: %d", pathLength, currentPath[1]);
	sprintf(msg, "Move to coastal town -- next move in path: %s", idToAbbrev(currentPath[1]));
	registerBestPlay(idToAbbrev(currentPath[1]), msg);
      }
    }
    else{
      registerBestPlay(idToAbbrev(getRandomMove(gameState)), "Moving randomly because Dracula's location is unknown.");
    }
  }
    */
  }
}
