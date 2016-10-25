#include "GuessLocation.h"

void guessDraculasLocation(LocationID trail[TRAIL_SIZE], Set possibleLocations[TRAIL_SIZE]){
  Map europe = newMap();
  addConnections(europe);
  Set a[TRAIL_SIZE];
  Set b[TRAIL_SIZE];
  int i,j, k;

  LocationID currentTrailLocation;
  LocationID currentAdjacentLocation;
  
  int numAdjacent = 0;
  LocationID* adjacent = NULL;
  
  int numPreviousLocations = 0;
  LocationID* previousLocations = NULL; 
  
  for(i=0; i<TRAIL_SIZE; i++){
    a[i] = newSet();
    b[i] = newSet();
  }
  
  //Starting from trail[0], go through and add all possible locations to Set a[0].
  //For trail[1..5], we find all seas/cities adjacent to each element of the previous set and add them to Set a[1..5].
  //If we have a known location at position n in the trail, Set a[n] is simply that location.
  for(i=0; i < TRAIL_SIZE; i++){
    currentTrailLocation = trail[i];
    //we are looking at the first item in the trail; no previous nodes to check adjacency.
    if(i == 0){
      numPreviousLocations = 0;
      previousLocations = NULL;
      numAdjacent = 0;
      adjacent = 0;
      //We add the current location (if it is a known location).
      if(currentTrailLocation < NUM_MAP_LOCATIONS && currentTrailLocation >= 0){
	insertInto(a[0], currentTrailLocation);
	continue;
      }
      //Otherwise, we simply add every possible city/sea.
      for(j = 0; j < NUM_MAP_LOCATIONS; j++){
	if((currentTrailLocation == CITY_UNKNOWN && isLand(j)) ||
	   (currentTrailLocation == SEA_UNKNOWN && isSea(j)) ||
	   (currentTrailLocation == UNKNOWN_LOCATION) ||
	   currentTrailLocation == HIDE || 
	   (currentTrailLocation >= DOUBLE_BACK_1 && currentTrailLocation <= DOUBLE_BACK_5)){
	  insertInto(a[0], j);
	}
      }
    }
    else {
      /*if(currentTrailLocation >= DOUBLE_BACK_1 && currentTrailLocation <= DOUBLE_BACK_5){
      //handle double backs
      //do nothing for now
      }
      else */if (currentTrailLocation == CITY_UNKNOWN ||
		 currentTrailLocation == SEA_UNKNOWN ||
		 currentTrailLocation == UNKNOWN_LOCATION ||
		 (currentTrailLocation >= DOUBLE_BACK_1 && currentTrailLocation <= DOUBLE_BACK_5) ||
		 currentTrailLocation == HIDE){
	//If dracula is in an unknown location, we get the previous locations...
	previousLocations = toArray(a[i-1], &numPreviousLocations);
	//...find every location adjacent to those locations...
	//(for each location in the previous list of possible locations)
	for(j = 0; j < numPreviousLocations; j++){
	  //we get the adjacent locations
	  numAdjacent = getAdjacent(europe, previousLocations[j], NULL);
	  adjacent = malloc(numAdjacent*sizeof(LocationID));
	  getAdjacent(europe, previousLocations[j], adjacent);
	  //...for each adjacent location, we check if they match our trail position...
	  for(k = 0; k < numAdjacent; k++){
	    currentAdjacentLocation = adjacent[k];
	    if((currentTrailLocation == CITY_UNKNOWN && isLand(currentAdjacentLocation)) ||
	       (currentTrailLocation == SEA_UNKNOWN && isSea(currentAdjacentLocation)) ||
	       (currentTrailLocation == UNKNOWN_LOCATION) ||
	       (currentTrailLocation >= DOUBLE_BACK_1 && currentTrailLocation <= DOUBLE_BACK_5) ||
	       currentTrailLocation == HIDE
	       ){
	      //...and insert them into the current set if they do.
	      insertInto(a[i], currentAdjacentLocation);
	    }
	    //Otherwise we just ignore the non-matching nodes and move on.
	  }
	  free(adjacent);
	}
      }
      else {
	//We have an actual known location
	insertInto(a[i], currentTrailLocation);	
      }
    }
  }
 
  //Starting from trail[5], go through and add all possible locations to Set b[5].
  //For trail[5..1], we find all seas/cities adjacent to each element of the previous set and add them to Set b[5..1].
  //If we have a known location at position n in the trail, Set b[n] is simply that location.
  for(i=5; i >= 0; i--){
    currentTrailLocation = trail[i];
    //we are looking at the first item in the trail; no previous nodes to check adjacency.
    if(i == 5){
      numPreviousLocations = 0;
      previousLocations = NULL;
      numAdjacent = 0;
      adjacent = 0;
      if(currentTrailLocation < NUM_MAP_LOCATIONS && currentTrailLocation >= 0){
	insertInto(b[5], currentTrailLocation);
	continue;
      }
      //We simply add every possible city/sea.
      for(j = 0; j < NUM_MAP_LOCATIONS; j++){
	if((currentTrailLocation == CITY_UNKNOWN && isLand(j)) ||
	   (currentTrailLocation == SEA_UNKNOWN && isSea(j)) ||
	   (currentTrailLocation == UNKNOWN_LOCATION) ||
	   (currentTrailLocation >= DOUBLE_BACK_1 && currentTrailLocation <= DOUBLE_BACK_5) ||
	   currentTrailLocation == HIDE){
	  insertInto(b[5], j);
	}
      }
    }
    else {
      /*if(currentTrailLocation >= DOUBLE_BACK_1 && currentTrailLocation <= DOUBLE_BACK_5){
      //handle double backs
      //do nothing for now
      }
      else*/ if (currentTrailLocation == CITY_UNKNOWN ||
	       currentTrailLocation == SEA_UNKNOWN ||
	       currentTrailLocation == UNKNOWN_LOCATION ||
	       (currentTrailLocation >= DOUBLE_BACK_1 && currentTrailLocation <= DOUBLE_BACK_5) ||
		 currentTrailLocation == HIDE){
	//If dracula is in an unknown location, we get the previous locations...
	previousLocations = toArray(b[i+1], &numPreviousLocations);
	//...find every location adjacent to those locations...
	//(for each location in the previous list of possible locations)
	for(j = 0; j < numPreviousLocations; j++){
	  //we get the adjacent locations
	  numAdjacent = getAdjacent(europe, previousLocations[j], NULL);
	  adjacent = malloc(numAdjacent*sizeof(LocationID));
	  getAdjacent(europe, previousLocations[j], adjacent);
	  //...for each adjacent location, we check if they match our trail position...
	  for(k = 0; k < numAdjacent; k++){
	    currentAdjacentLocation = adjacent[k];
	    if((currentTrailLocation == CITY_UNKNOWN && isLand(currentAdjacentLocation)) ||
	       (currentTrailLocation == SEA_UNKNOWN && isSea(currentAdjacentLocation)) ||
	       (currentTrailLocation == UNKNOWN_LOCATION ||
	       (currentTrailLocation >= DOUBLE_BACK_1 && currentTrailLocation <= DOUBLE_BACK_5) ||
	       currentTrailLocation == HIDE)){
	      //...and insert them into the current set if they do.
	      insertInto(b[i], currentAdjacentLocation);
	    }
	    //Otherwise we just ignore the non-matching nodes and move on.
	  }
	  free(adjacent);
	}
      }
      else {
	//We have an actual known location
	insertInto(b[i], currentTrailLocation);	
      }
    }
  }   

  //We find Set c[n], for n = 0..5,  by setting c[n] to intersection(a[n], b[n]).
  for(i=0; i < TRAIL_SIZE; i++){
    possibleLocations[i] = intersection(a[i], b[i]);
  }
  
  disposeMap(europe);
  return;
}
