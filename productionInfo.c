/**********************************************************************************************************************

                                            UBD - Unipolar Bipolar Driver

***********************************************************************************************************************
 * File name:               productionInfo.c
 * Creation date:           17.02.2016
 * Main creator:            Julien Rebetez
 * Company:                 REFAST GmbH
 *                          Copyright (c) 2015 REFAST GmbH
***********************************************************************************************************************
 * Content overview:        - ReadProdInfoFromEEPROM
***********************************************************************************************************************/


#include "includes.h" // File which contain all includes files

S_PROD g_Prod;
extern Si2c1 g_i2c1;
extern STimer1 g_Timer1; 


/**********************************************************************************************************************
 * Routine:                 ReadProdInfoFromEEPROM

 * Description:
 * Readout of all production info from external EEPROM	into working memory (RAM)	
 * 
 * Modified 27.02.2016 A. Staub:
 * Modified the code for that it works. 
 * 
 * Creator:                 Julien Rebetez
 * Date of creation:        17.02.2016
 * Last modification on:    27.02.2016
 * Modified by:             A. Staub
 * 
 * Input:                   -
 * Output:                  -
***********************************************************************************************************************/
void ReadProdInfoFromEEPROM(void)
{
    volatile unsigned long int uint32_WB;   //local work byte
    
    //read out the eeprom and store them into the variables
    uint32_WB = ROM24LC256_RdByte(EE_ADDR_H,EE_ADDR_L_SERIAL_NUMBER1,1);
    
    g_Timer1.uint8_TimeoutFlag = 1;     //set the timeout flag
    SetTimer(_TIMER1,_ENABLE,0,50);     //load the timer with 50ms
    while(g_Timer1.uint8_TimeoutFlag);  //wait the time
    g_Prod.uint32_SN = uint32_WB;        //LSB
    uint32_WB = ROM24LC256_RdByte(EE_ADDR_H,EE_ADDR_L_SERIAL_NUMBER2,1);
    
    g_Timer1.uint8_TimeoutFlag = 1;     //set the timeout flag
    SetTimer(_TIMER1,_ENABLE,0,50);     //load the timer with 50ms
    while(g_Timer1.uint8_TimeoutFlag);  //wait the time
    g_Prod.uint32_SN += (uint32_WB << 8);  //2. part
    uint32_WB = ROM24LC256_RdByte(EE_ADDR_H,EE_ADDR_L_SERIAL_NUMBER3,1);
    
    g_Timer1.uint8_TimeoutFlag = 1;     //set the timeout flag
    SetTimer(_TIMER1,_ENABLE,0,50);     //load the timer with 50ms
    while(g_Timer1.uint8_TimeoutFlag);  //wait the time
    g_Prod.uint32_SN += (uint32_WB << 16); //3. part
    uint32_WB = ROM24LC256_RdByte(EE_ADDR_H,EE_ADDR_L_SERIAL_NUMBER4,1);
    g_Prod.uint32_SN += (uint32_WB << 24); //MSB
    
    g_Timer1.uint8_TimeoutFlag = 1;     //set the timeout flag
    SetTimer(_TIMER1,_ENABLE,0,50);     //load the timer with 50ms
    while(g_Timer1.uint8_TimeoutFlag);  //wait the time    
    g_Prod.uint8_year = ROM24LC256_RdByte(EE_ADDR_H,EE_ADDR_L_PROD_YEAR,1);
    
    g_Timer1.uint8_TimeoutFlag = 1;     //set the timeout flag
    SetTimer(_TIMER1,_ENABLE,0,50);     //load the timer with 50ms
    while(g_Timer1.uint8_TimeoutFlag);  //wait the time
    g_Prod.uint8_week = ROM24LC256_RdByte(EE_ADDR_H,EE_ADDR_L_PROD_WEEK,1);
    
    g_Timer1.uint8_TimeoutFlag = 1;     //set the timeout flag
    SetTimer(_TIMER1,_ENABLE,0,50);     //load the timer with 50ms
    while(g_Timer1.uint8_TimeoutFlag);  //wait the time
    g_Prod.uint8_TS = ROM24LC256_RdByte(EE_ADDR_H,EE_ADDR_L_TEST_STATUS,1);
    
    g_Timer1.uint8_TimeoutFlag = 1;     //set the timeout flag
    SetTimer(_TIMER1,_ENABLE,0,50);     //load the timer with 50ms
    while(g_Timer1.uint8_TimeoutFlag);  //wait the time
    g_Prod.uint8_FW[0] = ROM24LC256_RdByte(EE_ADDR_H,EE_ADDR_L_FW_VERSION_1,1);
    
    g_Timer1.uint8_TimeoutFlag = 1;     //set the timeout flag
    SetTimer(_TIMER1,_ENABLE,0,50);     //load the timer with 50ms
    while(g_Timer1.uint8_TimeoutFlag);  //wait the time
    g_Prod.uint8_FW[1] = ROM24LC256_RdByte(EE_ADDR_H,EE_ADDR_L_FW_VERSION_2,1);
    
    g_Timer1.uint8_TimeoutFlag = 1;     //set the timeout flag
    SetTimer(_TIMER1,_ENABLE,0,50);     //load the timer with 50ms
    while(g_Timer1.uint8_TimeoutFlag);  //wait the time
    g_Prod.uint8_FW[2] = ROM24LC256_RdByte(EE_ADDR_H,EE_ADDR_L_FW_VERSION_3,1);
    
    g_Timer1.uint8_TimeoutFlag = 1;     //set the timeout flag
    SetTimer(_TIMER1,_ENABLE,0,50);     //load the timer with 50ms
    while(g_Timer1.uint8_TimeoutFlag);  //wait the time
    g_Prod.uint8_FW[3] = ROM24LC256_RdByte(EE_ADDR_H,EE_ADDR_L_FW_VERSION_4,1);
    
    g_Timer1.uint8_TimeoutFlag = 1;     //set the timeout flag
    SetTimer(_TIMER1,_ENABLE,0,50);     //load the timer with 50ms
    while(g_Timer1.uint8_TimeoutFlag);  //wait the time
    g_Prod.uint8_FW[4] = ROM24LC256_RdByte(EE_ADDR_H,EE_ADDR_L_FW_VERSION_5,1);
    
    g_Timer1.uint8_TimeoutFlag = 1;     //set the timeout flag
    SetTimer(_TIMER1,_ENABLE,0,50);     //load the timer with 50ms
    while(g_Timer1.uint8_TimeoutFlag);  //wait the time
    g_Prod.uint8_FW[5] = ROM24LC256_RdByte(EE_ADDR_H,EE_ADDR_L_FW_VERSION_6,1);
    
    g_Timer1.uint8_TimeoutFlag = 1;     //set the timeout flag
    SetTimer(_TIMER1,_ENABLE,0,50);     //load the timer with 50ms
    while(g_Timer1.uint8_TimeoutFlag);  //wait the time
    g_Prod.uint8_HW = ROM24LC256_RdByte(EE_ADDR_H,EE_ADDR_L_HW_VERSION,1); 
    
    
    //erase i2c receive buffer (safety reason)
    g_i2c1.uint8_RxRch++;
    g_i2c1.uint8_RxWch++;
}