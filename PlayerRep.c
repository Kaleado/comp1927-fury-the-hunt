#include "PlayerRep.h"
#include "stdio.h"

Player newPlayer(int id){
  Player newPlayer = malloc(sizeof(struct playerRep));
  newPlayer->id = id;
  //Dracula starts with a different amount of health than the Hunters.
  if(id == PLAYER_DRACULA){
    newPlayer->health = GAME_START_BLOOD_POINTS;
  }
  else {
    newPlayer->health = GAME_START_HUNTER_LIFE_POINTS;
  }
  newPlayer->location = UNKNOWN_LOCATION;
  newPlayer->history = newQueue();
  return newPlayer;
}

void destroyPlayer(Player p){
  free(p);
  return;
}

int getPlayerHealth(Player p){
  return p->health;
}

void setPlayerHealth(Player p, int newHealth){
  p->health = newHealth;
  return;
}

int damagePlayer(Player p, int amt){
  p->health -= amt;
  if(p->health <= 0){
    setPlayerLocation(p, ST_JOSEPH_AND_ST_MARYS);
    p->health = 9;
    return 1;
  }
  if(p->health > 9 && p->id != PLAYER_DRACULA){
    p->health = 9;
  }
  return 0;
}

LocationID getPlayerLocation(Player p){
  return p->location;
}

void setPlayerLocation(Player p, LocationID loc){
  p->location = loc;
  return;
}

void addToPlayerHistory(Player p, LocationID loc){
  if(lengthQueue(p->history) >= 6){
    leaveQueue(p->history);
  }
  enterQueue(p->history, loc);
  return;
}

//Gets the entirety of player p's history, reads them
//into arr, and returns the number of items read.
int getPlayerHistory(Player p, LocationID* arr){
  int i;
  int queueSize = lengthQueue(p->history);

  //printf("agiajdog\n");
  for(i = TRAIL_SIZE - 1; i >= 0; i--){
    if(i >= queueSize){
      arr[i] = UNKNOWN_LOCATION;
    }
    else {
      arr[i] = leaveQueue(p->history);
      enterQueue(p->history, arr[i]);
    }
    //printf("%d, ", arr[i]);
  }
  return 0;
}
