/**********************************************************************************************************************

                                            UBD - Unipolar Bipolar Driver

***********************************************************************************************************************
 * File name:               bipolar.h
 * Creation date:           21.12.2015
 * Main creator:            A. Staub / J. Rebetez
 * Company:                 REFAST GmbH
 *                          Copyright (c) 2015 REFAST GmbH
***********************************************************************************************************************
 * Content overview :       - BIPOLAR
***********************************************************************************************************************/

/**********************************************************************************************************************
 * Routine:                 BIPOLAR_H

 * Description:
 * ...
 * 
 * Creator:                 A. Staub / J. Rebetez
 * Date of creation:        21.12.2015
 * Last modification on:    -
 * Modified by:             - 
 * 
 * Input:                   -
 * Output:                  -
***********************************************************************************************************************/

#ifndef BIPOLAR_H
#define	BIPOLAR_H

extern void bi_init(void);
extern void bi_move(void);
extern void bi_acc(void);
extern void bi_run(void);
extern void bi_dec(void);
extern void bi_CheckCalc(void);


#define	_BiPhON		1       //definition ON for the output of the bipolar stepper motor
#define _BiPhOFF    0       //definition OFF for the output of the bipolar stepper motor
#define _BiFreqMax  1000    //definition of maximal frequency


typedef struct          //definitions of multiple variable for unipolar
{
    volatile unsigned long int uint32_RealPos;      //real position of the motor
    volatile unsigned long int uint32_GoalPos;      //goal position of the motor
    volatile unsigned short int uint16_RunFreq;       //run frequency for normal run
    volatile unsigned char uint8_PhA1;          //output state of phase A1
    volatile unsigned char uint8_PhA2;          //output state of phase A2
    volatile unsigned char uint8_PhB1;          //output state of phase B1
    volatile unsigned char uint8_PhB2;          //output state of phase B2
    volatile unsigned char uint8_Status;        //motor status - definition see uni_init()
 //variables for timer 2
    volatile unsigned short int uint16_LastTime;      //last time to wait
    volatile unsigned short int uint16_Count;         //counter for the timer - how many time to wait the interrupt time
    volatile unsigned short int uint16_RunLastTime;   //last time to wait in run mode
    volatile unsigned short int uint16_RunCount;      //counter - how many time to wait the interrupt time run mode 
    volatile unsigned short int uint16_SwOnLastTime;  //last time to wait for one time the switch on phase
    volatile unsigned short int uint16_SwOnCount;     //counter - how many time to wait the interrupt time for switch on
    volatile unsigned short int uint16_SwOffLastTime; //last time to wait for one time the switch off phase 
    volatile unsigned short int uint16_SwOffCount;    //counter - how many time to wait the interrupt time for switch off
    volatile unsigned char uint8_AccArrPos;     //used to have access on the array
    volatile unsigned short int uint16_AccNumbStep;   //how much steps are to do with this acceleration / frequency
    volatile unsigned long int uint32_AccStop;      //stop position for the acceleration
    volatile unsigned char uint8_DecArrPos;     //used to have access on the array
    volatile unsigned short int uint16_DecNumbStep;   //how much steps are to do with this deceleration / frequency 
    volatile unsigned long int uint32_DecStart;     //when we have to start the deceleration
    volatile unsigned char uint1_NextStepIsRamp:1;  // Is the next step a ramp?
    volatile unsigned char uint1_IsDecNeeded:1;     // Is deceleration phase needed?
    volatile unsigned char uint1_CurrInCoilAtTheEnd:1; // Is current still needed after the last step?
    volatile unsigned char uint1_ErrConfig:1;           // Error flag of motor configuration
} Sbipol;                 //struct name is SUni (S for struct)

#endif	/* UNIPOLAR_H */
