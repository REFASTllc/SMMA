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
    volatile unsigned char tempchar, tempint, tempshortint, tempshort, templong, templongint, tempfloat, tempdouble;
    
    system_init();          //call subroutine
          
    asm("ei");              //enable all interrupts (code in assembler) use declaration "di" to disable all interrupts
    
 //!!!!!!!!!!!!!!!!!!!!!!!!!BE CAREFUL!!!!!!!!!!!!!!!!!!!!!!
 //we both play with the switch for the supply. I am still testing the LIN bus driver.
 //please inform me if you increase the voltage higher than 24V!!!
 //otherwise I think nothing will happen if we switch on both supplies (bipolar & lin)
 //but I switched off your supply to be sure! 
    //bipolar
/*#ifdef _BIPOLAR
    oBiResetSignal = 1;
    oVmotOnOff = 1;
    oBiEnaVmot = 1;
    oEnaVLINSupply = 0;
    oBiPhCurrCtrl = 1;
#endif
#ifdef _LIN
    oVmotOnOff = 1;
    oEnaVLINSupply = 1;
#endif
#ifdef _UNIPOLAR
    oVmotOnOff = 1;
    oEnaVLINSupply = 0;
#endif*/
 //!!!!!!!!!!!!!!!!!!!!!!!!!BE CAREFUL!!!!!!!!!!!!!!!!!!!!!!   
    
    periph_init();
//    DAC7571_WrByte(_NormalMode, 1240);  // 1V
//    DAC7571_WrByte(_NormalMode, 2381);  // 2V
//    DAC7571_WrByte(_NormalMode, 992);  // 0V8
    DAC7571_WrByte(_NormalMode, 2113);  // 1.7V
        
    while(1)
    {      
        if(g_Uni.uint8_Settings & 0x01) //verify if the unipolar motor has to move
            uni_move();                 //then call the subroutine
        if(g_Bipol.uint1_IsBipolEnabled)
            bi_move();
        if(!g_UART2txd.uint8_BufEmpty)  //send buffer not empty?
            IEC1bits.U2TXIE = 1;        //enable the send interrupt        
        if(!g_UART2rxd.uint8_BufEmpty)  //receive buffer not empty?
            cmdchk_check();             //call subroutine
        if(g_LIN.uint8_SlaveAnswerRequested)    //answer from LIN slave requested
        {
            //timeout or slave answer received
            if((g_LIN.uint8_SlaveTimeout) || (g_LIN.uint8_SlaveAnswerFinish))  
                LINATA6629_SendBackSlaveAnswer();
        }
        else
        {
            static unsigned short int temp = 0;
            if(temp >= 10000)
            { 
                adc_LaunchNextMeasure();    //call subroutine 
                oTestLed1 =! oTestLed1;
                temp = 0;
            }
            else
                temp++;
        }
    }
    /*while(1)
    {
        if(!IC2CONbits.ON)
        {
            IC2CONbits.ON = 1;
            T2CONbits.ON = 1;
            TMR2 = 0;
            nbreOverflowTMR2 = 0;
        }
    }*/
}   //end of main