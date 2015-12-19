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

extern SUART2txd g_UART2txd;
extern SUART2rxd g_UART2rxd;
extern SUni g_Uni;
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
extern T_A3981 A3981;

void main(void)
{   
    unsigned short int tempToggle = 0, countStall = 0;
    
    system_init();          //call subroutine
          
    asm("ei");              //enable all interrupts (code in assembler) use declaration "di" to disable all interrupts
    
    
 //!!!!!!!!!!!!!!!!!!!!!!!!!BE CAREFUL!!!!!!!!!!!!!!!!!!!!!!
 //we both play with the switch for the supply. I am still testing the LIN bus driver.
 //please inform me if you increase the voltage higher than 24V!!!
 //otherwise I think nothing will happen if we switch on both supplies (bipolar & lin)
 //but I switched off your supply to be sure! 
    //bipolar
    oVmotOnOff = 1;
    oBiEnaVmot = 0;
    oBiDirSignal = 0;
    //lin
    oEnaVLINSupply = 1;
 //!!!!!!!!!!!!!!!!!!!!!!!!!BE CAREFUL!!!!!!!!!!!!!!!!!!!!!!   
    
    periph_init();
    oBiResetSignal = 1;
    DAC7571_WrByte(_NormalMode, 886);
    
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
            SendOneDataSPI1(A3981.CONFIG1.REG);
            A3981.FAULT1.REG = GetLastDataSPI1();
            SendOneDataSPI1(A3981.CONFIG0.REG);
            A3981.FAULT0.REG = GetLastDataSPI1();
            if(A3981.FAULT1.BITS.ST && A3981.FAULT0.BITS.ST)
            {
                countStall++;
                oBiDirSignal =! oBiDirSignal;
            }
            //--- Toggle of led (alive test) ---//
            if(tempToggle >= 200)
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
