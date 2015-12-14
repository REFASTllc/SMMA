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
extern T_SPI SPI1;

void main(void)
{   
    unsigned int tempToggle = 0;
    
    system_init();          //call subroutine
          
    asm("ei");              //enable all interrupts (code in assembler) use declaration "di" to disable all interrupts
     
    oVmotOnOff = 1;
    oBiEnaVmot = 1;
    
    periph_init();
    oBiResetSignal = 1;
    DAC7571_WrByte(_NormalMode, 1240);
//    oBiDirSignal = 1;
    
    while(1)
    {
        if(g_Uni.uint8_Settings & 0x01) //verify if the unipolar motor has to move
        {
            uni_move();                 //then call the subroutine
        }
        else
        {
            //otherwise do nothing
        }     
        
        if(!g_UART2txd.uint8_BufEmpty)  //send buffer not empty?
            IEC1bits.U2TXIE = 1;        //enable the send interrupt
        else
        {
            //do nothing
        }
        
        if(!g_UART2rxd.uint8_BufEmpty)  //receive buffer not empty?
            cmdchk_check();             //call subroutine
        else
        {
        //--- Toggle of led (alive test) ---//
            if(tempToggle >= 30000)
            {
                oTestLed1 =! oTestLed1;
                tempToggle = 0;
            }
            else
                tempToggle++;
            if(IFS0bits.T3IF)
            {
                oBiStepSignal =! oBiStepSignal;
                IFS0bits.T3IF = 0;
                TMR3 = 0;
            }
        }
    }   
}   //end of main
