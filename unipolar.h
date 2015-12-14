/**********************************************************************************************************************

                                            UBD - Unipolar Bipolar Driver

***********************************************************************************************************************
 * File name:               unipolar.h
 * Creation date:           30.09.2015
 * Main creator:            A. Staub
 * Company:                 REFAST GmbH
 *                          Copyright (c) 2015 REFAST GmbH
***********************************************************************************************************************
 * Content overview :       - UNIPOLAR
***********************************************************************************************************************/

/**********************************************************************************************************************
 * Routine:                 UNIPOLAR_H

 * Description:
 * ...
 * 
 * Creator:                 A. Staub
 * Date of creation:        30.09.2015
 * Last modification on:    -
 * Modified by:             - 
 * 
 * Input:                   -
 * Output:                  -
***********************************************************************************************************************/

#ifndef UNIPOLAR_H
#define	UNIPOLAR_H

extern void uni_init(void);
extern void uni_move(void);
extern void uni_acc(void);
extern void uni_run(void);
extern void uni_dec(void);
extern void uni_CheckCalc(void);


#define	_UniPhON		1       //definition ON for the output of the unipolar stepper motor
#define _UniPhOFF       0       //definition OFF for the output of the unipolar stepper motor
#define _UniFreqMax     1000    //definition of maximal frequency


typedef struct          //definitions of multiple variable for unipolar
{
    volatile unsigned long int uint32_RealPos;      //real position of the motor
    volatile unsigned long int uint32_GoalPos;      //goal position of the motor
    //volatile unsigned int uint16_StepTime;            //step time (1 step = 100us, max time = 65535*100us = 6.5535s)
    volatile unsigned short int uint16_RunFreq;       //run frequency for normal run
    //volatile unsigned int uint16_RampFreqTime;        //ramp frequency and time for acceleration and deceleration
    volatile unsigned char uint8_PhA1;          //output state of phase A1
    volatile unsigned char uint8_PhA2;          //output state of phase A2
    volatile unsigned char uint8_PhB1;          //output state of phase B1
    volatile unsigned char uint8_PhB2;          //output state of phase B2
    volatile unsigned char uint8_Status;        //motor status - definition see uni_init()
    volatile unsigned char uint8_Settings;      //motor status - definition see uni_init()
    //volatile unsigned char uint8_RampCount;           //counter for ramp (acceleration and deceleration) 
    volatile unsigned char uint8_FsHsCount;     //full or half step counter
    //volatile unsigned char uint8_WRramp;              //write- and read-pointer for array RampFreq and RampStep
    //volatile unsigned char uint8_NumbRamp;            //number of ramps - how much ramps are stored in the array
    //volatile unsigned char *rampfreqptr;              //pointer for ramp frequency
    //volatile unsigned char *rampstepptr;              //pointer for ramp step
    //volatile unsigned char uint8_RampFreq[ArrSize];   //array for frequency of ramp
    //volatile unsigned char uint8_RampStep[ArrSize];   //array for ramp step
 //variables for timer 2
    volatile unsigned short int uint16_LastTime;      //last time to wait
    volatile unsigned short int uint16_Count;         //counter for the timer - how many time to wait the interrupt time
    volatile unsigned short int uint16_RunLastTime;   //last time to wait in run mode
    volatile unsigned short int uint16_RunCount;      //counter - how many time to wait the interrupt time run mode 
    volatile unsigned short int uint16_SwOnLastTime;  //last time to wait for one time the switch on phase
    volatile unsigned short int uint16_SwOnCount;     //counter - how many time to wait the interrupt time for switch on
    volatile unsigned short int uint16_SwOffLastTime; //last time to wait for one time the switch off phase 
    volatile unsigned short int uint16_SwOffCount;    //counter - how many time to wait the interrupt time for switch off
    //volatile unsigned int uint16_RampLastTime;        //last time to wait in ramp mode
    //volatile unsigned int uint16_RampCount;           //counter - how many time to wait the interrupt time ramp mode
 //variables for acceleration
    //volatile unsigned char uint8_AccNumbRamp;         //how much different ramps in acceleration are to execute 
    volatile unsigned char uint8_AccArrPos;     //used to have access on the array
    volatile unsigned short int uint16_AccNumbStep;   //how much steps are to do with this acceleration / frequency
    volatile unsigned long int uint32_AccStop;      //stop position for the acceleration
    //volatile unsigned int uint16_AccLastTime;         //last time to wait before acceleration
    //volatile unsigned int uint16_AccCount;            //counter - how many time to wait the interrupt time before acceleration
//variables for deceleration
    //volatile unsigned char uint8_DecNumbRamp;         //how much different ramps in deceleration are to execute 
    volatile unsigned char uint8_DecArrPos;     //used to have access on the array
    volatile unsigned short int uint16_DecNumbStep;   //how much steps are to do with this deceleration / frequency 
    volatile unsigned long int uint32_DecStart;     //when we have to start the deceleration
    //volatile unsigned int uint16_DecLastTime;         //last time to wait after deceleration
    //volatile unsigned int uint16_DecCount;            //counter - how many time to wait the interrupt time after deceleration 
} SUni;                 //struct name is SUni (S for struct)

#endif	/* UNIPOLAR_H */
