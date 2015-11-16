/**********************************************************************************************************************

                                            UBD - Unipolar Bipolar Driver

***********************************************************************************************************************
 * File name:               parameter.h
 * Creation date:           05.10.2015
 * Main creator:            A. Staub
 * Company:                 REFAST GmbH
 *                          Copyright (c) 2015 REFAST GmbH
***********************************************************************************************************************
 * Content overview :       - PARAMETER
***********************************************************************************************************************/

/**********************************************************************************************************************
 * Routine:                 PARAMETER_H

 * Description:
 * ...
 * 
 * Creator:                 A. Staub
 * Date of creation:        05.10.2015
 * Last modification on:    -
 * Modified by:             - 
 * 
 * Input:                   -
 * Output:                  -
 * Global variable:         -
***********************************************************************************************************************/

#ifndef PARAMETER_H
#define	PARAMETER_H

extern void param_init(void);  

//definition of the corresponding application limits
#define _UmotBipMax         30000   //<OK> maximum voltage for the bipolar application
#define _UmotLinMax         24000   //<OK> maximum voltage for the Lin application
#define _ImotLinMax         800     //<OK> maximum current for the Lin application
#define _ImotLinSCiLevelMax 1000    //<OK> maximal short-circuit current limit for the Lin application

//definition of the global limits
#define _Imin               20          //<OK> minimal ON current for the coils in [mA]
#define _Imax               2800        //<OK> maximal ON current for the coils in [mA]
#define _UmotMin            7000        //<OK> minimal test voltage for the actuators in [mV]
#define _UmotMax            45000       //<OK> maximal test voltage for the actuators in [mV]
#define _SCiLevelMin        100         //<OK> minimal short-circuit current limit in [mA]
#define _SCiLevelMax        3200        //<OK> maximal short-circuit current limit in [mA]
#define _SCtLevelMin        1           //<OK> minimal short-circuit time in [ms]
#define _SCtLevelMax        100         //<OK> maximal short-circuit time in [ms]
#define _AccNumbStepMin     1           //<OK> minimal number of steps for the frequency for acceleration
#define _AccNumbStepMax     1000        //<OK> maximal number of steps for the frequency for acceleration
#define _AccFreqMin         1           //<OK> minimal frequency for the number of steps in [Hz] for acceleration
#define _AccFreqMax         1000        //<OK> maximal frequency for the number of stpes in [Hz] for acceleration
#define _DecNumbStepMin     1           //<OK> minimal number of steps for the frequency for deceleration
#define _DecNumbStepMax     1000        //<OK> maximal number of steps for the frequency for deceleration
#define _DecFreqMin         1           //<OK> minimal frequency for the number of steps in [Hz] for deceleration
#define _DecFreqMax         1000        //<OK> maximal frequency for the number of stpes in [Hz] for deceleration
#define _StepModeUniMatMin  0           //<OK> supported or minimal step mode for unipolar and matrix actuators
#define _StepModeUniMatMax  2           //<OK> supported or maximal step mode for unipolar and matrix actuators
#define _StepCountMin       0           //<OK> minimal step count 
#define _StepCountMax       10000000    //<OK> maximal step count
#define _RunFreqMin         1           //<OK> minimal run speed in [Hz]
#define _RunFreqMax         1000        //<OK> maximal run speed in [Hz]
#define _DirMin             0           //<OK> minimal for direction
#define _DirMax             1           //<OK> maximal for direction
#define _CoilStateMin       0           //<OK> minimal for coil state
#define _CoilStateMax       1           //<OK> maximal for coil state
#define _SelectRampMin      0           //<OK> minimal for select ramp
#define _SelectRampMax      2           //<OK> maximal for select ramp
#define _AccOnDelayMin      1           //<OK> minimal time before start the move in [ms]
#define _AccOnDelayMax      1000        //<OK> maximal time before start the move in [ms]
#define _DecOffDelayMin     1           //<OK> minimal time after the move is finished in [ms]
#define _DecOffDelayMax     1000        //<OK> maximal time after the move is finished in [ms]
#define _AckMin             0           //<OK> minimal for protocol
#define _AckMax             1           //<OK> maximal for protocol
#define _BipRunIMin         20          //<OK> minimal chopper current during move in [mA]
#define _BipRunIMax         2800        //<OK> maximal chopper current during move in [mA]
#define _BipHoldIMin        20          //<OK> minimal holding chopper current in [mA]
#define _BipHoldIMax        2800        //<OK> maximal holding chopper current in [mA]
#define _BipAccIMin         20          //<OK> minimal chopper current for acceleration in [mA]
#define _BipAccIMax         2800        //<OK> maximal chopper current for acceleration in [mA]
#define _BipDecIMin         20          //<OK> minimal chopper current for deceleration in [mA]
#define _BipDecIMax         2800        //<OK> maximal chopper current for deceleration in [mA]
#define _BipIPctMin         0           //<OK> minimal percentage for the half step compensated in [%]
#define _BipIPctMax         100         //<OK> maximal percentage for the half step compensated in [%]


/*
#define StepModeMin       0           //OK range for step mode -> minimum
#define StepModeMax       5           //OK range for step mode -> maximum

#define FreeStepCountMin  0           //minimal free step count
#define FreeStepCountMax  7200000     //maximal free step count
#define BitMin            0           //OK range for bit --> minimum
#define BitMax            7           //OK range for bit --> maximum
#define ValPortMin        0           //OK minimal value in decimal from the I/O port 
#define ValPortMax        255         //OK maximal value in decimal from the I/O port
//#define TestVoltageMin    0           //minimal test voltage for the calibration
//#define TestVoltageMax    7           //maximal test voltage for the calibration
#define CurrentLevelMin   20          //OK minimal current level in [mA]
#define CurrentLevelMax   3000        //OK maximal current level in [mA]
#define _NumStepsMin        1       //<OK> minimal number of steps for the frequency 
#define _NumStepsMax        1000    //<OK> maximal number of steps for the frequency
#define _FreqMin            1           //OK minimal frequency for the number of steps in [Hz]
#define _FreqMax            20000       //OK maximal frequency for the number of stpes in [Hz]
*/ 
/*
#define SW1min            0           //OK minimal limit low of the switch position 1 in steps
#define SW1max            300000      //OK maximal limit high of the switch position 1 in steps
#define SW2min            0           //OK minimal limit low of the switch position 2 in steps
#define SW2max            300000      //OK maximal limit high of the switch position 2 in steps 
//#define SW1PosMin         0           //minimal switch position 1 in steps
//#define SW1PosMax         300000      //maximal switch position 1 in steps
//#define SW2PosMin         0           //minimal switch position 2 in steps
//#define SW2PosMax         300000      //maximal switch position 2 in steps
#define SWpwmMin          0           //OK minimal step position low by a PWM of 50% in steps
#define SWpwmMax          300000      //OK maximal step position high by a PWM of 50% in steps
#define SWpwmPosMin       0           //OK minimal position by 50% PWM of the hall-sensor in steps
#define SWpwmPosMax       300000      //OK maximal position by 50% PWM of the hall-sensor in steps
#define SWpwmValMin       0           //OK minimal % PWM of the hall-sensor 
#define SWpwmValMax       1000        //OK maximal % PWM of the hall-sensor (1000 means 100.0) 
#define SWfrqValMin       0           //OK minimal frequency (torque signal) in [Hz]
#define SWfrqValMax       1000000     //OK maximal frequency (torque singal) in [Hz] (1000000 means 100000.0)
#define SWtypeMin         0           //OK range for switch type --> minimum
#define SWtypeMax         3           //OK range for switch type --> maximum 
*/


typedef struct                      //definitions of multiple variable for command
{
//the follow parameters are used to protect the device
    volatile unsigned int uint16_UmotGlobMin;       //minimal actuator voltage in [mV]
    volatile unsigned int uint16_UmotGloMax;        //minimal actuator voltage in [mV]
    volatile unsigned int uint16_IGloMax;           //maximal actuator current in [mV]
//the follow parameter is used for that the user knows there was a POR
    volatile unsigned char uint8_NotPOR;            //POR detection; 1 = OK no POR detected; 0 = POR detected
//the follow parameters can be set by the customer
    volatile unsigned int uint16_Imin;              //minimal ON current for the coils in [mA]
    volatile unsigned int uint16_Imax;              //maximal ON current for the coils in [mA]
    volatile unsigned int uint16_Umot;              //test voltage for the actuators in [mV]
    volatile unsigned char uint8_MotTyp;            //motor type in ASCII
    volatile unsigned char uint8_StepMode;          //step mode of the actuator in decimal
    volatile unsigned long uint32_StepCount;        //step count in decimal
    volatile unsigned long uint32_FreeStepCount;    //free step count in decimal
    volatile unsigned int uint16_RunFreq;           //speed [steps / seconds]
    volatile unsigned char uint8_Direction;         //direction in decimal
    volatile unsigned char uint8_CoilState;         //coil switched on or off after move
    volatile unsigned char uint8_BipIPct;           //bipolar; current percentage for half step compensated in [%]
    volatile unsigned int uint16_BipRunI;           //bipolar; chopper current during move in [mA]
    volatile unsigned int uint16_BipHoldI;          //bipolar; chopper hold current in [mA]
    volatile unsigned char uint8_SelectRamp;        //select ramp option
    volatile unsigned int uint16_BipAccI;           //bipolar; chopper current for acceleration ramp in [mA]
    volatile unsigned int uint16_BipDecI;           //bipolar; chopper current for deceleration ramp in [mA]
    volatile unsigned int uint16_AccOnDelay;        //switching on time for the coils before start the move in [ms]   
    volatile unsigned int uint16_DecOffDelay;       //switching off time for the coils after the move in [ms]    
    volatile unsigned char uint8_Acknowledge;       //quittance after sending the run command
    volatile unsigned int uint16_AccNumbStep[50];   //array of: number of steps for the acceleration
    volatile unsigned int uint16_AccFreq[50];       //array of: frequency for the acceleration
    volatile unsigned char uint8_AccNumbRamp;       //acceleration - how many ramps are stored
    volatile unsigned int uint16_DecNumbStep[50];   //array of: number of steps for the deceleration
    volatile unsigned int uint16_DecFreq[50];       //array of: frequency for the deceleration
    volatile unsigned char uint8_DecNumbRamp;       //deceleration - how many ramps are stored  
    volatile unsigned char uint8_ValPort;           //value in decimal from the I/O port
    volatile unsigned char uint8_Bit;               //bit of the I/O port
    volatile unsigned char uint8_TestVoltage;       //known voltage reference for the calibartion 
    volatile unsigned int uint16_CurrentLevel[16];  //array of: current level for the uStepping [mA]
    volatile unsigned int uint16_SCiLevel;          //short-circuit current limit [mA]
    volatile unsigned char uint8_SCtLevel;          //wait time before switch off by a short-circuit [ms]
    volatile unsigned long uint32_Sw1min;           //limit low of the switch position 1 in steps
    volatile unsigned long uint32_Sw1max;           //limit high of the switch position 1 in steps
    volatile unsigned long uint32_Sw2min;           //limit low of the switch position 2 in steps
    volatile unsigned long uint32_Sw2max;           //limit high of the switch position 2 in steps
    volatile unsigned long uint32_Sw1pos;           //actually switch position 1 in steps
    volatile unsigned long uint32_Sw2pos;           //actually switch position 2 in steps
    volatile unsigned long uint32_SwPwmMin;         //step position limit low by a PWM value of 50%
    volatile unsigned long uint32_SwPwmMax;         //step position limit high by a PWM value of 50%
    volatile unsigned long uint32_SwPwmPos;         //position in steps by 50% PWM of the Hall-Sensor
    volatile unsigned int uint16_SwPwmVal;          //% PWM of the Hall-Sensor at the actual position 
    volatile unsigned char uint8_SwType;            //type of connected detection of the position
    volatile unsigned long uint32_SwFrqVal;         //frequency (torque signal) in [Hz]
    volatile unsigned int uint16_Ue;                //entry voltage in [mV]
    volatile unsigned int uint16_IcoilA1;           //current coil A1 in [mA]
    volatile unsigned int uint16_IcoilA2;           //current coil A2 in [mA]
    volatile unsigned int uint16_IcoilB1;           //current coil B1 in [mA]
    volatile unsigned int uint16_IcoilB2;           //current coil B2 in [mA] 
    volatile unsigned char uint8_ErrCode;           //error code which is send back to the master 
}   SParam;                         //struct name is SParam (S for struct)
extern SParam    g_Param;           //extern name is g_Param (g for global)

#endif	/* PARAMETER_H */