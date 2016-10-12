/**
 * Runs a player's game turn ...
 *
 * Can produce either a Hunter player or a Dracula player
 * depending on the setting of the I_AM_DRACULA #define
 *
 * This is a dummy version of the real player.c used when
 * you submit your AIs. It is provided so that you can
 * test whether your code is likely to compile ...
 *
 * Note that it used to drive both Hunter and Dracula AIs.
 * It first creates an appropriate view, and then invokes the
 * relevant decide-my-move function, which should use the 
 * registerBestPlay() function to send the move back.
 *
 * The real player.c applies a timeout, so that it will halt
 * decide-my-move after a fixed amount of time, if it doesn't
 * finish first. The last move registered will be the one
 * used by the game engine. This version of player.c won't stop
 * your decide-my-move function if it goes into an infinite
 * loop. Sort that out before you submit.
 *
 * Based on the program by David Collien, written in 2012
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Game.h"
#ifdef I_AM_DRACULA
#include "DracView.h"
#include "dracula.h"
#else
#include "HunterView.h"
#include "hunter.h"
#endif

// moves given by registerBestPlay are this long (including terminator)
#define MOVE_SIZE 3

// The minimum static globals I can get away with
static char latestPlay[MOVE_SIZE] = "";
static char latestMessage[MESSAGE_SIZE] = "";

int main(int argc, char *argv[])
{
#ifdef I_AM_DRACULA
  DracView gameState;
  char *plays = "GSR.... SST.... HLO.... MSZ.... DAT.V.. GBO.... SNU.... HLV.... MKL.... DIO.... GLE.... SMU.... HSW.... MBD.... DBS.... GBU.... SST.... HLO.... MSZ.... DVRT... GAM.... SNU.... HLV.... MBC.... DCNT... GCO.... SLI.... HIR.... MGA.... DBCT... GBU.... SBR.... HAO.... MKL.... DSOT.V. GLE.... SPR.... HLS.... MBD.... DSAT... GEC.... SVI.... HSR.... MCNT... DVAT... GLO.... SVE.... HMR.... MBS.... DSJT.M. GLO.... SVE.... HMR.... MBS.... DBET... GLV.... SGO.... HMI.... MVR.... DKLT.M. GSW.... STS.... HRO.... MBET... DBDT.M. GLO.... SIO.... HBI.... MSJT... DSZ.VM. GLV.... SBS.... HAS.... MZA.... DZAT.M. GED.... SVR.... HVE.... MZATD.. DMUT... GNS.... SSO.... HGO.... MBE.... DNUT... GAM.... SBC.... HTS.... MSJ.... DPRT.M. GCO.... SGA.... HIO.... MZA.... DBRT.M. GZU.... SKL.... HSA.... MSZV... DHAT... GZU.... SKL.... HSA.... MSZ.... DNS.... GMUT... SCD.... HVR.... MZA.... DEDT... GST.... SGA.... HBS.... MVI.... DMNT.M. GGO.... SBC.... HCN.... MBD.... DLVT.M. GMI.... SKL.... HGA.... MSO.... DSWT.M. GMR.... SCD.... HBC.... MSJ.... DLOT.M. GLE.... SKL.... HBE.... MVA.... DPL.V.. GNA.... SGA.... HSO.... MIO.... DEC..M. GPA.... SBC.... HSA.... MTS.... DNS..M. GST.... SCN.... HVR.... MMS.... DEDT.M. GST.... SCN.... HVR.... MMS.... DMNT.M. GGO.... SBS.... HSO.... MCG.... DLVT.M. GMI.... SIO.... HSA.... MTS.... DSWT.V. GMR.... SVA.... HVR.... MMS.... DLOT... GLE.... SSJ.... HBS.... MCG.... DPLT... GNA.... SZA.... HCN.... MTS.... DEC..M. GPA.... SSZ.... HGA.... MMS.... DNS..M. GST.... SVI.... HBC.... MCG.... DEDT.M. GFR.... SHA.... HKL.... MTS.... DMNT.M. GPR.... SNS.... HCD.... MMS.... DLV.VM. GPR.... SNS.... HCD.... MMS.... DSWT.M. GBR.... SEC.... HGA.... MCG.... DIR.... GHA.... SPL.... HBC.... MTS.... DDUT... GCO.... SLO.... HKL.... MMS.... DGWT.M. GLI.... SEC.... HCD.... MCG.... DAO..M. GHA.... SNS.... HGA.... MTS.... DBB..V. GCO.... SED.... HCN.... MIO.... DSNT.M. GFR.... SMN.... HVR.... MBS.... DMAT... GNU.... SLO.... HSA.... MCN.... DALT.M. GPR.... SLV.... HBE.... MGA.... DSRT.M. GPR.... SLV.... HBE.... MGA.... DTOT... GNU.... SSW.... HSJ.... MCN.... DBAT... GMU.... SLO.... HVA.... MVR.... DMS..M. GZU.... SLV.... HSA.... MSO.... DCGT.M. GGE.... SMN.... HIO.... MBC.... DTS..M. GST.... SLO.... HBS.... MCN.... DGOT.M. GFR.... SPL.... HVR.... MSZ.... DFLT.M. GNU.... SEC.... HBE.... MKL.... DROT.M. GLI.... SNS.... HSJ.... MBD.... DBIT... GZU.... SED.... HVA.... MVE.... DNPT.M. GZU.... SED.... HVA.... MVE....";
  PlayerMessage msgs[3] = { "", "", "" };
  gameState = newDracView(plays,msgs);
  decideDraculaMove(gameState);
  disposeDracView(gameState);
#else
  HunterView gameState;
  //char *plays = "GAT.... SED.... HZU.... MZU.... DC?.... GVA.... SMN.... HMU.... MMR....";
  char* plays = "GST.... SMA.... HSZ.... MSO.... DC?.V.. GFR.... SMA.... HKL.... MSO.... DSZT...";
   PlayerMessage msgs[3] = { "", "", "" };
   gameState = newHunterView(plays,msgs);
   decideHunterMove(gameState);
   disposeHunterView(gameState);
#endif 
   printf("Move: %s, Message: %s\n", latestPlay, latestMessage);
   return EXIT_SUCCESS;
}

// Saves characters from play (and appends a terminator)
// and saves characters from message (and appends a terminator)
void registerBestPlay (char *play, PlayerMessage message) {
   strncpy(latestPlay, play, MOVE_SIZE-1);
   latestPlay[MOVE_SIZE-1] = '\0';
     
   strncpy(latestMessage, message, MESSAGE_SIZE);
   latestMessage[MESSAGE_SIZE-1] = '\0';
}
