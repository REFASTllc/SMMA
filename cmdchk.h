/**********************************************************************************************************************

                                            UBD - Unipolar Bipolar Driver

***********************************************************************************************************************
 * File name:               cmdchk.h
 * Creation date:           05.12.2015
 * Main creator:            A. Staub
 * Company:                 REFAST GmbH
 *                          Copyright (c) 2015 REFAST GmbH
***********************************************************************************************************************
 * Content overview :       - CMDCHK
***********************************************************************************************************************/

/**********************************************************************************************************************
 * Routine:                 CMDCHK

 * Description:
 * ...
 * 
 * Creator:                 A. Staub
 * Date of creation:        05.12.2015
 * Last modification on:    -
 * Modified by:             - 
 * 
 * Input:                   -
 * Output:                  -
***********************************************************************************************************************/

#ifndef CMDCHK_H
#define	CMDCHK_H

extern void cmdchk_init(void);  
extern void cmdchk_check(void);
extern void cmdchk_def(void);

typedef struct                      //definitions of multiple variable for command
{
  volatile unsigned char uint8_ParamPos;        //parameter position of the command
  volatile unsigned char uint8_LetterPos;       //letter position of the command
  volatile unsigned long int uint32_Cmd1st4;        //first 4 letters of the command
  volatile unsigned long int uint32_Cmd2nd4;        //second 4 letters of the command
  volatile unsigned long int uint32_TempPara[101];  //Temporary parameter registers
  volatile unsigned char uint8_CmdID;           //command id
  volatile unsigned char uint8_GlobalLock;      //global lock for commands (0: disable / 1: enable)
} SCmdChk;                          //struct name is SCmdChk (S for struct)

#endif	/* CMDCHK_H */
