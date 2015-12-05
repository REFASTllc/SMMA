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
extern void funct_FrontLED(unsigned char uint8_color);
extern void funct_OutputHandler(unsigned char uint8_ToDo,unsigned char uint8_value);
extern unsigned char funct_IOhandler(unsigned char uint8_ToDo,unsigned char uint8_IO,unsigned char uint8_value);

#define _Active         1           //used for variable uint8_sending
#define _Inactive       0           //used for variable uint8_sending
#define _Acc            1           //acceleration used for variable uint8_kind
#define _Dec            0           //deceleration used for variable uint8_kind
#define _Step           1           //used for variable uint8_sort
#define _Freq           0           //used for variable uint8_sort
#define _FLEDblack      0           //front LED switched off used for uint8_color
#define _FLEDgreen      1           //front LED = green used for uint8_color
#define _FLEDred        2           //front LED = red used for uint8_color
#define _FLEDorange     3           //front LED = orange used for uint8_color
#define _ClrBit         0           //used for variable uint8_ToDo and to clear the bit/output (reason for value 0)
#define _SetBit         1           //used for variable uint8_ToDo and to set the bit/output (rason for value 1)
#define _GetPort        2           //used for variable uint8_ToDo
#define _SetPort        3           //used for variable uint8_ToDo
#define _GetBit         4           //used for variable uint8_ToDo
#define _Input          0           //used for uint8_IO
#define _Output         1           //used for uint8_IO


typedef struct                      //definitions of multiple variable for function
{
  volatile unsigned char uint8_ArrAscii[10];            //array of 10x byte to store the Ascii characters
  volatile unsigned char uint8_ArrAsciiPos;             //actually position of the last character in the array
} SFunct;                         //struct name is SFunct (S for struct)
extern SFunct    g_Funct;         //extern name is g_Funct (g for global)
 

#endif	/* FUNCTION_H */


