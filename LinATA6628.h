/**********************************************************************************************************************

                                            UBD - Unipolar Bipolar Driver

***********************************************************************************************************************
 * File name:               LinATA6628.h
 * Creation date:           19.12.2015
 * Main creator:            A. Staub
 * Company:                 REFAST GmbH
 *                          Copyright (c) 2015 REFAST GmbH
***********************************************************************************************************************
 * Content overview :       - LINATA6628
***********************************************************************************************************************/

/**********************************************************************************************************************
 * Routine:                 LINATA6628_H

 * Description:
 * ...
 * 
 * Creator:                 A. Staub
 * Date of creation:        19.12.2015
 * Last modification on:    -
 * Modified by:             - 
 * 
 * Input:                   -
 * Output:                  -
***********************************************************************************************************************/

#ifndef LINATA6628_H
#define	LINATA6628_H

extern void LINATA6628_init(void);

typedef struct                  //definitions of multiple variable for timer1
{
  volatile unsigned short int uint16_TimeoutCounter;    //LIN counter for timeout (time base 1ms)
} SLin;                         //struct name is SLin (S for struct)


#endif	/* LINATA6628_H */
