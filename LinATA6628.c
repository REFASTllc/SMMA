/**********************************************************************************************************************

                                            UBD - Unipolar Bipolar Driver

***********************************************************************************************************************
 * File name:               LinATA6628.c
 * Creation date:           19.12.2015
 * Main creator:            Alain Staub
 * Company:                 REFAST GmbH
 *                          Copyright (c) 2015 REFAST GmbH
***********************************************************************************************************************
 * Content overview:        - LINATA6628_init
***********************************************************************************************************************/


#include "includes.h"

SLin g_LIN; 
extern SParam g_Param;


/**********************************************************************************************************************
 * Routine:                 LINATA6628_init

 * Description:
 * Initialization of the LIN ATA6628 driver.
 * 
 * Creator:                 A. Staub
 * Date of creation:        19.12.2015
 * Last modification on:    -
 * Modified by:             - 
 * 
 * Input:                   -
 * Output:                  -
***********************************************************************************************************************/
void LINATA6628_init(void)
{
    if(g_Param.uint32_LinSpd <= 20000)  //baud rate smaller than 20kBaud
    {
        oSPModeSignalLIN = 0;   //disable high speed mode
    }
    else
    {
        oSPModeSignalLIN = 1;   //enable high speed mode
    }
    
    oEnaLINDriver = 1;          //enable LIN driver
}   //end of LINATA6628_init