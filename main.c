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
void main(void)
{   
    system_init();          //call subroutine
      
    asm("ei");              //enable all interrupts (code in assembler)  
                            //use declaration "di" to disable all interrupts
    /* SPI test program 
    while(1)
    {
        unsigned int i = 0;
        SendDataSPI1(0b0010011100011100);
        for(i = 0; i < 50000; i++)
            Nop();
        SendDataSPI1(0b0101000000100001);
        for(i = 0; i < 50000; i++)
            Nop();
    }
    */
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
            //do nothing
        }
    }   
}   //end of main






//old code's
        /*if(OSCCONbits.COSC == 3)
        {
            PORTDbits.RD0 = 1;
        }
        else
        {
            PORTDbits.RD0 = 0;
        }*/