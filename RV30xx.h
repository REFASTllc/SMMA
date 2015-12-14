/**********************************************************************************************************************

                                            UBD - Unipolar Bipolar Driver

***********************************************************************************************************************
 * File name:               RV30xx.h
 * Creation date:           13.12.2015
 * Main creator:            A. Staub
 * Company:                 REFAST GmbH
 *                          Copyright (c) 2015 REFAST GmbH
***********************************************************************************************************************
 * Content overview :       - RV30xx
***********************************************************************************************************************/

/**********************************************************************************************************************
 * Routine:                 RV30xx_H

 * Description:
 * ...
 * 
 * Creator:                 A. Staub
 * Date of creation:        13.12.2015
 * Last modification on:    -
 * Modified by:             - 
 * 
 * Input:                   -
 * Output:                  -
***********************************************************************************************************************/

#ifndef RV30xx_H
#define	RV30xx_H


extern void RV30xx_release(void);
extern void RV30xx_TempMeas(void);


#define _RV30xxAddr         0xAC    //definition of the address for the RTC
#define _RegControl1        0x00    //definition of register control 1
#define _RegControlInt      0x01    //definition of register control INT
#define _RegControlIntFlag  0x02    //definition of register control INT flag
#define _RegControlStatus   0x03    //definition of register control status
#define _RegControlReset    0x04    //definition of register control reset
#define _RegSeconds         0x08    //definition of register seconds
#define _RegMinutes         0x09    //definition of register minutes
#define _RegHours           0x0A    //definition of register hours
#define _RegDays            0x0B    //definition of register days
#define _RegWeekdays        0x0C    //definition of register weekdays
#define _RegMonths          0x0D    //definition of register months
#define _RegYears           0x0E    //definition of register years
#define _RegAseconds        0x10    //definition of register alarm seconds
#define _RegAminutes        0x11    //definition of register alarm minutes
#define _RegAhours          0x12    //definition of register alarm minutes
#define _RegAdays           0x13    //definition of register alarm days
#define _RegAweekdays       0x14    //definition of register alarm weekdays
#define _RegAmonths         0x15    //definition of register alarm months
#define _RegAyears          0x16    //definition of register alarm years
#define _RegTimerLow        0x18    //definition of register timer low
#define _RegTimerHigh       0x19    //definition of register timer high
#define _RegTemp            0x20    //definition of register temperature 
#define _RegROM1            0x28    //definition of register user ROM place 1
#define _RegROM2            0x29    //definition of register user ROM place 2
#define _RegROMcontrol      0x30    //definition of register ROM control
#define _RegXtalOffset      0x31    //definition of register XTAL offset
#define _RegXtalCoef        0x32    //definition of XTAL coef.
#define _RegXtalT0          0x33    //definition of XTAL T0
#define _RegRAM1            0x38    //definition of register user RAM place 1
#define _RegRAM2            0x39    //definition of register user RAM place 2
#define _RegRAM3            0x3A    //definition of register user RAM place 3
#define _RegRAM4            0x3B    //definition of register user RAM place 4
#define _RegRAM5            0x3C    //definition of register user RAM place 5
#define _RegRAM6            0x3D    //definition of register user RAM place 6
#define _RegRAM7            0x3E    //definition of register user RAM place 7
#define _RegRAM8            0x3F    //definition of register user RAM place 8

#endif	/* RV30xx_H */
