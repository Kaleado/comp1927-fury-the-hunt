// Player.h: player ADT definition.

#ifndef PLAYER_H
#define PLAYER_H

#include <stdlib.h>
#include <assert.h>
#include "Globals.h"
#include "Places.h"
#include "Queue.h"

struct playerRep{
  Queue history;
  PlayerID id;//This may be entirely unnecessary here.  
  int health;
  LocationID location;
};

typedef struct playerRep *Player;

int getPlayerHealth(Player);

LocationID getPlayerLocation(Player);

Player newPlayer(int);

void destroyPlayer(Player);

void setPlayerLocation(Player, LocationID);

void setPlayerHealth(Player p, int newHealth);

int damagePlayer(Player p, int amt);

void addToPlayerHistory(Player p, LocationID loc);

int getPlayerHistory(Player p, LocationID* arr);

LocationID getPreviousPlayerLocation(Player p, int index);

#endif
