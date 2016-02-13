/**********************************************************************************************************************

                                            UBD - Unipolar Bipolar Driver

***********************************************************************************************************************
 * File name:               adc.h
 * Creation date:           17.08.2015
 * Main creator:            J. Rebetez
 * Company:                 REFAST GmbH
 *                          Copyright (c) 2015 REFAST GmbH
***********************************************************************************************************************
 * Content overview:        - adc
***********************************************************************************************************************/


/**********************************************************************************************************************
 * Routine:                 ADC_H

 * Description:
 * ...
 * 
 * Creator:                 J. Rebetez
 * Date of creation:        17.08.2015
 * Last modification on:    -
 * Modified by:             - 
 * 
 * Input:                   -
 * Output:                  -
***********************************************************************************************************************/
#ifndef ADC_H
#define	ADC_H


#define _ADCenable      1
#define _ADCdisable     0


extern void InitADModule(void);
extern void InitADInterrupt(void);
extern void adc_EnableDisable(unsigned char uint8_ED);
extern void adc_LaunchNextMeasure(void);


typedef struct                  //definitions of multiple variable for timer2
{
    volatile unsigned char uint8_ConvStarted;       //1= conversion started / 0= conversion not started
    volatile unsigned long int uint32_UniIcoilA2;  
    volatile unsigned long int uint32_UniIcoilA1;
    volatile unsigned long int uint32_UniIcoilB2;
    volatile unsigned long int uint32_UniIcoilB1;
    volatile unsigned long int uint32_BipIcoilB;
    volatile unsigned long int uint32_BipVref;
    volatile unsigned long int uint32_Vmot;
    volatile unsigned long int uint32_Imot;
    volatile unsigned long int uint32_BipIcoilA;
    volatile unsigned long int uint32_Battery;
} SADC;                      //struct name is SADC (S for struct)

#endif	/* ADC_H */