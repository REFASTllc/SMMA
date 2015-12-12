/**********************************************************************************************************************

                                            UBD - Unipolar Bipolar Driver

***********************************************************************************************************************
 * File name:               DAC7571.h
 * Creation date:           12.12.2015
 * Main creator:            A. Staub
 * Company:                 REFAST GmbH
 *                          Copyright (c) 2015 REFAST GmbH
***********************************************************************************************************************
 * Content overview :       - DAC7571
***********************************************************************************************************************/

/**********************************************************************************************************************
 * Routine:                 DAC7571_H

 * Description:
 * ...
 * 
 * Creator:                 A. Staub
 * Date of creation:        12.12.2015
 * Last modification on:    -
 * Modified by:             - 
 * 
 * Input:                   -
 * Output:                  -
***********************************************************************************************************************/

#ifndef DAC7571_H
#define	DAC7571_H


extern void DAC7571_WrByte(unsigned char uint8_OpMode, unsigned int uint16_DACval);


#define _DAC7571_H      0x98    //definition of the address for the DAC
#define _NormalMode     0x00    //normal mode
#define _1kAGND         0x10    //1k to AGND
#define _100kAGND       0x20    //100k to AGND
#define _HighImp        0x30    //high impedance


#endif	/* DAC7571_H */


/* SOME INFORMATION'S
  
  Informations
  ------------
  Vcc Range:                    2.7 - 5.5V
  Communication protocol:       I2C (receive only, means only write operations are possible
  Max. clock frequency:         100kHz, 400kHz, 3.4Mbit/s
  Resolution:                   12-bit (0 ... 4095)
  
  Device addressing
  -----------------
  +--------------------------------+
  |     Slave address - Bit 0:7    |
  +---+---+---+---+---+---+----+---+
  | 7 | 6 | 5 | 4 | 3 | 2 |  1 | 0 |
  +---+---+---+---+---+---+----+---+
  | 1 | 0 | 0 | 1 | 1 | 0 | A0 | 0 |
  +---+---+---+---+---+---+----+---+
  Bit 7:2:  Fix code for DAC7571
  Bit 1:    Hardware code = Slave address
  Bit 0:    Read/Write bit
            1 = Read not supported
            0 = Write command
  
  Write operations
  ----------------
 
  +---+------+-----+-----+-----+-----+-----+---+
  | S | CB-W | ACK | MSB | ACK | LSB | ACK | P |
  +---+------+-----+-----+-----+-----+-----+---+ 
  S:      Start bit
  CB-W:   Control byte (slave address and write command)
  ACK:    Acknowledge
  MSB:    MS-Byte
  LSB:    LS-Byte
  P:      Stop bit
 
  +-----------------------------------------+---------------------------------------+
  |               MSB - Bit 0:7             |             LSB - Bit 0:7             |
  +---+---+-----+-----+-----+-----+----+----+----+----+----+----+----+----+----+----+
  | 7 | 6 |  5  |  4  |  3  |  2  |  1 |  0 |  7 |  6 |  5 |  4 |  3 |  2 |  1 |  0 |
  +---+---+-----+-----+-----+-----+----+----+----+----+----+----+----+----+----+----+
  | 0 | 0 | PD1 | PD2 | D11 | D10 | D9 | D8 | D7 | D6 | D5 | D4 | D3 | D2 | D1 | D0 |
  +---+---+-----+-----+-----+-----+----+----+----+----+----+----+----+----+----+----+
  MSB:
  Bit 7:6:  Fix code for DAC7571
  Bit 5:4:  Operation mode
            00 = Normal operation
            01 = 1k to AGND, PWD
            10 = 100k to AGND, PWD
            11 = High impedance, PWD
  Bit 3:0:  Higher bits of the 12-bit resolution 
  
  LSB:
  Bit 7:0:  Lower bits of the 12-bit resolution

  */

