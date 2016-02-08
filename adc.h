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


#define _AUTO   1
#define _MAN    2

extern void InitADModule(void);
extern void InitADInterrupt(void);
extern void adc_LaunchNextMeasure(void);


typedef struct                  //definitions of multiple variable for timer2
{
    volatile unsigned char uint8_ConvStarted;       //1= conversion started / 0= conversion not started
    //volatile unsigned char uint8_ChannelSelect;     //variable is used to select the channel
    volatile unsigned char uint8_NextMeasureID;     //help variable for measure ID
    volatile unsigned char uint8_MeasuredValueID;   //variable to know for what the ADC value stands
    volatile unsigned short int uint16_UniIcoilA2;  
    volatile unsigned short int uint16_UniIcoilA1;
    volatile unsigned short int uint16_UniIcoilB2;
    volatile unsigned short int uint16_UniIcoilB1;
    volatile unsigned short int uint16_BipIcoilB;
    volatile unsigned short int uint16_BipVref;
    volatile unsigned short int uint16_Vmot;
    volatile unsigned short int uint16_Imot;
    volatile unsigned short int uint16_BipIcoilA;
    volatile unsigned short int uint16_Battery;
} SADC;                      //struct name is SADC (S for struct)

#endif	/* ADC_H */