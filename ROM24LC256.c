/**********************************************************************************************************************

                                            UBD - Unipolar Bipolar Driver

***********************************************************************************************************************
 * File name:               ROM24LC256.c
 * Creation date:           12.12.2015
 * Main creator:            A. Staub
 * Company:                 REFAST GmbH
 *                          Copyright (c) 2015 REFAST GmbH
***********************************************************************************************************************
 * Content overview:        - ROM24LC256_WrByte
 *                          - ROM24LC256_RdByte
***********************************************************************************************************************/


#include "includes.h"


/**********************************************************************************************************************
 * Routine:                 ROM24LC256_WrByte

 * Description:
 * This subroutine fill up the send buffer from the I2C to write one byte into the EEPROM on a 
 * specific address given. The I2C communication will be launched directly and the routine waits until
 * the transfer is finished.   
 * 
 * Creator:                 A. Staub
 * Date of creation:        12.12.2015
 * Last modification on:    -
 * Modified by:             - 
 * 
 * Input:                   uint8_AdrH
 *                          uint8_AdrL
 *                          uint8_DataByte
 * Output:                  -
***********************************************************************************************************************/
void ROM24LC256_WrByte(unsigned char uint8_AdrH, unsigned char uint8_AdrL, unsigned char uint8_DataByte)
{
    //fill out the send buffer
    i2c_SendBufWr(_i2c1,_ROM24LC256addr);
    i2c_SendBufWr(_i2c1,uint8_AdrH);
    i2c_SendBufWr(_i2c1,uint8_AdrL);
    i2c_SendBufWr(_i2c1,uint8_DataByte);
    
    //define some important variables for the transfer
    g_i2c1.uint8_RdWr = 0;                  //command will be a write
    g_i2c1.uint8_RScount = 0;               //repeat start condition not used
    
    i2c_StartTransfer(_i2c1);   //launch the transfer
    
    do
    {
        //do nothing...
    }
    while(g_i2c1.uint8_Busy);   //until the transfer is finished
    
}   //end of ROM24LC256_WrByte


/**********************************************************************************************************************
 * Routine:                 ROM24LC256_RdByte

 * Description:
 * This subroutine fill up the send buffer from the I2C to read one byte from the EEPROM on a 
 * specific address given. The I2C communication will be launched directly and the routine waits until
 * the transfer is finished and returns the read value.  
 * 
 * Creator:                 A. Staub
 * Date of creation:        12.12.2015
 * Last modification on:    -
 * Modified by:             - 
 * 
 * Input:                   uint8_AdrH
 *                          uint8_AdrL
 *                          uint8_RdCount
 * Output:                  uint8_WB
***********************************************************************************************************************/
unsigned char ROM24LC256_RdByte(unsigned char uint8_AdrH, unsigned char uint8_AdrL, unsigned char uint8_RdCount)
{
    auto unsigned char uint8_WB;            //local work byte
    
    //fill out the send buffer
    i2c_SendBufWr(_i2c1,_ROM24LC256addr);
    i2c_SendBufWr(_i2c1,uint8_AdrH);
    i2c_SendBufWr(_i2c1,uint8_AdrL);
    uint8_WB = _ROM24LC256addr | 0x01;      //change inside the address the bit of write
    i2c_SendBufWr(_i2c1,uint8_WB);
    
    //define some important variables for the transfer
    g_i2c1.uint8_RdWr = 1;                  //command will be a read
    g_i2c1.uint8_RDcount = uint8_RdCount;   //how much byte(s) to read
    g_i2c1.uint8_RScount = 4;               //definition when the i2c has to launch a repeat start condition
    
    i2c_StartTransfer(_i2c1);   //launch the transfer 
    
    do
    {
        //do nothing...
    }
    while(g_i2c1.uint8_Busy);   //until the transfer is finished
    
    uint8_WB = i2c_ReceiveBufRd(_i2c1);     //read out one byte from the buffer
    
    return uint8_WB;                       
    
}   //end of ROM24LC256_WrByte
