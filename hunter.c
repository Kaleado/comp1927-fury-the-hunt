// hunter.c
// Implementation of your "Fury of Dracula" hunter AI

#include <stdlib.h>
#include <stdio.h>
#include "Game.h"
#include "HunterView.h"

int indexOfLocation(LocationID elmt, LocationID* arr, int num){
  int i;
  for(i = 0; i < num; i++){
    if(arr[i] == elmt){
      return i;
    }
  }
  return -1;
}

void decideHunterMove(HunterView gameState)
{
  // TODO ...
  // Replace the line below by something better
  LocationID dracsWhereabouts = whereIs(gameState, PLAYER_DRACULA);
  int numReachables = 0;
  LocationID* reachables = whereCanIgo(gameState, &numReachables, 1, 1, 1);
  char* bestPlay;
  PlayerID me = whoAmI(gameState);

  printf("%d\n", numReachables);
  if(numReachables == 0){
    bestPlay = idToAbbrev(whereIs(gameState, me));
  }
  else if(indexOfLocation(dracsWhereabouts, reachables, numReachables) != -1){
    bestPlay = idToAbbrev(dracsWhereabouts);
  }
  else{
    bestPlay = idToAbbrev(reachables[rand() % numReachables]);
  }
  printf("Best play: %s", idToName(abbrevToID(bestPlay)));
  registerBestPlay(bestPlay,"I'm on holiday in Geneva");
}
