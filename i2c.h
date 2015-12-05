/**********************************************************************************************************************

                                            UBD - Unipolar Bipolar Driver

***********************************************************************************************************************
 * File name:               i2c.h
 * Creation date:           05.12.2015
 * Main creator:            A. Staub
 * Company:                 REFAST GmbH
 *                          Copyright (c) 2015 REFAST GmbH
***********************************************************************************************************************
 * Content overview :       - I2C
***********************************************************************************************************************/

/**********************************************************************************************************************
 * Routine:                 I2C_H

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

#ifndef I2C_H
#define	I2C_H


extern void i2c_init(unsigned char uint8_i2cx);
extern void i2c_enable(unsigned char uint8_i2cx);
extern void i2c_disabe(unsigned char uint8_i2cx);
void i2c_InitInterrupt(unsigned char uint8_i2cx, unsigned char uint8_action, unsigned char uint8_mode);


#define _i2c1               1       //definition for the i2c module 1
#define _i2c2               2       //definition for the i2c module 2
#define _i2cENABLE          1       //definition for the uint8_action
#define _i2cDISABLE         0       //definition for the uint8_action
#define _i2cMASTER          1       //definition for the uint8_mode
#define _i2cSLAVE           0       //definition for the uint8_mode


typedef struct                      //definitions of multiple variable for function
{
  //volatile unsigned char uint8_ArrAscii[10];            //array of 10x byte to store the Ascii characters
} Si2c;                             //struct name is Si2c (S for struct)
extern Si2c    g_i2c;               //extern name is g_i2c (g for global)
 

#endif	/* I2C_H */


