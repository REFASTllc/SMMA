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
extern void bi_ImotToDAC(unsigned long int uint32_current);


#define	_BiPhON		1       //definition ON for the output of the bipolar stepper motor
#define _BiPhOFF    0       //definition OFF for the output of the bipolar stepper motor
#define _BiFreqMax  1500    //definition of maximal frequency

typedef union
{
    unsigned char REG;
    struct
    {
        unsigned char firstStepIsActived:1;
        unsigned char lastStepIsActived:1;
        unsigned char accelerationIsActived:1;
        unsigned char decelerationIsActived:1;
        unsigned char nextStepIsAllowed:1;
        unsigned char goalIsReached:1;
        unsigned char nu:1;
        unsigned char error:1;
    } BITS;
} T_STATUS;

typedef struct
{
    unsigned char statePhA1:1;          //output state of phase A1
    unsigned char statePhA2:1;          //output state of phase A2
    unsigned char statePhB1:1;          //output state of phase B1
    unsigned char statePhB2:1;          //output state of phase B2
} T_MOTOR;

typedef struct          //definitions of multiple variable for unipolar
{
// Custom defined types
    T_STATUS status;
    T_MOTOR motBipol;
//general variables  
    volatile unsigned long int uint32_RealPos;      //real position of the motor
    volatile unsigned long int uint32_GoalPos;      //goal position of the motor
    volatile unsigned short int uint16_RunFreq;       //run frequency for normal run
//variables for timer 23
    volatile unsigned long int uint32_IntTime;      //effective interrupt time
    volatile unsigned long int uint32_RunTime;      //run time for interrupt
    volatile unsigned long int uint32_SwOnTime;     //switch ON time for interrupt
    volatile unsigned long int uint32_SwOffTime;    //switch OFF time for interrupt
//continue general variables    
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
    volatile unsigned char uint1_IsBipolEnabled:1;      //
    volatile unsigned char uint1_IntTimeExpiredFlag;  //1= interrupt time expired / 0= interrupt time not expired
    volatile unsigned long int uint32_NextTime;     //next time for timer
} Sbipol;                 //struct name is SUni (S for struct)

#endif	/* UNIPOLAR_H */
