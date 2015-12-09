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
 *                          - funct_FreqToTimer2
 *                          - funct_msToTimer2
 *                          - funct_ReadRamp
 *                          - funct_CheckCmdSILIM
 *                          - funct_CheckCmdSMTYP
 *                          - funct_LoadDeviceInfo  
 *                          - funct_FrontLED
 *                          - funct_OutputHandler
 *                          - funct_IOhandler
***********************************************************************************************************************/


#include "includes.h"

SFunct g_Funct;       //global variables for struct  


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
    auto unsigned long uint32_WR;         //local work register
   
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
    auto unsigned long uint32_WR = 0;     //local work register
    auto unsigned long uint32_Mul = 1;    //local work register for multiplication
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
 * Last modification on:    -
 * Modified by:             - 
 * 
 * Input:                   uint32_Value
 *                          uint32_Min
 *                          uint32_Max
 * Output:                  uint8_Result
***********************************************************************************************************************/
unsigned char funct_CheckTol(unsigned long uint32_Value, unsigned long uint32_Min, unsigned long uint32_Max)
{
    auto unsigned long uint8_Result;        //local work byte
  
    if((uint32_Value >= uint32_Min) && (uint32_Value <= uint32_Max))
    {
        uint8_Result = 1;                   //result is within the tolerance
    }
    else
    {
        uint8_Result = 0;                   //result is not within the tolerance
    }
  
    return uint8_Result;    
}   //end of funct_CheckTol


/**********************************************************************************************************************
 * Routine:                 funct_FreqToTimer2

 * Description:
 * There are two local variables. One is the time base that is acutally the time base from the timer. 
 * The other one is for the rest value after the divison. 
 * First we calculate the needed wait time for the frequency by divide the time base with the frequency. 
 * The rest of this division we store into the second local variabel. Then we verify if there is a rest available.
 * If true:
 * We load the last time with this rest and subract this rest from the total time. Then we verify if the total time 
 * is not 0. If it is not 0, then write into the counter how much time the interrupt time must be executed and 
 * increment this counter by 1, because the last time is different from the interrupt time. If it 0, then we fix the 
 * counter to 1, because we only need the last time to wait
 * If false:
 * Here the last time correspond to the interrupt time, so we only have to store into the counter how many times we 
 * have to wait the interrupt time.   
 * 
 * Creator:                 A. Staub
 * Date of creation:        30.09.2015
 * Last modification on:    -
 * Modified by:             - 
 * 
 * Input:                   uint32_Freq
 *                          uint16_IntTime
 * Output:                  -
***********************************************************************************************************************/
void funct_FreqToTimer2(unsigned long uint32_Freq, unsigned int uint16_IntTime)
{
    auto unsigned long uint32_WB = 10000000;    //local work register for time base = 100ns
    auto unsigned long uint32_WB1 = 0;          //local work register for the rest (modulo)
    
    uint32_WB = uint32_WB / uint32_Freq;        //calculation the needed wait time
    uint32_WB1 = uint32_WB % uint16_IntTime;    //calculation the rest 
   
    if(uint32_WB1)                              //is a rest available?
    {
        g_Uni.uint16_LastTime = uint32_WB1;     //then load the last time with the rest time
        uint32_WB = uint32_WB - uint32_WB1;     //subtract the last time from the total time
    
        if(uint32_WB)                           //result not 0?
        {
            //then verify how many times it must wait the interrupt time
            g_Uni.uint16_Count = uint32_WB / uint16_IntTime;  
            g_Uni.uint16_Count++;               //and add plus 1, because the last time is different 
                                                //and not the interrupt time
        }
        else
        {
            g_Uni.uint16_Count = 1;             //otherwise set it fix to 1
        }   
    }
    else
    {
        g_Uni.uint16_LastTime = uint16_IntTime; //otherwise load the last time with the interrupt time
        //verify how many times more this interrupt time must be wait
        g_Uni.uint16_Count = uint32_WB / uint16_IntTime;    
    }   
}   //end of funct_FreqToTimer2


/**********************************************************************************************************************
 * Routine:                 funct_msToTimer2

 * Description:
 * This subroutine does the same thing as the 'funct_FreqToTimer2' the only difference is that we have not a 
 * frequency - we have a time in ms. So we multiply the time with 1000 to have the correct timebase (100ns) and make 
 * the same calculations (see 'funct_FreqToTimer2').   
 * 
 * Creator:                 A. Staub
 * Date of creation:        30.09.2015
 * Last modification on:    -
 * Modified by:             - 
 * 
 * Input:                   uint16_msTime
 *                          int16_IntTime
 * Output:                  -
***********************************************************************************************************************/
void funct_msToTimer2(unsigned int uint16_msTime, unsigned int uint16_IntTime)
{
    auto unsigned long uint32_WB = 10000;       //local work register for time base = 100ns
    auto unsigned long uint32_WB1 = 0;          //local work register for the rest (modulo)
    
    uint32_WB = uint32_WB * uint16_msTime;      //multipli to the ms time the needed time base 
    uint32_WB1 = uint32_WB % uint16_IntTime;    //calculation the rest
   
    if(uint32_WB1)                              //is a rest available?
    {
        g_Uni.uint16_LastTime = uint32_WB1;     //then load the last time with the rest time
        uint32_WB = uint32_WB - uint32_WB1;     //subtract the last time from the total time
    
        if(uint32_WB)                           //result not 0?
        {
            //then verify how many times it must wait the interrupt time
            g_Uni.uint16_Count = uint32_WB / uint16_IntTime;    
            //and add plus 1, because the last time is different and not the interrupt time
            g_Uni.uint16_Count++;
        }
        else
        {
            g_Uni.uint16_Count = 1;             //otherwise set it fix to 1
        }   
    }
    else
    {
        g_Uni.uint16_LastTime = uint16_IntTime; //otherwise load the last time with the interrupt time
        //verify how many times more this interrupt time must be wait
        g_Uni.uint16_Count = uint32_WB / uint16_IntTime;
    }   
}   //end of funct_msToTimer2


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
unsigned int funct_ReadRamp(unsigned char uint8_kind,unsigned char uint8_sort,unsigned char uint8_ArrPos)
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
 * Routine:                 funct_CheckCmdSMTYP

 * Description:
 * This subroutine compares the currently limits with the minimum and maximal tolerance, depending on the whished
 * application. Every good result increments the counter. It is used with the follow command(s):
 *  - cmd_SMTYP
 * This subroutine is quite the same as funct_CheckActApplTol instead of that we don't compare the received
 * parameters. Here we compare the values that are stored inside the g_Param.xxxxx
 * If the user wants to switch from unipolar to LIN and the parameters are not correct, then nothing is done.
 * If all is correct, then the system switch on the needed output. 
 * 
 * Creator:                 A. Staub
 * Date of creation:        17.10.2015
 * Last modification on:    -
 * Modified by:             - 
 * 
 * Input:                   -
 * Output:                  uint8_Result
***********************************************************************************************************************/
unsigned char funct_CheckCmdSMTYP(void)
{
    auto unsigned char uint8_Result = 0;
    
    //verify if main supply is switched off or unipolar application is selected
    if(g_CmdChk.uint32_TempPara[1] == 'N')     
    {
        //switch off each output
        g_Param.uint8_MotTyp = 'N';
        uint8_Result = 5;   //set to 5 for that the result pass
    }
    else if ((g_CmdChk.uint32_TempPara[1] == 'U') || (g_CmdChk.uint32_TempPara[1] == 'M'))
    {
        //then verify the stored parameters
        uint8_Result += funct_CheckTol(g_Param.uint16_Imin,_Imin,_Imax);
        uint8_Result += funct_CheckTol(g_Param.uint16_Imax,_Imin,_Imax);
        uint8_Result += funct_CheckTol(g_Param.uint16_Umot,_UmotMin,_UmotMax);
        uint8_Result += funct_CheckTol(g_Param.uint16_SCiLevel,_SCiLevelMin,_SCiLevelMax);
        uint8_Result += funct_CheckTol(g_Param.uint8_SCtLevel,_SCtLevelMin,_SCtLevelMax);          
                
        if(uint8_Result == 5)   //each parameter within the tolerance?
        {
            //for security switch off each output instead the main supply and switch on "unipolar"
            
            if(g_CmdChk.uint32_TempPara[1] == 'U')
            {
                g_Param.uint8_MotTyp = 'U';
            }
            else
            {
                g_Param.uint8_MotTyp = 'M';
            }
        }
        else
        { 
            g_Param.uint8_ErrCode = _UniSMTYP;  //set error code
        }
    }
    else if(g_CmdChk.uint32_TempPara[1] == 'B')     //verify limits for the bipolar application
    {
        uint8_Result = uint8_Result + funct_CheckTol(g_Param.uint16_Imin,_Imin,_Imax);
        uint8_Result = uint8_Result + funct_CheckTol(g_Param.uint16_Imax,_Imin,_Imax);
        uint8_Result = uint8_Result + funct_CheckTol(g_Param.uint16_Umot,_UmotMin,_UmotBipMax);
        uint8_Result = uint8_Result + funct_CheckTol(g_Param.uint16_SCiLevel,_SCiLevelMin,_SCiLevelMax);
        uint8_Result = uint8_Result + funct_CheckTol(g_Param.uint8_SCtLevel,_SCtLevelMin,_SCtLevelMax);          
               

        if(uint8_Result == 5)   //each parameter within the tolerance
        {
            //for security switch off each output instead the main supply and switch on "bipolar"
            g_Param.uint8_MotTyp = 'B';
        }
        else
        {
            g_Param.uint8_ErrCode = _BipSMTYP;  //set error code
        }   
    }
    else if(g_CmdChk.uint32_TempPara[1] == 'L')     //verify limits for the LIN application
    {
        uint8_Result = uint8_Result + funct_CheckTol(g_Param.uint16_Imin,_Imin,_ImotLinMax);
        uint8_Result = uint8_Result + funct_CheckTol(g_Param.uint16_Imax,_Imin,_ImotLinMax);
        uint8_Result = uint8_Result + funct_CheckTol(g_Param.uint16_Umot,_UmotMin,_UmotLinMax);
        uint8_Result = uint8_Result + funct_CheckTol(g_Param.uint16_SCiLevel,_SCiLevelMin,_ImotLinSCiLevelMax);
        uint8_Result = uint8_Result + funct_CheckTol(g_Param.uint8_SCtLevel,_SCtLevelMin,_SCtLevelMax);          
                
        if(uint8_Result == 5)   //each parameter within the tolerance
        {
            //for security switch off each output instead the main supply and switch on "LIN"
            g_Param.uint8_MotTyp = 'L';
        }
        else
        {
        g_Param.uint8_ErrCode = _LinSMTYP;  //set error code
        } 
    }
    else
    {
        g_Param.uint8_ErrCode = _UnknownMotTyp; //set error code
    }
    
    return uint8_Result;
}   //end of funct_CheckActApplTol


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
void funct_LoadDeviceInfo(void)
{
    uart2_sendbuffer('E');
    uart2_sendbuffer(',');
    uart2_sendbuffer('S');                   
    uart2_sendbuffer('M');
    uart2_sendbuffer('M');
    uart2_sendbuffer('A');
    uart2_sendbuffer(' ');
    uart2_sendbuffer('V');
    uart2_sendbuffer('0');
    uart2_sendbuffer('1');
    uart2_sendbuffer('.');
    uart2_sendbuffer('0');
    uart2_sendbuffer('0');
    uart2_sendbuffer(' ');
    uart2_sendbuffer('1');
    uart2_sendbuffer('1');
    uart2_sendbuffer('/');
    uart2_sendbuffer('1');
    uart2_sendbuffer('5');
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
void funct_FrontLED(unsigned char uint8_color)
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