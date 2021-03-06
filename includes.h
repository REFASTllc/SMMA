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

#define _XTAL_FREQ 80000000

//system header files
#include <xc.h>
#include <sys/attribs.h>
//#include <math.h>

//project related header files
#include "exception.h"
#include "defines.h"
#include "system.h"
#include "timers.h"
#include "INT.h"
#include "unipolar.h"
#include "bipolar.h"
#include "command.h"
#include "cmdchk.h"
#include "parameter.h"
#include "UART.h"
#include "spi.h"
#include "i2c.h"
#include "adc.h"
#include "outputCompare.h"
#include "inputCapture.h"
#include "function.h"
//Extern devices header files
#include "periph.h"
#include "A3981.h"
#include "ROM24LC256.h"
#include "DAC7571.h"
#include "RV30xx.h"
#include "LinATA6628.h"
#include "DebugBuf.h"
#include "productionInfo.h"
#include "ADS1115.h"

#endif	/* INCLUDES_H */

