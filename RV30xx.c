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
 *                          - RV30xx_init
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
    auto unsigned char uint8_WB;        //local work byte
    
    uint8_WB = 0x90;    //variable is used for the control status register
                        //0 = disable / 1 = enable
                        //bit7 =    Clk/Int, applies CLKOUT function CLKOUT pin (1)
                        //          applies INT function on INT CLKOUT pin (0)
                        //bit6..5 = TD1, TDO, select source for internal countdown timer
                        //          00= 32Hz
                        //          01= 8Hz
                        //          10= 1Hz
                        //          11= 0.5Hz
                        //bit4 =    SROn, en/di self recovery function
                        //bit3 =    EERE, en/di automatic EEPROM refresh every hour
                        //bit2 =    TAR, en/di countdown timer auto-reload mode 
                        //bit1 =    TE, en/di countdown timer
                        //bit0 =    WE, en/di 1Hz clock source for watch
    
    //fill out the send buffer
    i2c_SendBufWr(_i2c1,_RV30xxAddr);
    i2c_SendBufWr(_i2c1,_RegControlStatus);
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
 * Routine:                 RV30xx_init

 * Description:
 * Initialization of the RV30xx chip. 
 * 
 * Creator:                 A. Staub
 * Date of creation:        14.12.2015
 * Last modification on:    -
 * Modified by:             - 
 * 
 * Input:                   -
 * Output:                  -
***********************************************************************************************************************/
void RV30xx_init(void)
{
    auto unsigned char uint8_WB;    //local work byte
    
    uint8_WB = 0x10;    //variable is used for the control int register
                        //0 = disable / 1 = enable
                        //bit7..5 = unused
                        //bit4 =    SRIE, en/di self-recovery INT
                        //bit3 =    V2IE, en/di low voltage 2 detection
                        //bit2 =    V1IE, en/di low voltage 1 detection
                        //bit1 =    TIE, en/di countdown timer
                        //bit0 =    AIE, en/di alarm INT
    
    //fill out the send buffer
    i2c_SendBufWr(_i2c1,_RV30xxAddr);
    i2c_SendBufWr(_i2c1,_RegControlInt);
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
    
}   //end of RV30xx_init


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
        g_Param.sint16_Temp = i2c_ReceiveBufRd(_i2c1);  //read out one byte from the buffer 
        g_Param.sint16_Temp -= 60;                      //read value -60 to have the right    
    }
}   //end of RV30xx_TempMeas


/**********************************************************************************************************************
 * Routine:                 RV30xx_SetGetSec

 * Description:
 * This command read out the temperature. First it sets the register that we want to read and then 
 * it reads out the temperature register. 
 * 
 * Creator:                 A. Staub
 * Date of creation:        13.12.2015
 * Last modification on:    -
 * Modified by:             - 
 * 
 * Input:                   uint8_SetGet
 * Output:                  -
***********************************************************************************************************************/
void RV30xx_SetGetSec(unsigned char uint8_SetGet,unsigned char uint8_DataByte)
{
    auto unsigned char uint8_WB;        //local work byte
    
    if(uint8_SetGet)                    //write?
    {
        //fill out the send buffer
        i2c_SendBufWr(_i2c1,_RV30xxAddr);
        i2c_SendBufWr(_i2c1,_RegSeconds);
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
    }
    else                                //read?
    {
        //fill out the send buffer
        i2c_SendBufWr(_i2c1,_RV30xxAddr);
        i2c_SendBufWr(_i2c1,_RegSeconds);

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
        
        g_Param.uint8_Sec = i2c_ReceiveBufRd(_i2c1);  //read out one byte from the buffer 
        //convert value into decimal
        g_Param.uint8_Sec = (((g_Param.uint8_Sec & 0xF0) >> 4) * 10) + (g_Param.uint8_Sec & 0x0F);
    }
}   //end of RV30xx_SetGetSec
