/**********************************************************************************************************************

                                            UBD - Unipolar Bipolar Driver

***********************************************************************************************************************
 * File name:               RV30xx.c
 * Creation date:           13.12.2015
 * Main creator:            A. Staub
 * Company:                 REFAST GmbH
 *                          Copyright (c) 2015 REFAST GmbH
***********************************************************************************************************************
 * Content overview:        - RV30xx_release
 *                          - RV30xx_TempMeas
***********************************************************************************************************************/


#include "includes.h"

extern Si2c1 g_i2c1;
extern SParam g_Param;
/**********************************************************************************************************************
 * Routine:                 RV30xx_release

 * Description:
 * Use this routine after POR (power on reset) to release the RTC chip. 
 * First it resets/clears the POR flag and second it enables the temperature measure.   
 * 
 * Creator:                 A. Staub
 * Date of creation:        13.12.2015
 * Last modification on:    -
 * Modified by:             - 
 * 
 * Input:                   -
 * Output:                  -
***********************************************************************************************************************/
void RV30xx_release(void)
{
    //fill out the send buffer
    i2c_SendBufWr(_i2c1,_RV30xxAddr);
    i2c_SendBufWr(_i2c1,_RegControlStatus);
    i2c_SendBufWr(_i2c1,0x00);

    //define some important variables for the transfer
    g_i2c1.uint8_RdWr = 0;                  //command will be a write
    g_i2c1.uint8_RScount = 0;               //repeat start condition not used
    
    i2c_StartTransfer(_i2c1);   //launch the transfer
    
    do
    {
        //do nothing...
    }
    while(g_i2c1.uint8_Busy);   //until the transfer is finished
    
    
    //fill out the send buffer
    i2c_SendBufWr(_i2c1,_RV30xxAddr);
    i2c_SendBufWr(_i2c1,_RegROMcontrol);
    //Temp. interval = 1s
    //Temp. measure enabled
    //Clock out freq. = 32.768kHz
    //trickle charge disabled
    i2c_SendBufWr(_i2c1,0x02);
    
    //define some important variables for the transfer
    g_i2c1.uint8_RdWr = 0;                  //command will be a write
    g_i2c1.uint8_RScount = 0;               //repeat start condition not used
    
    i2c_StartTransfer(_i2c1);   //launch the transfer
    
    do
    {
        //do nothing...
    }
    while(g_i2c1.uint8_Busy);   //until the transfer is finished  
}   //end of RV30xx_release


/**********************************************************************************************************************
 * Routine:                 RV30xx_TempMeas

 * Description:
 * This command read out the temperature. First it sets the register that we want to read and then 
 * it reads out the temperature register. 
 * 
 * Creator:                 A. Staub
 * Date of creation:        13.12.2015
 * Last modification on:    -
 * Modified by:             - 
 * 
 * Input:                   -
 * Output:                  -
***********************************************************************************************************************/
void RV30xx_TempMeas(void)
{
    auto unsigned char uint8_WB;        //local work byte
    
    //fill out the send buffer
    i2c_SendBufWr(_i2c1,_RV30xxAddr);
    i2c_SendBufWr(_i2c1,_RegTemp);

    //define some important variables for the transfer
    g_i2c1.uint8_RdWr = 0;                  //command will be a write
    g_i2c1.uint8_RScount = 0;               //repeat start condition not used
    
    i2c_StartTransfer(_i2c1);   //launch the transfer
    
    do
    {
        //do nothing...
    }
    while(g_i2c1.uint8_Busy);   //until the transfer is finished
    
    
    //fill out the send buffer
    uint8_WB = _RV30xxAddr | 0x01;
    i2c_SendBufWr(_i2c1,uint8_WB);
    
    //define some important variables for the transfer
    g_i2c1.uint8_RdWr = 1;                  //command will be a read
    g_i2c1.uint8_RDcount = 1;               //how much byte(s) to read
    g_i2c1.uint8_RScount = 0;               //repeat start condition not used
    
    i2c_StartTransfer(_i2c1);   //launch the transfer
    
    do
    {
        //do nothing...
    }
    while(g_i2c1.uint8_Busy);   //until the transfer is finished
    
    if(g_i2c1.uint8_ErrACK)     //error acknowledge = no answer from the slave
    {
        g_Param.sint16_Temp = 255;  //error value
    }
    else if(g_i2c1.uint8_BusColl)   //error bus collision 
    {
        g_Param.sint16_Temp = 255;  //error value
    }
    else
    {
        g_Param.sint16_Temp = i2c_ReceiveBufRd(_i2c1); //read out one byte from the buffer
        //read value -60 to have the right 
        g_Param.sint16_Temp = -1;      
    }
}   //end of RV30xx_TempMeas
