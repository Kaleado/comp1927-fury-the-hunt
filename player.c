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
  char *plays = "GGE.... SGE.... HGE.... MGE.... DMA.V.. GMR.... SPA.... HGE.... MCF.... DCAT... GMR.... SBU.... HCF.... MGE.... DGRT... GCF.... SAM.... HGE.... MPA.... DALT... GGE.... SCO.... HPA.... MBU.... DMS.... GMR.... SCO.... HBU.... MAM.... DAO.... GMR.... SBO.... HAM.... MCO.... DGWT.V. GCF.... SBB.... HCO.... MCO.... DDUT.M. GGE.... SBO.... HCO.... MBO.... DIR..M. GMR.... SMA.... HBO.... MBB.... DLVT.M. GMR.... SBO.... HBB.... MBO.... DMNT... GCF.... SBB.... HBO.... MMA.... DEDT... GGE.... SBO.... HMA.... MBO.... DNS..M. GMR.... SMA.... HBO.... MBB.... DAM.VM. GMR.... SBO.... HBB.... MBO.... DCOT... GCF.... SBB.... HBO.... MMR.... DLIT.M. GBO.... SBO.... HMR.... MMR.... DBRT.M. GBB.... SMR.... HMR.... MZU.... DHAT.M. GBO.... SMR.... HZU.... MMR.... DNS.... GTO.... SZU.... HMR.... MMS.... DAMT.V. GBO.... SMR.... HMS.... MAL.... DCOT.M. GBB.... SMS.... HAL.... MMA.... DFRT.M. GBO.... SAL.... HMA.... MAL.... DLIT.M. GTO.... SMA.... HAL.... MSR.... DBRT.M. GBO.... SAL.... HSR.... MMR.... DHAT... GBB.... SSR.... HMR.... MZU.... DNS..M. GBO.... SMR.... HZU.... MMR.... DED.VM. GTO.... SZU.... HMR.... MMS.... DMNT.M. GBO.... SMR.... HMS.... MAL.... DLVT.M. GBB.... SMS.... HAL.... MMA.... DSWT.M. GBO.... SAL.... HMA.... MAL.... DLOT.M. GTO.... SMA.... HAL.... MSR.... DPLT... GBO.... SAL.... HSR.... MMR.... DEC..V. GBB.... SSR.... HMR.... MZU.... DLET.M. GBO.... SMR.... HZU.... MMR.... DBUT.M. GTO.... SZU.... HMR.... MMS.... DAMT.M. GBO.... SMR.... HMS.... MAL.... DCOT.M. GBB.... SMS.... HAL.... MMA.... DFRT.M. GBO.... SAL.... HMA.... MAL.... DLIT... GTO.... SMA.... HAL.... MSR.... DBR.VM. GBO.... SAL.... HSR.... MMR.... DHAT.M. GBB.... SSR.... HMR.... MZU.... DNS..M. GBO.... SMR.... HZU.... MMR.... DEDT.M. GTO.... SZU.... HMR.... MMS.... DMNT.M. GBO.... SMR.... HMS.... MAL.... DLVT.M. GBB.... SMS.... HAL.... MMA.... DSWT.V. GBO.... SAL.... HMA.... MAL.... DLOT.M. GTO.... SMA.... HAL.... MSR.... DPLT... GBO.... SAL.... HSR.... MMR.... DEC..M. GBB.... SSR.... HMR.... MZU.... DLET.M. GBO.... SMR.... HZU.... MMR.... DBUT.M. GTO.... SZU.... HMR.... MMS.... DAMT.M. GBO.... SMR.... HMS.... MAL.... DCO.VM. GBB.... SMS.... HAL.... MMA.... DFRT.M. GBO.... SAL.... HMA.... MAL.... DLIT... GTO.... SMA.... HAL.... MSR.... DBRT.M. GBO.... SAL.... HSR.... MMR.... DHAT.M. GBB.... SSR.... HMR.... MZU.... DNS..M. GBO.... SMR.... HZU.... MMR.... DEDT.V. GTO.... SZU.... HMR.... MMS.... DMNT.M. GBO.... SMR.... HMS.... MAL.... DLVT.M. GBB.... SMS.... HAL.... MMA.... DSWT.M. GBO.... SAL.... HMA.... MAL.... DLOT.M. GTO.... SMA.... HAL.... MSR.... DPLT... GBO.... SAL.... HSR.... MMR.... DEC..M. GBB.... SSR.... HMR.... MZU.... DLE.VM. GBO.... SMR.... HZU.... MMR.... DBUT.M. GTO.... SMI.... HMR.... MBUTD.. DAMT.M. GBO.... SMU.... HBU.... MSR.... DNS..M. GPA.... SST.... HSR.... MSN.... DEDT.M. GSR.... SPA.... HSN.... MMA.... DMNT... GBA.... SNA.... HMA.... MGR.... DLVT.V. GMS.... SBO.... HGR.... MCA.... DSWT... GMR.... SBB.... HCA.... MGR.... DLOT.M. GBU.... SBO.... HGR.... MCA.... DPLT... GSR.... SBO.... HCA.... MGR.... DEC..M. GSN.... SPA.... HGR.... MCA.... DNS..M. GMA.... SSR.... HCA.... MGR.... DAMT.M. GGR.... SBA.... HGR.... MCA.... DBU.VM. GCA.... SMS.... HCA.... MGR.... DCOT.M. GGR.... SMR.... HGR.... MCA.... DLIT.M. GCA.... SBUV... HCA.... MGR.... DBRT... GGR.... SSR.... HGR.... MCA.... DPRT... GCA.... SSN.... HCA.... MGR.... DVIT.M. GGR.... SMA.... HGR.... MCA.... DBDT... GCA.... SGR.... HCA.... MGR.... DKLT.M. GGR.... SCA.... HGR.... MCA.... DCDT.M. GCA.... SGR.... HCA.... MGR.... DGAT.M. GGR.... SCA.... HGR.... MCA.... DBCT.M. GCA.... SGR.... HCA.... MGR.... DBET.M. GGR.... SCA.... HGR.... MCA.... DSZT.M. GCA.... SGR.... HCA.... MGR.... DKL.VM. GGR.... SCA.... HGR.... MCA.... DCDT.M. GCA.... SGR.... HCA.... MGR.... DGAT.M. GGR.... SCA.... HGR.... MCA.... DBCT.M. GCA.... SGR.... HCA.... MGR.... DBET.M. GGR.... SCA.... HGR.... MCA.... DSZT.M. GCA.... SGR.... HCA.... MGR.... DKLT.V. GGR.... SCA.... HGR.... MCA.... DCDT.M. GCA.... SGR.... HCA.... MGR.... DGAT.M. GGR.... SCA.... HGR.... MCA.... DBCT.M. GCA.... SGR.... HCA.... MGR.... DBET.M. GGR.... SCA.... HGR.... MCA.... DSZT.M. GGR.... SAO.... HGR.... MAO.... DKLT.M. GGR.... SEC.... HGR.... MEC.... DCD.VM. GGR.... SAO.... HGR.... MAO.... DGAT.M. GGR.... SEC.... HGR.... MEC.... DBCT.M. GGR.... SAO.... HGR.... MAO.... DBET.M. GGR.... SEC.... HGR.... MEC.... DSJT.M. GGR.... SAO.... HGR.... MAO.... DSOT.M. GGR.... SEC.... HGR.... MEC.... DVRT.V. GGR.... SAO.... HGR.... MAO.... DCNT.M. GGR.... SEC.... HGR.... MEC.... DBCT.M. GGR.... SAO.... HGR.... MAO.... DKLT.M. GGR.... SEC.... HGR.... MEC.... DCDT.M. GGR.... SAO.... HGR.... MAO.... DGAT.M. GGR.... SEC.... HGR.... MEC.... DD2T.M. GGR.... SAO.... HGR.... MAO.... DHI.VM. GGR.... SEC.... HGR.... MEC.... DTP..M. GGR.... SAO.... HGR.... MAO....";
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
  char* plays = "GST.... SMA.... HMA.... MSO.... DCD.V.. GMU.... SGR.... HGR.... MBC.... DGAT... GNU.... SMA.... HAL.... MSO.... DCNT... GNU.... SMA.... HAL.... MVR.... DBCT... GPR.... SSN.... HGR.... MVR.... DC?T... GNU.... SLS.... HMA.... MBS.... DC?T... GFR.... SLS.... HCA.... MCNT... DD2T.V. GFR.... SLS.... HCA.... MCN.... DC?T.M. GNU.... SSN.... HAO.... MBCT... DBET... GNU.... SBB.... HCA.... MBETD.. DHIT... GPR.... SBB.... HGR.... MSZ.... DKLT.M. GVI.... SBO.... HCA.... MKLT... DC?T.M. GPR.... SBO.... HMA....";
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
