/**********************************************************************************************************************

                                            UBD - Unipolar Bipolar Driver

***********************************************************************************************************************
 * File name:               system.c
 * Creation date:           17.08.2015
 * Main creator:            J. Rebetez
 * Company:                 REFAST GmbH
 *                          Copyright (c) 2015 REFAST GmbH
***********************************************************************************************************************
 * Content overview:        - InitADModule
 *                          - InitADInterrupt
 *                          - adc_EnableDisable
 *                          - adc_LaunchNextMeasure
***********************************************************************************************************************/


#include "includes.h" // File which contain all includes files

SADC g_ADC;         


/**********************************************************************************************************************
 * Routine:                 InitADModule

 * Description:
 * Initialization of the AD module.  
 * 
 * Creator:                 A. Staub
 * Date of creation:        08.02.2016
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


    AD1CON2bits.OFFCAL = 0; // 1 = Enable Offset Calibration mode
                            //     VINH and VINL of the SHA are connected to VR-
                            // 0 = Disable Offset Calibration mode
                            //     The inputs to the SHA are controlled by AD1CHS or AD1CSSL

    AD1CON2bits.CSCNA = 1;  // 1 = Scan inputs
                            // 0 = Do not scan inputs

    AD1CON2bits.BUFS = 0;   // Only valid when BUFM = 1 (ADRES split into 2 x 8-word buffers).
                            // 1 = ADC is currently filling buffer 0x8-0xF, user should access data in 0x0-0x7
                            // 0 = ADC is currently filling buffer 0x0-0x7, user should access data in 0x8-0xF

    AD1CON2bits.SMPI3 = 1;  // 1111 = Interrupts at the completion of conversion for each 16th sample/convert sequence
    AD1CON2bits.SMPI2 = 0;  // .... 
    AD1CON2bits.SMPI1 = 1;  // 0001 = Interrupts at the completion of conversion for each 2nd sample/convert sequence
    AD1CON2bits.SMPI0 = 0;  // 0000 = Interrupts at the completion of conversion for each sample/convert sequence
                            
    AD1CON2bits.BUFM = 0;   // 1 = Buffer configured as two 8-word buffers, ADC1BUF(7...0), ADC1BUF(15...8)
                            // 0 = Buffer configured as one 16-word buffer ADC1BUF(15...0.)

    AD1CON2bits.ALTS = 0;   // 1 = Uses MUX A input multiplexer settings for first sample, then alternates between MUX B and
                            //     MUX A input multiplexer settings for all subsequent samples
                            // 0 = Always use MUX A input multiplexer settings
/*** AD1CON3 ******************************************************************/
    AD1CON3bits.ADRC = 0;   // 1 = ADC internal RC clock
                            // 0 = Clock derived from Peripheral Bus Clock (PBCLK)

    //SAMC = 31 (refer to the document "ADC calculations" for more details)
    AD1CON3bits.SAMC4 = 1;  // 11111 = 31 TAD
    AD1CON3bits.SAMC3 = 1;  // ...
    AD1CON3bits.SAMC2 = 1;  // ...
    AD1CON3bits.SAMC1 = 1;  // 00001 = 1 TAD 
    AD1CON3bits.SAMC0 = 1;  // 00000 = 0 TAD (Not allowed)

    //ADCS = 18 (refer to the document "ADC calculations" for more details)
    AD1CON3bits.ADCS7 = 0;  // 11111111 = TPB ? 2 ? (ADCS<7:0> + 1) = 512 ? TPB = TAD
    AD1CON3bits.ADCS6 = 0;  // ...
    AD1CON3bits.ADCS5 = 0;  // ...
    AD1CON3bits.ADCS4 = 1;  // ...
    AD1CON3bits.ADCS3 = 0;  // 00000001 = TPB ? 2 ? (ADCS<7:0> + 1) = 4 ? TPB = TAD
    AD1CON3bits.ADCS2 = 0;  // 00000000 = TPB ? 2 ? (ADCS<7:0> + 1) = 2 ? TPB = TAD
    AD1CON3bits.ADCS1 = 1;  // TPB is the PIC32 Peripheral Bus clock time period.
    AD1CON3bits.ADCS0 = 0;  // Refer to Section 6. ?Oscillator? (DS61112) for more information.
    
    //With that we have a sampling rate around 50ksps per channel. 
    
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
    AD1CSSLbits.CSSL2 = 1;  //unipolar; Icoil A2
    AD1CSSLbits.CSSL3 = 1;  //unipolar; Icoil A1
    AD1CSSLbits.CSSL4 = 1;  //unipolar; Icoil B2
    AD1CSSLbits.CSSL5 = 1;  //unipolar; Icoil B1
    AD1CSSLbits.CSSL6 = 1;  //bipolar; Icoil B
    AD1CSSLbits.CSSL7 = 1;  //bipolar; Vref
    AD1CSSLbits.CSSL8 = 1;  //Vmot
    AD1CSSLbits.CSSL9 = 1;  //Imot
    AD1CSSLbits.CSSL10 = 1; //bipolar; Icoil A
    AD1CSSLbits.CSSL11 = 1; //battery
    AD1CSSLbits.CSSL12 = 0; //skip AN12 for input scan
    AD1CSSLbits.CSSL13 = 0; //skip AN13 for input scan
    AD1CSSLbits.CSSL14 = 0; //skip AN14 for input scan
    AD1CSSLbits.CSSL15 = 0; //skip AN15 for input scan
    
    
    g_ADC.uint8_ConvStarted = 0;        //reset variable
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
    IFS1CLR = _IFS1_AD1IF_MASK; // Interrupt flag bit

    IPC6bits.AD1IP = 2;         // Interrupt priority bits (7 = high, 0 = no interrupt)
    IPC6bits.AD1IS = 2;         // Interrupt sub-priority bits (3 = high priority)

    IEC1SET = _IEC1_AD1IE_MASK; // Interrupt enabled
}


/**********************************************************************************************************************
 * Routine:                 adc_EnableDisable

 * Description:
 * Enable or disable the adc modul. 
 * 
 * Creator:                 A. Staub
 * Date of creation:        13.02.2016
 * Last modification on:    
 * Modified by:             
 * 
 * Input:                   uint8_ED
 * Output:                  -
***********************************************************************************************************************/
void adc_EnableDisable(unsigned char uint8_ED)
{
    if(uint8_ED)
        AD1CON1bits.ON = 1;     //ADC module is operating
    else
        AD1CON1bits.ON = 0;     //ADC module is not operating
}   //end of adc_EnableDisable


/**********************************************************************************************************************
 * Routine:                 adc_LaunchNextMeasure

 * Description:
 * ...
 * 
 * Creator:                 A. Staub
 * Date of creation:        17.08.2015
 * Last modification on:    
 * Modified by:             
 * 
 * Input:                   -
 * Output:                  -
***********************************************************************************************************************/
void adc_LaunchNextMeasure(void)
{
    if(g_ADC.uint8_ConvStarted)         //conversion in progress
    {
        //do nothing
    }
    else
    {
        g_ADC.uint8_ConvStarted = 1;    //signal that a conversion is started     
        AD1CON1bits.ASAM = 1;           //launch the conversion
                                                //note this bit is set back to 0 automatic after 
                                                //the interrupt is generated
    }
}   //end of adc_LaunchNextMeasure
