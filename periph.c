/**********************************************************************************************************************

                                            UBD - Unipolar Bipolar Driver

***********************************************************************************************************************
 * File name:               periph.c
 * Creation date:           07.12.2015
 * Main creator:            J. Rebetez
 * Company:                 REFAST GmbH
 *                          Copyright (c) 2015 REFAST GmbH
***********************************************************************************************************************
 * Content overview:        - 
***********************************************************************************************************************/

#include "includes.h"

/**********************************************************************************************************************
 * Routine:                 periph_init

 * Description:
 * This routine initialize all extern peripherics like RTCC, motor driver, ...
 * 
 * Creator:                 J. Rebetez
 * Date of creation:        07.12.2015
 * Last modification on:    -
 * Modified by:             - 
 * 
 * Input:                   -
 * Output:                  -
***********************************************************************************************************************/
void periph_init(void)
{
    InitA3981();
    
    //release of the RTC chip
    RV30xx_release();       //call subroutine
    RV30xx_init();          //call subroutine
    RV30xx_InitInterrupt(_Rv30xxENABLE);    //call subroutine
}