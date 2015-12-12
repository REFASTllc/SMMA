/**********************************************************************************************************************

                                            UBD - Unipolar Bipolar Driver

***********************************************************************************************************************
 * File name:               DAC7571.c
 * Creation date:           12.12.2015
 * Main creator:            A. Staub
 * Company:                 REFAST GmbH
 *                          Copyright (c) 2015 REFAST GmbH
***********************************************************************************************************************
 * Content overview:        - DAC7571_WrByte
***********************************************************************************************************************/


#include "includes.h"


/**********************************************************************************************************************
 * Routine:                 DAC7571_WrByte

 * Description:
 * This subroutine set the DAC value of the DAC7571 chip 
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
void DAC7571_WrByte(unsigned char uint8_OpMode, unsigned int uint16_DACval)
{
    auto unsigned int uint16_WI;        //local work integer
    auto unsigned char uint8_WB;
    
    //fill out the send buffer
    i2c_SendBufWr(_i2c1,_DAC7571_H);
    uint16_WI = uint16_DACval;          //store DAV value into the local work integer
    uint16_WI = uint16_WI >> 8;         //shift value 8 bits to right
    uint8_WB  = (uint16_WI | uint8_OpMode) & 0x000000FF;    //add the information for the operation mode
    i2c_SendBufWr(_i2c1,uint8_WB);
    uint8_WB = uint16_DACval & 0x000000FF;  //take the LSB of the DAC
    i2c_SendBufWr(_i2c1,uint8_WB);

    //define some important variables for the transfer
    g_i2c1.uint8_RdWr = 0;                  //command will be a write
    g_i2c1.uint8_RScount = 0;               //repeat start condition not used
    
    i2c_StartTransfer(_i2c1);   //launch the transfer
    
    do
    {
        //do nothing...
    }
    while(g_i2c1.uint8_Busy);   //until the transfer is finished
    
}   //end of DAC7571_WrByte

