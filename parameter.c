/**********************************************************************************************************************

                                            UBD - Unipolar Bipolar Driver

***********************************************************************************************************************
 * File name:               parameter.c
 * Creation date:           05.10.2015
 * Main creator:            Alain Staub
 * Company:                 REFAST GmbH
 *                          Copyright (c) 2015 REFAST GmbH
***********************************************************************************************************************
 * Content overview:        - param_init
***********************************************************************************************************************/


#include "includes.h"

SParam g_Param;             //global variables for struct  


/**********************************************************************************************************************
 * Routine:                 param_init

 * Description:
 * This routine initialize the basic things.
 * 
 * Creator:                 A. Staub
 * Date of creation:        05.10.2015
 * Last modification on:    -
 * Modified by:             - 
 * 
 * Input:                   -
 * Output:                  -
***********************************************************************************************************************/
void param_init(void)
{
    auto unsigned char uint8_WB;        //local work byte
  
//initialize the variables to protect the device
    g_Param.uint16_UmotGlobMin = 5000;  //minimal voltage to use the device
    g_Param.uint16_UmotGloMax = 48000;  //maximal voltage to use the device
    g_Param.uint16_IGloMax = 3200;      //maximal current to use the device
    
//initialize the variable for that the user knows there was a POR
    g_Param.uint8_NotPOR = 0;           //POR detected
    
//initialize the customer variables 
    g_Param.uint16_Imin = 20;           //range 20...2'800 mA
    g_Param.uint16_Imax = 800;          //range 20...2'800 mA
    g_Param.uint16_Umot = 12000;        //range 7'000...45'000 mV
    g_Param.uint16_SCiLevel = 1000;     //range 100...3'000 mA
    g_Param.uint8_SCtLevel = 10;        //range 1...100 ms 
    g_Param.uint8_MotTyp = 'N';         //U (unipolar), B (bipolar), M (matrix), L (LIN), N (OFF)
    g_Param.uint8_StepMode = 1;         //1 (full step two phase ON)
                                        //0 (full step one phase ON)
                                        //2 (half step)
                                        //3 (half step compensated) 
                                        //4 (micro step quarter)
                                        //5 (micro step sixteenth)
    g_Param.uint32_StepCount = 320;     //range 0...10'000'000
    g_Param.uint32_FreeStepCount = 80;  //range 0...10'000'000
    g_Param.uint16_RunFreq = 200;       //range 0...1'000 Hz
    g_Param.uint8_Direction = 0;        //0=R=CW / 1=L=CCW
    g_Param.uint8_CoilState = 0;        //0 (switched off) / 1 (switched on)
    g_Param.uint8_BipIPct = 61;         //range 0...100 %
    g_Param.uint16_BipRunI = 1400;      //range 20...2'800 mA
    g_Param.uint16_BipHoldI = 60;       //range 20...2'800 mA
    g_Param.uint8_SelectRamp = 0;       //0 (no ramps)
                                        //1 (only acceleration ramp)
                                        //2 (acceleration and deceleration ramp)
    g_Param.uint16_BipAccI = 1400;      //range 20...2'800 mA
    g_Param.uint16_BipDecI = 1400;      //range 20...2'800 mA
    g_Param.uint16_AccOnDelay = 50;     //range 0...1'000 ms
    g_Param.uint16_DecOffDelay = 50;    //range 0...1'000 ms
    g_Param.uint8_Acknowledge = 0;      //0 (without quittance)
                                        //1 (with quittance)
  
  for(uint8_WB=0; uint8_WB<50; uint8_WB++)  //define the ramp arrays 
  {
    //range 1...1'000
    g_Param.uint16_AccNumbStep[uint8_WB] = 1;
    g_Param.uint16_DecNumbStep[uint8_WB] = 1;
    //range 1...20'000
    g_Param.uint16_AccFreq[uint8_WB] = 200; 
    g_Param.uint16_DecFreq[uint8_WB] = 200;  
  } 
 
    g_Param.uint8_AccNumbRamp = 50;     //set the number of ramps for acceleration
    g_Param.uint8_DecNumbRamp = 50;     //set the number of ramps for deceleration
    //g_Param.uint8_ValPort = 0;          //range 0...255 <-- don't delete it, maybe I need it again A. Staub
    //g_Param.uint8_Bit = 0;              //range 0...7 <-- don't delete it, maybe I need it again A. Staub
    g_Param.uint8_TestVoltage = 0;      //range 0...7 V
  
    //define the current level array for microstepping
    g_Param.uint16_BipILevel[0] = 78;   //range 20...2'500 mA
    g_Param.uint16_BipILevel[1] = 156;  //range 20...2'500 mA
    g_Param.uint16_BipILevel[2] = 232;  //range 20...2'500 mA
    g_Param.uint16_BipILevel[3] = 306;  //range 20...2'500 mA
    g_Param.uint16_BipILevel[4] = 377;  //range 20...2'500 mA
    g_Param.uint16_BipILevel[5] = 444;  //range 20...2'500 mA
    g_Param.uint16_BipILevel[6] = 508;  //range 20...2'500 mA
    g_Param.uint16_BipILevel[7] = 566;  //range 20...2'500 mA
    g_Param.uint16_BipILevel[8] = 618;  //range 20...2'500 mA
    g_Param.uint16_BipILevel[9] = 665;  //range 20...2'500 mA
    g_Param.uint16_BipILevel[10] = 706; //range 20...2'500 mA
    g_Param.uint16_BipILevel[11] = 739; //range 20...2'500 mA
    g_Param.uint16_BipILevel[12] = 766; //range 20...2'500 mA
    g_Param.uint16_BipILevel[13] = 785; //range 20...2'500 mA
    g_Param.uint16_BipILevel[14] = 796; //range 20...2'500 mA
    g_Param.uint16_BipILevel[15] = 800; //range 20...2'500 mA
  
    g_Param.uint32_Sw1min = 200;        //range 0...300'000 
    g_Param.uint32_Sw1max = 250;        //range 0...300'000
    g_Param.uint32_Sw2min = 400;        //range 0...300'000
    g_Param.uint32_Sw2max = 450;        //range 0...300'000
    g_Param.uint32_Sw1pos = 0;          //range 0...300'000
    g_Param.uint32_Sw2pos = 0;          //range 0...300'000
    g_Param.uint32_SwPWMmin = 9000;     //range 0...300'000
    g_Param.uint32_SwPWMmax = 10000;    //range 0...300'000
    g_Param.uint32_SwPWMpos = 0;        //range 0...300'000
    g_Param.uint16_SwPWMval = 0;        //range 0.0...100.0
    g_Param.uint8_SwType = 2;           //2 (linear hall-sensor with PWM)
                                        //0 (mechanical end swithc)
                                        //1 (hall-sensor switch)
                                        //3 (digital encoder)
    g_Param.uint32_SwFrqVal = 0;        //range 0.0 - 100'000.0 Hz
    g_Param.uint16_Ue = 0;              //reset the result for Ue
    g_Param.uint16_IcoilA1 = 0;         //reset the result in current coil A1
    g_Param.uint16_IcoilA2 = 0;         //reset the result in current coil A2
    g_Param.uint16_IcoilB1 = 0;         //reset the result in current coil B1
    g_Param.uint16_IcoilB2 = 0;         //reset the result in current coil B2
    g_Param.uint8_ErrCode = 0;          //reset the error code
    g_Param.sint16_Temp = 0;            //reset the temperature
    g_Param.uint8_Sec = 0;              //reset the seconds
    g_Param.uint8_Min = 0;              //reset the minutes
    g_Param.uint8_Hrs = 0;              //reset the hours
    g_Param.uint8_Day = 1;              //reset the day
    g_Param.uint8_Wday = 1;             //reset the weekday
    g_Param.uint8_Month = 1;            //reset the weekday
    g_Param.uint8_Year = 0;             //reset the year
    g_Param.uint8_LinStatus = 0;        //reset the lin bus status
    g_Param.uint32_LinSpd = 9600;       //baud rate of the lin bus
    g_Param.uint16_LinTO = 500;         //timeout for the communication = 500ms
}   //end of param_init
