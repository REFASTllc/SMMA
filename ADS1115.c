/**********************************************************************************************************************

                                            UBD - Unipolar Bipolar Driver

***********************************************************************************************************************
 * File name:               ADS1115.c
 * Creation date:           22.02.2016
 * Main creator:            A. Staub
 * Company:                 REFAST GmbH
 *                          Copyright (c) 2015 REFAST GmbH
***********************************************************************************************************************
 * Content overview:        - ads1115_init
 *                          - ads1115_config
 *                          - ads1115_read
***********************************************************************************************************************/


#include "includes.h"

SADS1115 g_ADS1115;         //global variables for struct 
extern Si2c1 g_i2c1;


/**********************************************************************************************************************
 * Routine:                 ads1115_init

 * Description:
 * Initialization of the ADS1115 adc. 
 * 
 * Creator:                 A. Staub
 * Date of creation:        22.02.2016
 * Last modification on:    -
 * Modified by:             - 
 * 
 * Input:                   -
 * Output:                  -
***********************************************************************************************************************/
void ads1115_init(void)
{
    //put the pointer register on the config register
    i2c_SendBufWr(_i2c1,_ADS1115Addr);
    i2c_SendBufWr(_i2c1,_PR_ConfReg);
    
    //define the config register
    ads1115_config(_AIN0p_GND,_FS4096mV,_CCM,_860SPS,_ActiveLow,_NonLatchingComp,_DC);
    
    //add the registers
    i2c_SendBufWr(_i2c1,g_ADS1115.uint8_ConfigMSB);
    i2c_SendBufWr(_i2c1,g_ADS1115.uint8_ConfigLSB);

    //define some important variables for the transfer
    g_i2c1.uint8_RdWr = 0;                  //command will be a write
    g_i2c1.uint8_RScount = 0;               //repeat start condition not used
    
    i2c_StartTransfer(_i2c1);   //launch the transfer
    
    do
    {
        //do nothing...
    }
    while(g_i2c1.uint8_Busy);   //until the transfer is finished
}   //end of ads1115_init


/**********************************************************************************************************************
 * Routine:                 ads1115_config

 * Description:
 * Define the config register. 
 * 
 * Creator:                 A. Staub
 * Date of creation:        22.02.2016
 * Last modification on:    -
 * Modified by:             - 
 * 
 * Input:                   uint8_MUX
 *                          uint8_PGA
 *                          uint8_Mode
 *                          uint8_DR
 *                          uint8_CompPol
 *                          uint8_CompLat
 *                          uint8_CompQue
 * Output:                  -
***********************************************************************************************************************/
void ads1115_config(unsigned char uint8_MUX, unsigned char uint8_PGA, unsigned char uint8_Mode, unsigned char uint8_DR, unsigned char uint8_CompPol, unsigned long int uint8_CompLat, unsigned long int uint8_CompQue)
{
    //clear the config variables
    g_ADS1115.uint8_ConfigLSB = 0;
    g_ADS1115.uint8_ConfigMSB = 0;
    
    switch(uint8_MUX)
    {
        case (_AIN0p_AIN1n):
            //nothing to do
            break;
            
        case (_AIN0p_AIN3n):
            g_ADS1115.uint8_ConfigMSB |= 0x10;
            break;
            
        case (_AIN1p_AIN3n):
            g_ADS1115.uint8_ConfigMSB |= 0x20;
            break;
            
        case (_AIN2p_AIN3n):
            g_ADS1115.uint8_ConfigMSB |= 0x30;
            break;
            
        case (_AIN0p_GND):
            g_ADS1115.uint8_ConfigMSB |= 0x40;
            break;
            
        case (_AIN1p_GND):
            g_ADS1115.uint8_ConfigMSB |= 0x50;
            break;
            
        case (_AIN2p_GND):
            g_ADS1115.uint8_ConfigMSB |= 0x60;
            break;
            
        case (_AIN3p_GND):
            g_ADS1115.uint8_ConfigMSB |= 0x70;
            break;
            
        default:
            break;
    }
    
    switch(uint8_PGA)
    {
        case (_FS6144mV):
            //nothing to do
            break;
            
        case (_FS4096mV):
            g_ADS1115.uint8_ConfigMSB |= 0x02;
            break;
            
        case (_FS2048mV):
            g_ADS1115.uint8_ConfigMSB |= 0x04;
            break;
            
        case (_FS1024mV):
            g_ADS1115.uint8_ConfigMSB |= 0x06;
            break;
            
        case (_FS512mV):
            g_ADS1115.uint8_ConfigMSB |= 0x08;
            break;
            
        case (_FS256mV):
            g_ADS1115.uint8_ConfigMSB |= 0x0A;
            break;
            
        default:
            break;
    }
    
    switch(uint8_Mode)
    {
        case (_CCM):
            //nothing to do
            break;
            
        case (_PSSM):
            g_ADS1115.uint8_ConfigMSB |= 0x01;
            break;
    }
    
    switch (uint8_DR)
    {
        case(_8SPS):
            //nothing to do
            break;
            
        case(_16SPS):
            g_ADS1115.uint8_ConfigLSB |= 0x20;
            break;
            
        case(_32SPS):
            g_ADS1115.uint8_ConfigLSB |= 0x40;
            break;
            
        case(_64SPS):
            g_ADS1115.uint8_ConfigLSB |= 0x60;
            break;
            
        case(_128SPS):
            g_ADS1115.uint8_ConfigLSB |= 0x80;
            break;
            
        case(_250SPS):
            g_ADS1115.uint8_ConfigLSB |= 0xA0;
            break;
            
        case(_475SPS):
            g_ADS1115.uint8_ConfigLSB |= 0xC0;
            break;
            
        case(_860SPS):
            g_ADS1115.uint8_ConfigLSB |= 0xE0;
            break;
            
        default:
            break;
    }
    
    switch(uint8_CompPol)
    {
        case(_ActiveLow):
            //nothing to do
            break;
            
        case(_ActiveHigh):
            g_ADS1115.uint8_ConfigLSB |= 0x08;
            break;
            
        default:
            break;
    }
    
    switch(uint8_CompLat)
    {
        case (_NonLatchingComp):
            //nothing to do
            break;
            
        case(_LatchingComp):
            g_ADS1115.uint8_ConfigLSB |= 0x04;
            break;
            
        default:
            break;
    }
    
    switch(uint8_CompQue)
    {
        case (_AAOC):
            //nothing to do
            break;
            
        case (_AATC):
            g_ADS1115.uint8_ConfigLSB |= 0x01;
            break;
            
        case (_AAFC):
            g_ADS1115.uint8_ConfigLSB |= 0x02;
            break;
            
        case (_DC):
            g_ADS1115.uint8_ConfigLSB |= 0x03;
            break;
            
        default:
            break;
    }
    
}   //end of ads1115_config


/**********************************************************************************************************************
 * Routine:                 ads1115_read

 * Description:
 * Read out an adc value. 
 * 
 * Creator:                 A. Staub
 * Date of creation:        22.02.2016
 * Last modification on:    -
 * Modified by:             - 
 * 
 * Input:                   -
 * Output:                  uint32_result
***********************************************************************************************************************/
unsigned short int ads1115_read(void)
{
    volatile unsigned char uint8_WB;
    volatile unsigned char uint8_WB1;
    volatile unsigned short int uint16_WB = 0;
    
    //fill out the send buffer
    uint8_WB = _ADS1115Addr | 0x01;
    i2c_SendBufWr(_i2c1,uint8_WB);
    
    //define some important variables for the transfer
    g_i2c1.uint8_RdWr = 1;                  //command will be a read
    g_i2c1.uint8_RDcount = 2;               //how much byte(s) to read
    g_i2c1.uint8_RScount = 0;               //repeat start condition not used
    
    i2c_StartTransfer(_i2c1);   //launch the transfer
    
    do
    {
        //do nothing...
    }
    while(g_i2c1.uint8_Busy);       //until the transfer is finished
    
    
    if(g_i2c1.uint8_ErrACK)         //error acknowledge = no answer from the slave
    {
        return 0xFFFF;              //error value
    }
    else if(g_i2c1.uint8_BusColl)   //error bus collision 
    {
        return 0xFFFF;              //error value
    }
    else
    {
        uint8_WB1 = i2c_ReceiveBufRd(_i2c1);    //read out one byte from the buffer 
        uint8_WB = i2c_ReceiveBufRd(_i2c1);     //read out one byte from the buffer  
        
        uint16_WB = uint8_WB1 << 8;             //store the ADC
        uint16_WB |= uint8_WB;                  //and send it back
        
        return uint16_WB;
    }
}   //end of ads1115_read


/**********************************************************************************************************************
 * Routine:                 ads1115_SetChannel

 * Description:
 * This configurates the config register again and at the end the pointer will be set on the 
 * conversion register. 
 * 
 * Creator:                 A. Staub
 * Date of creation:        22.02.2016
 * Last modification on:    -
 * Modified by:             - 
 * 
 * Input:                   uint8_MUX
 * Output:                  -
***********************************************************************************************************************/
void ads1115_SetChannel(unsigned char uint8_MUX)
{
    //put the pointer register on the config register
    i2c_SendBufWr(_i2c1,_ADS1115Addr);
    i2c_SendBufWr(_i2c1,_PR_ConfReg);
    
    //define the config register
    ads1115_config(uint8_MUX,_FS4096mV,_CCM,_860SPS,_ActiveLow,_NonLatchingComp,_DC);
    
    //add the registers
    i2c_SendBufWr(_i2c1,g_ADS1115.uint8_ConfigMSB);
    i2c_SendBufWr(_i2c1,g_ADS1115.uint8_ConfigLSB);

    //define some important variables for the transfer
    g_i2c1.uint8_RdWr = 0;                  //command will be a write
    g_i2c1.uint8_RScount = 0;               //repeat start condition not used
    
    i2c_StartTransfer(_i2c1);   //launch the transfer
    
    do
    {
        //do nothing...
    }
    while(g_i2c1.uint8_Busy);   //until the transfer is finished
    
    
    //put the pointer register on the conversion register
    i2c_SendBufWr(_i2c1,_ADS1115Addr);
    i2c_SendBufWr(_i2c1,_PR_ConvReg);
    
    //define some important variables for the transfer
    g_i2c1.uint8_RdWr = 0;                  //command will be a write
    g_i2c1.uint8_RScount = 0;               //repeat start condition not used
    
    i2c_StartTransfer(_i2c1);   //launch the transfer
    
    do
    {
        //do nothing...
    }
    while(g_i2c1.uint8_Busy);   //until the transfer is finished  
}   //end of ads1115_SetChannel