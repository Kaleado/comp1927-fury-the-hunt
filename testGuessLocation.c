#include "testGuessLocation.h"

int main(){
  LocationID trail[6] = {
    28,
    60,
    -1,
    -1,
    -1,
    -1
  };
  Set possibleTrail[6];
  guessDraculasLocation(trail, possibleTrail);
  int i;
  for(i=0; i < 6; i++){
    showSet(possibleTrail[i]);
  }
  return 1;
}
