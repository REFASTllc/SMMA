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
 * Global variable:         -
***********************************************************************************************************************/

#ifndef COMMAND_H
#define	COMMAND_H

extern void cmd_init(void);  
extern void cmd_check(void);
extern void cmd_def(void);
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
//new commands:
extern void cmd_GINPB(void);

typedef struct                      //definitions of multiple variable for command
{
  volatile unsigned char uint8_ParamPos;        //parameter position of the command
  volatile unsigned char uint8_LetterPos;       //letter position of the command
  volatile unsigned long uint32_Cmd1st4;        //first 4 letters of the command
  volatile unsigned long uint32_Cmd2nd4;        //second 4 letters of the command
  volatile unsigned long uint32_TempPara[101];  //Temporary parameter registers
  //volatile unsigned char uint8_ErrCode;         //error code
  volatile unsigned char uint8_CmdID;           //command id
} SCmd;                             //struct name is SCmd (S for struct)
extern SCmd    g_Cmd;               //extern name is g_Cmd (g for global)

#endif	/* COMMAND_H */