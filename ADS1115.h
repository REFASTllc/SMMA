/**********************************************************************************************************************

                                            UBD - Unipolar Bipolar Driver

***********************************************************************************************************************
 * File name:               ADS1115.h
 * Creation date:           22.02.2016
 * Main creator:            A. Staub
 * Company:                 REFAST GmbH
 *                          Copyright (c) 2015 REFAST GmbH
***********************************************************************************************************************
 * Content overview :       - ADS1115
***********************************************************************************************************************/

/**********************************************************************************************************************
 * Routine:                 ADS1115_H

 * Description:
 * ...
 * 
 * Creator:                 A. Staub
 * Date of creation:        22.02.2016
 * Last modification on:    -
 * Modified by:             - 
 * 
 * Input:                   -
 * Output:                  -
***********************************************************************************************************************/

#ifndef ADS1115_H
#define	ADS1115_H


extern void ads1115_init(void);
extern void ads1115_config(unsigned char uint8_MUX, unsigned char uint8_PGA, unsigned char uint8_Mode, unsigned char uint8_DR, unsigned char uint8_CompPol, unsigned long int uint8_CompLat, unsigned long int uint8_CompQue);
extern unsigned short int ads1115_read(void);
extern void ads1115_SetChannel(unsigned char uint8_MUX, unsigned char uint8_PGA);


#define _ADS1115Addr            0x90    //definition of the address for the ADS
#define _PR_ConvReg             0x00    //pointer register on conversion register
#define _PR_ConfReg             0x01    //pointer register on config register
#define _PR_LoReg               0x02    //pointer register on lo tresh register
#define _PR_HiReg               0x03    //pointer register on hi tresh register

//defines for the mux
#define _AIN0p_AIN1n            0x00    //AIN0 positive & AIN1 negative
#define _AIN0p_AIN3n            0x01    //AIN0 positive & AIN3 negative
#define _AIN1p_AIN3n            0x02    //AIN1 positive & AIN3 negative
#define _AIN2p_AIN3n            0x03    //AIN2 positive & AIN3 negative
#define _AIN0p_GND              0x04    //AIN0 positive with GND
#define _AIN1p_GND              0x05    //AIN1 positive with GND
#define _AIN2p_GND              0x06    //AIN2 positive with GND
#define _AIN3p_GND              0x07    //AIN3 positive with GND

//defines for the PGA
#define _FS6144mV               0x00    //full scale +/- 6.144V
#define _FS4096mV               0x01    //full scale +/- 4.096V
#define _FS2048mV               0x02    //full scale +/- 2.048V
#define _FS1024mV               0x03    //full scale +/- 1.024V
#define _FS512mV                0x04    //full scale +/- 0.512V
#define _FS256mV                0x05    //full scale +/- 0.256V

//defines for mode
#define _CCM                    0x00    //continious conversion mode
#define _PSSM                   0x01    //power-down single shot mode

//defines for data rates
#define _8SPS                   0x00    //8 samples per second
#define _16SPS                  0x01    //16 sps
#define _32SPS                  0x02    //32 sps
#define _64SPS                  0x03    //64 sps
#define _128SPS                 0x04    //128 sps
#define _250SPS                 0x05    //250 sps
#define _475SPS                 0x06    //475 sps
#define _860SPS                 0x07    //860 sps

//defines for the comparator polarity
#define _ActiveLow              0x00    //active low
#define _ActiveHigh             0x01    //active high

//defines for the latching comparator
#define _NonLatchingComp        0x00    //non latching comparator
#define _LatchingComp           0x01    //latching comparator

//defines for the comparator queue and disable
#define _AAOC                   0x00    //assert after one conversion
#define _AATC                   0x01    //assert after two conversions
#define _AAFC                   0x02    //assert after four conversions
#define _DC                     0x03    //disable comparator


typedef struct                  //definitions of multiple variable for ADS1115
{
    volatile unsigned char uint8_ConfigMSB;     //MSB register of the config
    volatile unsigned char uint8_ConfigLSB;     //LSB register of the config
} SADS1115;                      //struct name is SADS1115 (S for struct)


#endif	/* ADS1115_H */
