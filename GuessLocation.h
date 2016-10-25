#ifndef GUESSLOCATION_H
#define GUESSLOCATION_H

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include "assert.h"
#include "Map.h"
#include "Set.h"
#include "Places.h"
#include "Globals.h"

void guessDraculasLocation(LocationID trail[TRAIL_SIZE], Set possibleLocations[TRAIL_SIZE]);

#endif
