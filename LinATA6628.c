/**********************************************************************************************************************

                                            UBD - Unipolar Bipolar Driver

***********************************************************************************************************************
 * File name:               LinATA6628.c
 * Creation date:           19.12.2015
 * Main creator:            Alain Staub
 * Company:                 REFAST GmbH
 *                          Copyright (c) 2015 REFAST GmbH
***********************************************************************************************************************
 * Content overview:        - LINATA6628_init
 *                          - LINATA6629_SendBackSlaveAnswer
 *                          - LINJE_Protocol
***********************************************************************************************************************/


#include "includes.h"

SLin g_LIN; 
SJE g_JE;
extern SParam g_Param;
extern SUART1rxd g_UART1rxd;
extern SCmdChk g_CmdChk; 


/**********************************************************************************************************************
 * Routine:                 LINATA6628_init

 * Description:
 * Initialization of the LIN ATA6628 driver.
 * 
 * Creator:                 A. Staub
 * Date of creation:        19.12.2015
 * Last modification on:    -
 * Modified by:             - 
 * 
 * Input:                   -
 * Output:                  -
***********************************************************************************************************************/
void LINATA6628_init(void)
{
    if(g_Param.uint32_LinSpd <= 20000)  //baud rate smaller than 20kBaud
    {
        oSPModeSignalLIN = 0;   //disable high speed mode
    }
    else
    {
        oSPModeSignalLIN = 1;   //enable high speed mode
    }
    
    oEnaLINDriver = 1;          //enable LIN driver
    
    //reset variables
    g_LIN.uint8_LinBusy = 0;
    g_LIN.uint8_SlaveReceiveCounter = 0;
    g_LIN.uint8_SlaveTimeout = 0;
    g_LIN.uint8_SlaveAnswerFinish = 0;
    g_LIN.uint8_LinBreakToSend = 0;
    g_LIN.uint8_MasterSendCounter = 0;
}   //end of LINATA6628_init


/**********************************************************************************************************************
 * Routine:                 LINATA6629_SendBackSlaveAnswer

 * Description:
 * Send back the received answer from the LIN.
 * 
 * Creator:                 A. Staub
 * Date of creation:        29.12.2015
 * Last modification on:    -
 * Modified by:             - 
 * 
 * Input:                   -
 * Output:                  -
***********************************************************************************************************************/
void LINATA6629_SendBackSlaveAnswer(void)
{
    auto unsigned char uint8_WB;    //local work byte
    
    if(g_LIN.uint8_SlaveTimeout)    //timeout occured
    {
        uart2_sendbuffer('X');      //first the letter X
        uart2_sendbuffer(',');      //add a comma
        g_Param.uint8_ErrCode = _LinTO;          //set error code
        uart2_sendbuffer(g_Param.uint8_ErrCode); //add error code
        
        if(g_Param.uint8_LinRes)        //slave answer with master telegram?
        {
            //do nothing
        }
        else
        {
            g_UART1rxd.uint16_Rch += g_LIN.uint8_MasterSendCounter; 
            if(g_UART1rxd.uint16_Rch >= g_UART1rxd.uint16_Wch)
            {
                g_UART1rxd.uint8_BufEmpty = 1;  //buffer empty
            }
            else
            {
                //do nothing
            }
        }
    }
    else
    {
        if(g_LIN.uint8_SlaveAnswerFinish)   //slave answer received
        {
            uart2_sendbuffer('E');      //first the letter E
            
            if(g_Param.uint8_LinRes)        //slave answer with master telegram?
            {
                //do nothing
            }
            else
            {
                g_UART1rxd.uint16_Rch += g_LIN.uint8_MasterSendCounter;      
            }
        }
        else
        {
            uart2_sendbuffer('X');      //first the letter X
            uart2_sendbuffer(',');      //add a comma
            g_Param.uint8_ErrCode = _LinTO;          //set error code
            uart2_sendbuffer(g_Param.uint8_ErrCode); //add error code
            if(g_Param.uint8_LinRes)        //slave answer with master telegram?
            {
                //do nothing
            }
            else
            {
                g_UART1rxd.uint16_Rch += g_LIN.uint8_MasterSendCounter; 
                if(g_UART1rxd.uint16_Rch >= g_UART1rxd.uint16_Wch)
                {
                    g_UART1rxd.uint8_BufEmpty = 1;  //buffer empty
                }
                else
                {
                    //do nothing
                }
            }
        }
    }
    
    if(!g_UART1rxd.uint8_BufEmpty)  //send buffer is not empty
    {
        do
        {
            uart2_sendbuffer(',');      //add a comma
            uint8_WB = uart1_receivebuffer();  //read out one byte from the receive buffer
            uart2_sendbuffer(uint8_WB); //store the character into the sendbuffer
        }
        while(!g_UART1rxd.uint8_BufEmpty);     
    }
    else
    {
        //do nothing
    }
    
    uart2_sendbuffer(13);      //add carriage return 
    
    g_LIN.uint8_SlaveAnswerRequested = 0;   //reset variable
    g_LIN.uint8_SlaveAnswerFinish = 0;      //reset variable
    g_LIN.uint8_SlaveTimeout = 0;           //reset variable
}   //end of LINATA6629_SendBackSlaveAnswer


/**********************************************************************************************************************
 * Routine:                 LINJE_Protocol

 * Description:
 * ...
 * 
 * Creator:                 A. Staub
 * Date of creation:        16.04.2016
 * Last modification on:    -
 * Modified by:             - 
 * 
 * Input:                   -
 * Output:                  -
***********************************************************************************************************************/
void LINJE_Protocol(void)
{
    volatile unsigned char uint8_Result = 0;
    volatile unsigned char uint8_WB = 0;
    
    //protocol without data byte, or 2, 4 or 8 data bytes
    if((g_CmdChk.uint8_ParamPos == 8) ||
        (g_CmdChk.uint8_ParamPos == 6) ||
        (g_CmdChk.uint8_ParamPos == 10) ||
        (g_CmdChk.uint8_ParamPos == 12) ||
        (g_CmdChk.uint8_ParamPos == 14))
    {    
        if(g_CmdChk.uint8_GlobalLock == 1)  //global lock enabled?
        {
            g_Param.uint8_ErrCode = _MotorInRun;        //set error code
            uart2_SendErrorCode(g_Param.uint8_ErrCode); //call subroutine
        }
        else
        {
            g_CmdChk.uint8_GlobalLock = 1;
            
            uart2_sendbuffer('E');
            uart2_sendbuffer(',');
            do
            {
                uint8_Result += funct_CheckTol(g_CmdChk.uint32_TempPara[0],0,255);
                funct_IntToAscii(g_CmdChk.uint32_TempPara[uint8_WB],_Active);
                uint8_WB++;
                uart2_sendbuffer(',');                  //add the comma
            }
            while(uint8_WB <= g_CmdChk.uint8_ParamPos);
            uart2_sendbuffer(13);
            
            g_CmdChk.uint8_GlobalLock = 0;
        }
    }
    else
    {
        //
    }
}   //end of LINJE_Protocol


/**********************************************************************************************************************
 * Routine:                 LINJE_ConvertASCII

 * Description:
 * ...
 * 
 * Creator:                 A. Staub
 * Date of creation:        16.04.2016
 * Last modification on:    -
 * Modified by:             - 
 * 
 * Input:                   uint8_Character
 * Output:                  uint8_Result
***********************************************************************************************************************/
unsigned char LINJE_ConvertASCII(unsigned char uint8_Character)
{
    volatile unsigned char uint8_Result = 0;
    
    uint8_Result = (uint8_Character - 48) & 0x000000FF;
    uint8_Result += ((uint8_Character - 48) & 0x0000FF00) * 10;
    uint8_Result += ((uint8_Character - 48) & 0x00FF0000) * 100;
    uint8_Result += ((uint8_Character - 48) & 0xFF000000) * 100;
    
    return uint8_Result;
}   //end of LINJE_ConvertASCII