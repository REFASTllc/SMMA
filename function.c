/**********************************************************************************************************************

                                            UBD - Unipolar Bipolar Driver

***********************************************************************************************************************
 * File name:               function.c
 * Creation date:           28.09.2015
 * Main creator:            Alain Staub
 * Company:                 REFAST GmbH
 *                          Copyright (c) 2015 REFAST GmbH
***********************************************************************************************************************
 * Content overview:        - funct_init
 *                          - funct_IntToAscii
 *                          - funct_AsciiToInt
 *                          - funct_CheckTol
 *                          - funct_FreqToTimer23
 *                          - funct_msToTimer23
 *                          - funct_ReadRamp
 *                          - funct_CheckCmdSILIM
 *                          - funct_CheckMotType
 *                          - funct_LoadDeviceInfo  
 *                          - funct_FrontLED
 *                          - funct_OutputHandler
 *                          - funct_IOhandler
 *                          - funct_StoreWdayIntoRSbuffer
 *                          - funct_StoreMonthIntoRSbuffer
 *                          - funct_ADCtoMiliUnit
 *                          - funct_MiliVoltToOhm
***********************************************************************************************************************/


#include "includes.h"

SFunct g_Funct;       //global variables for struct  
extern SCmdChk g_CmdChk;
extern SParam g_Param;
extern SUni g_Uni;
extern Sbipol g_Bipol;
extern T_A3981 A3981;
/**********************************************************************************************************************
 * Routine:                 funct_init

 * Description:
 * This routine initialize the basic things.
 * 
 * Creator:                 A. Staub
 * Date of creation:        30.09.2015
 * Last modification on:    -
 * Modified by:             - 
 * 
 * Input:                   -
 * Output:                  -
***********************************************************************************************************************/
void funct_init(void)
{
//initialize global function struct
    //...
    
//define outputs
    oVmotOnOff = 0;                 //switch off the general supply   
}   //end of funct_init


/**********************************************************************************************************************
 * Routine:                 funct_IntToAscii

 * Description:
 * Converts an integer number of maximal 10 digits into an ASCII. The seperated digits/numbers are stored into the 
 * array 'uint8_ArrAscii' where the first number is on the first position and the last number on the last.
 * For example - convert the number 5689:
 * Contend of the array will be from place 0 - 3: 9865 --> so the inverted way.   
 * The variable 'uint8_ArrAsciiPos' decide on which place from the array is to store the next digit. 
 * So you can use (outside this subroutine) this variable to place the numbers again into the right order by using 
 * the decrement operation until you reach at 0.  
 * If you need more spaces then 10 digits, than change the size in the function.h file.
 * 
 * Option uint8_sending:
 * If you set this to true (_Active), then this subroutine will add the converted result directly into the send
 * buffer of the RS232. If you set this (_Inactive), it will not do this. 
 * 
 * Creator:                 A. Staub
 * Date of creation:        30.09.2015
 * Last modification on:    -
 * Modified by:             - 
 * 
 * Input:                   uint32_DataByte
 *                          uint8_sending
 * Output:                  -
***********************************************************************************************************************/
void funct_IntToAscii(unsigned long uint32_DataByte, unsigned char uint8_sending)
{
    auto unsigned long int uint32_WR;         //local work register
   
    g_Funct.uint8_ArrAsciiPos = 0;        //clear the position of the array
  
    //ToDo:
    //dataByte modulo with 10 and add to the result the offset of the ASCII character '0'
    //dataByte division with 10
    //Until:
    //the result of the division is 0
    do      
    {
        uint32_WR = uint32_DataByte % 10;
        uint32_WR += '0';
    
        g_Funct.uint8_ArrAscii[g_Funct.uint8_ArrAsciiPos] = uint32_WR & 0xFF;
        g_Funct.uint8_ArrAsciiPos++;
    
        uint32_DataByte = uint32_DataByte / 10;
    }
    while(uint32_DataByte);   
    
    //is direct sending active; means should the result send directly back over the RS232?
    if(uint8_sending)   
    {
        do  //store each character into the send buffer
        {
            g_Funct.uint8_ArrAsciiPos--;
            uart2_sendbuffer(g_Funct.uint8_ArrAscii[g_Funct.uint8_ArrAsciiPos]); 
        }
        while(g_Funct.uint8_ArrAsciiPos);
    }
    else
    {
        //direct sending not active; do nothing
    }  
}   //end of funct_IntToAscii


/**********************************************************************************************************************
 * Routine:                 funct_AsciiToInt

 * Description:
 * Convert a received ASCII of maximal 10 characters into an integer. First write the number of received characters 
 * into the uint8_Counter and add 1. This because of the do-while loop. Inside the loop, take out one character after 
 * one and verify if it is a dot. By receiving a dot do nothing. Otherwise take the received character and subtract 
 * 48 to have the real number.
 * Multiplie this character with uint32_Mul and add it with the end result.   
 * If you need more spaces then 10 characters, than change the size in the function.h file.
 * 
 * Creator:                 A. Staub
 * Date of creation:        30.09.2015
 * Last modification on:    -
 * Modified by:             - 
 * 
 * Input:                   -
 * Output:                  uint32_WR
***********************************************************************************************************************/
unsigned long funct_AsciiToInt(void)
{
    auto unsigned long int uint32_WR = 0;     //local work register
    auto unsigned long int uint32_Mul = 1;    //local work register for multiplication
    auto unsigned char uint8_WB;          //local work byte for the received character 
    auto unsigned char uint8_Counter;     //local work byte
  
    uint8_Counter = g_Funct.uint8_ArrAsciiPos + 1;
  
    //ToDo:
    //take one character after one and multiplie it with the variable uint32_Mul
    //if the character is a dot, then divide the result by 10
    //Until:
    //the position of the array is 0
    do
    {
        uint8_WB = g_Funct.uint8_ArrAscii[g_Funct.uint8_ArrAsciiPos];
        g_Funct.uint8_ArrAsciiPos--;
        uint8_Counter--;
    
        if(uint8_WB == '.')
        {
            //do nothing
        }
        else
        {
            //received character minus 48 to have the real number
            uint8_WB = uint8_WB - 48;
            //multiplie the number with the actually factor of uint32_Mul
            uint32_WR = uint32_WR + (uint8_WB * uint32_Mul);
            //factor of multiplication * 10
            uint32_Mul = uint32_Mul * 10;
        }
    }
    while(uint8_Counter);
    
    return uint32_WR;                     //return the converted result    
}   //end of funct_AsciiToInt


/**********************************************************************************************************************
 * Routine:                 funct_CheckTol

 * Description:
 * This subroutine compares the value with the minimum and maximal tolerance. 
 * If the result is within the tolerance it returns 1 otherwise the value 0.
 * 
 * Creator:                 A. Staub
 * Date of creation:        30.09.2015
 * Last modification on:    30.01.2016
 * Modified by:             J. Rebetez
 * Modification:            Optimization of the function. This one don't use a local variable anymore.
 * 
 * Input:                   uint32_Value
 *                          uint32_Min
 *                          uint32_Max
 * Output:                  uint8_Result
***********************************************************************************************************************/
unsigned char funct_CheckTol(unsigned long uint32_Value, unsigned long uint32_Min, unsigned long uint32_Max)
{ 
    if((uint32_Value >= uint32_Min) && (uint32_Value <= uint32_Max))
        return 1;                   //result is within the tolerance
    else
        return 0;                   //result is not within the tolerance
}   //end of funct_CheckTol


/**********************************************************************************************************************
 * Routine:                 funct_FreqToTimer23

 * Description:
 * Use this smalle subroutine if you want to convert a frequency into a counter for the timer 23. 
 * The time base is 25ns, so the frequency of this time base is 40MHz.
 * By making a division of 40MHz with the frequency given, it gives the number of counts; how much 
 * time we have to count with the 25ns time base to achive this time. Of course some frequencies 
 * have some small conversation errors but this are less than 0.01%.
 * 
 * Creator:                 A. Staub
 * Date of creation:        23.12.2015
 * Last modification on:    
 * Modified by:             
 * 
 * Input:                   uint32_Freq
 * Output:                  uint32_WB
***********************************************************************************************************************/
unsigned long int funct_FreqToTimer23(unsigned long int uint32_Freq)
{
    auto unsigned long int uint32_WB = 40000000;    //local work register for time base = 25ns
                                                    //calculated with 1 / 25ns = 40MHz
    
    uint32_WB = uint32_WB / uint32_Freq;            //calculation the needed wait time
                                                    //for example by 400 Hz (2.5ms):
                                                    //40'000'000 / 400 = 100k
                                                    //100k * 25ns time base = 2.5ms
                                                    //error of = 0%
                                                    //for example by 759 Hz (1.3175231ms):
                                                    //40'000'000 / 759 = 52k700
                                                    //52k7 * 25ns time base = 1.3175ms
                                                    //error of = 0.00175%
    
    return uint32_WB;       //send back the result
}   //end of funct_FreqToTimer23


/**********************************************************************************************************************
 * Routine:                 funct_msToTimer23

 * Description:
 * Use this small subroutine to convert a given time in ms into a counter for the timer 23.
 * The time base is 25ns and this is a frequency of 40MHz. Like we want to have the time in ms, we 
 * devide this time by 1000 and have 40kHz. Multiply this 40'000 with the wished time in ms 
 * and this give you the amounts of 25ns counts. 
 * 
 * Creator:                 A. Staub
 * Date of creation:        23.12.2015
 * Last modification on:    
 * Modified by:             
 * 
 * Input:                   uint32_msTime
 * Output:                  uint32_WB
***********************************************************************************************************************/
unsigned long int funct_msToTimer23(unsigned long int uint32_msTime)
{
    auto unsigned long int uint32_WB = 40000;       //local work register for time base = 25ns
                                                    //calculated with 40MHz / 1000 (for ms) = 40kHz
    
    uint32_WB = uint32_WB * uint32_msTime;          //calculation the needed wait time
                                                    //for example by 50ms:
                                                    //40'000 * 50 = 2M
                                                    //2M * 25ns time base = 50ms
                                                    //error of = 0%
                                                    //for example by 23ms:
                                                    //40'000 * 23 = 920k
                                                    //920 * 25ns time base = 23ms
                                                    //error of = 0%
    
    return uint32_WB;       //send back the result 
}   //end of funct_msToTimer23


/**********************************************************************************************************************
 * Routine:                 funct_ReadRamp

 * Description:
 * This subroutine reads out a variable from the ramp array acceleration or deceleration 
 * 
 * Creator:                 A. Staub
 * Date of creation:        03.10.2015
 * Last modification on:    -
 * Modified by:             - 
 * 
 * Input:                   uint8_kind (_Acc, _Dec)
 *                          uint8_sort (_Step, _Freq)
 *                          uint8_ArrPos (0...50)
 * Output:                  uint32_Freq 
 *                          OR
 *                          uint32_Step
***********************************************************************************************************************/
unsigned short int funct_ReadRamp(unsigned char uint8_kind,unsigned char uint8_sort,unsigned char uint8_ArrPos)
{
    if(uint8_kind)                  //read out a variable from the acceleration array?
    {
        if(uint8_sort)              //read out the number of steps?
        {
            return g_Param.uint16_AccNumbStep[uint8_ArrPos];
        }
        else                        //otherwise read out the frequency
        {
            return g_Param.uint16_AccFreq[uint8_ArrPos];
        }
    }
    else                            //otherwise read out a variable from the deceleration array
    {
        if(uint8_sort)              //read out the number of steps?
        {
            return g_Param.uint16_DecNumbStep[uint8_ArrPos];
        }
        else                        //otherwise read out the frequency
        {
            return g_Param.uint16_DecFreq[uint8_ArrPos];
        }
    }
}   //end of funct_ReadRamp


/**********************************************************************************************************************
 * Routine:                 funct_CheckCmdSILIM

 * Description:
 * This subroutine compares the value with the minimum and maximal tolerance, depending on the selected
 * application. Every good result increments the counter. It is used with the follow command(s):
 *  - cmd_SILIM
 * 
 * Creator:                 A. Staub
 * Date of creation:        17.10.2015
 * Last modification on:    -
 * Modified by:             - 
 * 
 * Input:                   -
 * Output:                  uint8_Result
***********************************************************************************************************************/
unsigned char funct_CheckCmdSILIM(void)
{
    auto unsigned char uint8_Result = 0;
    
    //verify if main supply is switched off or unipolar application is selected
    if((g_Param.uint8_MotTyp == 'N') || (g_Param.uint8_MotTyp == 'U') || (g_Param.uint8_MotTyp == 'M'))     
    {
        //then verify the received parameters
        uint8_Result += funct_CheckTol(g_CmdChk.uint32_TempPara[1],_Imin,_Imax);
        uint8_Result += funct_CheckTol(g_CmdChk.uint32_TempPara[2],_Imin,_Imax);
        uint8_Result += funct_CheckTol(g_CmdChk.uint32_TempPara[3],_UmotMin,_UmotMax);
        uint8_Result += funct_CheckTol(g_CmdChk.uint32_TempPara[4],_SCiLevelMin,_SCiLevelMax);
        uint8_Result += funct_CheckTol(g_CmdChk.uint32_TempPara[5],_SCtLevelMin,_SCtLevelMax);          
                
        if(uint8_Result == 5)   //each parameter within the tolerance?
        {
            //do nothing
        }
        else
        { 
            g_Param.uint8_ErrCode = _UniSILIM;  //set error code
        }
    }
    else if(g_Param.uint8_MotTyp == 'B')    //verify limits for the bipolar application
    {
        uint8_Result += funct_CheckTol(g_CmdChk.uint32_TempPara[1],_Imin,_Imax);
        uint8_Result += funct_CheckTol(g_CmdChk.uint32_TempPara[2],_Imin,_Imax);
        uint8_Result += funct_CheckTol(g_CmdChk.uint32_TempPara[3],_UmotMin,_UmotBipMax);
        uint8_Result += funct_CheckTol(g_CmdChk.uint32_TempPara[4],_SCiLevelMin,_SCiLevelMax);
        uint8_Result += funct_CheckTol(g_CmdChk.uint32_TempPara[5],_SCtLevelMin,_SCtLevelMax);          
                
        if(uint8_Result == 5)   //each parameter within the tolerance
        {
            //do nothing
        }
        else
        {
            g_Param.uint8_ErrCode = _BipSILIM;  //set error code
        }   
    }
    else if(g_Param.uint8_MotTyp == 'L')    //verify limits for the LIN application
    {
        uint8_Result += funct_CheckTol(g_CmdChk.uint32_TempPara[1],_Imin,_ImotLinMax);
        uint8_Result += funct_CheckTol(g_CmdChk.uint32_TempPara[2],_Imin,_ImotLinMax);
        uint8_Result += funct_CheckTol(g_CmdChk.uint32_TempPara[3],_UmotMin,_UmotLinMax);
        uint8_Result += funct_CheckTol(g_CmdChk.uint32_TempPara[4],_SCiLevelMin,_ImotLinSCiLevelMax);
        uint8_Result += funct_CheckTol(g_CmdChk.uint32_TempPara[5],_SCtLevelMin,_SCtLevelMax);          
                
        if(uint8_Result == 5)   //each parameter within the tolerance
        {
            //do nothing
        }
        else
        {
        g_Param.uint8_ErrCode = _LinSILIM;  //set error code
        } 
    }
    else
    {
        g_Param.uint8_ErrCode = _UnknownMotTyp; //set error code
    }
}   //end of funct_CheckActApplTol


/**********************************************************************************************************************
 * Routine:                 funct_CheckMotType

 * Description:
 * This small subroutine switch off or on the outputs needed for the different motor types. 
 * 
 * Creator:                 A. Staub
 * Date of creation:        25.01.2016
 * Last modification on:    -
 * Modified by:             - 
 * 
 * Input:                   -
 * Output:                  -
***********************************************************************************************************************/
void funct_CheckMotType(unsigned char uint8_MotType)
{
    if(uint8_MotType == 'N')                //switch off the main supply  
    {
        g_Param.uint8_MotTyp = 'N';         //set the motor type
        oVmotOnOff = 0;                     //switch off the main supply
        oEnaVLINSupply = 0;                 //switch off the lin supply
        oBiEnaVmot = 0;                     //switch off the bipolar supply     
    }
    else if (uint8_MotType == 'U')          //switch on the unipolar part
    {
        g_Param.uint8_MotTyp = 'U';         //set the motor type
        oVmotOnOff = 1;                     //switch on the main supply
        oEnaVLINSupply = 0;                 //switch off the lin supply
        oBiEnaVmot = 0;                     //switch off the bipolar supply
    }
    else if (uint8_MotType == 'M')
    {
        g_Param.uint8_MotTyp = 'M';         //set the motor type
        oVmotOnOff = 1;                     //switch on the main supply
        oEnaVLINSupply = 0;                 //switch off the lin supply
        oBiEnaVmot = 0;                     //switch off the bipolar supply
    }
    else if(uint8_MotType == 'B')           //verify limits for the bipolar application
    {
        g_Param.uint8_MotTyp = 'B';         //set the motor type
        oEnaVLINSupply = 0;                 //switch off the lin supply
        oBiEnaVmot = 1;                     //switch on the bipolar supply 
        oVmotOnOff = 1;                     //switch on the main supply 
    }
    else if(uint8_MotType == 'L')           //verify limits for the LIN application
    {
        g_Param.uint8_MotTyp = 'L';         //set the motor type
        oBiEnaVmot = 0;                     //switch off the bipolar supply 
        oEnaVLINSupply = 1;                 //switch on the lin supply
        oVmotOnOff = 1;                     //switch on the main supply
    }
    else
    {
        g_Param.uint8_MotTyp = 'N';         //set the motor type
        oVmotOnOff = 0;                     //switch off the main supply
        oEnaVLINSupply = 0;                 //switch off the lin supply
        oBiEnaVmot = 0;                     //switch off the bipolar supply 
    }
}   //end of funct_CheckMotType


/**********************************************************************************************************************
 * Routine:                 funct_LoadDeviceInfo

 * Description:
 * Load the send buffer of the UART2 with the device information's for GVER & RAZ command
 * 
 * Creator:                 A. Staub
 * Date of creation:        19.10.2015
 * Last modification on:    -
 * Modified by:             - 
 * 
 * Input:                   -
 * Output:                  -
***********************************************************************************************************************/
extern S_PROD prod;
void funct_LoadDeviceInfo(void)
{
    auto signed short int sint16_WB;    //local work integer
    unsigned char i = 0;
    
    uart2_sendbuffer('E');
    uart2_sendbuffer(',');
    uart2_sendbuffer('S');                   
    uart2_sendbuffer('M');
    uart2_sendbuffer('M');
    uart2_sendbuffer('A');
    uart2_sendbuffer(' ');
    uart2_sendbuffer('S');
    uart2_sendbuffer('/');
    uart2_sendbuffer('N');
    uart2_sendbuffer(' ');
    funct_IntToAscii(prod.serialNumber, _Active);   // serial number
    uart2_sendbuffer(' ');
    uart2_sendbuffer('F');
    uart2_sendbuffer('W');
    uart2_sendbuffer(' ');
    uart2_sendbuffer('v');
    for(;i<5;i++)
        uart2_sendbuffer(prod.fwVersion[i]);   // FW version
    uart2_sendbuffer(' ');
    uart2_sendbuffer('H');
    uart2_sendbuffer('W');
    uart2_sendbuffer(' ');
    uart2_sendbuffer(prod.hwVersion);
    uart2_sendbuffer(' ');
    uart2_sendbuffer('S');
    uart2_sendbuffer('T');
    uart2_sendbuffer('A');
    uart2_sendbuffer('T');
    uart2_sendbuffer('U');
    uart2_sendbuffer('S');
    uart2_sendbuffer(' ');
    funct_IntToAscii(prod.testStatus, _Active);
 /*   uart2_sendbuffer('0');
    uart2_sendbuffer('1');
    uart2_sendbuffer('.');
    uart2_sendbuffer('0');
    uart2_sendbuffer('0');
    uart2_sendbuffer(' ');
    uart2_sendbuffer('1');
    uart2_sendbuffer('1');
    uart2_sendbuffer('/');
    uart2_sendbuffer('1');
    uart2_sendbuffer('5');*/
    uart2_sendbuffer(' ');
    sint16_WB = g_Param.sint16_Temp;
    if(sint16_WB < 0)           //temp. smaller than 0°
    {
        uart2_sendbuffer('-');  
        sint16_WB ^= 0xFFFF;    //value XOR with 1         
        
    }
    else
    {
        uart2_sendbuffer('+');  //temp. greater than -1°
    }
    funct_IntToAscii(sint16_WB,_Active);
    uart2_sendbuffer('°');
    uart2_sendbuffer('C');
    uart2_sendbuffer(13);
}   //end of funct_LoadDeviceInfo


/**********************************************************************************************************************
 * Routine:                 funct_FrontLED

 * Description:
 * Define the color for the front LED
 * 
 * Creator:                 A. Staub
 * Date of creation:        28.11.2015
 * Last modification on:    -
 * Modified by:             - 
 * 
 * Input:                   uint8_color
 * Output:                  -
***********************************************************************************************************************/
void FrontLED(unsigned char uint8_color)
{
    switch(uint8_color)
    {
        case (0):   //switch off LED
            oFrontLedGRN = 0;
            oFrontLedRED = 0;
            break;
        
        case (1):   //switch on green LED
            oFrontLedGRN = 1;
            oFrontLedRED = 0;
            break;
            
        case (2):   //switch on red LED
            oFrontLedGRN = 0;
            oFrontLedRED = 1;
            break;
            
        case (3):   //switch on orange LED
            oFrontLedGRN = 1;
            oFrontLedRED = 1;
            break;
            
        default:    //not defined - switch off LED
            oFrontLedGRN = 0;
            oFrontLedRED = 0;
            break;
    }
}   //end of funct_FrontLED


/**********************************************************************************************************************
 * Routine:                 funct_OutputHandler

 * Description:
 * This subroutine handels the output port. It can set directly an output according to the received
 * parameter or clear and set an output pin separatly. 
 * Please note:
 * It is IMPORTANT to not change the definition of _ClrBit and _SetBit from the file function.h
 * 
 * Creator:                 A. Staub
 * Date of creation:        28.11.2015
 * Last modification on:    -
 * Modified by:             - 
 * 
 * Input:                   uint8_ToDo
 *                          uint8_value
 * Output:                  -
***********************************************************************************************************************/
void funct_OutputHandler(unsigned char uint8_ToDo,unsigned char uint8_value)
{
    if(uint8_ToDo == _SetPort)  //set directly a port?
    {
        oSinkSource0 = uint8_value;
        oSinkSource1 = uint8_value >> 1;
        oSinkSource2 = uint8_value >> 2;
        oSinkSource3 = uint8_value >> 3;
        oSinkSource4 = uint8_value >> 4;
        oSinkSource5 = uint8_value >> 5;
        oSinkSource6 = uint8_value >> 6;
        oSinkSource7 = uint8_value >> 7;
    }
    else if ((uint8_ToDo == _SetBit) || (uint8_ToDo == _ClrBit))    //clear or set a bit/output?    
    {
        switch(uint8_value) //verify which output must be set or clear
        {
            case (0):       //output 0?
                oSinkSource0 = uint8_ToDo;
                break;
                
            case (1):       //output 1?
                oSinkSource1 = uint8_ToDo;
                break;
                
            case (2):       //output 2?
                oSinkSource2 = uint8_ToDo;
                break;
                
            case (3):       //output 3?
                oSinkSource3 = uint8_ToDo;
                break;
                
            case (4):       //output 4?
                oSinkSource4 = uint8_ToDo;
                break;
                
            case (5):       //output 5?
                oSinkSource5 = uint8_ToDo;
                break;
                
            case (6):       //output 6?
                oSinkSource6 = uint8_ToDo;
                break;
                
            case (7):       //output 7?
                oSinkSource7 = uint8_ToDo;
                break;
                
            default:        //do nothing
                break;
        }
    }
    else
    {
        //do nothing - not defined!
    }
}   //end of funct_SetIOhandler


/**********************************************************************************************************************
 * Routine:                 funct_IOhandler

 * Description:
 * This subroutine handels the IO's to read out a port or read out a bit of the port.
 * 
 * Creator:                 A. Staub
 * Date of creation:        28.11.2015
 * Last modification on:    -
 * Modified by:             - 
 * 
 * Input:                   uint8_ToDo
 *                          uint8_IO
 *                          uint8_value
 * Output:                  -
***********************************************************************************************************************/
unsigned char funct_IOhandler(unsigned char uint8_ToDo,unsigned char uint8_IO,unsigned char uint8_value)
{
    auto unsigned char uint8_Result = 0;    //local work byte
    
    if(uint8_ToDo == _GetPort)  //read a complete port?
    {
        if(uint8_IO == _Input)          //from the input?
        {
            uint8_Result |= iSinkSource7 << 7;
            uint8_Result |= iSinkSource6 << 6;
            uint8_Result |= iSinkSource5 << 5;
            uint8_Result |= iSinkSource4 << 4;
            uint8_Result |= iSinkSource3 << 3;
            uint8_Result |= iSinkSource2 << 2;
            uint8_Result |= iSinkSource1 << 1;
            uint8_Result |= iSinkSource0; 
        }
        else if (uint8_IO == _Output)   //from the output?
        {
            uint8_Result |= oSinkSource7 << 7;
            uint8_Result |= oSinkSource6 << 6;
            uint8_Result |= oSinkSource5 << 5;
            uint8_Result |= oSinkSource4 << 4;
            uint8_Result |= oSinkSource3 << 3;
            uint8_Result |= oSinkSource2 << 2;
            uint8_Result |= oSinkSource1 << 1;
            uint8_Result |= oSinkSource0; 
        }
        else
        {
            //do nothing - not defined!
        }
    }
    else if (uint8_ToDo == _GetBit) //read only one bit?
    {
        if(uint8_IO == _Input)          //from the input?
        {
            switch(uint8_value)
            {
                case (0):   //input 0?
                    uint8_Result = iSinkSource0;
                    break;
                    
                case (1):   //input 1?
                    uint8_Result = iSinkSource1;
                    break;
                    
                case (2):   //input 2?
                    uint8_Result = iSinkSource2;
                    break;
                    
                case (3):   //input 3?
                    uint8_Result = iSinkSource3;
                    break;
                    
                case (4):   //input 4?
                    uint8_Result = iSinkSource4;
                    break;
                    
                case (5):   //input 5?
                    uint8_Result = iSinkSource5;
                    break;
                    
                case (6):   //input 6?
                    uint8_Result = iSinkSource6;
                    break;
                    
                case (7):   //input 7?
                    uint8_Result = iSinkSource7;
                    break;
                    
                default:
                    uint8_Result = 0;
                    break;  
            }
        }
        else if (uint8_IO == _Output)   //from the output?
        {
            switch(uint8_value)
            {
                case (0):   //output 0?
                    uint8_Result = oSinkSource0;
                    break;
                    
                case (1):   //output 1?
                    uint8_Result = oSinkSource1;
                    break;
                    
                case (2):   //output 2?
                    uint8_Result = oSinkSource2;
                    break;
                    
                case (3):   //output 3?
                    uint8_Result = oSinkSource3;
                    break;
                    
                case (4):   //output 4?
                    uint8_Result = oSinkSource4;
                    break;
                    
                case (5):   //output 5?
                    uint8_Result = oSinkSource5;
                    break;
                    
                case (6):   //output 6?
                    uint8_Result = oSinkSource6;
                    break;
                    
                case (7):   //output 7?
                    uint8_Result = oSinkSource7;
                    break;
                    
                default:
                    uint8_Result = 0;
                    break;                    
            }
        }
        else
        {
            //do nothing - not defined!
        }
    }
    else
    {
        //do nothing - not defined!
    }
    
    return uint8_Result;
}   //end of funct_IOhandler


/**********************************************************************************************************************
 * Routine:                 funct_WdayFillBuffer

 * Description:
 * Store weekday into RS232 buffer
 * 
 * Creator:                 A. Staub
 * Date of creation:        28.11.2015
 * Last modification on:    -
 * Modified by:             - 
 * 
 * Input:                   uint8_ToDo
 *                          uint8_IO
 *                          uint8_value
 * Output:                  -
***********************************************************************************************************************/
void funct_StoreWdayIntoRSbuffer(void)
{
    switch (g_Param.uint8_Wday)
    {
        case 1:
            uart2_sendbuffer('S');      //then the comma
            uart2_sendbuffer('u');      //then the comma
            uart2_sendbuffer('n');      //then the comma
            break;
                    
        case 2:
            uart2_sendbuffer('M');      //then the comma
            uart2_sendbuffer('o');      //then the comma
            uart2_sendbuffer('n');      //then the comma
            break;
                    
        case 3:
            uart2_sendbuffer('T');      //then the comma
            uart2_sendbuffer('u');      //then the comma
            uart2_sendbuffer('e');      //then the comma
            break;
                    
        case 4:
            uart2_sendbuffer('W');      //then the comma
            uart2_sendbuffer('e');      //then the comma
            uart2_sendbuffer('d');      //then the comma
            break;
                    
        case 5:
            uart2_sendbuffer('T');      //then the comma
            uart2_sendbuffer('h');      //then the comma
            uart2_sendbuffer('u');      //then the comma
            break;
                    
        case 6:
            uart2_sendbuffer('F');      //then the comma
            uart2_sendbuffer('r');      //then the comma
            uart2_sendbuffer('i');      //then the comma
            break;
                    
        case 7:
            uart2_sendbuffer('S');      //then the comma
            uart2_sendbuffer('a');      //then the comma
            uart2_sendbuffer('t');      //then the comma
            break;
                    
        default:
            //do nothing, this case will never called, because the value is verified 
            //already at the beginning
            break;
    }
}   //end of funct_WdayFillBuffer


/**********************************************************************************************************************
 * Routine:                 funct_StoreMonthIntoRSbuffer

 * Description:
 * Store month into RS232 buffer
 * 
 * Creator:                 A. Staub
 * Date of creation:        28.11.2015
 * Last modification on:    -
 * Modified by:             - 
 * 
 * Input:                   uint8_ToDo
 *                          uint8_IO
 *                          uint8_value
 * Output:                  -
***********************************************************************************************************************/
void funct_StoreMonthIntoRSbuffer(void)
{
    switch (g_Param.uint8_Month)
    {
        case 1:
            uart2_sendbuffer('J');      //then the comma
            uart2_sendbuffer('a');      //then the comma
            uart2_sendbuffer('n');      //then the comma
            break;
                    
        case 2:
            uart2_sendbuffer('F');      //then the comma
            uart2_sendbuffer('e');      //then the comma
            uart2_sendbuffer('b');      //then the comma
            break;
                    
        case 3:
            uart2_sendbuffer('M');      //then the comma
            uart2_sendbuffer('a');      //then the comma
            uart2_sendbuffer('r');      //then the comma
            break;
                    
        case 4:
            uart2_sendbuffer('A');      //then the comma
            uart2_sendbuffer('p');      //then the comma
            uart2_sendbuffer('r');      //then the comma
            break;
                    
        case 5:
            uart2_sendbuffer('M');      //then the comma
            uart2_sendbuffer('a');      //then the comma
            uart2_sendbuffer('y');      //then the comma
            break;
                    
        case 6:
            uart2_sendbuffer('J');      //then the comma
            uart2_sendbuffer('u');      //then the comma
            uart2_sendbuffer('n');      //then the comma
            break;
                    
        case 7:
            uart2_sendbuffer('J');      //then the comma
            uart2_sendbuffer('u');      //then the comma
            uart2_sendbuffer('l');      //then the comma
            break;
                    
        case 8:
            uart2_sendbuffer('A');      //then the comma
            uart2_sendbuffer('u');      //then the comma
            uart2_sendbuffer('g');      //then the comma
            break;
                    
        case 9:
            uart2_sendbuffer('S');      //then the comma
            uart2_sendbuffer('e');      //then the comma
            uart2_sendbuffer('p');      //then the comma
            break;
                    
        case 16:
            uart2_sendbuffer('O');      //then the comma
            uart2_sendbuffer('c');      //then the comma
            uart2_sendbuffer('t');      //then the comma
            break;
                    
        case 17:
            uart2_sendbuffer('N');      //then the comma
            uart2_sendbuffer('o');      //then the comma
            uart2_sendbuffer('v');      //then the comma
            break;
                    
        case 18:
            uart2_sendbuffer('D');      //then the comma
            uart2_sendbuffer('e');      //then the comma
            uart2_sendbuffer('c');      //then the comma
            break;
                    
        default:
            //do nothing, this case will never called, because the value is verified 
            //already at the beginning
            break;
    }
}   //end of funct_StoreMonthIntoRSbuffer


/**********************************************************************************************************************
 * Routine:                 funct_ADCtoMiliUnit

 * Description:
 * Use this routine to convert any ADC values to a result in mili, for example mV, mA, etc.
 * The subroutine needs the read ADC value and the ADC step which is the conversion factor.
 * 
 * ADCstep:
 * If the read value is not counterfeit, means you did not use a resistance divider to change the real value,
 * then it will be always the ADCres / Vnom, here = 1024 / 3.3 = 310.3 = 310
 * If for example a resistance divider was used of 15, the ADCstep will be:
 *      ADCres =        2^10 = 1024
 *      ResDivFact =    (R259 + R262) / R259 = (390k + 24k) / 24k = 17.25 
 *      ADC supply =    3.3
 *      Vnom =          3.3 * 17.25 = 56.925
 *      ADCstep =       ADCres / Vnom = 1024 / 56.925 = 17.99 = 18  
 *      ERRnom =        100% - [(100% * (1024/18)) / 56.925] = 0.063%
 * Example with the current:
 *      ADCres =        2^0 = 1024
 *      Factor I =      INA193 * Rshunt = 20 * 0.05 = 1
 *      Factor is 1, means there is not changing in the ADC value. The value is directly in mA
 *      and as factor for ADCstep can be used 310.     
 * 
 * Creator:                 A. Staub
 * Date of creation:        23.12.2015
 * Last modification on:    
 * Modified by:             
 * 
 * Input:                   uint32_ADCvalue
 *                          uint16_ADCstep
 * Output:                  uint32_WB
***********************************************************************************************************************/
unsigned long int funct_ADCtoMiliUnit(unsigned long int uint32_ADCvalue,unsigned short int uint16_ADCstep)
{
    auto unsigned long int uint32_WB = 0;   //local work register for the result
    
    //1000m range
    uint32_WB = (uint32_ADCvalue / uint16_ADCstep) * 1000;
    uint32_ADCvalue %= uint16_ADCstep;
    uint32_ADCvalue *= 10;
    
    //100m range
    uint32_WB += (uint32_ADCvalue / uint16_ADCstep) * 100;
    uint32_ADCvalue %= uint16_ADCstep;
    uint32_ADCvalue *= 10;
    
    //10m range
    uint32_WB += (uint32_ADCvalue / uint16_ADCstep) * 10;
    uint32_ADCvalue %= uint16_ADCstep;
    uint32_ADCvalue *= 10;
    
    //1m range
    uint32_WB += (uint32_ADCvalue / uint16_ADCstep);
    uint32_ADCvalue %= uint16_ADCstep;
    uint32_ADCvalue *= 10;
    
    return uint32_WB;       //send back the result 
}   //end of funct_ADCtoMiliUnit


/**********************************************************************************************************************
 * Routine:                 funct_MiliVoltToOhm

 * Description:
 * This routine converts a voltage in mV to an ohm value. The current source is 10mA that is the reason why
 * the result will be directly in ohm and stored into the sendbuffer of the UART with one number after the 
 * comma. 
 * 
 * Creator:                 A. Staub
 * Date of creation:        18.02.2016
 * Last modification on:    
 * Modified by:             
 * 
 * Input:                   uint32_Milivolt
 * Output:                  uint32_WB
***********************************************************************************************************************/
void funct_MiliVoltToOhm(unsigned long int uint32_Milivolt)
{
    volatile unsigned long int uint32_WB;
    
    uint32_WB = uint32_Milivolt / 10;
    funct_IntToAscii(uint32_WB,_Active);     
    uart2_sendbuffer('.');    
    uint32_WB = uint32_Milivolt % 10;
    funct_IntToAscii(uint32_WB,_Active); 
    uart2_sendbuffer('E'); 
}   //end of funct_MiliVoltToOhm