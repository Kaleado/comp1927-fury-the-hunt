// Map.h ... interface to Map data type

#ifndef MAP_H
#define MAP_H

#include "Queue.h"
#include "Places.h"

typedef struct edge{
    LocationID  start;
    LocationID  end;
    TransportID type;
} Edge;

// graph representation is hidden 
typedef struct MapRep *Map; 

// operations on graphs 
Map  newMap();  
void disposeMap(Map g); 
void showMap(Map g); 
int  numV(Map g);
int  numE(Map g, TransportID t);
int connections(Map, LocationID, LocationID, TransportID[]);
int findPath(Map, LocationID, LocationID, int, int*);
int  getlen(Map g, LocationID start, LocationID to);
int getAdjacent(Map g, LocationID start, LocationID* adjacent);
int isAdjacent(Map g, LocationID start, LocationID end);
void addConnections(Map);

#endif
