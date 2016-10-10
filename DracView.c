// DracView.c ... DracView ADT implementation

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "Globals.h"
#include "Game.h"
#include "GameView.h"
#include "DracView.h"
#include "Map.h"
#include "Queue.h"
#include "PlayerRep.h"
     
struct dracView {
   GameView game;
   char vamp[NUM_MAP_LOCATIONS];
   char trap[NUM_MAP_LOCATIONS];
   char *pastPlays;
   char research;
   char rest[NUM_PLAYERS-1];
};
     

// Creates a new DracView to summarise the current state of the game
DracView newDracView(char *pastPlays, PlayerMessage messages[]) {
   // initialise dracView
   DracView dracView = malloc(sizeof(struct dracView));
   dracView->game = newGameView(pastPlays, messages);
   dracView->research = 0;
   memset(dracView->vamp,0,NUM_MAP_LOCATIONS);
   memset(dracView->trap,0,NUM_MAP_LOCATIONS);
   dracView->pastPlays = malloc(strlen(pastPlays));
   strcpy(dracView->pastPlays, pastPlays);
   int player = 0;

   // analyse pastPlays;
   int where = 0;
   int n;
   char name[2];
   int lastLocation[NUM_PLAYERS] = {0};
   LocationID vampLoc;
   LocationID loc;
   
   for (;where < strlen(pastPlays);where+=8) {
      player = (where/8)%5;
      name[0] = pastPlays[where+1];
      name[1] = pastPlays[where+2];
      loc = abbrevToID(name);
      if (player!=PLAYER_DRACULA) {
         if (lastLocation[player] == loc) {
            dracView->rest[player] = 1;
         } else {
            dracView->rest[player] = 0;
         }
         dracView->research = dracView->rest[0]+dracView->rest[1]+dracView->rest[2]+dracView->rest[3];
         for (n = 3;n < 7;n++) {
            switch (pastPlays[where+n]) {
               case 'T': dracView->trap[loc]--;break;
               case 'V': dracView->vamp[loc]--;break;
            }
         }
      } else {
         for (n = 3;n < 5;n++) {
            switch (pastPlays[where+n]) {
               case 'T': dracView->trap[loc]++;break;
               case 'V': dracView->vamp[loc]++;vampLoc=loc;break;
            }
         }
         if (pastPlays[where+6] == 'V') {
            dracView->vamp[vampLoc]--;
         }
      }
      lastLocation[player] = loc;
   }

   return dracView;
}
     
     
// Frees all memory previously allocated for the DracView toBeDeleted
void disposeDracView(DracView toBeDeleted) {
   memset(toBeDeleted->pastPlays,0,strlen(toBeDeleted->pastPlays));
   memset(toBeDeleted->vamp,0,NUM_MAP_LOCATIONS);
   memset(toBeDeleted->trap,0,NUM_MAP_LOCATIONS);
   memset(toBeDeleted->rest,0,NUM_PLAYERS-1);
   free(toBeDeleted->pastPlays);
   toBeDeleted->pastPlays = NULL;
   disposeGameView(toBeDeleted->game);
   free(toBeDeleted);
   toBeDeleted = NULL;
}


//// Functions to return simple information about the current state of the game

// Get the current round
Round giveMeTheRound(DracView currentView)
{
   return getRound(currentView->game);
}

// Get the current score
int giveMeTheScore(DracView currentView)
{
   return getScore(currentView->game);
}

// Get the current health points for a given player
int howHealthyIs(DracView currentView, PlayerID player)
{
   return getHealth(currentView->game, player);
}

// Get the current location id of a given player
LocationID whereIs(DracView currentView, PlayerID player)
{
   return getLocation(currentView->game, player);
}

// Get the most recent move of a given player
void lastMove(DracView currentView, PlayerID player,
                 LocationID *start, LocationID *end)
{
   LocationID trail[TRAIL_SIZE];
   getHistory(currentView->game, player, trail);
   *start = (trail[TRAIL_SIZE-2]==UNKNOWN_LOCATION)?UNKNOWN_LOCATION:trail[TRAIL_SIZE-2];
   *end = (trail[TRAIL_SIZE-1]==UNKNOWN_LOCATION)?UNKNOWN_LOCATION:trail[TRAIL_SIZE-1];

   return;
}

// Find out what minions are placed at the specified location
void whatsThere(DracView currentView, LocationID where,
                         int *numTraps, int *numVamps)
{
   if (isLand(where)) {
      *numTraps = currentView->trap[where];
      *numVamps = currentView->vamp[where];
   } else {
      *numTraps = 0;
      *numVamps = 0;
   }
   return;
}

//// Functions that return information about the history of the game

// Fills the trail array with the location ids of the last 6 turns
void giveMeTheTrail(DracView currentView, PlayerID player,
                            LocationID trail[TRAIL_SIZE])
{
   getHistory(currentView->game, player, trail);
   return;
}

//// Functions that query the map to find information about connectivity

// What are my (Dracula's) possible next moves (locations)
LocationID *whereCanIgo(DracView currentView, int *numLocations, int road, int sea) {
   int numLoc; 
   LocationID trail[TRAIL_SIZE]; 
   LocationID *adjacent = connectedLocations(currentView->game, &numLoc, getLocation(currentView->game,PLAYER_DRACULA), PLAYER_DRACULA, getRound(currentView->game), road, 0, sea); //possible locations for dracula
   LocationID *locations  = malloc(sizeof(LocationID)*numLoc); //more than enough space. to be optimised properly!!

   giveMeTheTrail(currentView, PLAYER_DRACULA, trail);
   int inTrail;
   int locationIndex = 0;
   int i;
   int d;
   for(i=0;i<numLoc;i++) {
      inTrail = FALSE;
      for(d=0;d<TRAIL_SIZE;d++) {
         if(adjacent[i] == trail[d]) {
            inTrail = TRUE; //so we dont add it to the possible locations.
	      }
	   }
      if(inTrail == FALSE)	{
	      locations[locationIndex] = adjacent[i]; //not in the trail, add it to the possible locations. 
	      locationIndex++;
      }
   }
   *numLocations = locationIndex;
   for (i = 0; i < numLoc; i++) printf("\n%s to %s, num = %d, blood = %d\n", idToAbbrev(getLocation(currentView->game,PLAYER_DRACULA)),idToAbbrev(adjacent[i]), i, getHealth(currentView->game, PLAYER_DRACULA));
   free(adjacent); 
   return locations;
}

// What are the specified player's next possible moves
LocationID *whereCanTheyGo(DracView currentView, int *numLocations,
                           PlayerID player, int road, int rail, int sea)
{
   LocationID *locations;

   if (player == PLAYER_DRACULA) {
       locations = whereCanIgo(currentView, numLocations, road,sea);   
   } else {
       locations = connectedLocations(currentView->game, 
				      numLocations, 
				      getLocation(currentView->game, player),
				      player, 
				      getRound(currentView->game),
				      road, rail, sea);
   }

   return locations;
}
