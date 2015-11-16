/**********************************************************************************************************************

                                            UBD - Unipolar Bipolar Driver

***********************************************************************************************************************
 * File name:               function.h
 * Creation date:           30.09.2015
 * Main creator:            A. Staub
 * Company:                 REFAST GmbH
 *                          Copyright (c) 2015 REFAST GmbH
***********************************************************************************************************************
 * Content overview :       - FUNCTION
***********************************************************************************************************************/

/**********************************************************************************************************************
 * Routine:                 FUNCTION_H

 * Description:
 * ...
 * 
 * Creator:                 A. Staub
 * Date of creation:        30.09.2015
 * Last modification on:    -
 * Modified by:             - 
 * 
 * Input:                   -
 * Output:                  -
 * Global variable:         -
***********************************************************************************************************************/

#ifndef FUNCTION_H
#define	FUNCTION_H


extern void funct_init(void);
extern void funct_IntToAscii(unsigned long uint32_DataByte,unsigned char uint8_sending);
extern unsigned long funct_AsciiToInt(void);
extern unsigned char funct_CheckTol(unsigned long uint32_Value,unsigned long uint32_Min,unsigned long uint32_Max);
extern void funct_FreqToTimer2(unsigned long uint32_Freq, unsigned int uint16_IntTime);
extern void funct_msToTimer2(unsigned int uint16_msTime, unsigned int uint16_IntTime);
extern unsigned int funct_ReadRamp(unsigned char uint8_kind,unsigned char uint8_sort,unsigned char uint8_ArrPos);
extern unsigned char funct_CheckCmdSILIM(void);
extern unsigned char funct_CheckCmdSMTYP(void);
extern void funct_LoadDeviceInfo(void);


#define _Active         1           //used for variable uint8_sending
#define _Inactive       0           //used for variable uint8_sending
#define _Acc            1           //acceleration used for variable uint8_kind
#define _Dec            0           //deceleration used for variable uint8_kind
#define _Step           1           //used for variable uint8_sort
#define _Freq           0           //used for variable uint8_sort


typedef struct                      //definitions of multiple variable for function
{
  volatile unsigned char uint8_ArrAscii[10];            //array of 10x byte to store the Ascii characters
  volatile unsigned char uint8_ArrAsciiPos;             //actually position of the last character in the array
} SFunct;                         //struct name is SFunct (S for struct)
extern SFunct    g_Funct;         //extern name is g_Funct (g for global)
 

#endif	/* FUNCTION_H */


