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
  char *plays = "GBU.... SBU.... HBU.... MBU.... DMA.V.. GBU.... SPA.... HPA.... MBU.... DCAT... GPA.... SCF.... HLE.... MBU.... DGRT... GCF.... SMR.... HLE.... MPA.... DALT... GMR.... SPA.... HBU.... MCF.... DSRT... GPA.... SPA.... HPA.... MMR.... DSNT... GPA.... SCF.... HLE.... MPA.... DLST.V. GCF.... SMR.... HLE.... MPA.... DCAT.M. GMR.... SPA.... HBU.... MCF.... DGRT.M. GPA.... SPA.... HPA.... MMR.... DMAT.M. GPA.... SCF.... HLE.... MPA.... DALT.M. GCF.... SMR.... HLE.... MPA.... DSRT.M. GMR.... SPA.... HBU.... MCF.... DSNT.M. GPA.... SPA.... HPA.... MMR.... DLS.VM. GPA.... SCF.... HLE.... MPA.... DCAT.M. GCF.... SMR.... HLE.... MPA.... DGRT.M. GMR.... SPA.... HBU.... MCF.... DMAT.M. GPA.... SPA.... HPA.... MMR.... DALT.M. GPA.... SCF.... HLE.... MPA.... DMS..M. GCF.... SMR.... HLE.... MPA.... DAO..V. GMR.... SPA.... HBU.... MCF.... DCAT.M. GPA.... SPA.... HPA.... MMR.... DGRT.M. GPA.... SCF.... HLE.... MPA.... DMAT.M. GCF.... SMR.... HLE.... MPA.... DLST.M. GMR.... SPA.... HBU.... MCF.... DSNT... GPA.... SBU.... HBO.... MMR.... DBB.... GBU.... SBO.... HCF.... MPA.... DAO..M. GBO.... SCF.... HMR.... MBU.... DCAT.M. GCF.... SMR.... HBU.... MBO.... DGRT.M. GMR.... SBU.... HBO.... MCF.... DALT.M. GBU.... SBO.... HCF.... MMR.... DMAT.M. GBO.... SCF.... HMR.... MBU.... DLST... GCF.... SMR.... HBU.... MBO.... DAO.... GMR.... SBU.... HBO.... MCF.... DCAT.M. GBU.... SBO.... HCF.... MMR.... DGRT.M. GBO.... SCF.... HMR.... MBU.... DALT.M. GCF.... SMR.... HBU.... MBO.... DMAT.M. GMR.... SBU.... HBO.... MCF.... DLST.M. GBU.... SBO.... HCF.... MMR.... DAO.... GBO.... SCF.... HMR.... MBU.... DCA.VM. GCF.... SMR.... HBU.... MBO.... DGRT.M. GMR.... SBU.... HBO.... MCF.... DALT.M. GBU.... SBO.... HCF.... MMR.... DMAT.M. GBO.... SCF.... HMR.... MBU.... DLST.M. GCF.... SMR.... HBU.... MBO.... DAO.... GMR.... SBU.... HBO.... MCF.... DCAT.V. GBU.... SBO.... HCF.... MMR.... DGRT.M. GBO.... SCF.... HMR.... MBU.... DALT.M. GCF.... SMR.... HBU.... MBO.... DMAT.M. GMR.... SBU.... HBO.... MCF.... DLST.M. GBU.... SBO.... HCF.... MMR.... DAO.... GBO.... SCF.... HMR.... MBU.... DCAT.M. GCF.... SMR.... HBU.... MBO.... DGR.VM. GMR.... SBU.... HBO.... MCF.... DALT.M. GBU.... SBO.... HCF.... MMR.... DMAT.M. GBO.... SCF.... HMR.... MBU.... DLST.M. GCF.... SMR.... HBU.... MBO.... DAO.... GMR.... SBU.... HBO.... MCF.... DCAT.M. GBU.... SBO.... HCF.... MMR.... DGRT.V. GBO.... SCF.... HMR.... MBU.... DALT.M. GCF.... SMR.... HBU.... MBO.... DMAT.M. GMR.... SBU.... HBO.... MCF.... DLST.M. GBU.... SBO.... HCF.... MMR.... DAO.... GBO.... SCF.... HMR.... MBU.... DCAT.M. GCF.... SMR.... HBU.... MBO.... DGRT.M. GMR.... SBU.... HBO.... MCF.... DAL.VM. GBU.... SBO.... HCF.... MMR.... DMAT.M. GBO.... SCF.... HMR.... MBU.... DLST.M. GCF.... SMR.... HBU.... MBO.... DAO.... GMR.... SBU.... HBO.... MCF.... DCAT.M. GBU.... SSR.... HCF.... MBO.... DGRT.M. GSR.... SALV... HBO.... MSR.... DALT... GALTD.. SBA.... HSR.... MSR.... DMAT.M. GAL.... SSR.... HSR.... MAL.... DLST.M. GAL.... SSR.... HAL.... MBA.... DAO.... GSN.... SAL.... HBA.... MSR.... DEC..M. GSN.... SBA.... HSR.... MSR.... DLOT.M. GSN.... SSR.... HSR.... MAL.... DMNT... GLST... SSR.... HAL.... MBA.... DED.VM. GSN.... SAL.... HBA.... MSR.... DNS.... GSN.... SBA.... HSR.... MSR.... DAMT... GSN.... SSR.... HSR.... MAL.... DCOT... GLS.... SSR.... HAL.... MBA.... DFRT.M. GSN.... SAL.... HBA.... MSR.... DLIT.M. GSN.... SBA.... HSR.... MSR.... DBRT.V. GSN.... SSR.... HSR.... MAL.... DPRT... GLS.... SSR.... HAL.... MBA.... DVIT.M. GSN.... SAL.... HBA.... MSR.... DBDT.M. GSN.... SBA.... HSR.... MSR.... DKLT.M. GSN.... SSR.... HSR.... MAL.... DCDT.M. GLS.... SSR.... HAL.... MBA.... DGAT.M. GSN.... SAL.... HBA.... MSR.... DBC.VM. GSN.... SBA.... HSR.... MSR.... DBET.M. GSN.... SSR.... HSR.... MAL.... DSZT.M. GLS.... SSR.... HAL.... MBA.... DKLT.M. GSN.... SAL.... HBA.... MSR.... DCDT.M. GSN.... SBA.... HSR.... MSR.... DGAT.M. GSN.... SSR.... HSR.... MAL.... DBCT.V. GLS.... SSR.... HAL.... MBA.... DBET.M. GSN.... SAL.... HBA.... MSR.... DSZT.M. GSN.... SBA.... HSR.... MSR.... DKLT.M. GSN.... SSR.... HSR.... MAL.... DCDT.M. GLS.... SSR.... HAL.... MBA.... DGAT.M. GSN.... SAL.... HBA.... MSR.... DBCT.M. GSN.... SBA.... HSR.... MSR.... DBE.VM. GSN.... SSR.... HSR.... MAL.... DSZT.M. GLS.... SPA.... HSN.... MBA.... DKLT.M. GAL.... SNA.... HLS.... MBO.... DCDT.M. GAL.... SCF.... HLS.... MPA.... DGAT.M. GBA.... SNA.... HAL.... MNA.... DBCT.M. GBO.... SCF.... HAL.... MCF.... DBET.V. GPA.... SNA.... HBA.... MNA.... DSOT.M. GNA.... SCF.... HBO.... MCF.... DVRT.M. GCF.... SNA.... HPA.... MNA.... DCNT.M. GNA.... SCF.... HNA.... MCF.... DGAT.M. GCF.... SNA.... HCF.... MNA.... DCDT.M. GNA.... SCF.... HNA.... MCF.... DKLT.M. GCF.... SNA.... HCF.... MNA.... DBE.VM. GNA.... SCF.... HNA.... MCF.... DBCT.M. GCF.... SNA.... HCF.... MNA.... DCNT.M. GNA.... SCF.... HNA.... MCF.... DGAT.M. GCF.... SNA.... HCF.... MNA.... DCDT.M. GNA.... SCF.... HNA.... MCF.... DKLT.M. GCF.... SNA.... HCF.... MNA.... DBET.V. GNA.... SCF.... HNA.... MCF.... DBCT.M. GCF.... SNA.... HCF.... MNA.... DCNT.M. GNA.... SCF.... HNA.... MCF.... DGAT.M. GCF.... SNA.... HCF.... MNA.... DCDT.M. GNA.... SCF.... HNA.... MCF.... DKLT.M. GCF.... SNA.... HCF.... MNA.... DBET.M. GNA.... SCF.... HNA.... MCF.... DBC.VM. GCF.... SNA.... HCF.... MNA.... DCNT.M. GNA.... SCF.... HNA.... MCF.... DGAT.M. GCF.... SNA.... HCF.... MNA.... DCDT.M. GNA.... SCF.... HNA.... MCF.... DKLT.M. GCF.... SNA.... HCF.... MNA.... DBET.M. GNA.... SLE.... HNA.... MLE.... DBCT.V. GLE.... SLE.... HLE.... MNA.... DCNT.M. GLE.... SCO.... HNA.... MLE.... DBS..M. GCO.... SHA.... HLE.... MLE.... DVRT.M. GHA.... SHA.... HLE.... MCO.... DSOT.M. GHA.... SHA.... HCO.... MHA.... DSAT.M. GHA.... SFR.... HHA.... MHA.... DIO..M. GFR.... SLI.... HHA.... MHA.... DAT.VM. GLI.... SFR.... HHA.... MFR.... DVAT... GFR.... SCO.... HFR.... MLI.... DSJT.M. GCO.... SCO.... HLI.... MFR.... DBET.M. GCO.... SHA.... HFR.... MCO.... DBCT.M. GHA.... SHA.... HCO.... MCO.... DCNT... GHA.... SHA.... HCO.... MHA.... DBS..V. GHA.... SFR.... HHA.... MHA.... DVRT.M. GFR.... SLI.... HHA.... MHA.... DSOT.M. GLI.... SFR.... HHA.... MFR.... DSAT.M. GFR.... SCO.... HFR.... MLI.... DIO..M. GCO.... SCO.... HLI.... MFR.... DATT.M. GCO.... SHA.... HFR.... MCO.... DVAT... GHA.... SHA.... HCO.... MCO.... DSJ.VM. GHA.... SHA.... HCO.... MHA.... DBET.M. GHA.... SFR.... HHA.... MHA.... DBCT.M. GFR.... SLI.... HHA.... MHA.... DCNT... GLI.... SFR.... HHA.... MFR.... DBS..M. GBR.... SST.... HPR.... MLI.... DVRT.M. GPR.... SST.... HPR.... MBR.... DSOT.V. GFR.... SPA.... HPR.... MPR.... DSAT.M. GLI.... SNA.... HVE.... MFR.... DVAT.M. GBR.... SLE.... HFL.... MLI.... DATT.M. GPR.... SPA.... HRO.... MBR.... DIO.... GFR.... SPA.... HNP.... MPR.... DBS..M. GLI.... SNA.... HNP.... MFR.... DCNT.M. GBR.... SLE.... HMI.... MLI.... DGA.VM. GPR.... SPA.... HGO.... MBR.... DBCT.M. GFR.... SPA.... HMR.... MPR.... DBET.M. GLI.... SNA.... HTO.... MFR.... DKLT... GBR.... SLE.... HBA.... MLI.... DCDT... GPR.... SPA.... HBA.... MBR.... DHIT.M. GFR.... SPA.... HSR.... MPR....";
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
