/**********************************************************************************************************************

                                            UBD - Unipolar Bipolar Driver

***********************************************************************************************************************
 * File name:               system.c
 * Creation date:           13.08.2015
 * Main creator:            A. Staub
 * Company:                 REFAST GmbH
 *                          Copyright (c) 2015 REFAST GmbH
***********************************************************************************************************************
 * Content overview:        - system_init
 *                          - system_osc
 *                          - system_IOs
 *                          - system_NoticeModule
 *                          - configuration bits
***********************************************************************************************************************/


#include "includes.h"


/**********************************************************************************************************************
 * Routine:                 configuration bits

 * Description:
 * This is the code for the compiler directive token from the follow page/link:
 * file:///Applications/microchip/xc32/v1.40/docs/config_docs/32mx360f512l.html
 * 
 * Note - A. Staub:
 * POSCMOD should be normally on EC for an external clock, but if I set this on that option, the
 * system does not work anymore. Means I can't switch on an output or use a timer. 
 * If I set this on HS that is normally used for an external crystal source it works. 
 * I verfied the register OSCCON in debug mode and the timer clock source - all works well. 
 * 
 * Creator:                 A. Staub
 * Date of creation:        30.07.2015
 * Last modification on:    -
 * Modified by:             - 
 * 
 * Input:                   -
 * Output:                  -
***********************************************************************************************************************/
//Config settings
//POSCMOD = HS, FNOSC = PRIPLL, FWDTEN = OFF
//PLLIDIV = DIV_2, PLLMUL = MUL_20
//PBDIV = 1
//Main clock = 4MHz / 1 * 20    = 80MHz
//Peripheral clock = 80MHz /1  =  80MHz

//Configuration Bit settings
//SYSCLK = 80 MHz (8MHz Crystal/ FPLLIDIV * FPLLMUL / FPLLODIV)
//PBCLK = 80 MHz
//Primary Osc w/PLL (XT+,HS+,EC+PLL)
//WDT OFF
//Other options are don't care

#pragma config FPLLMUL = MUL_20, FPLLIDIV = DIV_1, FPLLODIV = DIV_1
#pragma config POSCMOD = HS, FNOSC = PRIPLL, FPBDIV = DIV_1
#pragma config FSOSCEN = ON, DEBUG = ON, ICESEL = ICS_PGx1 

//  end of configuration bits


/**********************************************************************************************************************
 * Routine:                 system_init

 * Description:
 * This routine initialize the basic things like the oscillator source for the system.
 * 
 * Creator:                 A. Staub
 * Date of creation:        30.07.2015
 * Last modification on:    -
 * Modified by:             - 
 * 
 * Input:                   -
 * Output:                  -
***********************************************************************************************************************/
void system_init(void)
{   
//system initialization    
    system_osc();                           //call subroutine
    InitADModule();
    system_IOs();                           //call subroutine
//    system_NoticeModule();                  //call subroutine
//interrupt initialization    
    INT_init();                             //call subroutine
//function initialization
    funct_init();                           //call subroutine
//unipolar initialization
    uni_init();                             //call subroutine
//parameter initialization
    param_init();                           //call subroutine
//command check initialization
    cmdchk_init();                          //call subroutine
//UART2 initialization
    uart_init(_UART2_);                     //call subroutine
    uart_set(_UART2_,_EVEN,_1_STOP,_NON_INVERTED,_NO_AUTOBAUD,38400);   //call subroutine
    uart_InitInterrupt(_UART2_,_ENABLE);    //call subroutine
    uart_enable(_UART2_);                   //call subroutine
//timer 2 initialization for unipolar driver
    timers_Init(_TIMER2);                   //call subroutine and initialize timer 2
    timers_SetInterrupt(_TIMER2,_ENABLE);   //call subroutine and set interrupt for timer 2
    timers_Init(_TIMER3);
//front LED = black status (off)
    funct_FrontLED(_FLEDblack);             //call subroutine and switch on green LED
//--- Initialization of SPI ---//
    InitSPI(_SPI_1);
    InitSPIInterrupt(_SPI_1, _ENABLE);
    EnableSPI1();
//i2c initialization
    i2c_init(_i2c1);                                //call subroutine
    i2c_enable(_i2c1);                              //call subroutine
    i2c_InitInterrupt(_i2c1,_i2cENABLE,_i2cMASTER); //call subroutine
    
//switch on the debugging LED's
    oTestLed1 = 0;                          //switch on the output for the test LED1
    oTestLed2 = 0;                          //switch on the output for the test LED2
    
}   //end of system_init


/**********************************************************************************************************************
 * Routine:                 system_osc

 * Description:
 * Not used for the moment...
 * 
 * Creator:                 A. Staub
 * Date of creation:        25.09.2015
 * Last modification on:    -
 * Modified by:             - 
 * 
 * Input:                   -
 * Output:                  -
***********************************************************************************************************************/
void system_osc(void)
{
    //do nothing
}   //end of system_osc


/**********************************************************************************************************************
 * Routine:                 system_IOs

 * Description:
 * Initialization of all IOs of the microcontroller.
 * 
 * Creator:                 A. Staub
 * Date of creation:        30.07.2015
 * Last modification on:    -
 * Modified by:             - 
 * 
 * Input:                   -
 * Output:                  -
***********************************************************************************************************************/
void system_IOs(void)
{
    //AD1PCFG = 0xffffffff;
    
//+++ Port A +++
//Port A register
    PORTA = 0x00;               //reset PORTA0:7_9:10_14:15
  
//Port A tris register (0= output / 1 = input)
    TRISAbits.TRISA0 = 1;       //RA0 = Detection of the SD Card
    TRISAbits.TRISA1 = 1;       //RA1 = Free
    TRISAbits.TRISA2 = 0;       //RA2 = STRn signal of bipolar driver
    TRISAbits.TRISA3 = 0;       //RA3 = RES signal of LIN driver
    TRISAbits.TRISA4 = 0;       //RA4 = SP_MODE signal of LIN driver
    TRISAbits.TRISA5 = 0;       //RA5 = Enable of LIN driver
    TRISAbits.TRISA6 = 1;       //RA6 = Sink / Source input 7
    TRISAbits.TRISA7 = 0;       //RA7 = sink / source output 0
    TRISAbits.TRISA9 = 1;       //RA9 = Free
    TRISAbits.TRISA10 = 1;      //RA10 = Vref+
    TRISAbits.TRISA14 = 0;      //RA14 = Enable of V LIN supply
    TRISAbits.TRISA15 = 0;      //RA15 = CMD_MODE signal of RF module
    
//Port A open drain configuration register (0= disable / 1 = enable)
    ODCAbits.ODCA0 = 0;         //RA0 open-drain disable
    ODCAbits.ODCA1 = 0;         //RA1 open-drain disable
    ODCAbits.ODCA2 = 0;         //RA2 open-drain disable
    ODCAbits.ODCA3 = 0;         //RA3 open-drain disable
    ODCAbits.ODCA4 = 0;         //RA4 open-drain disable
    ODCAbits.ODCA5 = 0;         //RA5 open-drain disable
    ODCAbits.ODCA6 = 0;         //RA6 open-drain disable
    ODCAbits.ODCA7 = 0;         //RA7 open-drain disable
    ODCAbits.ODCA9 = 1;         //RA9 = sink / source input 7
    ODCAbits.ODCA10 = 0;        //RA10 open-drain disable
    ODCAbits.ODCA14 = 0;        //RA14 open-drain disable
    ODCAbits.ODCA15 = 0;        //RA15 open-drain disable
    
//+++ Port B +++  
//Port B register
    PORTB = 0x00;               //reset PORTB0:15
    
//Port B tris register (0= output / 1 = input)
    TRISBbits.TRISB0 = 0;       //RB0 = Front red led
    TRISBbits.TRISB1 = 0;       //RB1 = Front green led
    TRISBbits.TRISB2 = 1;       //RB2 = Measure of Icoil A2 (Unipolar)
    TRISBbits.TRISB3 = 1;       //RB3 = Measure of Icoil A1 (Unipolar)
    TRISBbits.TRISB4 = 1;       //RB4 = Measure of Icoil B2 (Unipolar)
    TRISBbits.TRISB5 = 1;       //RB5 = Measure of Icoil B1 (Unipolar)
    TRISBbits.TRISB6 = 1;       //RB6 = Measure of Icoil B (bipolar)
    TRISBbits.TRISB7 = 1;       //RB7 = Measure of voltage reference of Ref signal
    TRISBbits.TRISB8 = 1;       //RB8 = Measure of motor supply and coil resistor
    TRISBbits.TRISB9 = 1;       //RB9 = Measure of current of the motor
    TRISBbits.TRISB10 = 1;      //RB10 = Measure of Icoil A (bipolar)
    TRISBbits.TRISB11 = 1;      //RB11 = Measure of the voltage of the backup battery
    TRISBbits.TRISB12 = 1;      //RB12 = Free
    TRISBbits.TRISB13 = 1;      //RB13 = Free
    TRISBbits.TRISB14 = 0;      //RB14 = Output to switch on / off Umot
    TRISBbits.TRISB15 = 0;      //RB15 = Enable to switch the measure to coil resistor
    
//Port B open drain configuration register (0= disable / 1 = enable)
    ODCBbits.ODCB0 = 0;         //RB0 open-drain disable
    ODCBbits.ODCB1 = 0;         //RB1 open-drain disable
    ODCBbits.ODCB2 = 0;         //RB2 open-drain disable
    ODCBbits.ODCB3 = 0;         //RB3 open-drain disable
    ODCBbits.ODCB4 = 0;         //RB4 open-drain disable
    ODCBbits.ODCB5 = 0;         //RB5 open-drain disable
    ODCBbits.ODCB6 = 0;         //RB6 open-drain disable
    ODCBbits.ODCB7 = 0;         //RB7 open-drain disable
    ODCBbits.ODCB8 = 0;         //RB8 open-drain disable
    ODCBbits.ODCB9 = 0;         //RB9 open-drain disable
    ODCBbits.ODCB10 = 0;        //RB10 open-drain disable
    ODCBbits.ODCB11 = 0;        //RB11 open-drain disable
    ODCBbits.ODCB12 = 0;        //RB12 open-drain disable
    ODCBbits.ODCB13 = 0;        //RB13 open-drain disable
    ODCBbits.ODCB14 = 0;        //RB14 open-drain disable
    ODCBbits.ODCB15 = 0;        //RB15 open-drain disable
     
//+++ Port C +++   
//Port C register
    PORTC = 0x00;               //reset PORTC1:4_12:15
    
//Port C tris register (0= output / 1 = input)
    TRISCbits.TRISC1 = 1;       //RC1 = T2CK
    TRISCbits.TRISC2 = 1;       //RC2 = T3CK
    TRISCbits.TRISC3 = 1;       //RC3 = T4CK
    TRISCbits.TRISC4 = 1;       //RC4 = T5CK
    TRISCbits.TRISC12 = 1;      //RC12 = OSC1
    TRISCbits.TRISC13 = 1;      //RC13 = SOSCI
    TRISCbits.TRISC14 = 0;      //RC14 = RESET signal of bipolar driver
    TRISCbits.TRISC15 = 0;      //RC15 = Switch of the communication between LIN driver and RF module
    
//+++ Port D +++   
//Port D register
    PORTD = 0x00;               //reset PORTD0:15  
    
//Port D tris register (0= output / 1 = input)
    TRISDbits.TRISD0 = 0;       //RD0 = Step signal of bipolar driver
    TRISDbits.TRISD1 = 1;       //RD1 = Diag signal of bipolar driver
    TRISDbits.TRISD2 = 0;       //RD2 = DIR signal of bipolar driver
    TRISDbits.TRISD3 = 0;       //RD3 = Phase current control A & B
    TRISDbits.TRISD4 = 0;       //RD4 = Output to switch on / off coil A1 from unipolar driver 
    TRISDbits.TRISD5 = 0;       //RD5 = Output to switch on / off coil A2 from unipolar driver
    TRISDbits.TRISD6 = 0;       //RD6 = Output to switch on / off coil B1 from unipolar driver
    TRISDbits.TRISD7 = 0;       //RD7 = Output to switch on / off coil B2 from unipolar driver
    TRISDbits.TRISD8 = 1;       //RD8 = sink / source input 6
    TRISDbits.TRISD9 = 1;       //RD9 = sink / source input 3
    TRISDbits.TRISD10 = 0;      //RD10 = Enable of the constant current source
    TRISDbits.TRISD11 = 0;      //RD11 = Enable of the motor supply
    TRISDbits.TRISD12 = 0;      //RD12 = Control relay for coil A
    TRISDbits.TRISD13 = 0;      //RD13 = Control relay for coil B
    TRISDbits.TRISD14 = 1;      //RD14 = UART 1 CTS
    TRISDbits.TRISD15 = 1;      //RD15 = UART 1 RTS
    
//Port D open drain configuration register (0= disable / 1 = enable)
    ODCDbits.ODCD0 = 0;         //RD0 open-drain disable
    ODCDbits.ODCD1 = 0;         //RD1 open-drain disable
    ODCDbits.ODCD2 = 0;         //RD2 open-drain disable
    ODCDbits.ODCD3 = 0;         //RD3 open-drain disable
    ODCDbits.ODCD4 = 0;         //RD4 open-drain disable
    ODCDbits.ODCD5 = 0;         //RD5 open-drain disable
    ODCDbits.ODCD6 = 0;         //RD6 open-drain disable
    ODCDbits.ODCD7 = 0;         //RD7 open-drain disable
    ODCDbits.ODCD8 = 0;         //RD8 open-drain disable
    ODCDbits.ODCD9 = 0;         //RD9 open-drain disable
    ODCDbits.ODCD10 = 0;        //RD10 open-drain disable
    ODCDbits.ODCD11 = 0;        //RD11 open-drain disable
    ODCDbits.ODCD12 = 0;        //RD12 open-drain disable
    ODCDbits.ODCD13 = 0;        //RD13 open-drain disable
    ODCDbits.ODCD14 = 0;        //RD14 open-drain disable
    ODCDbits.ODCD15 = 0;        //RD15 open-drain disable
    
//+++ Port E +++      
//Port E register
    PORTE = 0x00;               //reset PORTE0:9 
    
//Port E tris register (0= output / 1 = input)
    TRISEbits.TRISE0 = 0;       //RE0= sink / source output 1
    TRISEbits.TRISE1 = 0;       //RE1= sink / source output 2
    TRISEbits.TRISE2 = 0;       //RE2= sink / source output 6
    TRISEbits.TRISE3 = 0;       //RE3= sink / source output 7
    TRISEbits.TRISE4 = 1;       //RE4= Free
    TRISEbits.TRISE5 = 0;       //RE5= Test LED 1
    TRISEbits.TRISE6 = 0;       //RE6= Test LED 2
    TRISEbits.TRISE7 = 1;       //RE7= Free
    TRISEbits.TRISE8 = 1;       //RE8= sink / source input 0
    TRISEbits.TRISE9 = 1;       //RE9= INT signal of RTCC chip
    
//Port E open drain configuration register (0= disable / 1 = enable)
    ODCEbits.ODCE0 = 0;         //RE0 open-drain disable
    ODCEbits.ODCE1 = 0;         //RE1 open-drain disable
    ODCEbits.ODCE2 = 0;         //RE2 open-drain disable
    ODCEbits.ODCE3 = 0;         //RE3 open-drain disable
    ODCEbits.ODCE4 = 0;         //RE4 open-drain disable
    ODCEbits.ODCE5 = 0;         //RE5 open-drain disable
    ODCEbits.ODCE6 = 0;         //RE6 open-drain disable
    ODCEbits.ODCE7 = 0;         //RE7 open-drain disable
    ODCEbits.ODCE8 = 0;         //RE8 open-drain disable
    ODCEbits.ODCE9 = 0;         //RE9 open-drain disable
    
//+++ Port F +++  
//Port F register
    PORTF = 0x00;               //reset PORTF0:8_12:13
    
//Port E tris register (0= output / 1 = input)
    TRISFbits.TRISF0 = 1;       //RF0= sink / source input 1
    TRISFbits.TRISF1 = 1;       //RF1= sink / source input 2
    TRISFbits.TRISF2 = 1;       //RF2= UART 1 Rx
    TRISFbits.TRISF3 = 0;       //RF3= UART 1 Tx
    TRISFbits.TRISF4 = 1;       //RF4= UART 2 Rx
    TRISFbits.TRISF5 = 0;       //RF5= UARt 2 Tx
    TRISFbits.TRISF6 = 1;       //RF6= SPI 1 SCK
    TRISFbits.TRISF7 = 1;       //RF7= SPI 1 SDI
    TRISFbits.TRISF8 = 1;       //RF8= SPI 1 SDO
    TRISFbits.TRISF12 = 1;      //RF12= UART 2 CTS
    TRISFbits.TRISF13 = 1;      //RF13= UART 2 RTS
    
//Port F open drain configuration register (0= disable / 1 = enable)
    ODCFbits.ODCF0 = 0;         //RF0 open-drain disable
    ODCFbits.ODCF1 = 0;         //RF1 open-drain disable
    ODCFbits.ODCF2 = 0;         //RF2 open-drain disable
    ODCFbits.ODCF3 = 0;         //RF3 open-drain disable
    ODCFbits.ODCF4 = 0;         //RF4 open-drain disable
    ODCFbits.ODCF5 = 0;         //RF5 open-drain disable
    ODCFbits.ODCF6 = 0;         //RF6 open-drain disable
    ODCFbits.ODCF7 = 0;         //RF7 open-drain disable
    ODCFbits.ODCF8 = 0;         //RF8 open-drain disable
    ODCFbits.ODCF12 = 0;        //RF12 open-drain disable
    ODCFbits.ODCF13 = 0;        //RF13 open-drain disable
    
//+++ Port G +++     
//Port G register
    PORTG = 0x00;               //reset PORTG0:3_6:9_12:15
    
//Port E tris register (0= output / 1 = input)
    TRISGbits.TRISG0 = 1;       //RG0= sink / source input 5
    TRISGbits.TRISG1 = 1;       //RG1= sink / source input 4
    TRISGbits.TRISG2 = 1;       //RG2= I2C 1 SCL
    TRISGbits.TRISG3 = 1;       //RG3= I2C 1 SDA
    TRISGbits.TRISG6 = 1;       //RG6= SPI 2 SCK
    TRISGbits.TRISG7 = 1;       //RG7= SPI 2 SDI
    TRISGbits.TRISG8 = 1;       //RG8= SPI 2 SDO
    TRISGbits.TRISG9 = 1;       //RG9= SPI 2 SS
    TRISGbits.TRISG12 = 0;      //RG12= sink / source output 3
    TRISGbits.TRISG13 = 0;      //RG13= sink / source output 4
    TRISGbits.TRISG14 = 0;      //RG14= sink / source output 5
    TRISGbits.TRISG15 = 1;      //RG15= Detection of USB connection
    
//Port G open drain configuration register (0= disable / 1 = enable)
    ODCGbits.ODCG0 = 0;         //RG0 open-drain disable
    ODCGbits.ODCG1 = 0;         //RG1 open-drain disable
    ODCGbits.ODCG2 = 0;         //RG2 open-drain disable
    ODCGbits.ODCG3 = 0;         //RG3 open-drain disable
    ODCGbits.ODCG6 = 0;         //RG6 open-drain disable
    ODCGbits.ODCG7 = 0;         //RG7 open-drain disable
    ODCGbits.ODCG8 = 0;         //RG8 open-drain disable
    ODCGbits.ODCG9 = 0;         //RG9 open-drain disable
    ODCGbits.ODCG12 = 0;        //RG12 open-drain disable
    ODCGbits.ODCG13 = 0;        //RG13 open-drain disable
    ODCGbits.ODCG14 = 0;        //RG14 open-drain disable
    ODCGbits.ODCG15 = 0;        //RG15 open-drain disable         
}   //end of system_IOs


/**********************************************************************************************************************
 * Routine:                 system_NoticeModule

 * Description:
 * This is the code for the compiler directive token from the follow page/link:
 * file:///Applications/microchip/xc32/v1.40/docs/config_docs/32mx360f512l.html
 * 
 * Creator:                 A. Staub
 * Date of creation:        30.07.2015
 * Last modification on:    -
 * Modified by:             - 
 * 
 * Input:                   -
 * Output:                  -
***********************************************************************************************************************/
void system_NoticeModule(void)
{
//change notice control register 
    CNCONbits.ON = 0;           //change notice module on bit
                                //0= CN module is disabled
                                //1= CN module is enabled
    CNCONbits.SIDL = 0;         //stop in idle mode bit
                                //0= continue operation in idle mode
                                //1= discontinue operation when device enters Idle mode
       
//input change notification interrupt enable register (0= disable / 1= enable)
    CNENbits.CNEN0 = 0;         //CNEN0 disabled
    CNENbits.CNEN1 = 0;         //CNEN1 disabled
    CNENbits.CNEN2 = 0;         //CNEN2 disabled
    CNENbits.CNEN3 = 0;         //CNEN3 disabled
    CNENbits.CNEN4 = 0;         //CNEN4 disabled
    CNENbits.CNEN5 = 0;         //CNEN5 disabled
    CNENbits.CNEN6 = 0;         //CNEN6 disabled
    CNENbits.CNEN7 = 0;         //CNEN7 disabled
    CNENbits.CNEN8 = 0;         //CNEN8 disabled
    CNENbits.CNEN9 = 0;         //CNEN9 disabled
    CNENbits.CNEN10 = 0;        //CNEN10 disabled
    CNENbits.CNEN11 = 0;        //CNEN11 disabled
    CNENbits.CNEN12 = 0;        //CNEN12 disabled
    CNENbits.CNEN13 = 0;        //CNEN13 disabled
    CNENbits.CNEN14 = 0;        //CNEN14 disabled
    CNENbits.CNEN15 = 0;        //CNEN15 disabled
    CNENbits.CNEN16 = 0;        //CNEN16 disabled
    CNENbits.CNEN17 = 0;        //CNEN17 disabled
    CNENbits.CNEN18 = 0;        //CNEN18 disabled
    CNENbits.CNEN19 = 0;        //CNEN19 disabled
    CNENbits.CNEN20 = 0;        //CNEN20 disabled
    CNENbits.CNEN21 = 0;        //CNEN21 disabled
        
//input change notification pull-up enable (0= disable / 1 = enable)
    CNPUEbits.CNPUE0 = 0;       //CNPUE0 disabled
    CNPUEbits.CNPUE1 = 0;       //CNPUE1 disabled
    CNPUEbits.CNPUE2 = 0;       //CNPUE2 disabled
    CNPUEbits.CNPUE3 = 0;       //CNPUE3 disabled
    CNPUEbits.CNPUE4 = 0;       //CNPUE4 disabled
    CNPUEbits.CNPUE5 = 0;       //CNPUE5 disabled
    CNPUEbits.CNPUE6 = 0;       //CNPUE6 disabled
    CNPUEbits.CNPUE7 = 0;       //CNPUE7 disabled
    CNPUEbits.CNPUE8 = 0;       //CNPUE8 disabled
    CNPUEbits.CNPUE9 = 0;       //CNPUE9 disabled
    CNPUEbits.CNPUE10 = 0;      //CNPUE10 disabled
    CNPUEbits.CNPUE11 = 0;      //CNPUE11 disabled
    CNPUEbits.CNPUE12 = 0;      //CNPUE12 disabled
    CNPUEbits.CNPUE13 = 0;      //CNPUE13 disabled
    CNPUEbits.CNPUE14 = 0;      //CNPUE14 disabled
    CNPUEbits.CNPUE15 = 0;      //CNPUE15 disabled
    CNPUEbits.CNPUE16 = 0;      //CNPUE16 disabled
    CNPUEbits.CNPUE17 = 0;      //CNPUE17 disabled
    CNPUEbits.CNPUE18 = 0;      //CNPUE18 disabled
    CNPUEbits.CNPUE19 = 0;      //CNPUE19 disabled
    CNPUEbits.CNPUE20 = 0;      //CNPUE20 disabled
    CNPUEbits.CNPUE21 = 0;      //CNPUE21 disabled    
}   //end of system_NoticeModule


