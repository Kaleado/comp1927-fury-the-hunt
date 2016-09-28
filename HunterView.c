// HunterView.c ... HunterView ADT implementation

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "Globals.h"
#include "Game.h"
#include "GameView.h"
#include "HunterView.h"

     
struct hunterView {
   GameView game;
   char *pastPlays;
   char research;
   PlayerID player;
   char rest[NUM_PLAYERS-1];
};
     

// Creates a new HunterView to summarise the current state of the game
HunterView newHunterView(char *pastPlays, PlayerMessage messages[])
{
   //printf("newHunterView\n");
   HunterView hunterView = malloc(sizeof(struct hunterView));
   hunterView->game = newGameView(pastPlays, messages);
   hunterView->pastPlays = malloc(strlen(pastPlays));
   strcpy(hunterView->pastPlays, pastPlays);
   hunterView->player = whoAmI(hunterView);

    return hunterView;
}
     
     
// Frees all memory previously allocated for the HunterView toBeDeleted
void disposeHunterView(HunterView toBeDeleted)
{
   //printf("disposeHunterView\n");
   disposeGameView(toBeDeleted->game);
   free( toBeDeleted );
   toBeDeleted = NULL;
}




//// Functions to return simple information about the current state of the game

// Get the current round
Round giveMeTheRound(HunterView currentView)
{
   //printf("giveMeTheRound\n");
   return getRound(currentView->game);
}

// Get the id of current player
PlayerID whoAmI(HunterView currentView)
{
   //printf("whoAmI\n");
   return getCurrentPlayer(currentView->game);
}

// Get the current score
int giveMeTheScore(HunterView currentView)
{
   //printf("giveMeTheScore\n");
   return getScore(currentView->game);
}

// Get the current health points for a given player
int howHealthyIs(HunterView currentView, PlayerID player)
{
   //printf("howHealthyIs\n");
   return getHealth(currentView->game, player);
}



// Get the current location id of a given player
LocationID whereIs(HunterView currentView, PlayerID player)
{
   //printf("whereIs\n");

   if (player == PLAYER_DRACULA) {
      LocationID dracLoc = getLocation(currentView->game, PLAYER_DRACULA);

      if (dracLoc < MIN_MAP_LOCATION){
         return UNKNOWN_LOCATION;
      } else if (dracLoc > MAX_MAP_LOCATION) {
         return dracLoc;
      }
         
      if (idToType(dracLoc) == LAND) {
         if (abbrevToID("CD") == getLocation(currentView->game, PLAYER_DRACULA)) {
            return dracLoc;
         } else if (dracLoc == getLocation(currentView->game, PLAYER_LORD_GODALMING) || dracLoc == getLocation(currentView->game, PLAYER_DR_SEWARD) || dracLoc == getLocation(currentView->game, PLAYER_VAN_HELSING) || dracLoc == getLocation(currentView->game, PLAYER_MINA_HARKER)) {
            return dracLoc;
         } else {
            return CITY_UNKNOWN;
         }
      }else if (idToType(getLocation(currentView->game, PLAYER_DRACULA)) == SEA){
         return SEA_UNKNOWN;
      }else{
         return UNKNOWN_LOCATION;
      }
   } else {
      return getLocation(currentView->game, player);
   }
   printf("couldn't whereIs\n");
}



//// Functions that return information about the history of the game

// Fills the trail array with the location ids of the last 6 turns
void giveMeTheTrail(HunterView currentView, PlayerID player,
                            LocationID trail[TRAIL_SIZE])
{
   //printf("giveMeTheTrail\n");
   getHistory(currentView->game, player, trail);
   return;
}



//// Functions that query the map to find information about connectivity

// What are my possible next moves (locations)
LocationID *whereCanIgo(HunterView currentView, int *numLocations,
                        int road, int rail, int sea)
{
  PlayerID currPlayer = getCurrentPlayer(currentView->game);
  return connectedLocations(currentView->game, numLocations, getLocation(currentView->game,currPlayer), currPlayer, getRound(currentView->game),road,rail,sea);  
}

// What are the specified player's next possible moves
LocationID *whereCanTheyGo(HunterView currentView, int *numLocations,
                           PlayerID player, int road, int rail, int sea)
{
   //printf("whereCanTheyGo\n");
   LocationID *locations;
   
   if (player == PLAYER_DRACULA) {
      LocationID dracLoc = whereIs(currentView, PLAYER_DRACULA);
      if(dracLoc == CITY_UNKNOWN || dracLoc == SEA_UNKNOWN || dracLoc == UNKNOWN_LOCATION){
         numLocations = 0;
         locations = NULL;
      } else {
         LocationID *adjacent = connectedLocations(currentView->game, numLocations, getLocation(currentView->game,PLAYER_DRACULA), PLAYER_DRACULA, getRound(currentView->game),road, 0,sea);
         
         LocationID trail[TRAIL_SIZE];
         int numLoc = *numLocations;
         giveMeTheTrail(currentView, PLAYER_DRACULA, trail);
         LocationID *locations  = malloc(sizeof(LocationID)*numLoc);
         int inTrail = FALSE;
         int locationIndex = 0;
         int i;
         int d;
         for(i=0;i<numLoc;i++)
         {
            inTrail = FALSE;
            for(d=0;d<TRAIL_SIZE;d++)
            {
               if(adjacent[i] == trail[d])
               {
                  inTrail = TRUE;
                  break;
               }
            }
            if(inTrail == FALSE)
            {
               locations[locationIndex] = adjacent[i];
               locationIndex++;
            }
         }
         
         free(adjacent); 
         
         return locations;      }
      
   } else {
      locations = connectedLocations(currentView->game, numLocations, getLocation(currentView->game,player),player, getRound(currentView->game),road,rail,sea);
   }
   
   return locations;
}
