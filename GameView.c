// GameView.c ... GameView ADT implementation

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "Globals.h"
#include "Game.h"
#include "GameView.h"
#include "Map.h"
#include "PlayerRep.h"//A custom header file of our own formulation.
#include "Queue.h"//A custom header file of our own formulation.

struct gameView {  
  Round round;
  int score;
  PlayerID currentPlayer;
  int hello;
  Player players[NUM_PLAYERS];
  Map gameMap;
};

typedef struct gameView* GameView;

//Performs game logic for a given action string, such as "MGE....", including incrementing the round.
void executeMove(GameView gv, char* move){
#ifdef DEBUG
  printf("-----%s-----\n", __func__);//We use this statement in debugging to print the function being executed.
#endif
  char playerAbbr = move[0];//The following three variables are used to segment the action string, for later processing.
  char locationAbbr[] = {move[1], move[2]};
  char events[4] = {move[3], move[4], move[5], move[6]};
  
  PlayerID playerID;//The player that is performing this move.
  LocationID newLocation;//Used to 
  int isAtSea = 0;//Used to flag the player is at sea.
  int i;//Used as a counter.
  int index;//Used to store the index of the move that the player has doubled back to (for Dracula).
  LocationID hist[TRAIL_SIZE];//Used to store the player's history.

  //We discern the player executing the move.
  switch(playerAbbr){
  case 'G':
    playerID = PLAYER_LORD_GODALMING;
    break;
  case 'S':
    playerID = PLAYER_DR_SEWARD;
    break;
  case 'H':
    playerID = PLAYER_VAN_HELSING;
    break;
  case 'M':
    playerID = PLAYER_MINA_HARKER;
    break;
  case 'D':
    playerID = PLAYER_DRACULA;
    break;        
  }

  getPlayerHistory(gv->players[playerID], hist);

  //We first set the character's new location.
  //If the character has moved to an unknown location.
  if(locationAbbr[0] == 'C' && locationAbbr[1] == '?'){
    newLocation = CITY_UNKNOWN;
    isAtSea = 0;
  }
  else if(locationAbbr[0] == 'S' && locationAbbr[1] == '?'){
    newLocation = SEA_UNKNOWN;
    isAtSea = 1;
  }
  //If the character has doubled back (i.e. for Dracula only).
  else if(locationAbbr[0] == 'D' && locationAbbr[1] >= '1' && locationAbbr[1] <= '5'){
    index = locationAbbr[1] - '1';//We get the index of the place that Dracula is doubling back to.
    //We retrieve the player's history into the hist[] array.
    //    printf("hist[index] = %d", hist[index]);
    //We determine if the character was at sea.
    if(hist[index] == SEA_UNKNOWN || (validPlace(hist[index]) && isSea(hist[index])) ){
      isAtSea = 1;
    }
    newLocation = DOUBLE_BACK_1+locationAbbr[1]-'1';
  } 
  //If TP
  else if(locationAbbr[0] == 'T' && locationAbbr[1] == 'P') {
    newLocation = 17;
    isAtSea = 0;
  }
  //Otherwise, for any other valid location.
  else {
    newLocation = abbrevToID(locationAbbr);
    //We determine if the character was at sea.
    if(newLocation == SEA_UNKNOWN || (validPlace(newLocation) && isSea(newLocation)) ){
      isAtSea = 1;
    }
  }

  //Dracula takes damage if he is at sea.
  if(playerID == PLAYER_DRACULA && isAtSea ){
    damagePlayer(gv->players[playerID], 2);
  }
  //We examine all of the events given in the action string.
  if(playerID != PLAYER_DRACULA) {
    int finished = 0;
    for(i = 0; i < 4 && !finished; i++){
      switch(events[i]){
      case 'T':
        //Trap.
        if(damagePlayer(gv->players[playerID], 2)){
	  finished = 1;
	}
        break;
      case 'D':
        //Dracula.
        damagePlayer(gv->players[playerID], 4);
        damagePlayer(gv->players[PLAYER_DRACULA], 10);
        break;
      }
    }
  }
  //Dracula regains 10 blood if he's at his castle and not dead.
  if(playerID == PLAYER_DRACULA && newLocation == 17 && getPlayerHealth(gv->players[PLAYER_DRACULA]) > 0) {
    damagePlayer(gv->players[playerID], -10);
  }
   //If the player rested in a city.
  if(hist[0] == newLocation && playerID != PLAYER_DRACULA){
    damagePlayer(gv->players[playerID], -3);
  }
  //We update the player's location.
  setPlayerLocation(gv->players[playerID], newLocation);
  addToPlayerHistory(gv->players[playerID], newLocation);

  //if(playerID == PLAYER_DRACULA) printf ("*%s* ", idToAbbrev(newLocation));
  //We move to the next player.
  gv->currentPlayer++;
  if(gv->currentPlayer >= NUM_PLAYERS){
    gv->currentPlayer = 0;
    gv->round++;
  }
  return;
}

// Creates a new GameView to summarise the current state of the game
GameView newGameView(char *pastPlays, PlayerMessage messages[])
{
#ifdef DEBUG
  printf("-----%s-----\n", __func__);
#endif
  int currentPlayer;
  int nextMoveNumber;
  GameView gameView = malloc(sizeof(struct gameView));

  //We initialise the game information.
  gameView->currentPlayer = 0;
  gameView->round = 0;
  gameView->score = GAME_START_SCORE;

  //We also initialise each player.
  for(currentPlayer = 0; currentPlayer < NUM_PLAYERS; currentPlayer++){
    gameView->players[currentPlayer] = newPlayer(currentPlayer);
  }
  /*We assert that the  pastPlays string is valid - the string is valid if:
        - It consists of a multiple of eight characters (seven characters plus a terminator).
	- It is totally empty.
   */
  assert((strlen(pastPlays)+1) % 8 == 0 || strlen(pastPlays) == 0);
  int numberOfMovesMade = strlen(pastPlays)/8 + 1;
  if(strlen(pastPlays) == 0){
    numberOfMovesMade = 0;
  }
  //We go through and execute each move.
  char nextMove[8];
  for(nextMoveNumber = 0; nextMoveNumber < numberOfMovesMade; nextMoveNumber++){
#ifdef DEBUG
    printf("nextMoveNumber = %d\n", nextMoveNumber);
    printf("Trimmed movestring: %s\n", pastPlays + nextMoveNumber*8);
#endif
    sscanf(pastPlays + nextMoveNumber*8, "%s ", nextMove);
    //    printf("nextMove = '%s'\n", nextMove);
    //We execute the individual move.
    executeMove(gameView, nextMove);
  }
  //We also set up the map.
  gameView->gameMap = newMap();
  return gameView;
}     
     
// Frees all memory previously allocated for the GameView toBeDeleted
void disposeGameView(GameView toBeDeleted)
{
  int i;//Used as a counter.
  //We first free each player object we created.
  for(i = 0; i < NUM_PLAYERS; i++){
    destroyPlayer(toBeDeleted->players[i]);
  }
  disposeMap(toBeDeleted->gameMap);
  //Now we free the gameview itself.
  free(toBeDeleted);
  return;
}


//// Functions to return simple information about the current state of the game

// Get the current round
Round getRound(GameView currentView)
{
  return currentView->round;
}

// Get the id of current player - ie whose turn is it?
PlayerID getCurrentPlayer(GameView currentView)
{
  return currentView->currentPlayer;
}

// Get the current score
int getScore(GameView currentView)
{
  return currentView->score;
}

// Get the current health points for a given player
int getHealth(GameView currentView, PlayerID player)
{
  return getPlayerHealth(currentView->players[player]);
}

// Get the current location id of a given player
LocationID getLocation(GameView currentView, PlayerID player)
{
  return getPlayerLocation(currentView->players[player]);
}

//// Functions that return information about the history of the game

// Fills the trail array with the location ids of the last 6 turns
void getHistory(GameView currentView, PlayerID player,
		LocationID trail[TRAIL_SIZE])
{
  getPlayerHistory(currentView->players[player], trail);
  return;
}

//// Functions that query the map to find information about connectivity

//Finds a path from player's location to the destination.
int findPathTo(GameView currentView, LocationID destination, PlayerID player, LocationID* path){
  return findPath(currentView->gameMap, getLocation(currentView, player), destination, 100, path);
}

// Returns an array of LocationIDs for all directly connected locations
LocationID *connectedLocations(GameView currentView, int *numLocations,
                               LocationID from, PlayerID player, Round round,
                               int road, int rail, int sea)
{
  LocationID* locations = malloc(sizeof(LocationID)*NUM_MAP_LOCATIONS);//We malloc() the maximum possible number of locations found.
  TransportID types[MAX_TRANSPORT];
  int currentLocation;
  int locsFound = 0;
  int numConns;
  int i;
  int added;
  //  int trainAvailability = (player + round) % 4;
  
  for(currentLocation = MIN_MAP_LOCATION; currentLocation <= MAX_MAP_LOCATION; currentLocation++){
    //Get the available connections.
    numConns = connections(currentView->gameMap, from, currentLocation, types);
    added = 0;
    //Dracula cannot move to the hospital.
    if(currentLocation == ST_JOSEPH_AND_ST_MARYS && player == PLAYER_DRACULA){
      continue;
    }
    for(i = 0; i < numConns; i++){
      if(types[i] == ROAD && road){
	//printf("Found ROAD connection between %d and %d.\n", from, currentLocation);
	locations[locsFound] = currentLocation;
	added = 1;
      }
      //Dracula can't take the train.
      if(types[i] == RAIL && rail && player != PLAYER_DRACULA){// && trainAvailability != 0
	//printf("Found RAIL connection between %d and %d.\n", from, currentLocation);
	locations[locsFound] = currentLocation;
	added = 1;
      }
      if(types[i] == BOAT && sea){
	//printf("Found BOAT connection between %d and %d.\n", from, currentLocation);
	locations[locsFound] = currentLocation;
	added = 1;
      }
    }
    if(added){
      locsFound++;
    }

  }
  //We set the number of locations found and return;
  *numLocations = locsFound;
  return locations;
}
