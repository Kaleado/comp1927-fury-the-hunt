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
  char *plays = "GST.... SMA.... HMA.... MSO.... DMA.V.. GGE.... SMAVD.. HMAD... MSJ....";
  PlayerMessage msgs[3] = { "", "", "" };
  gameState = newDracView(plays,msgs);
  decideDraculaMove(gameState);
  disposeDracView(gameState);
#else
  HunterView gameState;
  //char *plays = "GAT....";
  //char *plays = "GAT.... SED.... HZU.... MZU.... DC?.... GVA.... SMN.... HMU.... MMR....";
  //char* plays = "GST.... SMA.... HSZ.... MSO.... DC?.V..";// GFR.... SMA.... HKL.... MSO.... DSZT..
  //char* plays = "GST.... SMA.... HSZ.... MSO.... DBD.V.. GST.... SMA.... HSZ.... MSO.... DD1T...";
  //char* plays = "GST.... SMA.... HSZ.... MSO.... DC?.V.. GFR.... SLS.... HKL.... MSJ.... DC?T... GST.... SSN.... HCD.... MSO.... DHIT... GFR.... SSR.... HKL.... MSJ.... DD1T... GST.... SMA.... HCD.... MSO.... DKLT... GFR.... SLS.... HKLTD.. MBC.... DC?T... GST.... SSN.... HCD.... MGA.... DC?T.V. GFR.... SSR.... HKL.... MKL.... DC?T.M. GST.... SMA.... HCD.... MCD.... DC?T.M. GFR.... SLS.... HKL.... MKL.... DD1T.M.";
  //char* plays = "GST.... SMA.... HSZ.... MSO.... DC?.V.. GBU.... SAL.... HBE.... MBE.... DHIT... GAM.... SAL.... HBE.... MBE.... DSRT... GAM.... SAL.... HBE.... MBE.... DSNT... GAM.... SAL.... HBE.... MBE.... DD2T... GAM.... SAL.... HBE.... MBE.... DALT... GBU.... SALTD.. HSJ.... MSJ.... DC?T.V. GAM.... SAL.... HBE.... MBE.... DC?T.M. GAM.... SAL.... HBE.... MBE.... DS?..M. GAM.... SAL.... HBE....";
  //char* plays = "GST.... SMA.... HSZ.... MSO.... DC?.V.. GPA.... SLS.... HJM.... MSJ.... DC?T... GLE.... SLS.... HBE.... MJM.... DC?T... GLE.... SLS.... HSJ.... MBE.... DD1T... GLE.... SLS.... HJM.... MSJ.... DC?T... GLE.... SLS.... HBE.... MJM.... DHIT... GLE.... SLS.... HSJ.... MBE.... DS?..V. GBU.... SSN.... HBE.... MKL.... DS?..M. GST.... SSR.... HKL.... MGA.... DS?..M. GMU.... STO.... HGA.... MCN.... DC?T.M. GMU.... SMR.... HBC.... MCN.... DC?T.M. GMU.... SMR.... HGA.... MCN.... DC?T.M. GMU.... SMR.... HBC.... MCN.... DC?T... GMU.... SMR.... HGA.... MCN.... DC?.V.. GMU.... SMR.... HBC.... MCN.... DC?T... GMU.... SMR.... HGA.... MCN.... DC?T.M. GMU.... SMR.... HBC.... MCN.... DD4T.M. GMU.... SMR.... HGA.... MCN.... DC?T.M. GMU.... SMR.... HBC.... MCN.... DC?T.M. GMU.... SMR.... HGA.... MCN.... DS?..V. GZA.... SMI.... HCN....";
  //char* plays = "GST.... SMA.... HSZ.... MSO.... DSZ.V.. GMU.... SGR.... HSZVD.. MSJ.... DC?T... GMI.... SAL.... HSZ.... MVA.... DC?T... GVE.... SAL.... HSZ.... MSA.... DC?T... GGO.... SAL.... HSZ.... MSO.... DD4T... GGO.... SAL.... HSZTD.. MVR.... DC?T... GGO.... SAL....";
  //char* plays = "GST.... SMA.... HSZ.... MSO.... DBA.V.. GST.... SMA.... HSZ.... MSO.... DTOT... GGE.... SGR.... HBD.... MSA.... DD2T... GMR.... SGR.... HVI.... MSO.... DS?.... GTOT... SGR.... HMU.... MSJ.... DC?T... GBATV.. SGR.... HNU.... MBE.... DC?T... GSR.... SGR.... HFR.... MKL.... DC?T... GSR.... SGR.... HFR.... MKL.... DNAT... GBO.... SGR.... HNU.... MBD.... DLET... GCF.... SGR.... HFR.... MVI.... DD1T... GTO.... SGR.... HNU.... MMU.... DEC..M. GSR.... SAL.... HST.... MZA.... DS?..M. GTO.... SSR.... HMU.... MSJ.... DS?..M. GTO.... SSR.... HMU.... MSJ.... DC?.VM. GTO.... STO.... HMU.... MZA.... DC?T.M. GTO.... STO.... HMU.... MZA.... DTOT.M. GTOTD.. STOD... HMI.... MMU.... DC?T...";
  //char* plays = "GST.... SMA.... HSZ.... MSO.... DSN.V.. GST.... SMA.... HSZ.... MSO.... DBB.... GST.... SMA.... HSZ.... MSO.... DC?T... GGE.... SCA.... HJM.... MBC.... DHIT... GPA.... SMA.... HJM.... MGA.... DD2T... GPA.... SMA.... HJM.... MGA.... DPAT... GPATD.. SGR.... HZA.... MKL.... DCFT.V. GPA.... SGR.... HZA.... MKL.... DMRT... GCFT... SMA.... HMU.... MBC.... DC?T.M. GCF.... SCA.... HZU.... MGA.... DC?T.M. GCF.... SMA.... HMU.... MCN.... DMUT.M. GGE.... SGR.... HMUTD.. MGA.... DZUT... GPA.... SMA.... HZUTD.. MCN.... DMIT... GPA.... SMA.... HJM.... MCN.... DHI.VM. GCF.... SCA.... HJM.... MVR.... DD1T.M. GCF.... SMA.... HJM.... MCN.... DC?T.M. GCF.... SCA.... HJM.... MVR.... DMRT... GCF.... SMA.... HJM.... MCN.... DS?.... GCF.... SMA.... HJM.... MCN.... DS?..M. GCF.... SMA.... HJM.... MCN.... DS?..V. GMRT... SGR.... HZA.... MGA.... DC?T.M. GMR....";
  //char* plays = "GST.... SMA.... HMA.... MSO.... DCD.V.. GMU.... SGR.... HGR.... MBC.... DKLT... GNU.... SMA.... HAL.... MSO.... DHIT... GNU.... SMA.... HAL.... MVR.... DD2T... GPR.... SSN.... HGR.... MVR.... DBCT... GNU.... SLS.... HMA.... MBS.... DBET... GFR.... SLS.... HCA.... MCN.... DC?T.V. GFR.... SLS.... HCA.... MCN.... DZAT.M. GNU.... SSN.... HAO.... MBCT... DMUT.M. GNU.... SBB.... HCA.... MBET... DC?T.M. GPR.... SBB.... HGR.... MSZ.... DC?T... GVI.... SBO.... HCA.... MKL.... DC?T... GPR.... SBO.... HMA.... MBC.... DMRT.M. GPR.... SBO.... HMA.... MBC.... DHI.VM. GPR.... SNA.... HGR.... MSO.... DC?T.M. GVI.... SNA.... HCA.... MSO.... DC?T.M. GMU.... SPA.... HLS.... MVA.... DD4T.M. GMU.... SNA.... HSN.... MSA.... DC?T.M. GNU.... SBO.... HBB.... MSA.... DC?T.M. GNU.... SBO.... HBB.... MSA.... DFLT.V. GMU.... SNA.... HBO.... MVA.... DHIT.M. GVI.... SNA.... HTO.... MAT.... DC?T.M. GPR.... SPA.... HBA.... MIO.... DD3T.M. GVI.... SLE.... HMS.... MTS.... DC?T.M. GZA.... SBU.... HMS.... MTS.... DVET.M. GZA.... SBU.... HMS.... MTS.... DAS..M. GMU.... SLE.... HMS.... MTS.... DS?..M. GVET... SNA.... HMS.... MTS.... DC?T.M. GMU.... SPA.... HBA.... MMS.... DHIT.M. GVI.... SGE.... HTO.... MMR.... DC?T.M. GBD.... SGE.... HCF.... MGE.... DC?T... GBD.... SGE.... HCF.... MGE.... DSZT... GSZTD.. SST.... HMR.... MST.... DZAT... GJM.... SST.... HMR.... MBU.... DC?T.M. GZAT... SZU.... HMS.... MBU.... DC?T.M. GBD.... SGE.... HBA.... MCO.... DD3T.M. GKL.... SGE.... HSR.... MHA.... DC?T.M. GKL.... SGE.... HSR.... MHA.... DKLT... GKLTD.. SST.... HTO.... MCO.... DC?T...";
  //char* plays = "GST.... SMA.... HSZ.... MSO.... DAS.... GST.... SMA.... HSZ.... MSO.... DVET... GGE.... SSR.... HBE.... MVR.... DC?T... GCF.... STO.... HSZ.... MBS.... DZUT... GTO.... SBA.... HBE.... MBS.... DC?T... GBA.... SBA.... HSZ.... MBS.... DD2T... GBA.... SBA.... HSZ.... MBS.... DMIT... GBA.... SBA.... HBE.... MBS.... DC?T.M. GBA.... SBA.... HSZ.... MBS.... DC?T.M. GBA.... SBA.... HBE.... MBS.... DC?T.M. GBA.... SBA.... HSZ.... MBS.... DVIT.M. GBA.... SBA.... HSZ.... MBS.... DD2T.M. GBA.... SBA.... HBE.... MBS.... DSZT.M. GBA.... SBA.... HSZTD.. MBS.... DC?.VM. GTO.... STO.... HBE.... MBS.... DC?T.M. GBA.... SBA.... HSZ.... MBS.... DCNT.M. GBA.... SBA.... HSZ.... MBS.... DVRT.M. GTO.... STO.... HBE.... MBS.... DC?T.M. GBA.... SBA.... HSZ.... MBS.... DD2T... GBA.... SBA.... HBE.... MBS.... DHIT.V. GTO.... STO.... HKL.... MBS.... DBS..M. GTO.... STO.... HKL.... MBS.... DS?..M. GBA.... SBA.... HSZ.... MBS.... DC?T.M. GTO.... STO.... HBE.... MBS.... DC?T.M. GBA.... SBA.... HSZ.... MBS.... DD5T.M. GBA.... SBA.... HBE.... MBS.... DHIT.M. GBA.... SBA.... HBE.... MBS.... DCN.V.. GMS.... SMS.... HSZ.... MCNVD.. DC?T... GTS.... STS.... HJM.... MBC.... DKLT.M. GTS.... STS.... HSZ.... MKLTD.. DCDT.M. GTS.... STS.... HJM.... MSJ.... DD2T.M. GTS.... STS.... HJM.... MSJ.... DSZT.M. GTS.... STS.... HSZTD.. MBE.... DBET... GTS.... STS....";
  char* plays = "GST.... SMA.... HMA.... MSO.... DCD.V.. GMU.... SGR.... HGR.... MBC.... DGAT... GNU.... SMA.... HAL.... MSO.... DCNT... GNU.... SMA.... HAL.... MVR.... DBCT... GPR.... SSN.... HGR.... MVR.... DC?T... GNU.... SLS.... HMA.... MBS.... DSZT... GFR.... SLS.... HCA.... MCNT... DC?T.V. GFR.... SLS.... HCA.... MCN.... DVIT.M. GNU.... SSN.... HAO.... MBCT... DZAT... GNU.... SBB.... HCA.... MBE.... DC?T... GPR.... SBB.... HGR.... MSZT... DC?T.M. GVIT... SBO.... HCA.... MKL.... DC?T... GPR.... SBO.... HMA.... MBC.... DC?T.M. GPR.... SBO.... HMA.... MBC.... DCO.V.. GPR.... SNA.... HGR.... MSO.... DC?T.M. GVI.... SNA.... HCA.... MSO.... DBUT.M. GMU.... SPA.... HLS.... MVA.... DPAT.M. GST.... SPATD.. HSN.... MSJ.... DC?T.M. GBUT... SPA.... HBB.... MJM.... DNAT.M. GBU.... SPA.... HBB.... MJM.... DBOT.V. GCO.... SBU.... HBOTD.. MZA.... DCFT.M. GAM.... SPA.... HTO.... MJM.... DGET... GCO.... SGETD.. HMR.... MZA.... DC?T... GCO.... SCFT... HGE.... MVI.... DNUT.M. GFR.... SNAT... HMR.... MPR.... DC?T... GFR.... SJM.... HMR.... MPR.... DVET... GFR.... SJM.... HMS.... MNUT... DHI.V.. GLI.... SSZ.... HAO.... MFR.... DC?T... GLI.... SZA.... HEC.... MFR.... DC?T.M. GNU.... SSJ.... HEC.... MLI.... DC?T... GFR.... SSJ.... HLE.... MHA.... DC?T.M. GFR.... SSJ.... HLE.... MHA.... DNPT.M. GLI.... SJM.... HPA.... MBR.... DC?T.V. GHA.... SZA.... HBU.... MBR.... DS?..M. GCO.... SMU.... HLE.... MBR.... DS?..M. GST.... SST.... HNA.... MBR.... DC?T.M. GBU.... SCO.... HCF.... MPR.... DC?T.M. GBU.... SCO.... HCF.... MPR.... DSAT.M. GCO.... SHA.... HPA.... MNU.... DC?T.M. GHA.... SBR.... HST.... MFR.... DC?.V.. GBR.... SHA.... HPA.... MNU.... DC?T... GHA.... SHA.... HCF.... MMU.... DC?T.M. GHA.... SLI.... HGE.... MVI.... DC?T.M. GHA.... SLI.... HGE.... MVI.... DKLT.M. GNS.... SHA.... HST.... MPR.... DCDT.M. GNS.... SCO.... HMU.... MNU.... DC?T.V. GHA.... SLI.... HMI.... MLI.... DC?T.M. GHA.... SBR.... HMR.... MFR.... DC?T.M. GLI.... SHA.... HGO.... MFR.... DC?T.M. GLI.... SHA.... HGO.... MFR.... DBET.M. GFR.... SCO.... HTS.... MNU.... DKLT.M. GFR.... SFR.... HNP.... MFR.... DC?T.M. GLI.... SNU.... HRO.... MCO.... DHI.VM. GNU.... SFR.... HBI.... MAM.... DC?T.M. GFR.... SCO.... HAS.... MBU.... DC?T.M. GFR.... SCO.... HAS.... MBU.... DSJT.M. GFR.... SBU.... HIO.... MST.... DSOT.M. GLI.... SPA.... HIO.... MPA.... DC?T.M. GLI.... SLE.... HSA.... MPA.... DC?T.V. GNU.... SBU.... HSA.... MST.... DCDT.M. GST.... SST.... HVA.... MMU.... DC?T.M. GST.... SST.... HVA.... MMU.... DC?T.M. GZU.... SBU.... HSOT... MMU.... DC?T... GMU.... SPA.... HSA.... MNU.... DS?..M. GVE.... SNA.... HVA.... MST.... DS?..M. GVE.... SNA.... HSJ.... MGE.... DC?.VM. GAS.... SBO.... HSO.... MPA.... DSOT.M. GAS.... SCF.... HSOTD.. MST.... DC?T.M. GAS.... STO.... HBE.... MZU.... DC?T.M. GBI.... SSR.... HBC.... MGE.... DC?T... GNP.... SBA.... HBE.... MGE.... DS?.... GNP.... STO.... HSJ.... MCF.... DC?T.V. GBI.... SBO.... HSO.... MPA.... DSOT... GBI.... SCF.... HSOTD.. MST.... DC?T.M. GNP.... SPA....";
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
