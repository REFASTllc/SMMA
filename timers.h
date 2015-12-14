/**********************************************************************************************************************

                                            UBD - Unipolar Bipolar Driver

***********************************************************************************************************************
 * File name:               timers.h
 * Creation date:           06.08.2015
 * Main creator:            J. Rebetez
 * Company:                 REFAST GmbH
 *                          Copyright (c) 2015 REFAST GmbH
***********************************************************************************************************************
 * Content overview :       - TIMERS
***********************************************************************************************************************/

/**********************************************************************************************************************
 * Routine:                 TIMERS_H

 * Description:
 * ...
 * 
 * Creator:                 J. Rebetez
 * Date of creation:        08.08.2015
 * Last modification on:    -
 * Modified by:             - 
 * 
 * Input:                   -
 * Output:                  -
***********************************************************************************************************************/

#ifndef TIMERS_H
#define	TIMERS_H

extern void timers_Init(unsigned char timerx);
extern void timers_SetInterrupt(unsigned char timerx, unsigned char action);
extern void timers_Set(unsigned char timer, unsigned char status, unsigned long int valueTMRReg, unsigned long int valuePRReg);

#define	_TIMER1		1
#define	_TIMER2		2
#define _TIMER3		3
#define	_TIMER4		4
#define _TIMER5		5
#define _TIMER23	6
#define _TIMER45	7

#define _ENABLE		1
#define	_DISABLE	0


typedef struct                  //definitions of multiple variable for timer2
{
  volatile unsigned short int uint16_LastTime;                //last time to wait
  volatile unsigned short int uint16_Count;                   //counter, how many time the interrupt time
  volatile unsigned short int uint16_IntTime;                 //interrupt time in 100ns steps  
} STimer2;                      //struct name is STimer2 (S for struct)

#endif	/* TIMERS_H */

