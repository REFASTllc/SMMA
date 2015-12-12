/**********************************************************************************************************************

                                            UBD - Unipolar Bipolar Driver

***********************************************************************************************************************
 * File name:               ROM24LC256.h
 * Creation date:           12.12.2015
 * Main creator:            A. Staub
 * Company:                 REFAST GmbH
 *                          Copyright (c) 2015 REFAST GmbH
***********************************************************************************************************************
 * Content overview :       - ROM24LC256
***********************************************************************************************************************/

/**********************************************************************************************************************
 * Routine:                 ROM24LC256_H

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

#ifndef ROM24LC256_H
#define	ROM24LC256_H


extern void ROM24LC256_WrByte(unsigned char uint8_AdrH, unsigned char uint8_AdrL, unsigned char uint8_DataByte);
extern unsigned char ROM24LC256_RdByte(unsigned char uint8_AdrH, unsigned char uint8_AdrL, unsigned char uint8_RdCount);

#define _ROM24LC256addr     0xA2    //definition of the address for the EEPROM


#endif	/* ROM24LC256_H */


/* SOME INFORMATION'S
  
  Informations
  ------------
  Vcc Range:                    1.7 - 5.5V
  Communication protocol:       I2C
  Max. clock frequency:         400kHz
  EEPROM address:               0x0000 - 0x7FFF
  
  PIN descriptions
  ----------------
  A0, A1, A2:
  Address of the eeprom (0-7). So up to eight devices may be connected to the same bus.
  Write-Protect (WP):
  Protection enabled if this pin is tied to Vcc
  Protection disabled if this pin is tied to GND (Vss)
  
  Device addressing
  -----------------
  +------------------------------------+
  |       Slave address - Bit 0:7      |
  +---+---+---+---+----+----+----+-----+
  | 7 | 6 | 5 | 4 |  3 |  2 |  1 |  0  |
  +---+---+---+---+----+----+----+-----+
  | 1 | 0 | 1 | 0 | A2 | A1 | A0 | R/W |
  +---+---+---+---+----+----+----+-----+
  Bit 7:4:  Fix code for 24LC256
  Bit 3:1:  Hardware code = Slave address
  Bit 0:    Read/Write bit
            1 = Read command
            0 = Write command
  
  Write operations
  ----------------
  Byte write:
  +---+------+-----+-----+-----+-----+-----+------+-----+---+
  | S | CB-W | ACK | AHB | ACK | ALB | ACK | DATA | ACK | P |
  +---+------+-----+-----+-----+-----+-----+------+-----+---+ 
  S:      Start bit
  CB-W:   Control byte (slave address and write command)
  ACK:    Acknowledge
  AHB:    Address High Byte
  ALB:    Address Low Byte
  DATA:   Data to store at this address
  P:      Stop bit
  
  Page write:
  +---+------+-----+-----+-----+-----+-----+-------+-----+-//-+--------+-----+---+
  | S | CB-W | ACK | AHB | ACK | ALB | ACK | DATA0 | ACK |    | DATA63 | ACK | P |
  +---+------+-----+-----+-----+-----+-----+-------+-----+-//-+--------+-----+---+ 
  S:      Start bit
  CB-W:   Control byte (slave address and write command)
  ACK:    Acknowledge
  AHB:    Address High Byte
  ALB:    Address Low Byte
  DATA0:  First data byte
  -//-:   Other 62 data byte with ACK after each data byte
  DATA63: Last data byte
  P:      Stop bit
  
  Read operations
  ---------------
  Current address read:
  +---+------+-----+------+------+---+
  | S | CB-R | ACK | DATA | NACK | P |
  +---+------+-----+------+------+---+
  S:      Start bit
  CB-R:   Control byte (slave address and read command)
  ACK:    Acknowledge
  DATA:   Data from the current address
  NACK:   No acknowledge
  P:      Stop bit  
  
  The 24LC256 contains an address counter that maintains the address of the last word 
  accessed, internally incremented by '1'. If the previous read access was to address
  'n', the next current address read operation would access data from address n + 1.
  
  Random read:
  +---+------+-----+-----+-----+-----+-----+----+------+-----+------+------+---+
  | S | CB-W | ACK | AHB | ACK | ALB | ACK | RS | CB-R | ACK | DATA | NACK | P |
  +---+------+-----+-----+-----+-----+-----+----+------+-----+------+------+---+    
  S:      Start bit
  CB-W:   Control byte (slave address and write command)
  ACK:    Acknowledge
  AHB:    Address High Byte
  ALB:    Address Low Byte
  RS:     Repeated start bit/condition
  CB-R:   Control byte (slave address and read command)
  DATA:   Data from the address 'high and low byte'
  NACK:   No acknowledge
  P:      Stop bit 
  
  Sequential read:
  Initiated in the same way as a random read except that after the transmits, the first
  data byte, the master issues an acknowledge as opposed to the stop condition used in a 
  random read. This acknowledge directs to transmit the next sequentially addressed 
  8-bit word until a NACK (no acknowledge) occurs. 
  Important: The internal address pointer will automatically roll over from address
  0x7FFF to address 0x0000.
  */

