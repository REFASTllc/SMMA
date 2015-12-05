/**********************************************************************************************************************

                                            UBD - Unipolar Bipolar Driver

***********************************************************************************************************************
 * File name:               includes.h
 * Creation date:           13.08.2015
 * Main creator:            A. Staub
 * Company:                 REFAST GmbH
 *                          Copyright (c) 2015 REFAST GmbH
***********************************************************************************************************************
 * Content overview :       - INCLUDES
***********************************************************************************************************************/

/**********************************************************************************************************************
 * Routine:                 INCLUDES_H

 * Description:
 * All includes are here included. In that way, there is only one file to include in all others
 * 
 * Creator:                 J. Rebetez
 * Date of creation:        06.08.2015
 * Last modification on:    -
 * Modified by:             - 
 * 
 * Input:                   -
 * Output:                  -
***********************************************************************************************************************/

#ifndef INCLUDES_H
#define	INCLUDES_H

//system header files
#include "xc.h"                     
#include "attribs.h"                                        
#include "p32mx360f512l.h"
//#include <math.h>

//project related header files
#include "defines.h"
#include "function.h"
#include "system.h"
#include "timers.h"
#include "INT.h"
#include "unipolar.h"
#include "command.h"
#include "cmdchk.h"
#include "parameter.h"
#include "UART.h"
#include "spi.h"
#include "i2c.h"

#endif	/* INCLUDES_H */

