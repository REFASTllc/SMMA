/**********************************************************************************************************************

                                            UBD - Unipolar Bipolar Driver

***********************************************************************************************************************
 * File name:               ADS1115.h
 * Creation date:           22.02.2016
 * Main creator:            A. Staub
 * Company:                 REFAST GmbH
 *                          Copyright (c) 2015 REFAST GmbH
***********************************************************************************************************************
 * Content overview :       - ADS1115
***********************************************************************************************************************/

/**********************************************************************************************************************
 * Routine:                 ADS1115_H

 * Description:
 * ...
 * 
 * Creator:                 A. Staub
 * Date of creation:        22.02.2016
 * Last modification on:    -
 * Modified by:             - 
 * 
 * Input:                   -
 * Output:                  -
***********************************************************************************************************************/

#ifndef ADS1115_H
#define	ADS1115_H


extern void ads1115_init(void);


#define _ADS1115Addr            0x90    //definition of the address for the ADS
#define _PR_ConvReg             0x00    //pointer register on conversion register
#define _PR_ConfReg             0x01    //pointer register on config register
#define _PR_LoReg               0x02    //pointer register on lo tresh register
#define _PR_HiReg               0x03    //pointer register on hi tresh register


#endif	/* ADS1115_H */
