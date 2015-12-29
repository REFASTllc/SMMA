/**********************************************************************************************************************

                                            UBD - Unipolar Bipolar Driver

***********************************************************************************************************************
 * File name:               system.c
 * Creation date:           17.08.2015
 * Main creator:            J. Rebetez
 * Company:                 REFAST GmbH
 *                          Copyright (c) 2015 REFAST GmbH
***********************************************************************************************************************
 * Content overview:        - InitADInterrupt
 *                          - InitADInterrupt
***********************************************************************************************************************/


#include "includes.h" // File which contain all includes files


/**********************************************************************************************************************
 * Routine:                 InitADInterrupt

 * Description:
 * Initialization of the AD module. 
 * 
 * Creator:                 J. Rebetez
 * Date of creation:        17.08.2015
 * Last modification on:    
 * Modified by:             
 * 
 * Input:                   -
 * Output:                  -
***********************************************************************************************************************/
void InitADModule(void)
{
//configure pin if digital or analog mode (1 = digital / 0 = analog)
    AD1PCFGbits.PCFG0 = 1;  //digital mode (programming pin)
    AD1PCFGbits.PCFG1 = 1;  //digital mode (programming pin)
    AD1PCFGbits.PCFG2 = 0;  //unipolar; Icoil A2
    AD1PCFGbits.PCFG3 = 0;  //unipolar; Icoil A1
    AD1PCFGbits.PCFG4 = 0;  //unipolar; Icoil B2
    AD1PCFGbits.PCFG5 = 0;  //unipolar; Icoil B1
    AD1PCFGbits.PCFG6 = 0;  //bipolar; Icoil B
    AD1PCFGbits.PCFG7 = 0;  //bipolar; Vref
    AD1PCFGbits.PCFG8 = 0;  //Vmot
    AD1PCFGbits.PCFG9 = 0;  //Imot
    AD1PCFGbits.PCFG10 = 0; //bipolar; Icoil A
    AD1PCFGbits.PCFG11 = 0; //battery
    AD1PCFGbits.PCFG12 = 1; //free
    AD1PCFGbits.PCFG13 = 1; //free
    AD1PCFGbits.PCFG14 = 1; //digital used for Vmot switch
    AD1PCFGbits.PCFG15 = 1; //digital used to switch the measure to coil resistor
    
/*** AD1CON1 ******************************************************************/
    AD1CON1bits.ON = 0;     // 1 = ADC module is operating
                            // 0 = ADC is off

    AD1CON1bits.SIDL = 1;   // 1 = Discontinue module operation when device enters Idle mode
                            // 0 = Continue module operation in Idle mode

    AD1CON1bits.FORM2 = 1;  // 011 = Signed Fractional 16-bit (DOUT = 0000 0000 0000 0000 sddd dddd dd00 0000)
    AD1CON1bits.FORM1 = 0;  // 010 = Fractional 16-bit (DOUT = 0000 0000 0000 0000 dddd dddd dd00 0000)
    AD1CON1bits.FORM0 = 0;  // 001 = Signed Integer 16-bit (DOUT = 0000 0000 0000 0000 ssss sssd dddd dddd)
                            // 000 = Integer 16-bit (DOUT = 0000 0000 0000 0000 0000 00dd dddd dddd)
                            // 111 = Signed Fractional 32-bit (DOUT = sddd dddd dd00 0000 0000 0000 0000)
                            // 110 = Fractional 32-bit (DOUT = dddd dddd dd00 0000 0000 0000 0000 0000)
                            // 101 = Signed Integer 32-bit (DOUT = ssss ssss ssss ssss ssss sssd dddd dddd)
                            // 100 = Integer 32-bit (DOUT = 0000 0000 0000 0000 0000 00dd dddd dddd)

    AD1CON1bits.SSRC2 = 1;  // 111 = Internal counter ends sampling and starts conversion (auto convert)
    AD1CON1bits.SSRC1 = 1;  // 010 = Timer3 period match ends sampling and starts conversion
    AD1CON1bits.SSRC0 = 1;  // 001 = Active transition on INT0 pin ends sampling and starts conversion
                            // 000 = Clearing SAMP bit ends sampling and starts conversion

    AD1CON1bits.CLRASAM = 1;// 1 = Stop conversions when the first ADC interrupt is generated. Hardware clears the ASAM bit when the
                            //     ADC interrupt is generated.
                            // 0 = Normal operation, buffer contents will be overwritten by the next conversion sequence

    AD1CON1bits.ASAM = 0;   // 1 = Sampling begins immediately after last conversion completes; SAMP bit is automatically set
                            // 0 = Sampling begins when SAMP bit is set

    AD1CON1bits.SAMP = 0;   // 1 = The ADC SHA is sampling
                            // 0 = The ADC sample/hold amplifier is holding
                            // When ASAM = 0, writing ?1? to this bit starts sampling.
                            // When SSRC = 000, writing ?0? to this bit will end sampling and start conversion.

    AD1CON1bits.DONE = 0;   // 1 = Analog-to-digital conversion is done
                            // 0 = Analog-to-digital conversion is not done or has not started
                            // Clearing this bit will not affect any operation in progress.
/*** AD1CON2 ******************************************************************/
    AD1CON2bits.VCFG2 = 0;  //      ADC VR+             ADC VR-
    AD1CON2bits.VCFG1 = 0;  // 000  AVDD                AVSS
    AD1CON2bits.VCFG0 = 0;  // 001  External VREF+ pin  AVSS
                            // 010  AVDD                External VREF- pin
                            // 011  External VREF+ pin  External VREF- pin
                            // 1xx  AVDD                AVSS


    AD1CON2bits.OFFCAL = 1; // 1 = Enable Offset Calibration mode
                            //     VINH and VINL of the SHA are connected to VR-
                            // 0 = Disable Offset Calibration mode
                            //     The inputs to the SHA are controlled by AD1CHS or AD1CSSL

    AD1CON2bits.CSCNA = 0;  // 1 = Scan inputs
                            // 0 = Do not scan inputs

    AD1CON2bits.BUFS = 0;   // Only valid when BUFM = 1 (ADRES split into 2 x 8-word buffers).
                            // 1 = ADC is currently filling buffer 0x8-0xF, user should access data in 0x0-0x7
                            // 0 = ADC is currently filling buffer 0x0-0x7, user should access data in 0x8-0xF

    AD1CON2bits.SMPI3 = 0;  // 1111 = Interrupts at the completion of conversion for each 16th sample/convert sequence
    AD1CON2bits.SMPI2 = 0;  // 1110 = Interrupts at the completion of conversion for each 15th sample/convert sequence
    AD1CON2bits.SMPI1 = 0;  // ...
    AD1CON2bits.SMPI0 = 0;  // 0001 = Interrupts at the completion of conversion for each 2nd sample/convert sequence
                            // 0000 = Interrupts at the completion of conversion for each sample/convert sequence

    AD1CON2bits.BUFM = 0;   // 1 = Buffer configured as two 8-word buffers, ADC1BUF(7...0), ADC1BUF(15...8)
                            // 0 = Buffer configured as one 16-word buffer ADC1BUF(15...0.)

    AD1CON2bits.ALTS = 1;   // 1 = Uses MUX A input multiplexer settings for first sample, then alternates between MUX B and
                            //     MUX A input multiplexer settings for all subsequent samples
                            // 0 = Always use MUX A input multiplexer settings
/*** AD1CON3 ******************************************************************/
    AD1CON3bits.ADRC = 0;   // 1 = ADC internal RC clock
                            // 0 = Clock derived from Peripheral Bus Clock (PBCLK)

    AD1CON3bits.SAMC4 = 0;  // 11111 = 31 TAD
    AD1CON3bits.SAMC3 = 1;  // ...
    AD1CON3bits.SAMC2 = 1;  // ...
    AD1CON3bits.SAMC1 = 1;  // 00001 = 1 TAD
    AD1CON3bits.SAMC0 = 1;  // 00000 = 0 TAD (Not allowed)

    AD1CON3bits.ADCS7 = 0;  // 11111111 = TPB ? 2 ? (ADCS<7:0> + 1) = 512 ? TPB = TAD
    AD1CON3bits.ADCS6 = 1;  // ...
    AD1CON3bits.ADCS5 = 1;  // ...
    AD1CON3bits.ADCS4 = 1;  // ...
    AD1CON3bits.ADCS3 = 1;  // 00000001 = TPB ? 2 ? (ADCS<7:0> + 1) = 4 ? TPB = TAD
    AD1CON3bits.ADCS2 = 1;  // 00000000 = TPB ? 2 ? (ADCS<7:0> + 1) = 2 ? TPB = TAD
    AD1CON3bits.ADCS1 = 1;  // TPB is the PIC32 Peripheral Bus clock time period.
    AD1CON3bits.ADCS0 = 1;  // Refer to Section 6. ?Oscillator? (DS61112) for more information.
    
//AD1CHS: ADC input select register
    AD1CHSbits.CH0NB = 0;   //negative input select for MUX B bit
                            //0= channel 0 negative input is VR-
                            //1= channel 0 negative input is AN1
    
    AD1CHSbits.CH0SB3 = 0;  //positive input select for MUX B bits
    AD1CHSbits.CH0SB2 = 0;  //1111= channel 0 positive input is AN15
    AD1CHSbits.CH0SB1 = 0;  //...
    AD1CHSbits.CH0SB0 = 0;  //0000= channel 0 positive input is AN0
    
    AD1CHSbits.CH0NA = 0;   //negative input select for MUX A bit
                            //0= channel 0 negative input is VR-
                            //1= channel 0 negative input is AN1
    
    AD1CHSbits.CH0SA3 = 0;  //positive input select for MUX A bits
    AD1CHSbits.CH0SA2 = 0;  //11111= channel 0 positive input is AN15
    AD1CHSbits.CH0SA1 = 0;  //...
    AD1CHSbits.CH0SA0 = 0;  //0000= channel 0 positive input is AN0
    
//AD1CSSL: ADC input scan select register
    AD1CSSLbits.CSSL0 = 0;  //skip AN0 for input scan
    AD1CSSLbits.CSSL1 = 0;  //skip AN1 for input scan
    AD1CSSLbits.CSSL2 = 0;  //skip AN2 for input scan
    AD1CSSLbits.CSSL3 = 0;  //skip AN3 for input scan
    AD1CSSLbits.CSSL4 = 0;  //skip AN4 for input scan
    AD1CSSLbits.CSSL5 = 0;  //skip AN5 for input scan
    AD1CSSLbits.CSSL6 = 0;  //skip AN6 for input scan
    AD1CSSLbits.CSSL7 = 0;  //skip AN7 for input scan
    AD1CSSLbits.CSSL8 = 0;  //skip AN8 for input scan
    AD1CSSLbits.CSSL9 = 0;  //skip AN9 for input scan
    AD1CSSLbits.CSSL10 = 0; //skip AN10 for input scan
    AD1CSSLbits.CSSL11 = 0; //skip AN11 for input scan
    AD1CSSLbits.CSSL12 = 0; //skip AN12 for input scan
    AD1CSSLbits.CSSL13 = 0; //skip AN13 for input scan
    AD1CSSLbits.CSSL14 = 0; //skip AN14 for input scan
    AD1CSSLbits.CSSL15 = 0; //skip AN15 for input scan
}


/**********************************************************************************************************************
 * Routine:                 InitADInterrupt

 * Description:
 * Initialization of the interrupts of the AD. 
 * 
 * Creator:                 J. Rebetez
 * Date of creation:        17.08.2015
 * Last modification on:    
 * Modified by:             
 * 
 * Input:                   -
 * Output:                  -
***********************************************************************************************************************/
void InitADInterrupt(void)
{
    IFS1bits.AD1IF = 0; // Interrupt flag bit

    IPC6bits.AD1IP = 7; // Interrupt priority bits (7 = high, 0 = no interrupt)
    IPC6bits.AD1IS = 3; // Interrupt sub-priority bits (3 = high priority)

    IEC1bits.AD1IE = 0; // Interrupt enable bit
}