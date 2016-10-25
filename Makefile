# Makefile for "Fury of Dracula": The Hunt

# change these to suit your local C environment
CC = gcc
CFLAGS = -Wall -Werror -gdwarf-2
# do not change the following line
BINS = dracula hunter
# add any other *.o files that your system requires
# (and add their dependencies below after DracView.o)
# if you're not using Map.o or Places.o, you can remove them
OBJS = GameView.o Map.o Places.o Map.o Queue.o PlayerRep.o Set.o
# add whatever system libraries you need here (e.g. -lm)
LIBS =

all : $(BINS)

testSet: testSet.o Map.o Places.o Set.o Queue.o
testGuessLocation: testGuessLocation.o GuessLocation.o Set.o Map.o Places.o Queue.o

dracula : dracPlayer.o dracula.o DracView.o $(OBJS) $(LIBS)
hunter : hunterPlayer.o hunter.o HunterView.o GuessLocation.o $(OBJS) $(LIBS)

dracPlayer.o : player.c Game.h DracView.h dracula.h
	$(CC) $(CFLAGS) -DI_AM_DRACULA -c player.c -o dracPlayer.o

hunterPlayer.o : player.c Game.h HunterView.h hunter.h
	$(CC) $(CFLAGS) -c player.c -o hunterPlayer.o

dracula.o : dracula.c Game.h DracView.h
hunter.o : hunter.c Game.h HunterView.h
Places.o : Places.c Places.h
PlayerRep.o : PlayerRep.c PlayerRep.h
Map.o : Map.c Map.h Places.h
Queue.o : Queue.c Queue.h
GameView.o : GameView.c Globals.h GameView.h
HunterView.o : HunterView.c Globals.h HunterView.h
DracView.o : DracView.c Globals.h DracView.h
Set.o : Set.c Set.h Places.h
GuessLocation.o: GuessLocation.c GuessLocation.h Set.h Map.h Places.h Globals.h

#Object files for testing purposes.
testSet.o : testSet.c Set.h
testGuessLocation.o : testGuessLocation.c GuessLocation.h

# if you use other ADTs, add dependencies for them here

clean :
	rm -f $(BINS) *.o core

