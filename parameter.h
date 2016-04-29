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
***********************************************************************************************************************/

#ifndef PARAMETER_H
#define	PARAMETER_H

extern void param_init(void);  

//definition of the corresponding application limits
#define _VmotBipMax         28000   //maximum voltage for the bipolar application
#define _VmotBipMin         7000    //minimum voltage for the bipolar application
#define _VmotUniMax         40000   //maximum voltage for the unipolar application
#define _VmotUniMin         7000    //minimum voltage for the unipolar application
#define _VmotLinMax         24000   //maximum voltage for the Lin application
#define _VmotLinMin         5000    //minimum voltage for the lin application
#define _ImotBipMax         3000    //maximum current for bipolar application
#define _ImotBipMin         50      //minimum current for bipolar application
#define _ImotUniMax         3000    //maximum current for unipolar application
#define _ImotUniMin         50      //minimum current for unipolar application
#define _ImotLinMax         800     //maximum current for the Lin application
#define _ImotLinMin         50      //minimum current for the lin application

//definition of the global limits
//#define _Imin               20          //<OK> minimal ON current for the coils in [mA]
//#define _Imax               2800        //<OK> maximal ON current for the coils in [mA]
//#define _UmotMin            7000        //<OK> minimal test voltage for the actuators in [mV]
//#define _UmotMax            40000       //<OK> maximal test voltage for the actuators in [mV]
#define _SCiLevelMin        0x0         //minimal short-circuit current limit in [mA]
#define _SCiLevelMax        0xFFFFFFFF  //maximal short-circuit current limit in [mA]
#define _SCtLevelMin        0x0         //minimal short-circuit time in [ms]
#define _SCtLevelMax        0xFFFFFFFF  //maximal short-circuit time in [ms]
#define _AccNumbStepMin     1           //minimal number of steps for the frequency for acceleration
#define _AccNumbStepMax     1000        //maximal number of steps for the frequency for acceleration
#define _AccFreqMin         1           //minimal frequency for the number of steps in [Hz] for acceleration
#define _AccFreqMax         1500        //maximal frequency for the number of stpes in [Hz] for acceleration
#define _DecNumbStepMin     1           //minimal number of steps for the frequency for deceleration
#define _DecNumbStepMax     1000        //maximal number of steps for the frequency for deceleration
#define _DecFreqMin         1           //minimal frequency for the number of steps in [Hz] for deceleration
#define _DecFreqMax         1500        //maximal frequency for the number of stpes in [Hz] for deceleration
#define _StepModeMin        0           //supported or minimal step mode for unipolar and matrix actuators
#define _StepModeMax        5           //supported or maximal step mode for unipolar and matrix actuators
#define _StepCountMin       0           //minimal step count 
#define _StepCountMax       40000000    //maximal step count
#define _FreeStepCountMin   0           //minimal free step count 
#define _FreeStepCountMax   40000000    //maximal free step count
#define _RunFreqMin         1           //minimal run speed in [Hz]
#define _RunFreqMax         1500        //maximal run speed in [Hz]
#define _DirMin             0           //minimal for direction
#define _DirMax             1           //maximal for direction
#define _CoilStateMin       0           //minimal for coil state
#define _CoilStateMax       1           //maximal for coil state
#define _SelectRampMin      0           //minimal for select ramp
#define _SelectRampMax      2           //maximal for select ramp
#define _AccOnDelayMin      5           //minimal time before start the move in [ms]
#define _AccOnDelayMax      1000        //maximal time before start the move in [ms]
#define _DecOffDelayMin     5           //minimal time after the move is finished in [ms]
#define _DecOffDelayMax     1000        //maximal time after the move is finished in [ms]
#define _AckMin             0           //minimal for protocol
#define _AckMax             1           //maximal for protocol
#define _BipRunIMin         20          //minimal chopper current during move in [mA]
#define _BipRunIMax         2800        //maximal chopper current during move in [mA]
#define _BipHoldIMin        20          //minimal holding chopper current in [mA]
#define _BipHoldIMax        2800        //maximal holding chopper current in [mA]
#define _BipAccIMin         20          //minimal chopper current for acceleration in [mA]
#define _BipAccIMax         2800        //maximal chopper current for acceleration in [mA]
#define _BipDecIMin         20          //minimal chopper current for deceleration in [mA]
#define _BipDecIMax         2800        //maximal chopper current for deceleration in [mA]
#define _BipIPctMin         0           //minimal percentage for the half step compensated in [%]
#define _BipIPctMax         100         //maximal percentage for the half step compensated in [%]
#define _BipILevelMin       20          //minimal micro stepping current level in [mA]
#define _BipILevelMax       2800        //maximal micro stepping current level in [mA]
#define _BitMin             0           //minimal range in decimal for bit
#define _BitMax             7           //maximal range in decimal for bit
#define _ValPortMin         0           //minimal value in decimal from the I/O port 
#define _ValPortMax         255         //maximal value in decimal from the I/O port
#define _SW1min             0           //minimal limit low of the switch position 1 in steps
#define _SW1max             300000      //maximal limit high of the switch position 1 in steps
#define _SW2min             0           //minimal limit low of the switch position 2 in steps
#define _SW2max             300000      //maximal limit high of the switch position 2 in steps
#define _SWpwmMin           0           //minimal step position low by a PWM of 50% in steps
#define _SWpwmMax           300000      //maximal step position high by a PWM of 50% in steps
#define _SWpwmPosMin        0           //minimal position by 50% PWM of the hall-sensor in steps
#define _SWpwmPosMax        300000      //maximal position by 50% PWM of the hall-sensor in steps
#define _SWpwmValMin        0           //minimal % PWM of the hall-sensor 
#define _SWpwmValMax        1000        //maximal % PWM of the hall-sensor (1000 means 100.0) 
#define _SWtypeMin          0           //range for switch type --> minimum
#define _SWtypeMax          3           //range for switch type --> maximum
#define _RomAddrMin         0           //range of the EEPROM address 
#define _RomAddrMax         32768       //range of the EEPROM adddress
#define _RomValMin          0           //range of the data for the EEPROM
#define _RomValMax          255         //range of the data for the EEPROM
#define _SecMin             0           //range for the seconds of the RTC
#define _SecMax             59          //range for the seconds of the RTC
#define _MinMin             0           //range for the minutes of the RTC
#define _MinMax             59          //range for the minutes of the RTC
#define _HrsMin             0           //range for the hours of the RTC
#define _HrsMax             23          //range for the hours of the RTC
#define _DayMin             1           //range for the days of the RTC
#define _DayMax             31          //range for the days of the RTC
#define _WdayMon            3592        //range for the weekday monday of the RTC
#define _WdayTue            4343        //range for the weekday tuesday of the RTC
#define _WdayWed            4482        //range for the weekday wednesday of the RTC
#define _WdayThu            4229        //range for the weekday thusday of the RTC
#define _WdayFri            2917        //range for the weekday friday of the RTC
#define _WdaySat            4058        //range for the weekday saturday of the RTC
#define _WdaySun            4252        //range for the weekday sunday of the RTC
#define _MoJan              3152        //range for the month january
#define _MoFeb              2780        //range for the month february
#define _MoMar              3456        //range for the month march
#define _MoApr              2406        //range for the month april
#define _MoMay              3463        //range for the month may
#define _MoJun              3352        //range for the month june
#define _MoJul              3350        //range for the month july
#define _MoAug              2445        //range for the month august
#define _MoSep              4094        //range for the month september
#define _MoOct              3678        //range for the month october
#define _MoNov              3700        //range for the month november
#define _MoDec              2581        //range for the month december
#define _YearMin            0           //range for the year
#define _YearMax            79          //range for the year
#define _LinSpdMin          100         //range for the lin speed communication
#define _LinSpdMax          200000      //range for the lin speed communication
#define _LinToMin           1           //range for the lin timeout communication
#define _LinToMax           10000       //range for the lin timeout communication
#define _LinLengthMin       0           //range for the lin slave answer
#define _LinLengthMax       40          //range for the lin slave answer
#define _LinParaMin         0           //range for the lin parameter
#define _LinParaMax         255         //range for the lin parameter
#define _LinResMin          0           //range for the lin response
#define _LinResMax          1           //range for the lin response
#define _BipSyrMin          0           //range for the bipolar SYR
#define _BipSyrMax          1           //range for the bipolar SYR
#define _BipDcyMin          0           //range for the bipolar DCY
#define _BipDcyMax          3           //range for the bipolar DCY
#define _BipHlrMin          0           //range for the bipolar HLR
#define _BipHlrMax          1           //range for the bipolar HLR
#define _BipTscMin          0           //range for the bipolar TSC
#define _BipTscMax          3           //range for the bipolar TSC
#define _BipOlMin           0           //range for the bipolar OL
#define _BipOlMax           3           //range for the bipolar OL
#define _BipStsMin          0           //range for the bipolar STS
#define _BipStsMax          2           //range for the bipolar STS
#define _BipCdMin           1           //range for the bipolar CD
#define _BipCdMax           255         //range for the bipolar CD
#define _BipPfdMin          0           //range for the bipolar PFD
#define _BipPfdMax          7           //range for the bipolar PFD
#define _BipTbkMin          0           //range for the bipolar TBK
#define _BipTbkMax          3           //range for the bipolar TBK
#define _BipTofMin          0           //range for the bipolar TOF
#define _BipTofMax          7           //range for the bipolar TOF
#define _BipFrqMin          0           //range for the bipolar FRQ
#define _BipFrqMax          7           //range for the bipolar FRQ
#define _BipSlewMin         0           //range for the bipolar SLEW
#define _BipSlewMax         1           //range for the bipolar SLEW
#define _SFRQbitMin         0           //range for the frequency bit
#define _SFRQbitMax         1           //range for the frequency bit
#define _SRUNbitMin         0           //range for the run bit
#define _SRUNbitMax         1           //range for the run bit

typedef struct                      //definitions of multiple variable for command
{
//the follow parameters can be set by the customer
    volatile unsigned short int uint16_Imin;              //minimal ON current for the coils in [mA]
    volatile unsigned short int uint16_Imax;              //maximal ON current for the coils in [mA]
    volatile unsigned short int uint16_Umot;              //test voltage for the actuators in [mV]
    volatile unsigned char uint8_MotTyp;            //motor type in ASCII ('U', 'B', 'L' or 'N')
    volatile unsigned char uint8_StepMode;          //step mode of the actuator in decimal
    volatile unsigned long int uint32_StepCount;        //step count in decimal
    volatile unsigned long int uint32_FreeStepCount;    //free step count in decimal
    volatile unsigned short int uint16_RunFreq;           //speed [steps / seconds]
    volatile unsigned char uint8_Direction;         //direction in decimal
    volatile unsigned char uint8_CoilState;         //coil switched on or off after move
    volatile unsigned char uint8_BipIPct;           //bipolar; current percentage for half step compensated in [%]
    volatile unsigned short int uint16_BipRunI;           //bipolar; chopper current during move in [mA]
    volatile unsigned short int uint16_BipHoldI;          //bipolar; chopper hold current in [mA]
    volatile unsigned char uint8_SelectRamp;        //select ramp option
    volatile unsigned short int uint16_BipAccI;           //bipolar; chopper current for acceleration ramp in [mA]
    volatile unsigned short int uint16_BipDecI;           //bipolar; chopper current for deceleration ramp in [mA]
    volatile unsigned short int uint16_BipILevel[16];     //array of: current level for the uStepping [mA]
    volatile unsigned short int uint16_AccOnDelay;        //switching on time for the coils before start the move in [ms]   
    volatile unsigned short int uint16_DecOffDelay;       //switching off time for the coils after the move in [ms]    
    volatile unsigned char uint8_Acknowledge;       //quittance after sending the run command
    volatile unsigned short int uint16_AccNumbStep[50];   //array of: number of steps for the acceleration
    volatile unsigned short int uint16_AccFreq[50];       //array of: frequency for the acceleration
    volatile unsigned char uint8_AccNumbRamp;       //acceleration - how many ramps are stored
    volatile unsigned short int uint16_DecNumbStep[50];   //array of: number of steps for the deceleration
    volatile unsigned short int uint16_DecFreq[50];       //array of: frequency for the deceleration
    volatile unsigned char uint8_DecNumbRamp;       //deceleration - how many ramps are stored  
    //volatile unsigned char uint8_ValPort;           //value in decimal from the I/O port <-- don't delte it, mybe I need it again A. Staub
    //volatile unsigned char uint8_Bit;               //bit of the I/O port <-- don't delete it, maybe I need it again A. Staub
    volatile unsigned char uint8_TestVoltage;       //known voltage reference for the calibartion 
    volatile unsigned short int uint16_SCiLevel;          //short-circuit current limit [mA]
    volatile unsigned char uint8_SCtLevel;          //wait time before switch off by a short-circuit [ms]
    volatile unsigned long int uint32_Sw1min;           //limit low of the switch position 1 in steps
    volatile unsigned long int uint32_Sw1max;           //limit high of the switch position 1 in steps
    volatile unsigned long int uint32_Sw2min;           //limit low of the switch position 2 in steps
    volatile unsigned long int uint32_Sw2max;           //limit high of the switch position 2 in steps
    volatile unsigned long int uint32_Sw1pos;           //actually switch position 1 in steps
    volatile unsigned long int uint32_Sw2pos;           //actually switch position 2 in steps
    volatile unsigned long int uint32_SwPWMmin;         //step position limit low by a PWM value of 50%
    volatile unsigned long int uint32_SwPWMmax;         //step position limit high by a PWM value of 50%
    volatile unsigned long int uint32_SwPWMpos;         //position in steps by 50% PWM of the Hall-Sensor
    volatile unsigned short int uint16_SwPWMval;          //% PWM of the Hall-Sensor at the actual position 
    volatile unsigned char uint8_SwType;            //type of connected detection of the position
    volatile unsigned long int uint32_SwFrqVal;     //frequency (torque signal) in [Hz]
    volatile unsigned short int uint16_Ue;                //entry voltage in [mV]
    volatile unsigned short int uint16_IcoilA1;           //current coil A1 in [mA]
    volatile unsigned short int uint16_IcoilA2;           //current coil A2 in [mA]
    volatile unsigned short int uint16_IcoilB1;           //current coil B1 in [mA]
    volatile unsigned short int uint16_IcoilB2;           //current coil B2 in [mA] 
    volatile unsigned char uint8_ErrCode;           //error code which is send back to the master 
    volatile signed short int sint16_Temp;          //temperature value in [°C]
    volatile unsigned char uint8_Sec;               //seconds in decimal
    volatile unsigned char uint8_Min;               //minutes in decimal
    volatile unsigned char uint8_Hrs;               //hours in decimal
    volatile unsigned char uint8_Day;               //day in decimal 
    volatile unsigned char uint8_Wday;              //weekday in decimal 
    volatile unsigned char uint8_Month;             //month in decimal
    volatile unsigned char uint8_Year;              //year in decimal
    volatile unsigned char uint8_LinStatus;         //status of the lin driver
    volatile unsigned long int uint32_LinSpd;       //speed (baud rate) of the lin driver
    volatile unsigned short int uint16_LinTO;       //timeout for the lin bus communication
    volatile unsigned char uint8_LinRes;            //response mode for the lin bus communication
    volatile unsigned char uint8_SYR;               //bipolar synchronous rectification mode
    volatile unsigned char uint8_DCY;               //bipolar decay mode selection
    volatile unsigned char uint8_HLR;               //bipolar selects slow decay and brake recirculation path
    volatile unsigned char uint8_TCS;               //bipolar overcurrent fault delay
    volatile unsigned char uint8_OL;                //bipolar open load current threshold
    volatile unsigned char uint8_STS;               //bipolar stall detection scheme
    volatile unsigned char uint8_PWMcount;          //bipolar PWM count for stall detection
    volatile unsigned char uint8_PFD;               //bipolar fast decay time for mixed decay
    volatile unsigned char uint8_TBK;               //bipolar blank time used in PWM mode
    volatile unsigned char uint8_TOF;               //bipolar off time in PWM mode
    volatile unsigned char uint8_FRQ;               //bipolar frequency
    volatile unsigned char uint8_SLEW;              //bipolar select slow rate control 
    volatile unsigned char uint8_FreqBit;           //frequency bit on output 1
    volatile unsigned char uint8_RunBit;            //run bit on output0
    volatile unsigned char uint8_GOcmdTrue;         //1= GO command / 0 = no GO command
    volatile unsigned char uint8_ERUN1;             //switch detection for ERUN command
    volatile unsigned char uint8_ERUN2;             //switch detection for ERUN command
    volatile unsigned char uint8_ERUNdetected;      //switch detected
    volatile unsigned char uint8_ERUNactive;        //first time in the command
}   SParam;                         //struct name is SParam (S for struct)

#endif	/* PARAMETER_H */
