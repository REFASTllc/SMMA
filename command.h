/**********************************************************************************************************************

                                            UBD - Unipolar Bipolar Driver

***********************************************************************************************************************
 * File name:               command.h
 * Creation date:           03.10.2015
 * Main creator:            A. Staub
 * Company:                 REFAST GmbH
 *                          Copyright (c) 2015 REFAST GmbH
***********************************************************************************************************************
 * Content overview :       - COMMAND
***********************************************************************************************************************/

/**********************************************************************************************************************
 * Routine:                 COMMAND_H

 * Description:
 * ...
 * 
 * Creator:                 A. Staub
 * Date of creation:        02.10.2015
 * Last modification on:    -
 * Modified by:             - 
 * 
 * Input:                   -
 * Output:                  -
***********************************************************************************************************************/

#ifndef COMMAND_H
#define	COMMAND_H

extern void cmd_SILIM(void);
extern void cmd_ETESTIN(void);
extern void cmd_SRACC(void);
extern void cmd_SRDEC(void);
extern void cmd_RUN(void);
extern void cmd_SPHC(void);
extern void cmd_GVER(void);
extern void cmd_RAZ(void);
extern void cmd_SMTYP(void);
extern void cmd_GMTYP(void);
extern void cmd_STEST(void);
extern void cmd_GILIM(void);
extern void cmd_GPHC(void);
extern void cmd_GRACC(void);
extern void cmd_GRDEC(void);
extern void cmd_GRUN(void);
extern void cmd_BREAK(void);
extern void cmd_MUMOT(void);
extern void cmd_SCOILON(void);
extern void cmd_SSMOD(void);
extern void cmd_GSMOD(void);
extern void cmd_SBIT(void);
extern void cmd_CBIT(void);
extern void cmd_GBIT(void);
extern void cmd_SOUT(void);
extern void cmd_GOUT(void);
extern void cmd_GINP(void);
extern void cmd_SMCRSTP(void);
extern void cmd_GMCRSTP(void);
extern void cmd_SSWLIM(void);
extern void cmd_GSWLIM(void);
extern void cmd_GSWPOS(void);
extern void cmd_SPWMLIM(void);
extern void cmd_GPWMLIM(void);
extern void cmd_GPWMPOS(void);
extern void cmd_GPWMVAL(void);
extern void cmd_SSWTYP(void);
extern void cmd_GSWTYP(void);
extern void cmd_GFRQVAL(void);
//new commands:
extern void cmd_GINPB(void);
extern void cmd_SROM(void);
extern void cmd_RROM(void);

#endif	/* COMMAND_H */
