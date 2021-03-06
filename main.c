/**********************************************************************************************************************

                                            UBD - Unipolar Bipolar Driver

***********************************************************************************************************************
 * File name:               main.c
 * Creation date:           30.07.2015
 * Main creator:            A. Staub
 * Company:                 REFAST GmbH
 *                          Copyright (c) 2015 REFAST GmbH
***********************************************************************************************************************
 * Content overview:        - configuration bits
 *                          - main
***********************************************************************************************************************/


#include "includes.h"

extern unsigned short nbreOverflowTMR2;
extern SUART2txd g_UART2txd;
extern SUART2rxd g_UART2rxd;
extern SUni g_Uni;
extern Sbipol g_Bipol;
extern T_SPI SPI1;
extern T_A3981 A3981;
extern SLin g_LIN;
extern SADC g_ADC;
extern SParam g_Param;
#ifdef DEBUG_ON
extern S_RING_BUF debugBuf;
#endif
/**********************************************************************************************************************
 * Routine:                 main

 * Description:
 * ..
 * 
 * Creator:                 A. Staub
 * Date of creation:        30.07.2015
 * Last modification on:    -
 * Modified by:             - 
 * 
 * Input:                   -
 * Output:                  -
***********************************************************************************************************************/
void main(void)
{  
    unsigned short int temp = 0; 
    
    system_init();          //call subroutine    
    
    asm("ei");              //enable all interrupts (code in assembler) use declaration "di" to disable all interrupts
        
    periph_init();
    DAC7571_WrByte(_NormalMode, 992);  // 0V8
    
#ifdef DEBUG_ON
    InitDebugBuf();

    
    for(temp = 0; temp < LOG_BUF_DEPTH;temp++)
        debugBuf.buffer[temp] = 0;
#endif
      
    FrontLED(_GREEN);   // End of the initialization
    while(1)
    {  
        //launch this subroutine every time to protect our device from high voltage or current
//        funct_ProtDev();
        
        if(g_Uni.uint8_Settings & 0x01) //verify if the unipolar motor has to move
        {
            uni_move();                 //then call the subroutine            
        }
        if(g_Bipol.uint1_IsBipolEnabled)
        {
            bi_move();           
        }
        if(!g_UART2txd.uint8_BufEmpty)  //send buffer not empty?
        {
            IEC1SET = _IEC1_U2TXIE_MASK;         
        }
        if(!g_UART2rxd.uint8_BufEmpty)  //receive buffer not empty?
        {
            cmdchk_check();             //call subroutine            
        }
        if(g_LIN.uint8_SlaveAnswerRequested)    //answer from LIN slave requested
        {
            //timeout or slave answer received
            if((g_LIN.uint8_SlaveTimeout) || (g_LIN.uint8_SlaveAnswerFinish))  
            {
                LINATA6629_SendBackSlaveAnswer();           
            }
        }
        else
        {
            static unsigned short int temp = 0;
            if(temp >= 10000)
            {
                temp = 0;
            }
            else
                temp++;
        }
        
        adc_LaunchNextMeasure();    //call subroutine 
        
        //free step count in opposite direction active?
        if((g_Param.uint8_ERUN1 != iSinkSource1) ||
          (g_Param.uint8_ERUN1 != iSinkSource2) ||
          (g_Param.uint8_ERUN1 != iSinkSource3) ||
          (g_Param.uint8_ERUN1 != iSinkSource4) ||
          (g_Param.uint8_ERUN2 != iSinkSource5) && g_Param.uint8_ERUNactive)
        {
            g_Param.uint8_ERUNdetected = 1;
            g_Bipol.uint32_RealPos = g_Bipol.uint32_GoalPos;
            g_Uni.uint32_RealPos = g_Uni.uint32_GoalPos;
        }
        else
        {
            //do nothing
        }
        
    }
}   //end of main
