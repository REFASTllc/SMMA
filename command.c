/**********************************************************************************************************************

                                            UBD - Unipolar Bipolar Driver

***********************************************************************************************************************
 * File name:               command.c
 * Creation date:           03.10.2015
 * Main creator:            Alain Staub
 * Company:                 REFAST GmbH
 *                          Copyright (c) 2015 REFAST GmbH
***********************************************************************************************************************
 * Content overview:        - cmd_SILIM
 *                          - cmd_ETESTIN
 *                          - cmd_SRACC
 *                          - cmd_SRDEC
 *                          - cmd_RUN
 *                          - cmd_SPHC
 *                          - cmd_GVER
 *                          - cmd_RAZ
 *                          - cmd_SMTYP
 *                          - cmd_GMTYP
 *                          - cmd_STEST
 *                          - cmd_GILIM
 *                          - cmd_GPHC
 *                          - cmd_GRACC
 *                          - cmd_GRDEC
 *                          - cmd_GRUN
 *                          - cmd_BREAK
 *                          - cmd_MUMOT
 *                          - cmd_SCOILON
 *                          - cmd_SSMOD
 *                          - cmd_GSMOD
 *                          - cmd_SBIT
 *                          - cmd_CBIT
 *                          - cmd_GBIT
 *                          - cmd_SOUT
 *                          - cmd_GOUT
 *                          - cmd_GINP
 *                          - cmd_SMCRSTP
 *                          - cmd_GMCRSTP
 *                          - cmd_SSWLIM
 *                          - cmd_GSWLIM
 *                          - cmd_GSWPOS
 *                          - cmd_SPWMLIM
 *                          - cmd_GPWMLIM
 *                          - cmd_GPWMPOS
 *                          - cmd_GPWMVAL
 *                          - cmd_SSWTYP
 *                          - cmd_GSWTYP
 *                          - cmd_GFRQVAL
 *                          //new commands:
 *                          - cmd_GINPB
 *                          - cmd_SROM
 *                          - cmd_RROM
 *                          - cmd_GTEMP
 *                          - cmd_SSEC
 *                          - cmd_GSEC
 *                          - cmd_SMIN
 *                          - cmd_GMIN
 *                          - cmd_SHRS
 *                          - cmd_SDAY
 *                          - cmd_GDAY
 *                          - cmd_SWDAY
 *                          - cmd_GWDAY
 *                          - cmd_SMONTH
 *                          - cmd_GMONTH
 *                          - cmd_SYEAR
 *                          - cmd_GYEAR
 *                          - cmd_GTIME
 *                          - cmd_GLINSTA
 *                          - cmd_SSPDLIN
 *                          - cmd_GSPDLIN
 *                          - cmd_STOLIN
 *                          - cmd_GTOLIN
 *                          - cmd_SLIN
 *                          - cmd_GBIPSTAS
 *                          - cmd_GBIPSTAL
 *                          - cmd_SBIPPD
 *                          - cmd_GBIPPD
 *                          - cmd_SBIPTSC
 *                          - cmd_GBIPTSC
 *                          - cmd_SBIPOL
 *                          - cmd_GBIPOL
 *                          - cmd_SBIPSTS
 *                          - cmd_GBIPSTS
 *                          - cmd_SBIPPFD
 *                          - cmd_GBIPPFD
 *                          - cmd_SBIPPWM
 *                          - cmd_GBIPPWM
 *                          - cmd_SBIPFRQ
 *                          - cmd_GBIPFRQ
 *                          - cmd_SBIPSLEW
 *                          - cmd_GBIPSLEW
 *                          - cmd_GBIPSST
 *                          - cmd_GVBAK
 *                          - cmd_SRESLIN
 *                          - cmd_GRESLIN
 *                          - cmd_SFRQBIT
 *                          - cmd_GFRQBIT
 *                          - cmd_SRUNBIT
 *                          - cmd_GRUNBIT
 *                          - cmd_RTESTIN
 *                          - cmd_SPROD
 *                          - cmd_STAT
***********************************************************************************************************************/


#include "includes.h" 

extern SCmdChk g_CmdChk;
extern SFunct g_Funct;
extern Si2c1 g_i2c1;
extern SParam g_Param;
extern SUni g_Uni;
extern Sbipol g_Bipol;
extern T_A3981 A3981;
extern SLin g_LIN;
extern SUART1txd g_UART1txd;  
extern SUART1rxd g_UART1rxd;
extern SADC g_ADC;
extern STimer1 g_Timer1; 
extern S_PROD g_Prod;
extern SUART2txd g_UART2txd;

/**********************************************************************************************************************
 * Routine:                 cmd_SILIM

 * Description:
 * Only allowed if the motor is not in run. Verify each parameter if it is within the tolerance and the number of 
 * received parameters are correct. If true then store the parameters. If something is wrong then send back the error. 
 * In the case where the main supply is switched off (uint8_MotTyp = 'N') or the application is set up for unipolar
 * actuators, we only verify if the parameters are within the tolerance. 
 * If true then we store this parameters, if false we send back a feedback to the customer.
 * In the case where the main supply is switched on for bipolar or LIN, we first verify if the parameters
 * are within the application tolerance. If not we send back the corresponding error. 
 * 
 * Creator:                 A. Staub
 * Date of creation:        15.10.2015
 * Last modification on:    -
 * Modified by:             - 
 * 
 * Input:                   -
 * Output:                  -
***********************************************************************************************************************/
void cmd_SILIM(void)
{
    auto unsigned char uint8_Result = 0;        //local work byte for the result 
  
    if(g_CmdChk.uint8_ParamPos == 6)            //number of received characters OK?
    {
        if(g_CmdChk.uint8_GlobalLock == 1)      //global lock active?
        {
            g_Param.uint8_ErrCode = _MotorInRun;        //set error code
            uart2_SendErrorCode(g_Param.uint8_ErrCode); //call subroutine
        }
        else
        {
            uint8_Result = funct_CheckCmdSILIM();   //call subroutine

            if(uint8_Result == 5)   //each parameter within the tolerance?
            {
                //then store the parameters and send back the OK
                g_Param.uint16_Imin = g_CmdChk.uint32_TempPara[1] & 0xFFFF;
                g_Param.uint16_Imax = g_CmdChk.uint32_TempPara[2] & 0xFFFF;
                g_Param.uint16_Umot = g_CmdChk.uint32_TempPara[3] & 0xFFFF;
                g_Param.uint16_SCiLevel = g_CmdChk.uint32_TempPara[4] & 0xFFFF;
                g_Param.uint8_SCtLevel = g_CmdChk.uint32_TempPara[5] & 0xFF;

                uart2_sendbuffer('E');          //first the letter E
                uart2_sendbuffer(13);           //with CR at the end
            }
            else
            {
                uart2_SendErrorCode(g_Param.uint8_ErrCode); //call subroutine
            }  
        }
    }
    else
    {
        g_Param.uint8_ErrCode = _NumbRecCharNotOK;  //set error code
        uart2_SendErrorCode(g_Param.uint8_ErrCode); //call subroutine
    }
}   //end of cmd_SILIM


/**********************************************************************************************************************
 * Routine:                 cmd_ETESTIN

 * Description:
 * Only allowed if the motor is not in run. Send back the:
 * I coil A1, I coil A2, I coil B1, I coil B2, Umot 
 * 
 * Creator:                 A. Staub
 * Date of creation:        19.10.2015
 * Last modification on:    -
 * Modified by:             - 
 * 
 * Input:                   -
 * Output:                  -
***********************************************************************************************************************/
void cmd_ETESTIN(void)
{
    volatile unsigned long int uint32_WB1;
    volatile unsigned long int uint32_WB2;
    volatile unsigned long int uint32_WB3;
    volatile unsigned long int uint32_WB4;
    volatile unsigned char uint8_Result = 0;
    
    if(g_CmdChk.uint8_ParamPos == 2)   //number of received characters OK?
    {  
        if(g_CmdChk.uint8_GlobalLock == 1)      //global lock active?
        {
            g_Param.uint8_ErrCode = _MotorInRun;        //set error code
            uart2_SendErrorCode(g_Param.uint8_ErrCode); //call subroutine
        }
        else
        {
            g_CmdChk.uint8_GlobalLock = 1;              //set global lock
            
            if(g_CmdChk.uint32_TempPara[1] == 'L')          //is motor type = LIN?
            {
                g_Param.uint8_ErrCode = _LinETESTIN;        //set error code
                uart2_SendErrorCode(g_Param.uint8_ErrCode); //call subroutine
            }
            //is motor type = unipolar or matrix?
            else if ((g_CmdChk.uint32_TempPara[1] == 'U') || (g_CmdChk.uint32_TempPara[1] == 'M'))
            {
                ads1115_SetChannel(_AIN1p_GND,_FS4096mV);   //set channel on Imot

                //define the outputs
                oVmotOnOff = 1;                 //switch off the main supply
                oBiEnaVmot = 0;                 //switch off the bipolar supply
                oEnaVLINSupply = 0;             //switch off the lin supply

                //measure coil A1
                oUniCoilA1 = 1;                     //switch on coil A1
                g_Timer1.uint8_TimeoutFlag = 1;     //set the timeout flag
                SetTimer(_TIMER1,_ENABLE,0,200);    //load the timer with 200ms
                while(g_Timer1.uint8_TimeoutFlag)   //rest in the while until flag is reseted
                {
                    //do nothing
                }        
                uint32_WB1 = ads1115_read();         //read out Imot
                oUniCoilA1 = 0;                     //switch off coil A1
                if(uint32_WB1)   //convert result in mV only if it is not 0
                {
                    uint32_WB1 = funct_ADCtoMiliUnit(uint32_WB1,8000);                             
                }
                
                //measure coil A2
                oUniCoilA2 = 1;                     //switch on coil A2
                g_Timer1.uint8_TimeoutFlag = 1;     //set the timeout flag
                SetTimer(_TIMER1,_ENABLE,0,200);    //load the timer with 200ms
                while(g_Timer1.uint8_TimeoutFlag)   //rest in the while until flag is reseted
                {
                    //do nothing
                }              
                uint32_WB2 = ads1115_read();         //read out Imot
                oUniCoilA2 = 0;                     //switch off coil A2
                if(uint32_WB2)   //convert result in mV only if it is not 0
                {
                    uint32_WB2 = funct_ADCtoMiliUnit(uint32_WB2,8000);                             
                }
                
                //measure coil B1
                oUniCoilB1 = 1;                     //switch on coil B1
                g_Timer1.uint8_TimeoutFlag = 1;     //set the timeout flag
                SetTimer(_TIMER1,_ENABLE,0,200);    //load the timer with 200ms
                while(g_Timer1.uint8_TimeoutFlag)   //rest in the while until flag is reseted
                {
                    //do nothing
                }
                uint32_WB3 = ads1115_read();         //read out Imot
                oUniCoilB1 = 0;                     //switch off coil B1
                if(uint32_WB3)   //convert result in mV only if it is not 0
                {
                    uint32_WB3 = funct_ADCtoMiliUnit(uint32_WB3,8000);                             
                }

                //measure coil B2
                oUniCoilB2 = 1;                     //switch on coil B2
                g_Timer1.uint8_TimeoutFlag = 1;     //set the timeout flag
                SetTimer(_TIMER1,_ENABLE,0,200);    //load the timer with 200ms
                while(g_Timer1.uint8_TimeoutFlag)   //rest in the while until flag is reseted
                {
                    //do nothing
                }  
                uint32_WB4 = ads1115_read();         //read out Imot
                oUniCoilB2 = 0;                     //switch off coil B2
                if(uint32_WB4)   //convert result in mV only if it is not 0
                {
                    uint32_WB4 = funct_ADCtoMiliUnit(uint32_WB4,8000);                             
                }

                //verify result
                uint8_Result += funct_CheckTol(uint32_WB1,g_Param.uint16_Imin,g_Param.uint16_Imax);
                uint8_Result += funct_CheckTol(uint32_WB2,g_Param.uint16_Imin,g_Param.uint16_Imax);
                uint8_Result += funct_CheckTol(uint32_WB3,g_Param.uint16_Imin,g_Param.uint16_Imax);
                uint8_Result += funct_CheckTol(uint32_WB4,g_Param.uint16_Imin,g_Param.uint16_Imax);
                if(uint8_Result == 4)
                {
                    uart2_sendbuffer('E');      //add an E
                    uart2_sendbuffer(',');      //add a ,
                }
                else
                {
                    uart2_sendbuffer('X');      //add an X
                    uart2_sendbuffer(',');      //add a ,
                    g_Param.uint8_ErrCode = _UniETESTIN;    //set error code
                    funct_IntToAscii(g_Param.uint8_ErrCode,_Active);    //add the error code
                    uart2_sendbuffer(',');      //add a ,
                }

                funct_IntToAscii(uint32_WB1,_Active);   //add the first result
                uart2_sendbuffer(',');      //add a ,
                funct_IntToAscii(uint32_WB2,_Active);   //add the second result
                uart2_sendbuffer(',');      //add a ,
                funct_IntToAscii(uint32_WB3,_Active);   //add the third result
                uart2_sendbuffer(',');      //add a ,
                funct_IntToAscii(uint32_WB4,_Active);   //add the fourth result
                uart2_sendbuffer(',');      //add a ,

                //convert Vmot
                ads1115_SetChannel(_AIN0p_GND,_FS4096mV);   //set channel on Vmot
                g_Timer1.uint8_TimeoutFlag = 1;     //set the timeout flag
                SetTimer(_TIMER1,_ENABLE,0,200);    //load the timer with 200ms
                while(g_Timer1.uint8_TimeoutFlag)   //rest in the while until flag is reseted
                {
                    //do nothing 
                }   
                uint32_WB1 = ads1115_read();         //read out Vmot
                oBiRelayCoilB = 0;                  //switch off relay for coil B
                if(uint32_WB1)   //convert result in mV only if it is not 0
                {
                    uint32_WB1 = funct_ADCtoMiliUnit(uint32_WB1,464);                             
                }
                funct_IntToAscii(uint32_WB1,_Active);   //add the Vmot result
                uart2_sendbuffer(13);      //add a CR        
            }
            else if (g_CmdChk.uint32_TempPara[1] == 'B')    //is motor type = bipolar
            {
                ads1115_SetChannel(_AIN1p_GND,_FS4096mV);   //set channel on Imot

                //define the outputs
                oVmotOnOff = 1;                 //switch off the main supply
                oBiEnaVmot = 0;                 //switch off the bipolar supply
                oEnaVLINSupply = 0;             //switch off the lin supply

                //measure coil A
                oBiRelayCoilA = 1;                  //switch on relay for coil A
                g_Timer1.uint8_TimeoutFlag = 1;     //set the timeout flag
                SetTimer(_TIMER1,_ENABLE,0,200);    //load the timer with 200ms
                while(g_Timer1.uint8_TimeoutFlag)   //rest in the while until flag is reseted
                {
                    //do nothing
                } 
                uint32_WB1 = ads1115_read();         //read out Imot
                oBiRelayCoilA = 0;                  //switch off relay for coil A
                if(uint32_WB1)   //convert result in mV only if it is not 0
                {
                    uint32_WB1 = funct_ADCtoMiliUnit(uint32_WB1,8000);                             
                }

                //measure coil B
                oBiRelayCoilB = 1;                  //switch on relay for coil B
                g_Timer1.uint8_TimeoutFlag = 1;     //set the timeout flag
                SetTimer(_TIMER1,_ENABLE,0,200);    //load the timer with 200ms
                while(g_Timer1.uint8_TimeoutFlag)   //rest in the while until flag is reseted
                {
                    //do nothing 
                }   
                uint32_WB2 = ads1115_read();         //read out Imot
                oBiRelayCoilB = 0;                  //switch off relay for coil B
                if(uint32_WB2)   //convert result in mV only if it is not 0
                {
                    uint32_WB2 = funct_ADCtoMiliUnit(uint32_WB2,8000);                             
                }
               
                //verify result
                uint8_Result += funct_CheckTol(uint32_WB1,g_Param.uint16_Imin,g_Param.uint16_Imax);
                uint8_Result += funct_CheckTol(uint32_WB2,g_Param.uint16_Imin,g_Param.uint16_Imax);
                if(uint8_Result == 2)
                {
                    uart2_sendbuffer('E');      //add an E
                    uart2_sendbuffer(',');      //add a ,
                }
                else
                {
                    uart2_sendbuffer('X');      //add an X
                    uart2_sendbuffer(',');      //add a ,
                    g_Param.uint8_ErrCode = _BipETESTIN;    //set error code
                    funct_IntToAscii(g_Param.uint8_ErrCode,_Active);    //add the error code
                    uart2_sendbuffer(',');      //add a ,
                }

                funct_IntToAscii(uint32_WB1,_Active);   //add the first result
                uart2_sendbuffer(',');      //add a ,
                funct_IntToAscii(uint32_WB1,_Active);   //add the first result
                uart2_sendbuffer(',');      //add a ,
                funct_IntToAscii(uint32_WB2,_Active);   //add the second result
                uart2_sendbuffer(',');      //add a ,
                funct_IntToAscii(uint32_WB2,_Active);   //add the second result
                uart2_sendbuffer(',');      //add a ,

                //convert Vmot
                ads1115_SetChannel(_AIN0p_GND,_FS4096mV);   //set channel on Vmot
                g_Timer1.uint8_TimeoutFlag = 1;     //set the timeout flag
                SetTimer(_TIMER1,_ENABLE,0,200);    //load the timer with 200ms
                while(g_Timer1.uint8_TimeoutFlag)   //rest in the while until flag is reseted
                {
                    //do nothing 
                }   
                uint32_WB1 = ads1115_read();         //read out Vmot
                oBiRelayCoilB = 0;                  //switch off relay for coil B
                if(uint32_WB1)   //convert result in mV only if it is not 0
                {
                    uint32_WB1 = funct_ADCtoMiliUnit(uint32_WB1,464);                             
                }
                funct_IntToAscii(uint32_WB1,_Active);   //add the Vmot result
                uart2_sendbuffer(13);      //add a CR        
            }
            else
            {
                g_Param.uint8_ErrCode = _UnknownMotTyp;     //set error code
                uart2_SendErrorCode(g_Param.uint8_ErrCode); //call subroutine
            }

            ads1115_SetChannel(_AIN0p_GND,_FS4096mV);   //set channel on Vmot
            
            g_CmdChk.uint8_GlobalLock = 0;  //clear global lock
        }
    }
    else
    {
        g_Param.uint8_ErrCode = _NumbRecCharNotOK;  //set error code
        uart2_SendErrorCode(g_Param.uint8_ErrCode); //call subroutine
    }
}   //end of cmd_ETESTIN


/**********************************************************************************************************************
 * Routine:                 cmd_SRACC

 * Description:
 * Only allowed if the motor is not in run. First verify if the number of received characters is inside the range.
 * If not, then send back the error.
 * Second verify if the number of received characters is odd-numbered. Because to have a valide result, there must be 
 * always a number of steps with a frequency together and with the command ID at the beginning the size will be
 * 3,6,9,12,15,18,21,24,27,30,33,...,...,...,...,...101 
 * Otherwise send back the error.
 * 
 * Third verify each received parameter with the tolerance until:
 * - all parameters are verified
 * - or the parameter was a comma which is signalized per 0xFFFFFFFF (32-bit)
 * Update the number of received parameters with the number of verified parameters, to not count anymore the parameters 
 * with a comma. Note 1 comma is enough to stop storing the parameters inside the arrays. 
 * So you cannot send SRACC,1,200,2,300,,4,400<CR> 
 * This will accept only 1,200 and 2,300.  
 * 
 * Fourth verify if the result is exactly the number of parameters -1, to be sure that each parameter is within 
 * the tolerance. -1 because we do not count the command ID with. Otherwise send back the error code.   
 * 
 * At the end, first erase the acceleration array. store the new parameters inside, and send back the OK.
 * 
 * Mod. 29.11.2015:
 * The part with the comma check "0xFFFFFFFF" does not really work like JE would at the beginning.
 * I deleted this from the subroutine because this makes not sence to use. Ask me if you need more
 * informations. Now the customer has always to send a pair of parameters (number of steps with a frequency).
 * 
 * Creator:                 A. Staub
 * Date of creation:        19.10.2015
 * Last modification on:    29.11.2015
 * Modified by:             A. Staub
 * 
 * Input:                   -
 * Output:                  -
***********************************************************************************************************************/
void cmd_SRACC(void)
{
    auto unsigned char uint8_Result = 0;    //local work byte for the result
    auto unsigned char uint8_WB1;           //local work byte for the loops
    auto unsigned char uint8_WB2;           //local work byte to store the results into the array  
  
    if((g_CmdChk.uint8_ParamPos > 1) && (g_CmdChk.uint8_ParamPos < 102))  //number of received characters inside the range?
    {
        if(g_CmdChk.uint8_ParamPos % 2)    //verify if the number of received parameters is odd-numbered (a pair) 
        {
            if(g_CmdChk.uint8_GlobalLock == 1)  //global lock active?
            {
                g_Param.uint8_ErrCode = _MotorInRun;        //set error code
                uart2_SendErrorCode(g_Param.uint8_ErrCode); //call subroutine
            }
            else
            {
                g_CmdChk.uint8_GlobalLock = 1;  //enable global lock
                
                //ToDo:
                //verify each received parameter with the tolerance (start with the 2nd parameter)
                //Until:
                //the local work byte has the same size as number of received characters
                uint8_WB1 = 1;
                do
                {
                    uint8_Result += funct_CheckTol(g_CmdChk.uint32_TempPara[uint8_WB1],_AccNumbStepMin,_AccNumbStepMax);
                    uint8_WB1++;                      //increment with 1 to take the next parameter
                    uint8_Result += funct_CheckTol(g_CmdChk.uint32_TempPara[uint8_WB1],_AccFreqMin,_AccFreqMax);
                    uint8_WB1++;                      //increment wiht 1 to take the next parameter
                }
                while(uint8_WB1 < g_CmdChk.uint8_ParamPos);
      
                //each parameter within the tolerance (uint8_WB1 - 1, because the 1st parameter is the cmd ID)?
                if(uint8_Result == (uint8_WB1-1))
                {
                    //first erase both arrays
                    for(uint8_WB1=0; uint8_WB1<50; uint8_WB1++)
                    {
                        g_Param.uint16_AccNumbStep[uint8_WB1] = 0;
                        g_Param.uint16_AccFreq[uint8_WB1] = 0;
                    }
                    //then store each parameter on the right place (start with the 2nd parameter)
                    uint8_WB1 = 1;
                    uint8_WB2 = 0;
                    do
                    {
                        g_Param.uint16_AccNumbStep[uint8_WB2] = g_CmdChk.uint32_TempPara[uint8_WB1] & 0xFFFF;
                        uint8_WB1++;        //increment with 1 to take the next parameter
                        g_Param.uint16_AccFreq[uint8_WB2] = g_CmdChk.uint32_TempPara[uint8_WB1] & 0xFFFF;
                        uint8_WB1++;        //increment with 1 to take the next parameter
                        uint8_WB2++;        //increment with 1 to store it on the right position of the array
                    }
                    while(uint8_WB1 < g_CmdChk.uint8_ParamPos);
        
                    //to know how many ramps are stored in the array copy the result in the follow variable
                    g_Param.uint8_AccNumbRamp = uint8_WB2;  
        
                    //send back the OK
                    uart2_sendbuffer('E');            //first the letter E
                    uart2_sendbuffer(13);             //then the CR at the end
                }
                //otherwise send back the error code
                else
                {
                    g_Param.uint8_ErrCode = _OutOfTolSRACC;     //set error code
                    uart2_SendErrorCode(g_Param.uint8_ErrCode); //call subroutine
                }
                
                g_CmdChk.uint8_GlobalLock = 0;  //disable global lock
            }  
        }
        else    //otherwise send back the error code
        {
            g_Param.uint8_ErrCode = _NumbRecCharNotOK;  //set error code
            uart2_SendErrorCode(g_Param.uint8_ErrCode); //call subroutine
        }
    }
    else
    {
        g_Param.uint8_ErrCode = _NumbRecCharNotOK;  //set error code
        uart2_SendErrorCode(g_Param.uint8_ErrCode); //call subroutine
    }
}   //end of cmd_SRACC


/**********************************************************************************************************************
 * Routine:                 cmd_SRDEC

 * Description:
 * Only allowed if the motor is not in run. First verify if the number of received characters is inside the range.
 * If not, then send back the error.
 * Second verify if the number of received characters is odd-numbered. Because to have a valide result, there must be 
 * always a number of steps with a frequency together and with the command ID at the beginning the size will be
 * 3,6,9,12,15,18,21,24,27,30,33,...,...,...,...,...101 
 * Otherwise send back the error.
 * 
 * Third verify each received parameter with the tolerance until:
 * - all parameters are verified
 * - or the parameter was a comma which is signalized per 0xFFFFFFFF (32-bit)
 * Update the number of received parameters with the number of verified parameters, to not count anymore the parameters 
 * with a comma. Note 1 comma is enough to stop storing the parameters inside the arrays. 
 * So you cannot send SRDEC,1,200,2,300,,4,400<CR> 
 * This will accept only 1,200 and 2,300.  
 * 
 * Fourth verify if the result is exactly the number of parameters -1, to be sure that each parameter is within 
 * the tolerance. -1 because we do not count the command ID with. Otherwise send back the error code.   
 * 
 * At the end, first erase the acceleration array. store the new parameters inside, and send back the OK.
 * 
 * Mod. 29.11.2015:
 * The part with the comma check "0xFFFFFFFF" does not really work like JE would at the beginning.
 * I deleted this from the subroutine because this makes not sence to use. Ask me if you need more
 * informations. Now the customer has always to send a pair of parameters (number of steps with a frequency).
 * 
 * Creator:                 A. Staub
 * Date of creation:        20.10.2015
 * Last modification on:    29.11.2015
 * Modified by:             A. Staub
 * 
 * Input:                   -
 * Output:                  -
***********************************************************************************************************************/
void cmd_SRDEC(void)
{
    auto unsigned char uint8_Result = 0;    //local work byte for the result
    auto unsigned char uint8_WB1;           //local work byte for the loops
    auto unsigned char uint8_WB2;           //local work byte to store the results into the array  
    
    if((g_CmdChk.uint8_ParamPos > 1) && (g_CmdChk.uint8_ParamPos < 102))    //number of received characters OK?
    {
        if(g_CmdChk.uint8_ParamPos % 2)    //verify if the number of received parameters is odd-numbered (a pair)
        {
            if(g_CmdChk.uint8_GlobalLock == 1)  //global lock active?
            {
                g_Param.uint8_ErrCode = _MotorInRun;        //set error code
                uart2_SendErrorCode(g_Param.uint8_ErrCode); //call subroutine
            }
            else
            {
                g_CmdChk.uint8_GlobalLock = 1;  //enable global lock
                
                //ToDo:
                //verify each received parameter with the tolerance (start with the 2nd parameter)
                //Until:
                //the local work byte has the same size as number of received characters
                uint8_WB1 = 1;
                do
                {
                    uint8_Result += funct_CheckTol(g_CmdChk.uint32_TempPara[uint8_WB1],_DecNumbStepMin,_DecNumbStepMax);
                    uint8_WB1++;                      //increment with 1 to take the next parameter
                    uint8_Result += funct_CheckTol(g_CmdChk.uint32_TempPara[uint8_WB1],_DecFreqMin,_DecFreqMax);
                    uint8_WB1++;                      //increment wiht 1 to take the next parameter
                }
                while(uint8_WB1 < g_CmdChk.uint8_ParamPos);
      
                //each parameter within the tolerance (uint8_WB1 - 1, because the 1st parameter is the cmd ID)?
                if(uint8_Result == (uint8_WB1-1))
                {
                    //first erase both arrays
                    for(uint8_WB1=0; uint8_WB1<50; uint8_WB1++)
                    {
                        g_Param.uint16_DecNumbStep[uint8_WB1] = 0;
                        g_Param.uint16_DecFreq[uint8_WB1] = 0;
                    }
                    //then store each parameter on the right place (start with the 2nd parameter)
                    uint8_WB1 = 1;
                    uint8_WB2 = 0;
                    do
                    {
                        g_Param.uint16_DecNumbStep[uint8_WB2] = g_CmdChk.uint32_TempPara[uint8_WB1] & 0xFFFF;
                        uint8_WB1++;            //increment with 1 to take the next parameter
                        g_Param.uint16_DecFreq[uint8_WB2] = g_CmdChk.uint32_TempPara[uint8_WB1] & 0xFFFF;
                        uint8_WB1++;            //increment with 1 to take the next parameter
                        uint8_WB2++;            //increment with 1 to store it on the right position of the array
                    }
                    while(uint8_WB1 < g_CmdChk.uint8_ParamPos);
        
                    //to know how many ramps are stored in the array copy the result in the follow variable
                    g_Param.uint8_DecNumbRamp = uint8_WB2; 
        
                    //send back the OK
                    uart2_sendbuffer('E');            //first the letter E
                    uart2_sendbuffer(13);             //then the CR at the end
                }
                //otherwise send back the error code
                else
                {
                    g_Param.uint8_ErrCode = _OutOfTolSRDEC;     //set error code
                    uart2_SendErrorCode(g_Param.uint8_ErrCode); //call subroutine
                }
                
                g_CmdChk.uint8_GlobalLock = 0;  //disable global lock
            }       
        } 
        else    //otherwise send back the error code
        {
            g_Param.uint8_ErrCode = _NumbRecCharNotOK;  //set error code
            uart2_SendErrorCode(g_Param.uint8_ErrCode); //call subroutine
        }
    }
    else
    {
        g_Param.uint8_ErrCode = _NumbRecCharNotOK;  //set error code
        uart2_SendErrorCode(g_Param.uint8_ErrCode); //call subroutine
    }
}   //end of cmd_SRDEC


/**********************************************************************************************************************
 * Routine:                 cmd_RUN

 * Description:
 * This command can only be executed if the motor is not in run, this is the first verification. 
 * Then it verifies if the number of received parameters is correct, otherwise we send back an error code. 
 * This command can only be used for unipolar or bipolar actuators, so if the motor type is LIN or N we send
 * back the corresponding error code. 
 * 
 * Note:
 * Not every parameter inside this command is used for unipolar / matrix actuators. Some are for the bipolar 
 * actuator, but the customer want to use the same command. 
 * 
 * Unipolar / Matrix:
 * Once all parameters are verified we store every received parameter into the corresponding register or 
 * set the corresponding bit for the actuator move. If some definitions are not OK then we send back an error. 
 * All parameters are verified before but if for example the customer wants to execute micro stepping with an 
 * unipolar driver then this is still not possible. 
 * The follow parameter are not used for these type of actuators:
 * - BipRunI
 * - BipRunI
 * - BipHoldI
 * - BipAccI
 * - BipDecI
 * Note: 
 * The 'ack' will be not send back inside this routine, because this is a pre-verification. Only if the 
 * 'plausibility check' from the source code inside 'unipolar.c' is verified and ok, we send back the 'ack'. 
 * A second 'ack' will be send back if the 'ack' bit is set true and the actuator reached his position. 
 * 
 * Bipolar:
 * The bipolar part does quite the same like the unipolar / matrix part. But here we use now the parameters:
 * - BipRunI
 * - BipRunI
 * - BipHoldI
 * - BipAccI
 * - BipDecI
 * And in the same time we have an SPI communication to set up the external driver. 
 * 
 * Creator:                 A. Staub / J. Rebetez
 * Date of creation:        22.10.2015
 * Last modification on:    -
 * Modified by:             - 
 * 
 * Input:                   -
 * Output:                  -
***********************************************************************************************************************/
void cmd_RUN(void)
{
    auto unsigned char uint8_Result = 0;    //local work byte for the result 
    auto unsigned char uint8_UniErrConfig = 0;  //local work byte for error configuration unipolar
    
    g_Bipol.uint1_ErrConfig = 0;
    
    if(g_CmdChk.uint8_ParamPos == 15)  //number of received characters OK?
    {
        if(g_CmdChk.uint8_GlobalLock == 1)  //global lock active?
        {
            g_Param.uint8_ErrCode = _MotorInRun;        //set error code
            uart2_SendErrorCode(g_Param.uint8_ErrCode); //call subroutine
        }
        else
        {
            uint8_Result += funct_CheckTol(g_CmdChk.uint32_TempPara[2],_StepModeMin,_StepModeMax);
            uint8_Result += funct_CheckTol(g_CmdChk.uint32_TempPara[3],_StepCountMin,_StepCountMax);
            uint8_Result += funct_CheckTol(g_CmdChk.uint32_TempPara[4],_RunFreqMin,_RunFreqMax);      
            uint8_Result += funct_CheckTol(g_CmdChk.uint32_TempPara[5],_DirMin,_DirMax);
            uint8_Result += funct_CheckTol(g_CmdChk.uint32_TempPara[6],_CoilStateMin,_CoilStateMax);
            uint8_Result += funct_CheckTol(g_CmdChk.uint32_TempPara[7],_BipRunIMin,_BipRunIMax);
            uint8_Result += funct_CheckTol(g_CmdChk.uint32_TempPara[8],_BipHoldIMin,_BipHoldIMax);
            uint8_Result += funct_CheckTol(g_CmdChk.uint32_TempPara[9],_SelectRampMin,_SelectRampMax);
            uint8_Result += funct_CheckTol(g_CmdChk.uint32_TempPara[10],_BipAccIMin,_BipAccIMax);
            uint8_Result += funct_CheckTol(g_CmdChk.uint32_TempPara[11],_BipDecIMin,_BipDecIMax);
            uint8_Result += funct_CheckTol(g_CmdChk.uint32_TempPara[12],_AccOnDelayMin,_AccOnDelayMax);
            uint8_Result += funct_CheckTol(g_CmdChk.uint32_TempPara[13],_DecOffDelayMin,_DecOffDelayMax);
            uint8_Result += funct_CheckTol(g_CmdChk.uint32_TempPara[14],_AckMin,_AckMax);
            
            if(uint8_Result == 13)      //each parameter within the tolerance?
            {
                //then store the parameters
                g_Param.uint8_StepMode = g_CmdChk.uint32_TempPara[2] & 0xFF;
                g_Param.uint32_StepCount = g_CmdChk.uint32_TempPara[3] & 0xFFFFFFFF;
                g_Param.uint16_RunFreq = g_CmdChk.uint32_TempPara[4] & 0xFFFF;
                g_Param.uint8_Direction = g_CmdChk.uint32_TempPara[5] & 0xFF;
                g_Param.uint8_CoilState = g_CmdChk.uint32_TempPara[6] & 0xFF;
                g_Param.uint16_BipRunI = g_CmdChk.uint32_TempPara[7] & 0xFFFF;
                g_Param.uint16_BipHoldI = g_CmdChk.uint32_TempPara[8] & 0xFFFF;
                g_Param.uint8_SelectRamp = g_CmdChk.uint32_TempPara[9] & 0xFF;
                g_Param.uint16_BipAccI = g_CmdChk.uint32_TempPara[10] & 0xFFFF;
                g_Param.uint16_BipDecI = g_CmdChk.uint32_TempPara[11] & 0xFFFF;
                g_Param.uint16_AccOnDelay = g_CmdChk.uint32_TempPara[12] & 0xFFFF;
                g_Param.uint16_DecOffDelay = g_CmdChk.uint32_TempPara[13] & 0xFFFF;
                g_Param.uint8_Acknowledge = g_CmdChk.uint32_TempPara[14] & 0xFF;
                
                //before continue verify type of motor to know what is to do!
                if(g_CmdChk.uint32_TempPara[1] == 'N') //type of motor = None?
                {
                    g_Param.uint8_ErrCode = _MotTypRUN;         //set error code
                    uart2_SendErrorCode(g_Param.uint8_ErrCode); //call subroutine
                }
                else if(g_CmdChk.uint32_TempPara[1] == 'L')    //type of motor = Lin?
                {
                    g_Param.uint8_ErrCode = _LinRUN;            //set error code
                    uart2_SendErrorCode(g_Param.uint8_ErrCode); //call subroutine                
                }
                else if(g_CmdChk.uint32_TempPara[1] == 'B')    //type of motor = Bipolar?
                {
                    //to decide what is to do when motor type changes
                    g_Param.uint8_MotTyp = g_CmdChk.uint32_TempPara[1] & 0xFF;
                    //call subroutine to switch on/off the outputs for the motor type
                    funct_CheckMotType(g_Param.uint8_MotTyp);
                    
                    
                    // Set the step mode
                    switch(g_Param.uint8_StepMode)
                    {
                        case 0: g_Bipol.uint1_ErrConfig = 1;    // Mode Full step one phase on is not supported by the A3981 driver ==> error
                        break;
                        case 1: A3981.CONFIG0.BITS.MS = 0;      // Mode Full step
                        break;
                        case 2: // TODO: Check the current compensated config
                        case 3: A3981.CONFIG0.BITS.MS = 1;      // Mode Half step
                        break;
                        case 4: A3981.CONFIG0.BITS.MS = 2;      // Mode quarter step
                        break;
                        case 5: A3981.CONFIG0.BITS.MS = 3;      // Mode sixteenth step
                        break;
                        default:g_Bipol.uint1_ErrConfig = 1;    // default ==> error
                        break;
                    }
                    g_Bipol.uint32_GoalPos = g_Param.uint32_StepCount;    //store step count into goal position         
                    g_Bipol.uint16_RunFreq = g_Param.uint16_RunFreq;      //store the run frequency
                    // Set the running direction
                    oBiDirSignal = !g_Param.uint8_Direction;
                    // State of supply after the end of the running
                    g_Bipol.uint1_CurrInCoilAtTheEnd = g_Param.uint8_CoilState;

                    switch(g_Param.uint8_SelectRamp)
                    {
                        case 0: g_Bipol.uint1_IsDecNeeded = 0;      // No ramp
                                g_Bipol.uint1_NextStepIsRamp = 0;
                        break;
                        case 1: g_Bipol.uint1_IsDecNeeded = 0;      // Increase ramp only
                                g_Bipol.uint1_NextStepIsRamp = 1;
                        break;
                        case 2: g_Bipol.uint1_IsDecNeeded = 1;      // Increase and decrease ramp
                                g_Bipol.uint1_NextStepIsRamp = 1;
                        break;
                        default:g_Bipol.uint1_ErrConfig = 1;        // Error case
                        break;
                    }
                    //define switch ON time - first convert and then store it
                    g_Bipol.uint32_SwOnTime = funct_msToTimer23(g_Param.uint16_AccOnDelay);
                    //define switch OFF time - first convert and then store it
                    g_Bipol.uint32_SwOffTime = funct_msToTimer23(g_Param.uint16_DecOffDelay);
                    
                    if(!g_Bipol.uint1_ErrConfig)
                    {
                        SendOneDataSPI1(A3981.CONFIG0.REG);
                        SendOneDataSPI1(A3981.CONFIG1.REG);
                        A3981.RUN.BITS.EN = 1;
                        g_Bipol.uint1_IsBipolEnabled = 1;
                        
                        //no error detected, so enable global lock
                        g_CmdChk.uint8_GlobalLock = 1;
                    }
                    else
                    {
                        g_Param.uint8_ErrCode = _OutOfTolRUN;       //set error code
                        uart2_SendErrorCode(g_Param.uint8_ErrCode); //call subroutine
                    }     
                }
                //type of motor = unipolar or matrix?
                else if((g_CmdChk.uint32_TempPara[1] == 'U') || (g_CmdChk.uint32_TempPara[1] == 'M'))
                {
                    //clear the settings before defining
                    g_Uni.uint8_Settings = 0;
                            
                    //to decide what is to do when motor type changes
                    g_Param.uint8_MotTyp = g_CmdChk.uint32_TempPara[1] & 0xFF;  
                    //call subroutine to switch on/off the outputs for the motor type
                    funct_CheckMotType(g_Param.uint8_MotTyp);
                           
                    if(g_Param.uint8_StepMode == 0) //which step mode?
                    {
                        g_Uni.uint8_Settings &= 0xFB;   //enable full step
                        g_Uni.uint8_Settings &= 0xBF;   //one phase ON
                    }
                    else if(g_Param.uint8_StepMode == 1)
                    {
                        g_Uni.uint8_Settings &= 0xFB;   //enable full step
                        g_Uni.uint8_Settings |= 0x40;   //two phase ON
                    }
                    else if(g_Param.uint8_StepMode == 2)
                    {
                        g_Uni.uint8_Settings |= 0x04;   //enable half step
                    }
                    else
                    {
                        //error - definition is half step compensated or micro step send error later
                        uint8_UniErrConfig = 1;     //set error
                        g_Uni.uint8_Settings = 0;   //erase settings
                    }
          
                    g_Uni.uint32_GoalPos = g_Param.uint32_StepCount;    //store step count into goal position         
                    g_Uni.uint16_RunFreq = g_Param.uint16_RunFreq;      //store the run frequency
                 
                    if(g_Param.uint8_Direction)     //define direction
                    {
                        g_Uni.uint8_Settings &= 0xEF;   //enable CCW
                    }
                    else
                    {
                        g_Uni.uint8_Settings |= 0x10;   //enable CW
                    }
          
                    if(g_Param.uint8_CoilState)     //define coil state
                    {
                        g_Uni.uint8_Settings |= 0x08;   //enable coil current
                    }
                    else
                    {
                        g_Uni.uint8_Settings &= 0xF7;   //disable coil current
                    }
          
                    if(g_Param.uint8_SelectRamp == 0)   //ramp disabled?
                    {
                        g_Uni.uint8_Settings &= 0xFD;   //disable ramp function
                    }
                    else if(g_Param.uint8_SelectRamp == 1)  //only acceleration?
                    {
                        g_Uni.uint8_Settings |= 0x02;   //enable ramp function
                        g_Uni.uint8_Settings &= 0xDF;   //only acceleration
                    }
                    else if(g_Param.uint8_SelectRamp == 2)  //both ramps active?
                    {
                        g_Uni.uint8_Settings |= 0x02;   //enable ramp function
                        g_Uni.uint8_Settings |= 0x20;   //both ramps acceleration and decelration
                    }
                    else
                    {
                        //error - definition is unknown send error later
                        uint8_UniErrConfig = 1;     //set error
                        g_Uni.uint8_Settings = 0;   //erase settings
                    }                   
          
                    //define switch ON time - first convert and then store it
                    g_Uni.uint32_SwOnTime = funct_msToTimer23(g_Param.uint16_AccOnDelay);
                    //define switch OFF time - first convert and then store it
                    g_Uni.uint32_SwOffTime = funct_msToTimer23(g_Param.uint16_DecOffDelay);
          
                    if(uint8_UniErrConfig)  //verify if we had an configuration error
                    {
                        //error detected, send error code
                        g_Param.uint8_ErrCode = _OutOfTolRUN;       //set error code
                        uart2_SendErrorCode(g_Param.uint8_ErrCode); //call subroutine
                        
                        g_Uni.uint8_Settings &= 0xFE;   //disable run bit
                        
                        //switch off output's
                        oUniCoilA1 = _UniPhOFF;     //output PhA1 = off
                        oUniCoilA2 = _UniPhOFF;     //output PhA2 = off
                        oUniCoilB1 = _UniPhOFF;     //output PhB1 = off
                        oUniCoilB2 = _UniPhOFF;     //output PhB2 = off
                    }
                    else
                    {
                        //nothing wrong, set the run bit
                        g_Uni.uint8_Settings |= 0x01;   //enable run bit
                        
                        //no error detected, so enable the global lock
                        g_CmdChk.uint8_GlobalLock = 1;
                    }      
                }
                else
                {
                    g_Param.uint8_ErrCode = _UnknownMotTyp;     //set error code
                    uart2_SendErrorCode(g_Param.uint8_ErrCode); //call subroutine
                }
            }
            else
            {
                g_Param.uint8_ErrCode = _OutOfTolRUN;       //set error code
                uart2_SendErrorCode(g_Param.uint8_ErrCode); //call subroutine
            }
        }      
    }
    else
    {
        g_Param.uint8_ErrCode = _NumbRecCharNotOK;  //set error code
        uart2_SendErrorCode(g_Param.uint8_ErrCode); //call subroutine
    }
}   //end of cmd_RUN


/**********************************************************************************************************************
 * Routine:                 cmd_SPHC

 * Description:
 * Used for bipolar actuators to set the current percentage used in half step mode current compensated.
 * 
 * Creator:                 A. Staub
 * Date of creation:        25.10.2015
 * Last modification on:    -
 * Modified by:             - 
 * 
 * Input:                   -
 * Output:                  -
***********************************************************************************************************************/
void cmd_SPHC(void)
{
    auto unsigned char uint8_Result = 0;    //local work byte for the result 
    
    if(g_CmdChk.uint8_ParamPos == 2)   //number of received characters OK?
    {
        //then verify the received parameters
        uint8_Result += funct_CheckTol(g_CmdChk.uint32_TempPara[1],_BipIPctMin,_BipIPctMax);
    
        if(uint8_Result == 1)         //each parameter within the tolerance?
        {   
            //then store the parameters and send back the OK 
            g_Param.uint8_BipIPct = g_CmdChk.uint32_TempPara[1] & 0xFF;      
            uart2_sendbuffer('E');
            uart2_sendbuffer(13);
        }
        else
        { 
            //g_Param.uint8_ErrCode = _BipSPHC;           //set error code
            //uart2_SendErrorCode(g_Param.uint8_ErrCode); //call subroutine
        }
    }
    else
    {
        g_Param.uint8_ErrCode = _NumbRecCharNotOK;  //set error code
        uart2_SendErrorCode(g_Param.uint8_ErrCode); //call subroutine
    }
}   //end of cmd_SPHC


/**********************************************************************************************************************
 * Routine:                 cmd_GVER

 * Description:
 * This subroutine sends back the information of the REFAST device.
 * Mod. 29.11.2015:
 * I modified the subroutine that the customer can read out the version even if the actuator
 * is executing steps. 
 * 
 * Creator:                 A. Staub
 * Date of creation:        10.10.2015
 * Last modification on:    29.11.2015
 * Modified by:             A. Staub
 * 
 * Input:                   -
 * Output:                  -
***********************************************************************************************************************/
void cmd_GVER(void)
{
    if(g_CmdChk.uint8_ParamPos == 1)    //number of received characters OK?
    {   
        RV30xx_TempMeas();          //measure the temperature
        funct_LoadDeviceInfo();     //call subroutine 
    }
    else
    {
        g_Param.uint8_ErrCode = _NumbRecCharNotOK;  //set error code
        uart2_SendErrorCode(g_Param.uint8_ErrCode); //call subroutine
    }
}   //end of cmd_GVER


/**********************************************************************************************************************
 * Routine:                 cmd_RAZ

 * Description:
 * This subroutine reset all to default value, switch off the outputs and sends back the information 
 * of the REFAST device.
 * 
 * Creator:                 A. Staub
 * Date of creation:        19.10.2015
 * Last modification on:    -
 * Modified by:             - 
 * 
 * Input:                   -
 * Output:                  -
***********************************************************************************************************************/
void cmd_RAZ(void)
{
    if(g_CmdChk.uint8_ParamPos == 1)   //number of received characters OK?
    {
        if(g_CmdChk.uint8_GlobalLock == 1)  //global lock enabled?
        {
            g_Param.uint8_ErrCode = _MotorInRun;        //set error code
            uart2_SendErrorCode(g_Param.uint8_ErrCode); //call subroutine
        }
        else
        {
            //send back a first quittance
            uart2_sendbuffer('E');
            uart2_sendbuffer(13);
        
            //send back the device information's
            funct_LoadDeviceInfo();         //call subroutine
            
            IEC1SET = _IEC1_U2TXIE_MASK;    //enable send routine
            
            //wait 500ms before executing the software reset
            g_Timer1.uint8_TimeoutFlag = 1;     //set the timeout flag
            SetTimer(_TIMER1,_ENABLE,0,500);    //load the timer with 500ms
            while(g_Timer1.uint8_TimeoutFlag);  //wait the time
            
            //launch software reset
            SoftReset();
        }     
    }
    else
    {
        g_Param.uint8_ErrCode = _NumbRecCharNotOK;  //set error code
        uart2_SendErrorCode(g_Param.uint8_ErrCode); //call subroutine
    }
}   //end of cmd_RAZ


/**********************************************************************************************************************
 * Routine:                 cmd_SMTYP

 * Description:
 * Only allowed if the motor is not in run. If all is correct, it sends back an E<CR>. 
 * 
 * Modification 25.01.2016 / A. Staub
 * Routine is much smaller now, because we don't need to verify the limits. 
 * 
 * Creator:                 A. Staub
 * Date of creation:        19.10.2015
 * Last modification on:    25.01.2016
 * Modified by:             A. Staub
 * 
 * Input:                   -
 * Output:                  -
***********************************************************************************************************************/
void cmd_SMTYP(void)
{
    auto unsigned char uint8_Result = 0;        //local work byte for the result 
  
    if(g_CmdChk.uint8_ParamPos == 2)            //number of received characters OK?
    {
        if(g_CmdChk.uint8_GlobalLock == 1)      //global lock active
        {
            g_Param.uint8_ErrCode = _MotorInRun;        //set error code
            uart2_SendErrorCode(g_Param.uint8_ErrCode); //call subroutine
        }
        else
        {
            //call subroutine to switch on/off the outputs for the motor type
            funct_CheckMotType(g_CmdChk.uint32_TempPara[1]);
                        
            uart2_sendbuffer('E');                  //first the letter E
            uart2_sendbuffer(13);                   //with CR at the end                  
        }      
    }
    else
    {
        g_Param.uint8_ErrCode = _NumbRecCharNotOK;  //set error code
        uart2_SendErrorCode(g_Param.uint8_ErrCode); //call subroutine
    }
}   //end of cmd_SMTYP


/**********************************************************************************************************************
 * Routine:                 cmd_GMTYP

 * Description:
 * Sends back the currently set motor type. 
 * 
 * Creator:                 A. Staub
 * Date of creation:        19.10.2015
 * Last modification on:    -
 * Modified by:             - 
 * 
 * Input:                   -
 * Output:                  -
***********************************************************************************************************************/
void cmd_GMTYP(void)
{
    if(g_CmdChk.uint8_ParamPos == 1)   //number of received characters OK?
    {
        uart2_sendbuffer('E');                    //first the letter E
        uart2_sendbuffer(',');                    //then the comma
        uart2_sendbuffer(g_Param.uint8_MotTyp);   //add the motor type
        uart2_sendbuffer(13);                     //add the CR at the end      
    }
    else
    {
        g_Param.uint8_ErrCode = _NumbRecCharNotOK;  //set error code
        uart2_SendErrorCode(g_Param.uint8_ErrCode); //call subroutine
    }
}   //end of cmd_GMTYP


/**********************************************************************************************************************
 * Routine:                 cmd_STEST

 * Description:
 * Perform a self-test sequence only possible if there is nothing activated, running, etc. 
 * 
 * Creator:                 A. Staub
 * Date of creation:        25.10.2015
 * Last modification on:    -
 * Modified by:             - 
 * 
 * Input:                   -
 * Output:                  -
***********************************************************************************************************************/
void cmd_STEST(void)
{  
    if(g_CmdChk.uint8_ParamPos == 1)   //number of received characters OK?
    {
        if(g_CmdChk.uint8_GlobalLock == 1)  //global lock enable?
        {
            g_Param.uint8_ErrCode = _MotorInRun;        //set error code
            uart2_SendErrorCode(g_Param.uint8_ErrCode); //call subroutine
        }
        else
        {
            g_CmdChk.uint8_GlobalLock = 1;  //enable global lock

            // General explanations: each test definition is described as follow:
            // Description of the test 
            // ==> module, peripheral, device tested
            
            // Check the motor supply voltage 
            // ==> uC ADC channel
            // Remark: Max 12VDC. If this test fail, next steps are not performed.
            
            // Check of the EEPROM memory: read a value, increment it, 
            // write it and read it again to check it.
            // ==> I2C communication
            
            // Read the temperature out from temp sensor 
            // ==> Check of the I2C communication
            
            // Check of the voltage of the button cell 
            // ==> uC ADC channel, battery voltage
            
            // Set a voltage reference 
            // ==> I2C communication, voltage reference chip
            
            // Communication with bipolar driver 
            // ==> SPI communication, bipolar driver status
            
            // Measure of the bipolar coils resistor 
            // ==> uC ADC channel
            
            // Run the unipolar motor 
            // ==> unipolar driver (MOSFETs)
            
            // Measure of the unipolar coils resistor 
            // ==> uC ADC channel
            
            // Run the LIN motor
            // ==> LIN communication
            
            // Measure of the VLIN supply voltage
            // uC ADC channel
            
            // Check all IOs: connect each input on one output. By changing
            // the state of the output, input state should also change.
            // ==> IOs
            
            // Measure of a frequency: use an output to generate a square signal
            // ==> uC input capture 1
            
            // Measure of a PWM signal: use an output to generate a PWM signal
            // ==> uC input capture 2
            
            
            //to define!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

            uart2_sendbuffer('E');                      //first the letter E
            uart2_sendbuffer(13);                       //add the CR at the end
            
            g_CmdChk.uint8_GlobalLock = 0;  //disable global lock
        }
    }
    else
    {
        g_Param.uint8_ErrCode = _NumbRecCharNotOK;  //set error code
        uart2_SendErrorCode(g_Param.uint8_ErrCode); //call subroutine
    }
}   //end of cmd_STEST


/**********************************************************************************************************************
 * Routine:                 cmd_GILIM

 * Description:
 * Sends back the currently set limits.  
 * Mod. 29.11.2015:
 * I changed the routine for that the customer can read out the set limits even if the actuator
 * is executing a move. 
 * 
 * Creator:                 A. Staub
 * Date of creation:        25.10.2015
 * Last modification on:    29.11.2015
 * Modified by:             A. Staub
 * 
 * Input:                   -
 * Output:                  -
***********************************************************************************************************************/
void cmd_GILIM(void)
{
    if(g_CmdChk.uint8_ParamPos == 1)   //number of received characters OK?
    {
        //send back the needed informations
        uart2_sendbuffer('E');                  //first the letter E
        uart2_sendbuffer(',');                  //add the comma
  
        //convert the minimal ON current into an ASCII and store it into the send buffer
        funct_IntToAscii(g_Param.uint16_Imin,_Active);
        uart2_sendbuffer(',');                  //add the comma
  
        //convert the maximal ON current into an ASCII and store it into the send buffer
        funct_IntToAscii(g_Param.uint16_Imax,_Active);
        uart2_sendbuffer(',');                  //add the comma
  
        //convert the test voltage into an ASCII and store it into the send buffer
        funct_IntToAscii(g_Param.uint16_Umot,_Active);
        uart2_sendbuffer(',');                  //add the comma
  
        //convert the short-circuit current limit into an ASCII and store it into the send buffer
        funct_IntToAscii(g_Param.uint16_SCiLevel,_Active);
        uart2_sendbuffer(',');                  //add the comma
  
        //convert the short-circuit wait time into an ASCII and store it into the send buffer
        funct_IntToAscii(g_Param.uint8_SCtLevel,_Active);
        uart2_sendbuffer(13);                   //add the CR at the end      
    }
    else
    {
        g_Param.uint8_ErrCode = _NumbRecCharNotOK;  //set error code
        uart2_SendErrorCode(g_Param.uint8_ErrCode); //call subroutine
    }   
}   //end of cmd_GILIM


/**********************************************************************************************************************
 * Routine:                 cmd_GPHC

 * Description:
 * Sends back the percentage of the current for bipolar actuators in half step mode current compensated if there is 
 * nothing activated, running, etc. 
 * 
 * Creator:                 A. Staub
 * Date of creation:        25.10.2015
 * Last modification on:    -
 * Modified by:             - 
 * 
 * Input:                   -
 * Output:                  -
***********************************************************************************************************************/
void cmd_GPHC(void)
{  
    if(g_CmdChk.uint8_ParamPos == 1)   //number of received characters OK?
    {
        if(g_Uni.uint8_Settings & 0x01) //is motor in run mode?
        {
            g_Param.uint8_ErrCode = _MotorInRun;        //set error code
            uart2_SendErrorCode(g_Param.uint8_ErrCode); //call subroutine
        }
        else
        {
            //send back the needed informations
            uart2_sendbuffer('E');                  //first the letter E
            uart2_sendbuffer(',');                  //add the comma
  
            //convert the percentage from the half step compensation into an ascii and store it into the send buffer
            funct_IntToAscii(g_Param.uint8_BipIPct,_Active);
            uart2_sendbuffer(13);                   //add the CR at the end
        }      
    }
    else
    {
        g_Param.uint8_ErrCode = _NumbRecCharNotOK;  //set error code
        uart2_SendErrorCode(g_Param.uint8_ErrCode); //call subroutine
    }
}   //end of cmd_GPHC


/**********************************************************************************************************************
 * Routine:                 cmd_GRACC

 * Description:
 * Sends back the acceleration ramp if there is nothing activated, running, etc. 
 * This can be a long protocol to send back - maximal 50 times number of steps and 50 times the corresponding frequency.  
 * Note - the send buffer needs a minimum size of:
 * - uint16_AccNumbStep max character length = 5 x 50 different steps = 250
 * - uint16_AccFreq max character lenght = 4 x 50 different frequency = 200
 * - Each parameter separated by a comma = 100 x 1 character = 100
 * - Started with the 'E' and ended with 'CR' = 2
 * total size = 552 bytes.  
 * 
 * Creator:                 A. Staub
 * Date of creation:        25.10.2015
 * Last modification on:    -
 * Modified by:             - 
 * 
 * Input:                   -
 * Output:                  -
***********************************************************************************************************************/
void cmd_GRACC(void)
{  
    auto unsigned char uint8_WB;              //local work byte
    
    if(g_CmdChk.uint8_ParamPos == 1)   //number of received characters OK?
    {
        if(g_CmdChk.uint8_GlobalLock == 1)  //global lock active?
        {
            g_Param.uint8_ErrCode = _MotorInRun;        //set error code
            uart2_SendErrorCode(g_Param.uint8_ErrCode); //call subroutine
        }
        else
        {
            g_CmdChk.uint8_GlobalLock = 1;  //enable global lock
            
            //send back the needed informations
            uart2_sendbuffer('E');                      //first the letter E
           
            for(uint8_WB=0; uint8_WB<50; uint8_WB++)    //read out both arrays
            {
                //is one of the next parameter = 0?
                if((g_Param.uint16_AccNumbStep[uint8_WB] == 0) || (g_Param.uint16_AccFreq[uint8_WB] == 0))
                {
                    uint8_WB = 50;                      //then leave the loop
                }
                else    //otherwise send the parameter back
                {
                    uart2_sendbuffer(',');              //add the comma
    
                    //convert the number of steps into an ASCII and store it into the send buffer
                    funct_IntToAscii(g_Param.uint16_AccNumbStep[uint8_WB],_Active);
                    uart2_sendbuffer(',');              //add the comma
    
                    //convert the frequency into an ASCII and store it into the send buffer
                    funct_IntToAscii(g_Param.uint16_AccFreq[uint8_WB],_Active);        
                }
            }
            uart2_sendbuffer(13);                       //add the CR at the end
            
            g_CmdChk.uint8_GlobalLock = 0;  //disable global lock
        }      
    }
    else
    {
        g_Param.uint8_ErrCode = _NumbRecCharNotOK;  //set error code
        uart2_SendErrorCode(g_Param.uint8_ErrCode); //call subroutine
    }
}   //end of cmd_GRACC


/**********************************************************************************************************************
 * Routine:                 cmd_GRDEC

 * Description:
 * Sends back the deceleration ramp if there is nothing activated, running, etc. 
 * This can be a long protocol to send back - maximal 50 times number of steps and 50 times the corresponding frequency.  
 * Note - the send buffer needs a minimum size of:
 * - uint16_DecNumbStep max character length = 5 x 50 different steps = 250
 * - uint16_DecFreq max character length = 4 x 50 different frequency = 200
 * - Each parameter separated by a comma = 100 x 1 character = 100
 * - Started with the 'E' and ended with 'CR' = 2
 * total size = 552 bytes.  
 * 
 * Creator:                 A. Staub
 * Date of creation:        25.10.2015
 * Last modification on:    -
 * Modified by:             - 
 * 
 * Input:                   -
 * Output:                  -
***********************************************************************************************************************/
void cmd_GRDEC(void)
{  
    auto unsigned char uint8_WB;              //local work byte
    
    if(g_CmdChk.uint8_ParamPos == 1)   //number of received characters OK?
    {
        if(g_CmdChk.uint8_GlobalLock == 1)  //global lock active?
        {
            g_Param.uint8_ErrCode = _MotorInRun;        //set error code
            uart2_SendErrorCode(g_Param.uint8_ErrCode); //call subroutine
        }
        else
        {
            g_CmdChk.uint8_GlobalLock = 1;  //enable global lock
            
            //send back the needed informations
            uart2_sendbuffer('E');                      //first the letter E
           
            for(uint8_WB=0; uint8_WB<50; uint8_WB++)    //read out both arrays
            {
                //is one of the next parameter = 0?
                if((g_Param.uint16_DecNumbStep[uint8_WB] == 0) || (g_Param.uint16_DecFreq[uint8_WB] == 0))
                {
                    uint8_WB = 50;                      //then leave the loop
                }
                else    //otherwise send the parameter back
                {
                    uart2_sendbuffer(',');              //add the comma
    
                    //convert the number of steps into an ASCII and store it into the send buffer
                    funct_IntToAscii(g_Param.uint16_DecNumbStep[uint8_WB],_Active);
                    uart2_sendbuffer(',');              //add the comma
    
                    //convert the frequency into an ASCII and store it into the send buffer
                    funct_IntToAscii(g_Param.uint16_DecFreq[uint8_WB],_Active);        
                }
            }
            uart2_sendbuffer(13);                       //add the CR at the end 
            
            g_CmdChk.uint8_GlobalLock = 0;  //disable global lock
        }      
    }
    else
    {
        g_Param.uint8_ErrCode = _NumbRecCharNotOK;  //set error code
        uart2_SendErrorCode(g_Param.uint8_ErrCode); //call subroutine
    }
}   //end of cmd_GRDEC


/**********************************************************************************************************************
 * Routine:                 cmd_GRUN

 * Description:
 * Send back the run parameter like:
 * motor type, step mode, step count, speed, direction, coil state, Irun, Ihold, selected ramp, ACC I chopper,
 * DEC I chopper, ACC ON delay, DEC OFF delay, protocol
 * 
 * Creator:                 A. Staub
 * Date of creation:        26.10.2015
 * Last modification on:    -
 * Modified by:             - 
 * 
 * Input:                   -
 * Output:                  -
***********************************************************************************************************************/
void cmd_GRUN(void)
{  
    if(g_CmdChk.uint8_ParamPos == 1)   //number of received characters OK?
    {
        //send back the needed informations
        uart2_sendbuffer('E');                      //first the letter E
        uart2_sendbuffer(',');                      //add the comma
        uart2_sendbuffer(g_Param.uint8_MotTyp);     //add the motor type
        uart2_sendbuffer(',');                      //add the comma
  
        //convert the step mode of the actuator into an ASCII and store it into the send buffer
        funct_IntToAscii(g_Param.uint8_StepMode,_Active);
        uart2_sendbuffer(',');                      //add the comma
  
        //convert the step count of the actuator into an ASCII and store it into the send buffer
        funct_IntToAscii(g_Param.uint32_StepCount,_Active);
        uart2_sendbuffer(',');                      //add the comma 
  
        //convert the speed into an ASCII and store it into the send buffer
        funct_IntToAscii(g_Param.uint16_RunFreq,_Active);
        uart2_sendbuffer(',');                      //add the comma
  
        //convert the direction into an ASCII and store it into the send buffer
        funct_IntToAscii(g_Param.uint8_Direction,_Active);
        uart2_sendbuffer(',');                      //add the comma
  
        //convert the coil state into an ASCII and store it into the send buffer
        funct_IntToAscii(g_Param.uint8_CoilState,_Active);
        uart2_sendbuffer(',');                      //add the comma
  
        //convert the current during move into an ASCII and store it into the send buffer
        funct_IntToAscii(g_Param.uint16_BipRunI,_Active);
        uart2_sendbuffer(',');                      //add the comma
  
        //convert the "holding" current into an ASCII and store it into the send buffer
        funct_IntToAscii(g_Param.uint16_BipHoldI,_Active);
        uart2_sendbuffer(',');                      //add the comma
  
        //convert the selected ramp into an ASCII and store it into the send buffer
        funct_IntToAscii(g_Param.uint8_SelectRamp,_Active);
        uart2_sendbuffer(',');                      //add the comma
  
        //convert the current for acceleration into an ASCII and store it into the send buffer
        funct_IntToAscii(g_Param.uint16_BipAccI,_Active);
        uart2_sendbuffer(',');                      //add the comma
  
        //convert the current for deceleration into an ASCII and store it into the send buffer
        funct_IntToAscii(g_Param.uint16_BipDecI,_Active);
        uart2_sendbuffer(',');                      //add the comma
 
        //convert the time into an ASCII and store it into the send buffer
        funct_IntToAscii(g_Param.uint16_AccOnDelay,_Active);
        uart2_sendbuffer(',');                      //add the comma
  
        //convert the time into an ASCII and store it into the send buffer
        funct_IntToAscii(g_Param.uint16_DecOffDelay,_Active);
        uart2_sendbuffer(',');                      //add the comma
  
        //convert the protocol quittance into an ASCII and store it into the send buffer
        funct_IntToAscii(g_Param.uint8_Acknowledge,_Active);
        uart2_sendbuffer(13);                       //add the CR at the end      
    }
    else
    {
        g_Param.uint8_ErrCode = _NumbRecCharNotOK;  //set error code
        uart2_SendErrorCode(g_Param.uint8_ErrCode); //call subroutine
    }
}   //end of cmd_GRUN


/**********************************************************************************************************************
 * Routine:                 cmd_BREAK

 * Description:
 * Stops immediately all running systems!!!
 * 
 * Modified 23.03.2016 by A. Staub:
 * Added the conditions.
 * 
 * Creator:                 A. Staub
 * Date of creation:        26.10.2015
 * Last modification on:    23.03.2016
 * Modified by:             A. Staub
 * 
 * Input:                   -
 * Output:                  -
***********************************************************************************************************************/
void cmd_BREAK(void)
{  
    if(g_CmdChk.uint8_ParamPos == 1)    //number of received characters OK?
    {
        g_CmdChk.uint8_GlobalLock = 0;  //global lock disable
        
    // Code for bipolar motor here below //
//        oBiResetSignal = 0;
//        oVmotOnOff = 0;
//        oBiEnaVmot = 0;
        g_Bipol.uint32_RealPos = 0;
        g_Bipol.uint32_GoalPos = 0;
        
        g_Bipol.uint1_IsBipolEnabled = 0;
        T4CON &= 0x7FFF;    //disable interrupt module
        TMR4 = 0x0;         //clear contents of the TMR4 and TMR5
        PR4 = 400;          //load the timer with 10us
        IFS0CLR = _IFS0_T5IF_MASK;  //clear interrupt flag
        g_Bipol.uint1_IntTimeExpiredFlag = 0;    //force the interrupt routine to load the new time
        
        if(!g_Bipol.uint1_CurrInCoilAtTheEnd) //coils current active after move?
        {
            //switch off all outputs
            A3981.RUN.BITS.EN = 0;
            SendOneDataSPI1(A3981.RUN.REG);
        }
        else
        {
            bi_ImotToDAC(g_Param.uint16_BipHoldI);
        }
        
        if(g_Param.uint8_RunBit)
        {
            oSinkSource0 = 0;
        }

        g_Bipol.status.BITS.nextStepIsAllowed = 0;     //clear 'NS', 'DEC', 'ACC', 'GOAL' and 'LS' bit
        g_Bipol.status.BITS.decelerationIsActived = 0;
        g_Bipol.status.BITS.accelerationIsActived = 0;
        g_Bipol.status.BITS.accelerationIsActived = 0;
        g_Bipol.status.BITS.goalIsReached = 0;
        g_Bipol.status.BITS.lastStepIsActived = 0;
        g_Bipol.status.BITS.firstStepIsActived = 1;     //set the bit 'FS'                          
        
        
    // Code for unipolar motor here below  
        g_Uni.uint8_Settings &= 0xFE;   //clear the 'RUN' bit
        g_Uni.uint8_Status &= 0xC1;     //clear 'NS', 'DEC', 'ACC', 'GOAL' and 'LS' bit
        g_Uni.uint8_Status |= 0x01;     //set the bit 'FS'
        
        g_Param.uint8_GOcmdTrue = 0;
        g_Uni.uint32_RealPos = 0;
        g_Uni.uint32_GoalPos = 0;
        
        T4CON &= 0x7FFF;                    //disable interrupt module
        TMR4 = 0x0;                         //clear contents of the TMR4 and TMR5
        PR4 = 400;                          //load the timer with 10us
        IFS0CLR = _IFS0_T5IF_MASK;          //clear interrupt flag
        
        if(g_Uni.uint8_Settings & 0x08)     //coils current active after move?
        {
            //then nothing is to do
        }
        else
        {
            //otherwise switch off all outputs
            oUniCoilA1 = _UniPhOFF;         //output PhA1 = off
            oUniCoilA2 = _UniPhOFF;         //output PhA2 = off
            oUniCoilB1 = _UniPhOFF;         //output PhB1 = off
            oUniCoilB2 = _UniPhOFF;         //output PhB2 = off
        }
        
        if(g_Param.uint8_RunBit)
        {
            oSinkSource0 = 0;
        }
 
        
    // Code for LIN motor here below // 
//        oVmotOnOff = 0;
        oEnaVLINSupply = 0;
        
        //send back the needed informations
        uart2_sendbuffer('E');                  //first the letter E
        uart2_sendbuffer(13);                   //add the CR at the end
        
        g_CmdChk.uint8_GlobalLock = 0;  //disable global lock
    }
    else
    {
        g_Param.uint8_ErrCode = _NumbRecCharNotOK;  //set error code
        uart2_SendErrorCode(g_Param.uint8_ErrCode); //call subroutine
    }    
}   //end of cmd_BREAK


/**********************************************************************************************************************
 * Routine:                 cmd_MUMOT

 * Description:
 * Send back the measured entry voltage Ue/Umot. 
 * 
 * Creator:                 A. Staub
 * Date of creation:        26.10.2015
 * Last modification on:    -
 * Modified by:             - 
 * 
 * Input:                   -
 * Output:                  -
***********************************************************************************************************************/
void cmd_MUMOT(void)
{  
    auto unsigned long int uint32_WB;   //local work byte
    
    if(g_CmdChk.uint8_ParamPos == 1)   //number of received characters OK?
    {
        //measure here the voltage or take the last measured parameter and send it back!
        uint32_WB = ads1115_read();     //read out Vmot
        
        //convert the result in mV only if the result is not 0!
        if(uint32_WB)
        {
            uint32_WB = funct_ADCtoMiliUnit(uint32_WB,464);
        }
        
        //send back the needed informations
        uart2_sendbuffer('E');                          //first the letter E
        uart2_sendbuffer(',');                          //add the comma
        //funct_IntToAscii(g_Param.uint16_Ue,_Active);    //add the voltage
        funct_IntToAscii(uint32_WB,_Active);    //add the voltage
        uart2_sendbuffer('m');                          //add the m
        uart2_sendbuffer('V');                          //add the V
        uart2_sendbuffer(13);                           //add the CR at the end      
    }
    else
    {
        g_Param.uint8_ErrCode = _NumbRecCharNotOK;  //set error code
        uart2_SendErrorCode(g_Param.uint8_ErrCode); //call subroutine
    }
}   //end of cmd_MUMOT


/**********************************************************************************************************************
 * Routine:                 cmd_SCOILON

 * Description:
 * This command can be used to switch on / off manually the coil's. 
 * 
 * Creator:                 A. Staub
 * Date of creation:        26.10.2015
 * Last modification on:    -
 * Modified by:             - 
 * 
 * Input:                   -
 * Output:                  -
***********************************************************************************************************************/
void cmd_SCOILON(void)
{  
    auto unsigned char uint8_Result = 0;    //local work byte
    auto unsigned char uint8_WB;            //local work byte
    
    if(g_CmdChk.uint8_ParamPos == 6)   //number of received characters OK?
    {
        if(g_CmdChk.uint8_GlobalLock == 1)  //global lock enabled?
        {
            g_Param.uint8_ErrCode = _MotorInRun;        //set error code
            uart2_SendErrorCode(g_Param.uint8_ErrCode); //call subroutine
        }
        else
        {
            //verify the limits if they are inside the tolerance
            uint8_WB = g_CmdChk.uint32_TempPara[1];
            if((uint8_WB == 'B') || (uint8_WB == 'M') || (uint8_WB == 'U'))
            {
                uint8_Result++;
            }
            else
            {
                //do nothing
            }
            uint8_Result += funct_CheckTol(g_CmdChk.uint32_TempPara[2],0,1);
            uint8_Result += funct_CheckTol(g_CmdChk.uint32_TempPara[3],0,1);      
            uint8_Result += funct_CheckTol(g_CmdChk.uint32_TempPara[4],0,1);
            uint8_Result += funct_CheckTol(g_CmdChk.uint32_TempPara[5],0,1);
            
            if(uint8_Result == 5)   //each parameter within the tolerance?
            {              
                //before continue verify type of motor to know what is to do!
                if(g_CmdChk.uint32_TempPara[1] == 'N') //type of motor = None?
                {
                    g_Param.uint8_ErrCode = _MotTypSCOILON;     //set error code
                    uart2_SendErrorCode(g_Param.uint8_ErrCode); //call subroutine
                }
                else if(g_CmdChk.uint32_TempPara[1] == 'L')    //type of motor = Lin?
                {
                    g_Param.uint8_ErrCode = _LinSCOILON;        //set error code
                    uart2_SendErrorCode(g_Param.uint8_ErrCode); //call subroutine                
                }
                else if(g_CmdChk.uint32_TempPara[1] == 'B')    //type of motor = Bipolar?
                {
                    //verify coil A
                    if((g_CmdChk.uint32_TempPara[2] == 1) && (g_CmdChk.uint32_TempPara[3] == 1))
                    {
                        oBiRelayCoilA = 1;
                    }
                    else
                    {
                        oBiRelayCoilA = 0;
                    }
                    
                    //verify coil B
                    if((g_CmdChk.uint32_TempPara[4] == 1) && (g_CmdChk.uint32_TempPara[5] == 1))
                    {
                        oBiRelayCoilB = 1;
                    }
                    else
                    {
                        oBiRelayCoilB = 0;
                    }
                    
                    uart2_sendbuffer('E');                  //first the letter E
                    uart2_sendbuffer(13);                   //add the CR at the end
                }
                //type of motor = unipolar or matrix?
                else if((g_CmdChk.uint32_TempPara[1] == 'U') || (g_CmdChk.uint32_TempPara[1] == 'M'))
                {
                    //verify coil A1
                    if(g_CmdChk.uint32_TempPara[2] == 1)
                    {
                        oUniCoilA1 = 1;
                    }
                    else
                    {
                        oUniCoilA1 = 0;
                    }
                    
                    //verify coil A2
                    if(g_CmdChk.uint32_TempPara[3] == 1)
                    {
                        oUniCoilA2 = 1;
                    }
                    else
                    {
                        oUniCoilA2 = 0;
                    }
                    
                    //verify coil B1
                    if(g_CmdChk.uint32_TempPara[4] == 1)
                    {
                        oUniCoilB1 = 1;
                    }
                    else
                    {
                        oUniCoilB1 = 0;
                    }
                    
                    //verify coil B2
                    if(g_CmdChk.uint32_TempPara[5] == 1)
                    {
                        oUniCoilB2 = 1;
                    }
                    else
                    {
                        oUniCoilB2 = 0;
                    }
                    
                    uart2_sendbuffer('E');                  //first the letter E
                    uart2_sendbuffer(13);                   //add the CR at the end
                }
                else
                {
                    g_Param.uint8_ErrCode = _UnknownMotTyp;     //set error code
                    uart2_SendErrorCode(g_Param.uint8_ErrCode); //call subroutine
                }
            }  
            else
            {
                g_Param.uint8_ErrCode = _OutOfTolSCOILON;   //set error code
                uart2_SendErrorCode(g_Param.uint8_ErrCode); //call subroutine
            }
        }      
    }
    else
    {
        g_Param.uint8_ErrCode = _NumbRecCharNotOK;  //set error code
        uart2_SendErrorCode(g_Param.uint8_ErrCode); //call subroutine
    }
}   //end of cmd_SCOILON


/**********************************************************************************************************************
 * Routine:                 cmd_SSMOD

 * Description:
 * This command can be used to define the step mode for the application. 
 * 
 * Creator:                 A. Staub
 * Date of creation:        28.11.2015
 * Last modification on:    -
 * Modified by:             - 
 * 
 * Input:                   -
 * Output:                  -
***********************************************************************************************************************/
void cmd_SSMOD(void)
{
    if(g_CmdChk.uint8_ParamPos == 2)   //number of received characters OK?
    {
        if(g_CmdChk.uint8_GlobalLock == 1)  //global lock enabled?
        {
            g_Param.uint8_ErrCode = _MotorInRun;        //set error code
            uart2_SendErrorCode(g_Param.uint8_ErrCode); //call subroutine
        }
        else
        {
            switch(g_CmdChk.uint32_TempPara[1])    //verify mode of step
            {
                case (0):       //full step one phase on
                    g_Param.uint8_StepMode = 0;
                    g_Uni.uint8_Settings &= 0xFB;   //enable full step
                    g_Uni.uint8_Settings &= 0xBF;   //one phase ON for unipolar
                    //send back the needed informations
                    uart2_sendbuffer('E');                  //first the letter E
                    uart2_sendbuffer(13);                   //add the CR at the end
                    break;
                    
                case (1):       //full step two phase on
                    g_Param.uint8_StepMode = 1;
                    g_Uni.uint8_Settings &= 0xFB;   //enable full step
                    g_Uni.uint8_Settings |= 0x40;   //two phase ON for unipolar
                    //send back the needed informations
                    uart2_sendbuffer('E');                  //first the letter E
                    uart2_sendbuffer(13);                   //add the CR at the end
                    break;
                    
                case (2):       //half step
                    g_Param.uint8_StepMode = 2;
                    g_Uni.uint8_Settings |= 0x04;   //enable half step for unipolar
                    //send back the needed informations
                    uart2_sendbuffer('E');                  //first the letter E
                    uart2_sendbuffer(13);                   //add the CR at the end
                    break;
                    
                case (3):       //half step current compensated
                    g_Param.uint8_StepMode = 3;
                    //send back the needed informations
                    uart2_sendbuffer('E');                  //first the letter E
                    uart2_sendbuffer(13);                   //add the CR at the end
                    break;
                    
                case (4):       //micro stepping (quarter)
                    g_Param.uint8_StepMode = 4;
                    //send back the needed informations
                    uart2_sendbuffer('E');                  //first the letter E
                    uart2_sendbuffer(13);                   //add the CR at the end
                    break;
                    
                case (5):       //micro stepping (sixteenth)
                    g_Param.uint8_StepMode = 5;
                    //send back the needed informations
                    uart2_sendbuffer('E');                  //first the letter E
                    uart2_sendbuffer(13);                   //add the CR at the end
                    break;
                    
                default:        //not defined - error
                    g_Param.uint8_ErrCode = _StepModeSSMOD;     //set error code
                    uart2_SendErrorCode(g_Param.uint8_ErrCode); //call subroutine
                    break;
            }
        }      
    }
    else
    {
        g_Param.uint8_ErrCode = _NumbRecCharNotOK;  //set error code
        uart2_SendErrorCode(g_Param.uint8_ErrCode); //call subroutine
    }
}   //end of cmd_SSMOD


/**********************************************************************************************************************
 * Routine:                 cmd_GSMOD

 * Description:
 * This command send back the current step mode 
 * 
 * Creator:                 A. Staub
 * Date of creation:        28.11.2015
 * Last modification on:    -
 * Modified by:             - 
 * 
 * Input:                   -
 * Output:                  -
***********************************************************************************************************************/
void cmd_GSMOD(void)
{
    if(g_CmdChk.uint8_ParamPos == 1)   //number of received characters OK?
    {
        uart2_sendbuffer('E');                    //first the letter E
        uart2_sendbuffer(',');                    //then the comma
        funct_IntToAscii(g_Param.uint8_StepMode,_Active); //add the step mode
        uart2_sendbuffer(13);                     //add the CR at the end      
    }
    else
    {
        g_Param.uint8_ErrCode = _NumbRecCharNotOK;  //set error code
        uart2_SendErrorCode(g_Param.uint8_ErrCode); //call subroutine
    }
}   //end of cmd_GSMOD


/**********************************************************************************************************************
 * Routine:                 cmd_SBIT

 * Description:
 * Verify the received parameters of this command, if all parameters are within the tolerance.
 * If all is correct, then set the corresponding bit / output 
 * 
 * Modification 25.02.2016 / A. Staub:
 * I changed the command to be compatible with the JE command. But the third parameter will
 * be not use. 
 * 
 * Creator:                 A. Staub
 * Date of creation:        28.11.2015
 * Last modification on:    25.02.2016
 * Modified by:             A. Staub
 * 
 * Input:                   -
 * Output:                  -
***********************************************************************************************************************/
void cmd_SBIT(void)
{
    auto unsigned char uint8_Result = 0;    //local work byte
    
    //number of received characters OK?
    if((g_CmdChk.uint8_ParamPos == 2) || (g_CmdChk.uint8_ParamPos == 3))           
    {
        //verify the limits if they are inside the tolerance
        uint8_Result += funct_CheckTol(g_CmdChk.uint32_TempPara[1],_BitMin,_BitMax);
        
        if(uint8_Result == 1)   //each parameter within the tolerance?
        {
            //set the corresponding bit / output by call the subroutine
            funct_OutputHandler(_SetBit,(g_CmdChk.uint32_TempPara[1] & 0xFF));
                   
            //send back the needed informations
            uart2_sendbuffer('E');                  //first the letter E
            uart2_sendbuffer(13);                   //add the CR at the end
        }
        else
        {
            g_Param.uint8_ErrCode = _OutOfTolBit;       //set error code
            uart2_SendErrorCode(g_Param.uint8_ErrCode); //call subroutine
        }
    }
    else
    {
        g_Param.uint8_ErrCode = _NumbRecCharNotOK;  //set error code
        uart2_SendErrorCode(g_Param.uint8_ErrCode); //call subroutine
    }  
}   //end of cmd_SBIT


/**********************************************************************************************************************
 * Routine:                 cmd_CBIT

 * Description:
 * Verify the received parameters of this command, if all parameters are within the tolerance.
 * If all is correct, then clear the corresponding bit / output
 * 
 * Modification 25.02.2016 / A. Staub:
 * I changed the command to be compatible with the JE command. But the third parameter will
 * be not use.  
 * 
 * Creator:                 A. Staub
 * Date of creation:        28.11.2015
 * Last modification on:    25.02.2016
 * Modified by:             A. Staub
 * 
 * Input:                   -
 * Output:                  -
***********************************************************************************************************************/
void cmd_CBIT(void)
{
    auto unsigned char uint8_Result = 0;    //local work byte
    
    //number of received characters OK?
    if((g_CmdChk.uint8_ParamPos == 2) || (g_CmdChk.uint8_ParamPos == 3))
    {
        //verify the limits if they are inside the tolerance
        uint8_Result += funct_CheckTol(g_CmdChk.uint32_TempPara[1],_BitMin,_BitMax);
        
        if(uint8_Result == 1)   //each parameter within the tolerance?
        {
            //set the corresponding bit / output by call the subroutine
            funct_OutputHandler(_ClrBit,(g_CmdChk.uint32_TempPara[1] & 0xFF));
                   
            //send back the needed informations
            uart2_sendbuffer('E');                  //first the letter E
            uart2_sendbuffer(13);                   //add the CR at the end
        }
        else
        {
            g_Param.uint8_ErrCode = _OutOfTolBit;       //set error code
            uart2_SendErrorCode(g_Param.uint8_ErrCode); //call subroutine
        }
    }
    else
    {
        g_Param.uint8_ErrCode = _NumbRecCharNotOK;  //set error code
        uart2_SendErrorCode(g_Param.uint8_ErrCode); //call subroutine
    }  
}   //end of cmd_CBIT


/**********************************************************************************************************************
 * Routine:                 cmd_GBIT

 * Description:
 * Verify the received parameters of this command, if all parameters are within the tolerance.
 * If all is correct, then send back the corresponding bit / output 
 * 
 * Creator:                 A. Staub
 * Date of creation:        28.11.2015
 * Last modification on:    -
 * Modified by:             - 
 * 
 * Input:                   -
 * Output:                  -
***********************************************************************************************************************/
void cmd_GBIT(void)
{
    auto unsigned char uint8_Result = 0;    //local work byte
    auto unsigned char uint8_value;         //local work byte
    
    if(g_CmdChk.uint8_ParamPos == 2)           //number of received characters OK?
    {
        //verify the limits if they are inside the tolerance
        uint8_Result += funct_CheckTol(g_CmdChk.uint32_TempPara[1],_BitMin,_BitMax);
        
        if(uint8_Result == 1)   //each parameter within the tolerance?
        {
            //get the corresponding bit / output by call the subroutine
            uint8_value = funct_IOhandler(_GetBit,_Output,(g_CmdChk.uint32_TempPara[1] & 0xFF));
                   
            //send back the needed informations
            uart2_sendbuffer('E');                  //first the letter E
            uart2_sendbuffer(',');                  //then the comma
            funct_IntToAscii(uint8_value,_Active);  //add the value
            uart2_sendbuffer(13);                   //add the CR at the end
        }
        else
        {
            g_Param.uint8_ErrCode = _OutOfTolBit;       //set error code
            uart2_SendErrorCode(g_Param.uint8_ErrCode); //call subroutine
        }
    }
    else
    {
        g_Param.uint8_ErrCode = _NumbRecCharNotOK;  //set error code
        uart2_SendErrorCode(g_Param.uint8_ErrCode); //call subroutine
    }  
}   //end of cmd_GBIT


/**********************************************************************************************************************
 * Routine:                 cmd_SOUT

 * Description:
 * Verify the received parameters of this command, if all parameters are within the tolerance.
 * If all is correct, then set the output port
 * 
 * Creator:                 A. Staub
 * Date of creation:        28.11.2015
 * Last modification on:    -
 * Modified by:             - 
 * 
 * Input:                   -
 * Output:                  -
***********************************************************************************************************************/
void cmd_SOUT(void)
{
    auto unsigned char uint8_Result = 0;    //local work byte
    
    if(g_CmdChk.uint8_ParamPos == 2)           //number of received characters OK?
    {
        //verify the limits if they are inside the tolerance
        uint8_Result += funct_CheckTol(g_CmdChk.uint32_TempPara[1],_ValPortMin,_ValPortMax);
        
        if(uint8_Result == 1)   //each parameter within the tolerance?
        {
            //set the corresponding bit / output by call the subroutine
            funct_OutputHandler(_SetPort,(g_CmdChk.uint32_TempPara[1] & 0xFF));
                   
            //send back the needed informations
            uart2_sendbuffer('E');                  //first the letter E
            uart2_sendbuffer(13);                   //add the CR at the end
        }
        else
        {
            g_Param.uint8_ErrCode = _OutOfTolValPort;   //set error code
            uart2_SendErrorCode(g_Param.uint8_ErrCode); //call subroutine
        }
    }
    else
    {
        g_Param.uint8_ErrCode = _NumbRecCharNotOK;  //set error code
        uart2_SendErrorCode(g_Param.uint8_ErrCode); //call subroutine
    }  
}   //end of cmd_SOUT


/**********************************************************************************************************************
 * Routine:                 cmd_GOUT

 * Description:
 * Verify the received parameters of this command, if all parameters are within the tolerance.
 * If all is correct, then send back the state of the output port 
 * 
 * Creator:                 A. Staub
 * Date of creation:        28.11.2015
 * Last modification on:    -
 * Modified by:             - 
 * 
 * Input:                   -
 * Output:                  -
***********************************************************************************************************************/
void cmd_GOUT(void)
{
    auto unsigned char uint8_value;         //local work byte
    
    if(g_CmdChk.uint8_ParamPos == 1)   //number of received characters OK?
    {
        //get the output port by call the subroutine
        uint8_value = funct_IOhandler(_GetPort,_Output,(g_CmdChk.uint32_TempPara[1] & 0xFF));
                   
        //send back the needed informations
        uart2_sendbuffer('E');                  //first the letter E
        uart2_sendbuffer(',');                  //then the comma
        funct_IntToAscii(uint8_value,_Active);  //add the value
        uart2_sendbuffer(13);                   //add the CR at the end      
    }
    else
    {
        g_Param.uint8_ErrCode = _NumbRecCharNotOK;  //set error code
        uart2_SendErrorCode(g_Param.uint8_ErrCode); //call subroutine
    } 
}   //end of cmd_GOUT


/**********************************************************************************************************************
 * Routine:                 cmd_GINP

 * Description:
 * Verify the received parameters of this command, if all parameters are within the tolerance.
 * If all is correct, then send back the state of the input port 
 * 
 * Creator:                 A. Staub
 * Date of creation:        28.11.2015
 * Last modification on:    -
 * Modified by:             - 
 * 
 * Input:                   -
 * Output:                  -
***********************************************************************************************************************/
void cmd_GINP(void)
{
    auto unsigned char uint8_value;         //local work byte
    
    if(g_CmdChk.uint8_ParamPos == 1)   //number of received characters OK?
    {
        //get the output port by call the subroutine
        uint8_value = funct_IOhandler(_GetPort,_Input,(g_CmdChk.uint32_TempPara[1] & 0xFF));
                   
        //send back the needed informations
        uart2_sendbuffer('E');                  //first the letter E
        uart2_sendbuffer(',');                  //then the comma
        funct_IntToAscii(uint8_value,_Active);  //add the value
        uart2_sendbuffer(13);                   //add the CR at the end      
    }
    else
    {
        g_Param.uint8_ErrCode = _NumbRecCharNotOK;  //set error code
        uart2_SendErrorCode(g_Param.uint8_ErrCode); //call subroutine
    }
}   //end of cmd_GINP


/**********************************************************************************************************************
 * Routine:                 cmd_SMCRSTP

 * Description:
 * Only allowed if the motor is not in run. First verify if the number of received parameters is inside the range.
 * If not, then send back the error code.
 * 
 * Second verify if the number of received parameters is 5, 9, 17, because to have a valide result, 
 * there must be always a current level for the microsteppint in 4th, 8th or 16th be. 
 * 5,9 or 17 because the command ID is counted as well. 
 * 
 * Third verify each received parameter with the toelrance until all parameters are verified. 
 * Update the number of received parameters with the number of verified parameters. 
 * 
 * Fourth verify if the result is exactly the number of parameters -1, to be sure tht each parameter is 
 * within the tolerance. -1 because we don't count the command ID with. 
 * 
 * At the end, first erase the acceleration array, store the new parameters inside and send back the OK.  
 * 
 * Creator:                 A. Staub
 * Date of creation:        29.11.2015
 * Last modification on:    -
 * Modified by:             - 
 * 
 * Input:                   -
 * Output:                  -
***********************************************************************************************************************/
void cmd_SMCRSTP(void)
{
    auto unsigned char uint8_Result = 0;    //local work byte for the result
    auto unsigned char uint8_WB1;           //local work byte for the loops
    auto unsigned char uint8_WB2;           //local work byte to store the results into the array
    
    if((g_CmdChk.uint8_ParamPos > 1) && (g_CmdChk.uint8_ParamPos < 18))   //number of received characters OK?
    {
        //number of received characters are a pair of 4th, 8th or 17th (+1 with command ID)
        if((g_CmdChk.uint8_ParamPos == 5) || (g_CmdChk.uint8_ParamPos == 9) || (g_CmdChk.uint8_ParamPos == 17))
        {
            if(g_CmdChk.uint8_GlobalLock == 1)  //global lock enabled?
            {
                g_Param.uint8_ErrCode = _MotorInRun;        //set error code
                uart2_SendErrorCode(g_Param.uint8_ErrCode);  //call subroutine
            }
            else
            {
                g_CmdChk.uint8_GlobalLock = 1;  //set global lock
                
                //ToDo: 
                //verify each received parameter with the tolerance (start with the 2nd paramter)
                //Until:
                //the local work byte has the same size as number of received characters
                uint8_WB1 = 1;
                do
                {
                    uint8_Result += funct_CheckTol(g_CmdChk.uint32_TempPara[uint8_WB1],_BipILevelMin,_BipILevelMax);
                    uint8_WB1++;    //increment with 1 to take the next parameter
                }
                while(uint8_WB1 < g_CmdChk.uint8_ParamPos);
                
                //each paramter within the tolerance (uint8_WB1 - 1, because the 1st parameter is the cmd ID)?
                if(uint8_Result == (uint8_WB1-1))
                {
                    //first erase the array
                    for(uint8_WB1=0; uint8_WB1<16; uint8_WB1++)
                    {
                        g_Param.uint16_BipILevel[uint8_WB1] = 0;
                    }
                    
                    //then store each parameter on the right place (start with the 2nd paramter)
                    uint8_WB1 = 1;
                    uint8_WB2 = 0;
                    do
                    {
                        g_Param.uint16_BipILevel[uint8_WB2] = g_CmdChk.uint32_TempPara[uint8_WB1] & 0xFFFF;
                        uint8_WB1++;    //increment with 1 to take the next paramter
                        uint8_WB2++;    //increment with 1 to store it on the right position of the array
                    }
                    while(uint8_WB1 < g_CmdChk.uint8_ParamPos);
                    
                    //send back the OK
                    uart2_sendbuffer('E');            //first the letter E
                    uart2_sendbuffer(13);             //then the CR at the end
                    
                    g_CmdChk.uint8_GlobalLock = 0;  //disable global lock
                }
                else
                {
                    g_Param.uint8_ErrCode = _OutOfTolSMCRSTP;   //set error code
                    uart2_SendErrorCode(g_Param.uint8_ErrCode); //call subroutine
                }
            }
        }
        else
        {
            g_Param.uint8_ErrCode = _NumbRecCharNotOK;  //set error code
            uart2_SendErrorCode(g_Param.uint8_ErrCode); //call subroutine
        }
    }  
    else
    {
        g_Param.uint8_ErrCode = _NumbRecCharNotOK;  //set error code
        uart2_SendErrorCode(g_Param.uint8_ErrCode); //call subroutine
    }
}   // end of cmd_SMCRSTP


/**********************************************************************************************************************
 * Routine:                 cmd_GMCRSTP

 * Description:
 * Only allowed if the motor is not in run. Send back the current level for the micro stepping.  
 * 
 * Creator:                 A. Staub
 * Date of creation:        29.11.2015
 * Last modification on:    -
 * Modified by:             - 
 * 
 * Input:                   -
 * Output:                  -
***********************************************************************************************************************/
void cmd_GMCRSTP(void)
{
    auto unsigned char uint8_WB;    //local work byte
    
    if(g_CmdChk.uint8_ParamPos == 1)   //number of received characters OK?
    {
        if(g_CmdChk.uint8_GlobalLock == 1)  //global lock enabled?
        {
            g_Param.uint8_ErrCode = _MotorInRun;        //set error code
            uart2_SendErrorCode(g_Param.uint8_ErrCode);  //call subroutine
        }
        else 
        {
            g_CmdChk.uint8_GlobalLock = 1;  //enable global lock
            
            uart2_sendbuffer('E');          //first the letter E
            
            //read out the array
            for(uint8_WB=0; uint8_WB<16; uint8_WB++)
            {
                if(g_Param.uint16_BipILevel[uint8_WB] == 0) //is the parameter = 0?
                {
                    uint8_WB = 16;  //leave the for loop
                }
                else    //otherwise send the parameter back
                {
                    uart2_sendbuffer(',');  //add the comma
                    //convert the parameter and store it into the send buffer
                    funct_IntToAscii(g_Param.uint16_BipILevel[uint8_WB],_Active);
                }
            }
            
            uart2_sendbuffer(13);           //add the <CR> at the end
            
            g_CmdChk.uint8_GlobalLock = 0;  //disable global lock
        }
    }
    else
    {
        g_Param.uint8_ErrCode = _NumbRecCharNotOK;  //set error code
        uart2_SendErrorCode(g_Param.uint8_ErrCode); //call subroutine
    }
}   //end of cmd_GMCRSTP


/**********************************************************************************************************************
 * Routine:                 cmd_SSWLIM

 * Description:
 * Defines the position limits for switch or hall-sensor switch. 
 * 
 * Creator:                 A. Staub
 * Date of creation:        30.11.2015
 * Last modification on:    25.03.2016
 * Modified by:             J. Rebetez
 * Modification:            As this command must still be defined, it will at the beginning not be used. If the 
 *                          define _NOT_USED_CMD is defined, SMMA will answer with X,255 code. Without define, code here
 *                          below will be executed.
 * 
 * Input:                   -
 * Output:                  -
***********************************************************************************************************************/
void cmd_SSWLIM(void)
{
#ifndef _NOT_USED_CMD
    auto unsigned char uint8_Result = 0;    //local work byte
    
    if(g_CmdChk.uint8_ParamPos == 5)   //number of received characters OK?
    {
        if(g_CmdChk.uint8_GlobalLock == 1)  //global lock active?
        {
            g_Param.uint8_ErrCode = _MotorInRun;        //set error code
            uart2_SendErrorCode(g_Param.uint8_ErrCode); //call subroutine
        }
        else
        {
            uint8_Result += funct_CheckTol(g_CmdChk.uint32_TempPara[1],_SW1min,_SW1max);
            uint8_Result += funct_CheckTol(g_CmdChk.uint32_TempPara[2],_SW1min,_SW1max);
            uint8_Result += funct_CheckTol(g_CmdChk.uint32_TempPara[3],_SW2min,_SW2max);
            uint8_Result += funct_CheckTol(g_CmdChk.uint32_TempPara[4],_SW2min,_SW2max);

            if(uint8_Result == 4)   //received parameters are within the tolerance?
            {
                g_Param.uint32_Sw1min = g_CmdChk.uint32_TempPara[1];
                g_Param.uint32_Sw1max = g_CmdChk.uint32_TempPara[2];
                g_Param.uint32_Sw2min = g_CmdChk.uint32_TempPara[3];
                g_Param.uint32_Sw2max = g_CmdChk.uint32_TempPara[4];

                //send back the needed informations
                uart2_sendbuffer('E');                  //first the letter E
                uart2_sendbuffer(13);                   //add the CR at the end
            }
            else
            {
                g_Param.uint8_ErrCode = _OutOfTolSSWLIM;  //set error code
                uart2_SendErrorCode(g_Param.uint8_ErrCode); //call subroutine
            }
        }
    }
    else
    {
        g_Param.uint8_ErrCode = _NumbRecCharNotOK;  //set error code
        uart2_SendErrorCode(g_Param.uint8_ErrCode); //call subroutine
    }
#endif
#ifdef _NOT_USED_CMD
    g_Param.uint8_ErrCode = _UnknownCmd;  //set error code
    uart2_SendErrorCode(g_Param.uint8_ErrCode); //call subroutine
#endif
}   //end of cmd_SSWLIM 

/**********************************************************************************************************************
 * Routine:                 cmd_GSWLIM

 * Description:
 * Get the position limits for switch or hall-sensor switch. 
 * 
 * Creator:                 J. Rebetez
 * Date of creation:        30.11.2015
 * Last modification on:    25.03.2016
 * Modified by:             J. Rebetez
 * Modification:            As this command must still be defined, it will at the beginning not be used. If the 
 *                          define _NOT_USED_CMD is defined, SMMA will answer with X,255 code. Without define, code here
 *                          below will be executed.
 * 
 * Input:                   -
 * Output:                  -
***********************************************************************************************************************/
void cmd_GSWLIM(void)
{    
#ifndef _NOT_USED_CMD
    if(g_CmdChk.uint8_ParamPos == 1)   //number of received characters OK?
    {
        uart2_sendbuffer('E');          //first the letter E
            
        uart2_sendbuffer(',');  //add the comma
        //convert the parameter and store it into the send buffer
        funct_IntToAscii(g_Param.uint32_Sw1min,_Active);
        uart2_sendbuffer(',');  //add the comma
        funct_IntToAscii(g_Param.uint32_Sw1max,_Active);
        uart2_sendbuffer(',');  //add the comma
        funct_IntToAscii(g_Param.uint32_Sw2min,_Active);
        uart2_sendbuffer(',');  //add the comma
        funct_IntToAscii(g_Param.uint32_Sw2max,_Active);
        uart2_sendbuffer(13);   //add the CR at the end
    } 
    else
    {
        g_Param.uint8_ErrCode = _NumbRecCharNotOK;  //set error code
        uart2_SendErrorCode(g_Param.uint8_ErrCode); //call subroutine
    }
#endif
#ifdef _NOT_USED_CMD
    g_Param.uint8_ErrCode = _UnknownCmd;  //set error code
    uart2_SendErrorCode(g_Param.uint8_ErrCode); //call subroutine
#endif
}   //end of cmd_GSWLIM 

/**********************************************************************************************************************
 * Routine:                 cmd_GSWPOS

 * Description:
 * Get the currently switch-position for switch or hall-sensor. 
 * 
 * Creator:                 J. Rebetez
 * Date of creation:        30.11.2015
 * Last modification on:    25.03.2016
 * Modified by:             J. Rebetez
 * Modification:            As this command must still be defined, it will at the beginning not be used. If the 
 *                          define _NOT_USED_CMD is defined, SMMA will answer with X,255 code. Without define, code here
 *                          below will be executed.
 * 
 * Input:                   -
 * Output:                  -
***********************************************************************************************************************/
void cmd_GSWPOS(void)
{    
#ifndef _NOT_USED_CMD
    auto unsigned char uint8_Result = 0;

    if(g_CmdChk.uint8_ParamPos == 1)   //number of received characters OK?
    {
        uint8_Result += funct_CheckTol(g_Param.uint32_Sw1pos,g_Param.uint32_Sw1min,g_Param.uint32_Sw1max);
        uint8_Result += funct_CheckTol(g_Param.uint32_Sw2pos,g_Param.uint32_Sw2min,g_Param.uint32_Sw2max);
        if(uint8_Result == 2)
        {
            uart2_sendbuffer('E');          //first the letter E
        }
        else
        {
            uart2_sendbuffer('X');  //add the comma
            g_Param.uint8_ErrCode = _OutOfTolGSWPOS;  //set error code
            uart2_sendbuffer(',');  //add the comma
            funct_IntToAscii(g_Param.uint8_ErrCode,_Active);
        }   
        uart2_sendbuffer(',');  //add the comma
        //convert the parameter and store it into the send buffer
        funct_IntToAscii(g_Param.uint32_Sw1pos,_Active);
        uart2_sendbuffer(',');  //add the comma
        funct_IntToAscii(g_Param.uint32_Sw2pos,_Active);
        uart2_sendbuffer(13);   //add the CR at the end
    } 
    else
    {
        g_Param.uint8_ErrCode = _NumbRecCharNotOK;  //set error code
        uart2_SendErrorCode(g_Param.uint8_ErrCode); //call subroutine
    }
#endif
#ifdef _NOT_USED_CMD
    g_Param.uint8_ErrCode = _UnknownCmd;  //set error code
    uart2_SendErrorCode(g_Param.uint8_ErrCode); //call subroutine
#endif
}   //end of cmd_GSWPOS 

/**********************************************************************************************************************
 * Routine:                 cmd_SPWMLIM

 * Description:
 * Set PWM limits for linear hall-detector
 * 
 * Creator:                 J. Rebetez
 * Date of creation:        30.11.2015
 * Last modification on:    25.03.2016
 * Modified by:             J. Rebetez
 * Modification:            As this command must still be defined, it will at the beginning not be used. If the 
 *                          define _NOT_USED_CMD is defined, SMMA will answer with X,255 code. Without define, code here
 *                          below will be executed.
 * 
 * Input:                   -
 * Output:                  -
***********************************************************************************************************************/
void cmd_SPWMLIM(void)
{
#ifndef _NOT_USED_CMD
    auto unsigned char uint8_Result = 0;    //local work byte
    
    if(g_CmdChk.uint8_ParamPos == 3)   //number of received characters OK?
    {
        if(g_CmdChk.uint8_GlobalLock == 1)  //global lock active?
        {
            g_Param.uint8_ErrCode = _MotorInRun;        //set error code
            uart2_SendErrorCode(g_Param.uint8_ErrCode); //call subroutine
        }
        else
        {
            uint8_Result += funct_CheckTol(g_CmdChk.uint32_TempPara[1],_SWpwmMin,_SWpwmMax);
            uint8_Result += funct_CheckTol(g_CmdChk.uint32_TempPara[2],_SWpwmMin,_SWpwmMax);

            if(uint8_Result == 2)   //received parameters are within the tolerance?
            {
                g_Param.uint32_SwPWMmin = g_CmdChk.uint32_TempPara[1];
                g_Param.uint32_SwPWMmax = g_CmdChk.uint32_TempPara[2];

                //send back the needed informations
                uart2_sendbuffer('E');                  //first the letter E
                uart2_sendbuffer(13);                   //add the CR at the end
            }
            else
            {
                g_Param.uint8_ErrCode = _OutOfTolSPWMLIM;  //set error code
                uart2_SendErrorCode(g_Param.uint8_ErrCode); //call subroutine
            }   
        }
    }
    else
    {
        g_Param.uint8_ErrCode = _NumbRecCharNotOK;  //set error code
        uart2_SendErrorCode(g_Param.uint8_ErrCode); //call subroutine
    }
#endif
#ifdef _NOT_USED_CMD
    g_Param.uint8_ErrCode = _UnknownCmd;  //set error code
    uart2_SendErrorCode(g_Param.uint8_ErrCode); //call subroutine
#endif
}   //end of cmd_SPWMLIM 

/**********************************************************************************************************************
 * Routine:                 cmd_GPWMLIM

 * Description:
 * Get PWM limits for linear hall-detector. 
 * 
 * Creator:                 J. Rebetez
 * Date of creation:        30.11.2015
 * Last modification on:    25.03.2016
 * Modified by:             J. Rebetez
 * Modification:            As this command must still be defined, it will at the beginning not be used. If the 
 *                          define _NOT_USED_CMD is defined, SMMA will answer with X,255 code. Without define, code here
 *                          below will be executed.
 * 
 * Input:                   -
 * Output:                  -
***********************************************************************************************************************/
void cmd_GPWMLIM(void)
{    
#ifndef _NOT_USED_CMD
    if(g_CmdChk.uint8_ParamPos == 1)   //number of received characters OK?
    {
        uart2_sendbuffer('E');          //first the letter E
            
        uart2_sendbuffer(',');  //add the comma
        //convert the parameter and store it into the send buffer
        funct_IntToAscii(g_Param.uint32_SwPWMmin,_Active);
        uart2_sendbuffer(',');  //add the comma
        funct_IntToAscii(g_Param.uint32_SwPWMmax,_Active);
        uart2_sendbuffer(13);   //add the CR at the end
    } 
    else
    {
        g_Param.uint8_ErrCode = _NumbRecCharNotOK;  //set error code
        uart2_SendErrorCode(g_Param.uint8_ErrCode); //call subroutine
    }
#endif
#ifdef _NOT_USED_CMD
    g_Param.uint8_ErrCode = _UnknownCmd;  //set error code
    uart2_SendErrorCode(g_Param.uint8_ErrCode); //call subroutine
#endif
}   //end of cmd_GPWMLIM

/**********************************************************************************************************************
 * Routine:                 cmd_GPWMPOS

 * Description:
 * Get position for linear hall-detector. 
 * 
 * Creator:                 J. Rebetez
 * Date of creation:        30.11.2015
* Last modification on:    25.03.2016
 * Modified by:             J. Rebetez
 * Modification:            As this command must still be defined, it will at the beginning not be used. If the 
 *                          define _NOT_USED_CMD is defined, SMMA will answer with X,255 code. Without define, code here
 *                          below will be executed.
 * 
 * Input:                   -
 * Output:                  -
***********************************************************************************************************************/
void cmd_GPWMPOS(void)
{  
#ifndef _NOT_USED_CMD
    auto unsigned char uint8_Result = 0;
    
    if(g_CmdChk.uint8_ParamPos == 1)   //number of received characters OK?
    {
        uint8_Result += funct_CheckTol(g_Param.uint32_SwPWMpos,g_Param.uint32_SwPWMmin,g_Param.uint32_SwPWMmax);
        uint8_Result += funct_CheckTol(g_Param.uint16_SwPWMval,470,530);
        if(uint8_Result == 2)
        {
            uart2_sendbuffer('E');          //first the letter E
        }
        else
        {
            uart2_sendbuffer('X');  //add the comma
            g_Param.uint8_ErrCode = _OutOfTolGPWMPOS;  //set error code
            uart2_sendbuffer(',');  //add the comma
            funct_IntToAscii(g_Param.uint8_ErrCode,_Active);
        }   
        uart2_sendbuffer(',');  //add the comma
        //convert the parameter and store it into the send buffer
        funct_IntToAscii(g_Param.uint32_SwPWMpos,_Active);
        uart2_sendbuffer(',');  //add the comma
        funct_IntToAscii(g_Param.uint16_SwPWMval,_Inactive);
        do{
            g_Funct.uint8_ArrAsciiPos--;
            uart2_sendbuffer(g_Funct.uint8_ArrAscii[g_Funct.uint8_ArrAsciiPos]);
            if(g_Funct.uint8_ArrAsciiPos == 1)
            {
                uart2_sendbuffer('.');
            }
        }while(g_Funct.uint8_ArrAsciiPos);
        uart2_sendbuffer(13);   //add the CR at the end
    } 
    else
    {
        g_Param.uint8_ErrCode = _NumbRecCharNotOK;  //set error code
        uart2_SendErrorCode(g_Param.uint8_ErrCode); //call subroutine
    }
#endif
#ifdef _NOT_USED_CMD
    g_Param.uint8_ErrCode = _UnknownCmd;  //set error code
    uart2_SendErrorCode(g_Param.uint8_ErrCode); //call subroutine
#endif
}   //end of cmd_GPWMLIM

/**********************************************************************************************************************
 * Routine:                 cmd_GPWMVAL

 * Description:
 * Get PWM in % duty-cycle for linear hall-detector. 
 * 
 * Creator:                 J. Rebetez
 * Date of creation:        30.11.2015
 * Last modification on:    -
 * Modified by:             - 
 * 
 * Input:                   -
 * Output:                  -
***********************************************************************************************************************/
extern unsigned int nbreTMR2Overflow;
S_IC dataIC;
unsigned long eventTime[3];
unsigned short eventMultiplicator[3];
void cmd_GPWMVAL(void)
{
    unsigned char nbreSamples = 1, actualSample, i;
    
    if(g_CmdChk.uint8_ParamPos == 1)   //number of received characters OK?
    {
        while(IC2CONbits.ICBNE)
            actualSample = IC2BUF;
        dataIC.timeoutMeas = 1;
        nbreTMR2Overflow = 0;
        TMR2 = 0;
        IFS0CLR = _IFS0_T2IF_MASK;
        SetTimer(_TIMER1, _ENABLE, 0, 1500);
        IC2CONbits.ON = 1;
        T2CONbits.ON = 1;
        for(actualSample = 0; actualSample < nbreSamples; actualSample++)
        {
            while(IC2CONbits.ON && dataIC.timeoutMeas);
            if(dataIC.timeoutMeas)
                FormatBufToRealValues(&dataIC, _MEAS_PWM);
        }
        SetTimer(_TIMER1, _DISABLE, 0, 1500);
        if(dataIC.timeoutMeas)
        {
            uart2_sendbuffer('E');          //first the letter E
            uart2_sendbuffer(',');  //add the comma
            //convert the parameter and store it into the send buffer
            funct_IntToAscii(dataIC.dutyCycle,_Inactive);
            do{
                g_Funct.uint8_ArrAsciiPos--;
                uart2_sendbuffer(g_Funct.uint8_ArrAscii[g_Funct.uint8_ArrAsciiPos]);
                if(g_Funct.uint8_ArrAsciiPos == 1)
                {
                    uart2_sendbuffer('.');
                }
            }while(g_Funct.uint8_ArrAsciiPos);
            uart2_sendbuffer(13);   //add the CR at the end
        }
        else
        {
            uart2_sendbuffer('X');  //add the comma
            g_Param.uint8_ErrCode = _OutOfTolGPWMVAL;  //set error code
            uart2_sendbuffer(',');  //add the comma
            funct_IntToAscii(g_Param.uint8_ErrCode,_Active);
        }   
    } 
    else
    {
        g_Param.uint8_ErrCode = _NumbRecCharNotOK;  //set error code
        uart2_SendErrorCode(g_Param.uint8_ErrCode); //call subroutine
    }
}   //end of cmd_GPWMVAL

/**********************************************************************************************************************
 * Routine:                 cmd_SSWTYP

 * Description:
 * Set type of switch (switch, hall-detector, linear hall-detector, encoder)
 * 
 * Creator:                 J. Rebetez
 * Date of creation:        30.11.2015
 * Last modification on:    -
 * Modified by:             - 
 * 
 * Input:                   -
 * Output:                  -
***********************************************************************************************************************/
void cmd_SSWTYP(void)
{
    if(g_CmdChk.uint8_ParamPos == 2)   //number of received characters OK?
    {
        if(g_CmdChk.uint8_GlobalLock == 1)  //global lock active?
        {
            g_Param.uint8_ErrCode = _MotorInRun;        //set error code
            uart2_SendErrorCode(g_Param.uint8_ErrCode); //call subroutine
        }
        else
        {
            if(funct_CheckTol(g_CmdChk.uint32_TempPara[1],_SWtypeMin,_SWtypeMax))   //received parameters are within the tolerance?
            {
                g_Param.uint8_SwType = g_CmdChk.uint32_TempPara[1];

                //send back the needed informations
                uart2_sendbuffer('E');                  //first the letter E
                uart2_sendbuffer(13);                   //add the CR at the end
            }
            else
            {
                g_Param.uint8_ErrCode = _OutOfTolSSWTYP;  //set error code
                uart2_SendErrorCode(g_Param.uint8_ErrCode); //call subroutine
            } 
        }
    }
    else
    {
        g_Param.uint8_ErrCode = _NumbRecCharNotOK;  //set error code
        uart2_SendErrorCode(g_Param.uint8_ErrCode); //call subroutine
    }
}   //end of cmd_SSWTYP

/**********************************************************************************************************************
 * Routine:                 cmd_GSWTYP

 * Description:
 * Get type of switch (switch, hall-detector, linear hall-detector, encoder)
 * 
 * Creator:                 J. Rebetez
 * Date of creation:        30.11.2015
 * Last modification on:    -
 * Modified by:             - 
 * 
 * Input:                   -
 * Output:                  -
***********************************************************************************************************************/
void cmd_GSWTYP(void)
{    
    if(g_CmdChk.uint8_ParamPos == 1)   //number of received characters OK?
    {
        uart2_sendbuffer('E');
        uart2_sendbuffer(',');  //add the comma
        funct_IntToAscii(g_Param.uint8_SwType,_Active);
        uart2_sendbuffer(13);
    } 
    else
    {
        g_Param.uint8_ErrCode = _NumbRecCharNotOK;  //set error code
        uart2_SendErrorCode(g_Param.uint8_ErrCode); //call subroutine
    }
}   //end of cmd_GSWTYP

/**********************************************************************************************************************
 * Routine:                 cmd_GFRQVAL

 * Description:
 * Get frequency for torque measurement
 * 
 * Creator:                 J. Rebetez
 * Date of creation:        30.11.2015
 * Last modification on:    -
 * Modified by:             - 
 * 
 * Input:                   -
 * Output:                  -
***********************************************************************************************************************/
void cmd_GFRQVAL(void)
{    
    if(g_CmdChk.uint8_ParamPos == 1)   //number of received characters OK?
    {
        while(IC1CONbits.ICBNE)
        {
            volatile unsigned short temp = IC1BUF;
        }
        dataIC.timeoutMeas = 1;
        nbreTMR2Overflow = 0;
        TMR2 = 0;
        SetTimer(_TIMER1, _ENABLE, 0, 2500);
        IFS0CLR = _IFS0_T2IF_MASK;
        T2CONbits.ON = 1;
        IC1CONbits.ON = 1;
        while(IC1CONbits.ON && dataIC.timeoutMeas);
        SetTimer(_TIMER1, _DISABLE, 0, 1500);
        if(dataIC.timeoutMeas)
        {
            FormatBufToRealValues(&dataIC, _MEAS_FREQ);
            uart2_sendbuffer('E');
            uart2_sendbuffer(',');  //add the comma
            funct_IntToAscii(dataIC.frequency, _Active);
            uart2_sendbuffer(13);
        }
        else
        {
            g_Param.uint8_ErrCode = _OutOfTolGPWMVAL;  //set error code
            uart2_SendErrorCode(g_Param.uint8_ErrCode); //call subroutine
        }
    } 
    else
    {
        g_Param.uint8_ErrCode = _NumbRecCharNotOK;  //set error code
        uart2_SendErrorCode(g_Param.uint8_ErrCode); //call subroutine
    }
}   //end of cmd_GFRQVAL


/**********************************************************************************************************************
 * Routine:                 cmd_GINPB

 * Description:
 * Verify the received parameters of this command, if all parameters are within the tolerance.
 * If all is correct, then send back the state of the input bit. 
 * 
 * Creator:                 A. Staub
 * Date of creation:        29.11.2015
 * Last modification on:    -
 * Modified by:             - 
 * 
 * Input:                   -
 * Output:                  -
***********************************************************************************************************************/
void cmd_GINPB(void)
{
    auto unsigned char uint8_Result = 0;    //local work byte
    auto unsigned char uint8_value;         //local work byte
    
    if(g_CmdChk.uint8_ParamPos == 2)           //number of received characters OK?
    {
        //verify the limits if they are inside the tolerance
        uint8_Result += funct_CheckTol(g_CmdChk.uint32_TempPara[1],_BitMin,_BitMax);
        
        if(uint8_Result == 1)   //each parameter within the tolerance?
        {
            //get the corresponding bit / output by call the subroutine
            uint8_value = funct_IOhandler(_GetBit,_Input,(g_CmdChk.uint32_TempPara[1] & 0xFF));
                   
            //send back the needed informations
            uart2_sendbuffer('E');                  //first the letter E
            uart2_sendbuffer(',');                  //then the comma
            funct_IntToAscii(uint8_value,_Active);  //add the value
            uart2_sendbuffer(13);                   //add the CR at the end
        }
        else
        {
            g_Param.uint8_ErrCode = _OutOfTolBit;       //set error code
            uart2_SendErrorCode(g_Param.uint8_ErrCode); //call subroutine
        }
    }
    else
    {
        g_Param.uint8_ErrCode = _NumbRecCharNotOK;  //set error code
        uart2_SendErrorCode(g_Param.uint8_ErrCode); //call subroutine
    } 
}   //end of cmd_GINPB


/**********************************************************************************************************************
 * Routine:                 cmd_SROM

 * Description:
 * Verify the received parameters of this command, if all parameters are within the tolerance.
 * If all is correct, then store the byte into the EEPROM.  
 * 
 * Creator:                 A. Staub
 * Date of creation:        12.12.2015
 * Last modification on:    -
 * Modified by:             - 
 * 
 * Input:                   -
 * Output:                  -
***********************************************************************************************************************/
void cmd_SROM(void)
{
    auto unsigned char uint8_Result = 0;    //local work byte
    auto unsigned char uint8_AdrH;          //local work byte for address high
    auto unsigned char uint8_AdrL;          //local work byte for address low
    
    if(g_CmdChk.uint8_ParamPos == 3)           //number of received characters OK?
    {
        if(g_CmdChk.uint8_GlobalLock == 1)  //global lock active?
        {
            g_Param.uint8_ErrCode = _MotorInRun;        //set error code
            uart2_SendErrorCode(g_Param.uint8_ErrCode); //call subroutine
        }
        else
        {
            //verify the limits if they are inside the tolerance
            uint8_Result += funct_CheckTol(g_CmdChk.uint32_TempPara[1],_RomAddrMin,_RomAddrMax);
            uint8_Result += funct_CheckTol(g_CmdChk.uint32_TempPara[2],_RomValMin,_RomValMax);

            if(uint8_Result == 2)       //verify the result
            {
                uint8_AdrH = g_CmdChk.uint32_TempPara[1];   //store address high and low into AdrH
                uint8_AdrH = uint8_AdrH >> 8;               //shift AdrH 8 bits to right
                uint8_AdrH &= 0x000000FF;                   //clear all bits after the first 8

                uint8_AdrL = g_CmdChk.uint32_TempPara[1];   //store address high and low into AdrL
                uint8_AdrL &= 0x000000FF;                   //clear all bits after the first 8

                ROM24LC256_WrByte(uint8_AdrH,uint8_AdrL,g_CmdChk.uint32_TempPara[2]);

                if(g_i2c1.uint8_ErrACK)     //error acknowledge = no answer from the slave
                {
                    uart2_sendbuffer('X');      //first the letter X
                    uart2_sendbuffer(',');      //then the comma
                    g_Param.uint8_ErrCode = _AckSROMRROM;   //set error code
                    funct_IntToAscii(g_Param.uint8_ErrCode,_Active);
                }
                else if(g_i2c1.uint8_BusColl)   //error bus collision 
                {
                    uart2_sendbuffer('X');      //first the letter X
                    uart2_sendbuffer(',');      //then the comma
                    g_Param.uint8_ErrCode = _BusCollSROMRROM;   //set error code
                    funct_IntToAscii(g_Param.uint8_ErrCode,_Active);
                }
                else
                {
                    uart2_sendbuffer('E');      //first the letter E
                }
                uart2_sendbuffer(13);                   //add the CR at the end        
            }
            else
            {
                g_Param.uint8_ErrCode = _OutOfTolSROMRROM;  //set error code
                uart2_SendErrorCode(g_Param.uint8_ErrCode); //call subroutine
            }
        }
    }
    else
    {
        g_Param.uint8_ErrCode = _NumbRecCharNotOK;  //set error code
        uart2_SendErrorCode(g_Param.uint8_ErrCode); //call subroutine
    } 
}   //end of cmd_SROM


/**********************************************************************************************************************
 * Routine:                 cmd_RROM

 * Description:
 * Verify the received parameters of this command, if all parameters are within the tolerance.
 * If all is correct, then read out the temperature and send it back. 
 * 
 * Creator:                 A. Staub
 * Date of creation:        14.12.2015
 * Last modification on:    -
 * Modified by:             - 
 * 
 * Input:                   -
 * Output:                  -
***********************************************************************************************************************/
void cmd_RROM(void)
{
    auto unsigned char uint8_Result = 0;    //local work byte
    auto unsigned char uint8_AdrH;          //local work byte for address high
    auto unsigned char uint8_AdrL;          //local work byte for address low
    
    if(g_CmdChk.uint8_ParamPos == 2)        //number of received characters OK?
    {
        if(g_CmdChk.uint8_GlobalLock == 1)  //global lock active?
        {
            g_Param.uint8_ErrCode = _MotorInRun;        //set error code
            uart2_SendErrorCode(g_Param.uint8_ErrCode); //call subroutine
        }
        else
        {
            //verify the limits if they are inside the tolerance
            uint8_Result += funct_CheckTol(g_CmdChk.uint32_TempPara[1],_RomAddrMin,_RomAddrMax);

            if(uint8_Result == 1)       //verify the result
            {
                uint8_AdrH = g_CmdChk.uint32_TempPara[1];   //store address high and low into AdrH
                uint8_AdrH = uint8_AdrH >> 8;               //shift AdrH 8 bits to right
                uint8_AdrH &= 0x000000FF;                   //clear all bits after the first 8

                uint8_AdrL = g_CmdChk.uint32_TempPara[1];   //store address high and low into AdrL
                uint8_AdrL &= 0x000000FF;                   //clear all bits after the first 8

                //read out one byte and store it into uin8_Result
                uint8_Result = ROM24LC256_RdByte(uint8_AdrH,uint8_AdrL,1);  

                if(g_i2c1.uint8_ErrACK)     //error acknowledge = no answer from the slave
                {
                    uart2_sendbuffer('X');      //first the letter X
                    uart2_sendbuffer(',');      //then the comma
                    g_Param.uint8_ErrCode = _AckSROMRROM;   //set error code
                    funct_IntToAscii(g_Param.uint8_ErrCode,_Active);
                }
                else if(g_i2c1.uint8_BusColl)   //error bus collision 
                {
                    uart2_sendbuffer('X');      //first the letter X
                    uart2_sendbuffer(',');      //then the comma
                    g_Param.uint8_ErrCode = _BusCollSROMRROM;   //set error code
                    funct_IntToAscii(g_Param.uint8_ErrCode,_Active);
                }
                else
                {
                    uart2_sendbuffer('E');      //first the letter E
                    uart2_sendbuffer(',');      //then the comma
                    funct_IntToAscii(uint8_Result,_Active);
                }
                uart2_sendbuffer(13);           //add the CR at the end
            }
            else
            {
                g_Param.uint8_ErrCode = _OutOfTolSROMRROM;  //set error code
                uart2_SendErrorCode(g_Param.uint8_ErrCode); //call subroutine
            }
        }
    }
    else
    {
        g_Param.uint8_ErrCode = _NumbRecCharNotOK;  //set error code
        uart2_SendErrorCode(g_Param.uint8_ErrCode); //call subroutine
    } 
}   //end of cmd_RROM


/**********************************************************************************************************************
 * Routine:                 cmd_GTEMP

 * Description:
 * Verify the received parameters of this command, if all parameters are within the tolerance.
 * If all is correct, then read out the byte from the EEPROM and send it back. 
 * 
 * Creator:                 A. Staub
 * Date of creation:        14.12.2015
 * Last modification on:    -
 * Modified by:             - 
 * 
 * Input:                   -
 * Output:                  -
***********************************************************************************************************************/
void cmd_GTEMP(void)
{
    auto signed short int sint16_WB;        //local work int
    
    if(g_CmdChk.uint8_ParamPos == 1)        //number of received characters OK?
    {
        RV30xx_TempMeas();      //read out the temperature
        
        if(g_Param.sint16_Temp == 255)  //signalization of an error active
        {
            uart2_sendbuffer('X');      //first the letter X
            uart2_sendbuffer(',');      //then the comma
            g_Param.uint8_ErrCode = _BusCollRTC;  //set error code
            funct_IntToAscii(g_Param.uint8_ErrCode,_Active);
            uart2_sendbuffer(13);       //then the CR
        }
        else
        {
            uart2_sendbuffer('E');      //first the letter X
            uart2_sendbuffer(',');      //then the comma
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
            uart2_sendbuffer(13);       //add the CR at the end
        }
    }
    else
    {
        g_Param.uint8_ErrCode = _NumbRecCharNotOK;  //set error code
        uart2_SendErrorCode(g_Param.uint8_ErrCode); //call subroutine
    } 
}   //end of cmd_GTEMP


/**********************************************************************************************************************
 * Routine:                 cmd_SSEC

 * Description:
 * Verify the received parameters of this command, if all parameters are within the tolerance.
 * If all is correct, then set the new seconds value
 * 
 * Creator:                 A. Staub
 * Date of creation:        14.12.2015
 * Last modification on:    -
 * Modified by:             - 
 * 
 * Input:                   -
 * Output:                  -
***********************************************************************************************************************/
void cmd_SSEC(void)
{
    auto unsigned char uint8_Result = 0;    //local work byte
    auto unsigned char uint8_WB;            //local work byte
    
    if(g_CmdChk.uint8_ParamPos == 2)        //number of received characters OK?
    {
        //verify the limits if they are inside the tolerance
        uint8_Result += funct_CheckTol(g_CmdChk.uint32_TempPara[1],_SecMin,_SecMax);
        
        if(uint8_Result == 1)       //verify the result
        {
            uint8_WB = g_CmdChk.uint32_TempPara[1];                 //store decimal value
            uint8_WB = (((uint8_WB / 10) << 4) | (uint8_WB % 10));  //convert to BCD
            
            RV30xx_SetGetSec(_Set,uint8_WB);    //call subroutine
            
            if(g_i2c1.uint8_BusColl || g_i2c1.uint8_ErrACK)
            {
                uart2_sendbuffer('X');      //first the letter X
                uart2_sendbuffer(',');      //then the comma
                g_Param.uint8_ErrCode = _BusCollRTC;  //set error code
                funct_IntToAscii(g_Param.uint8_ErrCode,_Active);
                uart2_sendbuffer(13);       //then the CR
            }
            else
            {
                uart2_sendbuffer('E');      //first the letter X
                uart2_sendbuffer(13);       //then the CR
            }
        }
        else
        {
            g_Param.uint8_ErrCode = _OutOfTolSSEC;      //set error code
            uart2_SendErrorCode(g_Param.uint8_ErrCode); //call subroutine
        }
    }
    else
    {
        g_Param.uint8_ErrCode = _NumbRecCharNotOK;  //set error code
        uart2_SendErrorCode(g_Param.uint8_ErrCode); //call subroutine
    } 
}   //end of cmd_SSEC


/**********************************************************************************************************************
 * Routine:                 cmd_GSEC

 * Description:
 * Verify the received parameters of this command, if all parameters are within the tolerance.
 * If all is correct, then read out the seconds value
 * 
 * Creator:                 A. Staub
 * Date of creation:        14.12.2015
 * Last modification on:    -
 * Modified by:             - 
 * 
 * Input:                   -
 * Output:                  -
***********************************************************************************************************************/
void cmd_GSEC(void)
{
    if(g_CmdChk.uint8_ParamPos == 1)        //number of received characters OK?
    {
        RV30xx_SetGetSec(_Get,0);           //read out the seconds
        
        if(g_i2c1.uint8_BusColl || g_i2c1.uint8_ErrACK)
        {
            uart2_sendbuffer('X');      //first the letter X
            uart2_sendbuffer(',');      //then the comma
            g_Param.uint8_ErrCode = _BusCollRTC;  //set error code
            funct_IntToAscii(g_Param.uint8_ErrCode,_Active);
            uart2_sendbuffer(13);       //then the CR
        }
        else
        {
            uart2_sendbuffer('E');      //first the letter X
            uart2_sendbuffer(',');      //then the comma
            funct_IntToAscii(g_Param.uint8_Sec,_Active);
            uart2_sendbuffer(13);       //then the CR
        }
    }
    else
    {
        g_Param.uint8_ErrCode = _NumbRecCharNotOK;  //set error code
        uart2_SendErrorCode(g_Param.uint8_ErrCode); //call subroutine
    } 
}   //end of cmd_GSEC


/**********************************************************************************************************************
 * Routine:                 cmd_SMIN

 * Description:
 * Verify the received parameters of this command, if all parameters are within the tolerance.
 * If all is correct, then set the new minutes value.
 * 
 * Creator:                 A. Staub
 * Date of creation:        16.12.2015
 * Last modification on:    -
 * Modified by:             - 
 * 
 * Input:                   -
 * Output:                  -
***********************************************************************************************************************/
void cmd_SMIN(void)
{
    auto unsigned char uint8_Result = 0;    //local work byte
    auto unsigned char uint8_WB;            //local work byte
    
    if(g_CmdChk.uint8_ParamPos == 2)        //number of received characters OK?
    {
        //verify the limits if they are inside the tolerance
        uint8_Result += funct_CheckTol(g_CmdChk.uint32_TempPara[1],_MinMin,_MinMax);
        
        if(uint8_Result == 1)       //verify the result
        {
            uint8_WB = g_CmdChk.uint32_TempPara[1];                 //store decimal value
            uint8_WB = (((uint8_WB / 10) << 4) | (uint8_WB % 10));  //convert to BCD
            
            RV30xx_SetGetMin(_Set,uint8_WB);    //call subroutine
            
            if(g_i2c1.uint8_BusColl || g_i2c1.uint8_ErrACK)
            {
                uart2_sendbuffer('X');      //first the letter X
                uart2_sendbuffer(',');      //then the comma
                g_Param.uint8_ErrCode = _BusCollRTC;  //set error code
                funct_IntToAscii(g_Param.uint8_ErrCode,_Active);
                uart2_sendbuffer(13);       //then the CR
            }
            else
            {
                uart2_sendbuffer('E');      //first the letter X
                uart2_sendbuffer(13);       //then the CR
            }
        }
        else
        {
            g_Param.uint8_ErrCode = _OutOfTolSMIN;      //set error code
            uart2_SendErrorCode(g_Param.uint8_ErrCode); //call subroutine
        }
    }
    else
    {
        g_Param.uint8_ErrCode = _NumbRecCharNotOK;  //set error code
        uart2_SendErrorCode(g_Param.uint8_ErrCode); //call subroutine
    } 
}   //end of cmd_SMIN


/**********************************************************************************************************************
 * Routine:                 cmd_GMIN

 * Description:
 * Verify the received parameters of this command, if all parameters are within the tolerance.
 * If all is correct, then read out the minutes value.
 * 
 * Creator:                 A. Staub
 * Date of creation:        16.12.2015
 * Last modification on:    -
 * Modified by:             - 
 * 
 * Input:                   -
 * Output:                  -
***********************************************************************************************************************/
void cmd_GMIN(void)
{
    if(g_CmdChk.uint8_ParamPos == 1)        //number of received characters OK?
    {
        RV30xx_SetGetMin(_Get,0);           //read out the minutes
        
        if(g_i2c1.uint8_BusColl || g_i2c1.uint8_ErrACK)
        {
            uart2_sendbuffer('X');      //first the letter X
            uart2_sendbuffer(',');      //then the comma
            g_Param.uint8_ErrCode = _BusCollRTC;  //set error code
            funct_IntToAscii(g_Param.uint8_ErrCode,_Active);
            uart2_sendbuffer(13);       //then the CR
        }
        else
        {
            uart2_sendbuffer('E');      //first the letter X
            uart2_sendbuffer(',');      //then the comma
            funct_IntToAscii(g_Param.uint8_Min,_Active);
            uart2_sendbuffer(13);       //then the CR
        }
    }
    else
    {
        g_Param.uint8_ErrCode = _NumbRecCharNotOK;  //set error code
        uart2_SendErrorCode(g_Param.uint8_ErrCode); //call subroutine
    } 
}   //end of cmd_GSEC


/**********************************************************************************************************************
 * Routine:                 cmd_SHRS

 * Description:
 * Verify the received parameters of this command, if all parameters are within the tolerance.
 * If all is correct, then set the new hours value.
 * 
 * Creator:                 A. Staub
 * Date of creation:        16.12.2015
 * Last modification on:    -
 * Modified by:             - 
 * 
 * Input:                   -
 * Output:                  -
***********************************************************************************************************************/
void cmd_SHRS(void)
{
    auto unsigned char uint8_Result = 0;    //local work byte
    auto unsigned char uint8_WB;            //local work byte
    
    if(g_CmdChk.uint8_ParamPos == 2)        //number of received characters OK?
    {
        //verify the limits if they are inside the tolerance
        uint8_Result += funct_CheckTol(g_CmdChk.uint32_TempPara[1],_HrsMin,_HrsMax);
        
        if(uint8_Result == 1)       //verify the result
        {
            uint8_WB = g_CmdChk.uint32_TempPara[1];                 //store decimal value
            uint8_WB = (((uint8_WB / 10) << 4) | (uint8_WB % 10));  //convert to BCD
            uint8_WB &= 0x1F;                                       //clear bit 7...5 to use 24h system
            
            RV30xx_SetGetHrs(_Set,uint8_WB);    //call subroutine
            
            if(g_i2c1.uint8_BusColl || g_i2c1.uint8_ErrACK)
            {
                uart2_sendbuffer('X');      //first the letter X
                uart2_sendbuffer(',');      //then the comma
                g_Param.uint8_ErrCode = _BusCollRTC;  //set error code
                funct_IntToAscii(g_Param.uint8_ErrCode,_Active);
                uart2_sendbuffer(13);       //then the CR
            }
            else
            {
                uart2_sendbuffer('E');      //first the letter X
                uart2_sendbuffer(13);       //then the CR
            }
        }
        else
        {
            g_Param.uint8_ErrCode = _OutOfTolSHRS;      //set error code
            uart2_SendErrorCode(g_Param.uint8_ErrCode); //call subroutine
        }
    }
    else
    {
        g_Param.uint8_ErrCode = _NumbRecCharNotOK;  //set error code
        uart2_SendErrorCode(g_Param.uint8_ErrCode); //call subroutine
    } 
}   //end of cmd_SHRS


/**********************************************************************************************************************
 * Routine:                 cmd_GHRS

 * Description:
 * Verify the received parameters of this command, if all parameters are within the tolerance.
 * If all is correct, then read out the hours value.
 * 
 * Creator:                 A. Staub
 * Date of creation:        16.12.2015
 * Last modification on:    -
 * Modified by:             - 
 * 
 * Input:                   -
 * Output:                  -
***********************************************************************************************************************/
void cmd_GHRS(void)
{
    if(g_CmdChk.uint8_ParamPos == 1)        //number of received characters OK?
    {
        RV30xx_SetGetHrs(_Get,0);           //read out the hours
        
        if(g_i2c1.uint8_BusColl || g_i2c1.uint8_ErrACK)
        {
            uart2_sendbuffer('X');      //first the letter X
            uart2_sendbuffer(',');      //then the comma
            g_Param.uint8_ErrCode = _BusCollRTC;  //set error code
            funct_IntToAscii(g_Param.uint8_ErrCode,_Active);
            uart2_sendbuffer(13);       //then the CR
        }
        else
        {
            uart2_sendbuffer('E');      //first the letter X
            uart2_sendbuffer(',');      //then the comma
            funct_IntToAscii(g_Param.uint8_Hrs,_Active);
            uart2_sendbuffer(13);       //then the CR
        }
    }
    else
    {
        g_Param.uint8_ErrCode = _NumbRecCharNotOK;  //set error code
        uart2_SendErrorCode(g_Param.uint8_ErrCode); //call subroutine
    } 
}   //end of cmd_GHRS


/**********************************************************************************************************************
 * Routine:                 cmd_SDAY

 * Description:
 * Verify the received parameters of this command, if all parameters are within the tolerance.
 * If all is correct, then set the new day value.
 * 
 * Creator:                 A. Staub
 * Date of creation:        16.12.2015
 * Last modification on:    -
 * Modified by:             - 
 * 
 * Input:                   -
 * Output:                  -
***********************************************************************************************************************/
void cmd_SDAY(void)
{
    auto unsigned char uint8_Result = 0;    //local work byte
    auto unsigned char uint8_WB;            //local work byte
    
    if(g_CmdChk.uint8_ParamPos == 2)        //number of received characters OK?
    {
        //verify the limits if they are inside the tolerance
        uint8_Result += funct_CheckTol(g_CmdChk.uint32_TempPara[1],_DayMin,_DayMax);
        
        if(uint8_Result == 1)       //verify the result
        {
            uint8_WB = g_CmdChk.uint32_TempPara[1];                 //store decimal value
            uint8_WB = (((uint8_WB / 10) << 4) | (uint8_WB % 10));  //convert to BCD
            
            RV30xx_SetGetDay(_Set,uint8_WB);    //call subroutine
            
            if(g_i2c1.uint8_BusColl || g_i2c1.uint8_ErrACK)
            {
                uart2_sendbuffer('X');      //first the letter X
                uart2_sendbuffer(',');      //then the comma
                g_Param.uint8_ErrCode = _BusCollRTC;  //set error code
                funct_IntToAscii(g_Param.uint8_ErrCode,_Active);
                uart2_sendbuffer(13);       //then the CR
            }
            else
            {
                uart2_sendbuffer('E');      //first the letter X
                uart2_sendbuffer(13);       //then the CR
            }
        }
        else
        {
            g_Param.uint8_ErrCode = _OutOfTolSDAY;      //set error code
            uart2_SendErrorCode(g_Param.uint8_ErrCode); //call subroutine
        }
    }
    else
    {
        g_Param.uint8_ErrCode = _NumbRecCharNotOK;  //set error code
        uart2_SendErrorCode(g_Param.uint8_ErrCode); //call subroutine
    } 
}   //end of cmd_SDAY


/**********************************************************************************************************************
 * Routine:                 cmd_GDAY

 * Description:
 * Verify the received parameters of this command, if all parameters are within the tolerance.
 * If all is correct, then read out the day value.
 * 
 * Creator:                 A. Staub
 * Date of creation:        16.12.2015
 * Last modification on:    -
 * Modified by:             - 
 * 
 * Input:                   -
 * Output:                  -
***********************************************************************************************************************/
void cmd_GDAY(void)
{
    if(g_CmdChk.uint8_ParamPos == 1)        //number of received characters OK?
    {
        RV30xx_SetGetDay(_Get,0);           //read out the hours
        
        if(g_i2c1.uint8_BusColl || g_i2c1.uint8_ErrACK)
        {
            uart2_sendbuffer('X');      //first the letter X
            uart2_sendbuffer(',');      //then the comma
            g_Param.uint8_ErrCode = _BusCollRTC;  //set error code
            funct_IntToAscii(g_Param.uint8_ErrCode,_Active);
            uart2_sendbuffer(13);       //then the CR
        }
        else
        {
            uart2_sendbuffer('E');      //first the letter X
            uart2_sendbuffer(',');      //then the comma
            funct_IntToAscii(g_Param.uint8_Day,_Active);
            uart2_sendbuffer(13);       //then the CR
        }
    }
    else
    {
        g_Param.uint8_ErrCode = _NumbRecCharNotOK;  //set error code
        uart2_SendErrorCode(g_Param.uint8_ErrCode); //call subroutine
    } 
}   //end of cmd_GDAY


/**********************************************************************************************************************
 * Routine:                 cmd_SWDAY

 * Description:
 * Verify the received parameters of this command, if all parameters are within the tolerance.
 * If all is correct, then set the new weekday value.
 * 
 * Creator:                 A. Staub
 * Date of creation:        16.12.2015
 * Last modification on:    -
 * Modified by:             - 
 * 
 * Input:                   -
 * Output:                  -
***********************************************************************************************************************/
void cmd_SWDAY(void)
{
    auto unsigned char uint8_Result = 0;    //local work byte
    auto unsigned short int uint16_WI;      //local work integer
    
    if(g_CmdChk.uint8_ParamPos == 2)        //number of received characters OK?
    {
        //verify the limits if they are inside the tolerance
        uint8_Result += funct_CheckTol(g_CmdChk.uint32_TempPara[1],_WdayMon,_WdayMon);
        uint8_Result += funct_CheckTol(g_CmdChk.uint32_TempPara[1],_WdayTue,_WdayTue);
        uint8_Result += funct_CheckTol(g_CmdChk.uint32_TempPara[1],_WdayWed,_WdayWed);
        uint8_Result += funct_CheckTol(g_CmdChk.uint32_TempPara[1],_WdayThu,_WdayThu);
        uint8_Result += funct_CheckTol(g_CmdChk.uint32_TempPara[1],_WdayFri,_WdayFri);
        uint8_Result += funct_CheckTol(g_CmdChk.uint32_TempPara[1],_WdaySat,_WdaySat);
        uint8_Result += funct_CheckTol(g_CmdChk.uint32_TempPara[1],_WdaySun,_WdaySun);
        
        if(uint8_Result == 1)       //verify the result
        {
            uint16_WI = g_CmdChk.uint32_TempPara[1];                 //store decimal value
            
            switch(uint16_WI)
            {
                case (_WdayMon):
                    RV30xx_SetGetWday(_Set,2);   //call subroutine
                    break;
                    
                case (_WdayTue):
                    RV30xx_SetGetWday(_Set,3);   //call subroutine
                    break;
                    
                case (_WdayWed):
                    RV30xx_SetGetWday(_Set,4);   //call subroutine
                    break;
                    
                case (_WdayThu):
                    RV30xx_SetGetWday(_Set,5);   //call subroutine
                    break;
                    
                case (_WdayFri):
                    RV30xx_SetGetWday(_Set,6);   //call subroutine
                    break;
                    
                case (_WdaySat):
                    RV30xx_SetGetWday(_Set,7);   //call subroutine
                    break;
                    
                case (_WdaySun):
                    RV30xx_SetGetWday(_Set,1);   //call subroutine
                    break;
                    
                default:
                    //do nothing, this case will never called, because the value is verified 
                    //already at the beginning
                    break;
            }
            
            if(g_i2c1.uint8_BusColl || g_i2c1.uint8_ErrACK)
            {
                uart2_sendbuffer('X');      //first the letter X
                uart2_sendbuffer(',');      //then the comma
                g_Param.uint8_ErrCode = _BusCollRTC;  //set error code
                funct_IntToAscii(g_Param.uint8_ErrCode,_Active);
                uart2_sendbuffer(13);       //then the CR
            }
            else
            {
                uart2_sendbuffer('E');      //first the letter X
                uart2_sendbuffer(13);       //then the CR
            }
        }
        else
        {
            g_Param.uint8_ErrCode = _OutOfTolSWDAY;     //set error code
            uart2_SendErrorCode(g_Param.uint8_ErrCode); //call subroutine
        }
    }
    else
    {
        g_Param.uint8_ErrCode = _NumbRecCharNotOK;  //set error code
        uart2_SendErrorCode(g_Param.uint8_ErrCode); //call subroutine
    } 
}   //end of cmd_SWDAY


/**********************************************************************************************************************
 * Routine:                 cmd_GWDAY

 * Description:
 * Verify the received parameters of this command, if all parameters are within the tolerance.
 * If all is correct, then read out the weekday value.
 * 
 * Creator:                 A. Staub
 * Date of creation:        16.12.2015
 * Last modification on:    -
 * Modified by:             - 
 * 
 * Input:                   -
 * Output:                  -
***********************************************************************************************************************/
void cmd_GWDAY(void)
{
    if(g_CmdChk.uint8_ParamPos == 1)        //number of received characters OK?
    {
        RV30xx_SetGetWday(_Get,0);          //read out the hours
        
        if(g_i2c1.uint8_BusColl || g_i2c1.uint8_ErrACK)
        {
            uart2_sendbuffer('X');      //first the letter X
            uart2_sendbuffer(',');      //then the comma
            g_Param.uint8_ErrCode = _BusCollRTC;  //set error code
            funct_IntToAscii(g_Param.uint8_ErrCode,_Active);
            uart2_sendbuffer(13);       //then the CR
        }
        else
        {
            uart2_sendbuffer('E');      //first the letter X
            uart2_sendbuffer(',');      //then the comma         
            funct_StoreWdayIntoRSbuffer();  //call subroutine
            uart2_sendbuffer(13);       //then the CR
        }
    }
    else
    {
        g_Param.uint8_ErrCode = _NumbRecCharNotOK;  //set error code
        uart2_SendErrorCode(g_Param.uint8_ErrCode); //call subroutine
    } 
}   //end of cmd_GWDAY


/**********************************************************************************************************************
 * Routine:                 cmd_SMONTH

 * Description:
 * Verify the received parameters of this command, if all parameters are within the tolerance.
 * If all is correct, then set the new month value.
 * 
 * Creator:                 A. Staub
 * Date of creation:        16.12.2015
 * Last modification on:    -
 * Modified by:             - 
 * 
 * Input:                   -
 * Output:                  -
***********************************************************************************************************************/
void cmd_SMONTH(void)
{
    auto unsigned char uint8_Result = 0;    //local work byte
    auto unsigned short int uint16_WI;      //local work integer
    
    if(g_CmdChk.uint8_ParamPos == 2)        //number of received characters OK?
    {
        //verify the limits if they are inside the tolerance
        uint8_Result += funct_CheckTol(g_CmdChk.uint32_TempPara[1],_MoJan,_MoJan);
        uint8_Result += funct_CheckTol(g_CmdChk.uint32_TempPara[1],_MoFeb,_MoFeb);
        uint8_Result += funct_CheckTol(g_CmdChk.uint32_TempPara[1],_MoMar,_MoMar);
        uint8_Result += funct_CheckTol(g_CmdChk.uint32_TempPara[1],_MoApr,_MoApr);
        uint8_Result += funct_CheckTol(g_CmdChk.uint32_TempPara[1],_MoMay,_MoMay);
        uint8_Result += funct_CheckTol(g_CmdChk.uint32_TempPara[1],_MoJun,_MoJun);
        uint8_Result += funct_CheckTol(g_CmdChk.uint32_TempPara[1],_MoJul,_MoJul);
        uint8_Result += funct_CheckTol(g_CmdChk.uint32_TempPara[1],_MoAug,_MoAug);
        uint8_Result += funct_CheckTol(g_CmdChk.uint32_TempPara[1],_MoSep,_MoSep);
        uint8_Result += funct_CheckTol(g_CmdChk.uint32_TempPara[1],_MoOct,_MoOct);
        uint8_Result += funct_CheckTol(g_CmdChk.uint32_TempPara[1],_MoNov,_MoNov);
        uint8_Result += funct_CheckTol(g_CmdChk.uint32_TempPara[1],_MoDec,_MoDec);
        
        if(uint8_Result == 1)       //verify the result
        {
            uint16_WI = g_CmdChk.uint32_TempPara[1];                 //store decimal value
            
            switch(uint16_WI)
            {
                case (_MoJan):
                    RV30xx_SetGetMonth(_Set,1);   //call subroutine
                    break;
                    
                case (_MoFeb):
                    RV30xx_SetGetMonth(_Set,2);   //call subroutine
                    break;
                    
                case (_MoMar):
                    RV30xx_SetGetMonth(_Set,3);   //call subroutine
                    break;
                    
                case (_MoApr):
                    RV30xx_SetGetMonth(_Set,4);   //call subroutine
                    break;
                    
                case (_MoMay):
                    RV30xx_SetGetMonth(_Set,5);   //call subroutine
                    break;
                    
                case (_MoJun):
                    RV30xx_SetGetMonth(_Set,6);   //call subroutine
                    break;
                    
                case (_MoJul):
                    RV30xx_SetGetMonth(_Set,7);   //call subroutine
                    break;
                    
                case (_MoAug):
                    RV30xx_SetGetMonth(_Set,8);   //call subroutine
                    break;
                    
                case (_MoSep):
                    RV30xx_SetGetMonth(_Set,9);   //call subroutine
                    break;
                    
                case (_MoOct):
                    RV30xx_SetGetMonth(_Set,16);   //call subroutine
                    break;
                    
                case (_MoNov):
                    RV30xx_SetGetMonth(_Set,17);   //call subroutine
                    break;
                    
                case (_MoDec):
                    RV30xx_SetGetMonth(_Set,18);   //call subroutine
                    break;
                    
                default:
                    //do nothing, this case will never called, because the value is verified 
                    //already at the beginning
                    break;
            }
            
            if(g_i2c1.uint8_BusColl || g_i2c1.uint8_ErrACK)
            {
                uart2_sendbuffer('X');      //first the letter X
                uart2_sendbuffer(',');      //then the comma
                g_Param.uint8_ErrCode = _BusCollRTC;  //set error code
                funct_IntToAscii(g_Param.uint8_ErrCode,_Active);
                uart2_sendbuffer(13);       //then the CR
            }
            else
            {
                uart2_sendbuffer('E');      //first the letter X
                uart2_sendbuffer(13);       //then the CR
            }
        }
        else
        {
            g_Param.uint8_ErrCode = _OutOfTolSMONTH;    //set error code
            uart2_SendErrorCode(g_Param.uint8_ErrCode); //call subroutine
        }
    }
    else
    {
        g_Param.uint8_ErrCode = _NumbRecCharNotOK;  //set error code
        uart2_SendErrorCode(g_Param.uint8_ErrCode); //call subroutine
    } 
}   //end of cmd_SMONTH


/**********************************************************************************************************************
 * Routine:                 cmd_GMONTH

 * Description:
 * Verify the received parameters of this command, if all parameters are within the tolerance.
 * If all is correct, then read out the month value.
 * 
 * Creator:                 A. Staub
 * Date of creation:        16.12.2015
 * Last modification on:    -
 * Modified by:             - 
 * 
 * Input:                   -
 * Output:                  -
***********************************************************************************************************************/
void cmd_GMONTH(void)
{
    if(g_CmdChk.uint8_ParamPos == 1)        //number of received characters OK?
    {
        RV30xx_SetGetMonth(_Get,0);         //read out the hours
        
        if(g_i2c1.uint8_BusColl || g_i2c1.uint8_ErrACK)
        {
            uart2_sendbuffer('X');      //first the letter X
            uart2_sendbuffer(',');      //then the comma
            g_Param.uint8_ErrCode = _BusCollRTC;  //set error code
            funct_IntToAscii(g_Param.uint8_ErrCode,_Active);
            uart2_sendbuffer(13);       //then the CR
        }
        else
        {
            uart2_sendbuffer('E');      //first the letter X
            uart2_sendbuffer(',');      //then the comma            
            funct_StoreMonthIntoRSbuffer(); //call subroutine
            uart2_sendbuffer(13);       //then the CR
        }
    }
    else
    {
        g_Param.uint8_ErrCode = _NumbRecCharNotOK;  //set error code
        uart2_SendErrorCode(g_Param.uint8_ErrCode); //call subroutine
    } 
}   //end of cmd_GMONTH


/**********************************************************************************************************************
 * Routine:                 cmd_SYEAR

 * Description:
 * Verify the received parameters of this command, if all parameters are within the tolerance.
 * If all is correct, then set the new year value.
 * 
 * Creator:                 A. Staub
 * Date of creation:        16.12.2015
 * Last modification on:    -
 * Modified by:             - 
 * 
 * Input:                   -
 * Output:                  -
***********************************************************************************************************************/
void cmd_SYEAR(void)
{
    auto unsigned char uint8_Result = 0;    //local work byte
    auto unsigned char uint8_WB;            //local work byte
    
    if(g_CmdChk.uint8_ParamPos == 2)        //number of received characters OK?
    {
        //verify the limits if they are inside the tolerance
        uint8_Result += funct_CheckTol(g_CmdChk.uint32_TempPara[1],_YearMin,_YearMax);
        
        if(uint8_Result == 1)       //verify the result
        {
            uint8_WB = g_CmdChk.uint32_TempPara[1];                 //store decimal value
            uint8_WB = (((uint8_WB / 10) << 4) | (uint8_WB % 10));  //convert to BCD
            
            RV30xx_SetGetYear(_Set,uint8_WB);    //call subroutine
            
            if(g_i2c1.uint8_BusColl || g_i2c1.uint8_ErrACK)
            {
                uart2_sendbuffer('X');      //first the letter X
                uart2_sendbuffer(',');      //then the comma
                g_Param.uint8_ErrCode = _BusCollRTC;  //set error code
                funct_IntToAscii(g_Param.uint8_ErrCode,_Active);
                uart2_sendbuffer(13);       //then the CR
            }
            else
            {
                uart2_sendbuffer('E');      //first the letter X
                uart2_sendbuffer(13);       //then the CR
            }
        }
        else
        {
            g_Param.uint8_ErrCode = _OutOfTolSYEAR;     //set error code
            uart2_SendErrorCode(g_Param.uint8_ErrCode); //call subroutine
        }
    }
    else
    {
        g_Param.uint8_ErrCode = _NumbRecCharNotOK;  //set error code
        uart2_SendErrorCode(g_Param.uint8_ErrCode); //call subroutine
    } 
}   //end of cmd_SYEAR


/**********************************************************************************************************************
 * Routine:                 cmd_GYEAR

 * Description:
 * Verify the received parameters of this command, if all parameters are within the tolerance.
 * If all is correct, then read out the year value.
 * 
 * Creator:                 A. Staub
 * Date of creation:        16.12.2015
 * Last modification on:    -
 * Modified by:             - 
 * 
 * Input:                   -
 * Output:                  -
***********************************************************************************************************************/
void cmd_GYEAR(void)
{
    if(g_CmdChk.uint8_ParamPos == 1)        //number of received characters OK?
    {
        RV30xx_SetGetYear(_Get,0);           //read out the hours
        
        if(g_i2c1.uint8_BusColl || g_i2c1.uint8_ErrACK)
        {
            uart2_sendbuffer('X');      //first the letter X
            uart2_sendbuffer(',');      //then the comma
            g_Param.uint8_ErrCode = _BusCollRTC;  //set error code
            funct_IntToAscii(g_Param.uint8_ErrCode,_Active);
            uart2_sendbuffer(13);       //then the CR
        }
        else
        {
            uart2_sendbuffer('E');      //first the letter X
            uart2_sendbuffer(',');      //then the comma
            uart2_sendbuffer('2');      //then the comma
            uart2_sendbuffer('0');      //then the comma
            funct_IntToAscii(g_Param.uint8_Year,_Active);
            uart2_sendbuffer(13);       //then the CR
        }
    }
    else
    {
        g_Param.uint8_ErrCode = _NumbRecCharNotOK;  //set error code
        uart2_SendErrorCode(g_Param.uint8_ErrCode); //call subroutine
    } 
}   //end of cmd_GYEAR


/**********************************************************************************************************************
 * Routine:                 cmd_GTIME

 * Description:
 * Verify the received parameters of this command, if all parameters are within the tolerance.
 * If all is correct, then read out all time parameters.
 * 
 * Creator:                 A. Staub
 * Date of creation:        16.12.2015
 * Last modification on:    -
 * Modified by:             - 
 * 
 * Input:                   -
 * Output:                  -
***********************************************************************************************************************/
void cmd_GTIME(void)
{
    auto unsigned char uint8_WB;        //local work byte
    
    if(g_CmdChk.uint8_ParamPos == 1)        //number of received characters OK?
    {
        if(g_CmdChk.uint8_GlobalLock == 1)  //global lock active?
        {
            g_Param.uint8_ErrCode = _MotorInRun;        //set error code
            uart2_SendErrorCode(g_Param.uint8_ErrCode); //call subroutine
        }
        else
        {
            uint8_WB = 7;                       //counter for do while

            do
            {
                switch (uint8_WB)
                {
                    case 7:
                        RV30xx_SetGetWday(_Get,0);      //read out the weekday
                        break;

                    case 6:
                        RV30xx_SetGetDay(_Get,0);       //read out the day
                        break;

                    case 5:
                        RV30xx_SetGetMonth(_Get,0);     //read out the month
                        break;

                    case 4:
                        RV30xx_SetGetYear(_Get,0);      //read out the year
                        break;

                    case 3:
                        RV30xx_SetGetHrs(_Get,0);       //read out the hours
                        break;

                    case 2:
                        RV30xx_SetGetMin(_Get,0);       //read out the minutes
                        break;

                    case 1:
                        RV30xx_SetGetSec(_Get,0);       //read out the seconds
                        break;

                    default:
                        //do nothing
                        break;
                }

                uint8_WB--;

                if(g_i2c1.uint8_BusColl || g_i2c1.uint8_ErrACK)
                {
                    //error occured, exit the while loop
                    uint8_WB = 0;
                }
                else
                {
                    //do nothing
                }
            }
            while(uint8_WB);    //until counter is not 0

            if(g_i2c1.uint8_BusColl || g_i2c1.uint8_ErrACK)
            {
                uart2_sendbuffer('X');      //first the letter X
                uart2_sendbuffer(',');      //then the comma
                g_Param.uint8_ErrCode = _BusCollRTC;  //set error code
                funct_IntToAscii(g_Param.uint8_ErrCode,_Active);
                uart2_sendbuffer(13);       //then the CR
            }
            else
            {
                uart2_sendbuffer('E');      //first the letter X
                uart2_sendbuffer(',');      //then the comma
                funct_StoreWdayIntoRSbuffer();  //call subroutine
                uart2_sendbuffer(' ');      //then the space
                funct_IntToAscii(g_Param.uint8_Day,_Active);
                uart2_sendbuffer('t');      //then the t
                uart2_sendbuffer('h');      //then the h
                uart2_sendbuffer(' ');      //then the space
                funct_StoreMonthIntoRSbuffer(); //call subroutine
                uart2_sendbuffer(' ');      //then the space
                uart2_sendbuffer('2');      //then the 2
                uart2_sendbuffer('0');      //then the 0
                funct_IntToAscii(g_Param.uint8_Year,_Active);
                uart2_sendbuffer(',');      //then the comma
                uart2_sendbuffer(' ');      //then the space
                funct_IntToAscii(g_Param.uint8_Hrs,_Active);
                uart2_sendbuffer(':');      //then the double point
                funct_IntToAscii(g_Param.uint8_Min,_Active);
                uart2_sendbuffer(':');      //then the double point
                funct_IntToAscii(g_Param.uint8_Sec,_Active);
                uart2_sendbuffer(13);       //then the CR         
            }
        }
    }
    else
    {
        g_Param.uint8_ErrCode = _NumbRecCharNotOK;  //set error code
        uart2_SendErrorCode(g_Param.uint8_ErrCode); //call subroutine
    } 
}   //end of cmd_GTIME


/**********************************************************************************************************************
 * Routine:                 cmd_GSTALIN

 * Description:
 * Read the status (input pin) of the lin bus driver (NRES) and send it back.
 * 
 * Creator:                 A. Staub
 * Date of creation:        18.12.2015
 * Last modification on:    -
 * Modified by:             - 
 * 
 * Input:                   -
 * Output:                  -
***********************************************************************************************************************/
void cmd_GSTALIN(void)
{
    if(g_CmdChk.uint8_ParamPos == 1)    //number of received characters OK?
    {
        g_Param.uint8_LinStatus = iNRESSignalLIN;   //read input
        
        uart2_sendbuffer('E');      //first the letter E
        uart2_sendbuffer(',');      //then the comma
        funct_IntToAscii(g_Param.uint8_LinStatus,_Active);
        uart2_sendbuffer(13);       //then the CR
    }
    else
    {
        g_Param.uint8_ErrCode = _NumbRecCharNotOK;  //set error code
        uart2_SendErrorCode(g_Param.uint8_ErrCode); //call subroutine
    } 
}   //end of cmd_GSTALIN


/**********************************************************************************************************************
 * Routine:                 cmd_SSPDLIN

 * Description:
 * Verify the received parameters of this command, if all parameters are within the tolerance then set
 * up the new baud rate for the lin communication. 
 * Calculate the error between the desired baud rate and the effective baud rate and send this
 * error back. 
 * 
 * Creator:                 A. Staub
 * Date of creation:        19.12.2015
 * Last modification on:    -
 * Modified by:             - 
 * 
 * Input:                   -
 * Output:                  -
***********************************************************************************************************************/
void cmd_SSPDLIN(void)
{
    auto unsigned char uint8_Result = 0;    //local work byte
    auto unsigned char uint8_Err;           //local work byte for the error
    auto unsigned long int uint32_WLI1;     //local work long integer 1
    auto unsigned long int uint32_WLI2;     //local work long integer 2
    
    if(g_CmdChk.uint8_ParamPos == 2)        //number of received characters OK?
    {
        if(g_CmdChk.uint8_GlobalLock == 1)  //global lock active?
        {
            g_Param.uint8_ErrCode = _MotorInRun;        //set error code
            uart2_SendErrorCode(g_Param.uint8_ErrCode); //call subroutine
        }
        else
        {
            //verify the limits if they are inside the tolerance
            uint8_Result += funct_CheckTol(g_CmdChk.uint32_TempPara[1],_LinSpdMin,_LinSpdMax);

            if(uint8_Result == 1)       //verify the result
            {
                //set up the new baud rate
                uart_set(_UART1_,_NONE,_1_STOP,_NON_INVERTED,_NO_AUTOBAUD,g_CmdChk.uint32_TempPara[1]);

                //store the baud rate
                g_Param.uint32_LinSpd = g_CmdChk.uint32_TempPara[1];

                if(g_Param.uint32_LinSpd <= 20000)  //baud rate smaller than 20kBaud
                {
                    oSPModeSignalLIN = 0;   //disable high speed mode
                }
                else
                {
                    oSPModeSignalLIN = 1;   //enable high speed mode
                }

                //calculation of the error 
                uint32_WLI1 = _FREQ_OSC / (16 * (U1BRG + 1)); //calculate the effective baud rage
                uint32_WLI1 = uint32_WLI1 * 10000;                      
                uint32_WLI2 = g_CmdChk.uint32_TempPara[1] * 10000;
                //calculate the error between the desired and the effective baud rate
                uint8_Err = (uint32_WLI1 - uint32_WLI2) / g_CmdChk.uint32_TempPara[1];

                uart2_sendbuffer('E');      //first the letter E
                uart2_sendbuffer(',');      //add a comma
                uart2_sendbuffer('0');      //add a 0
                uart2_sendbuffer('.');      //add a point
                funct_IntToAscii(uint8_Err,_Active);
                uart2_sendbuffer(13);       //then the CR
            }
            else
            {
                g_Param.uint8_ErrCode = _OutOfTolSSPDLIN;     //set error code
                uart2_SendErrorCode(g_Param.uint8_ErrCode); //call subroutine
            }
        }
    }
    else
    {
        g_Param.uint8_ErrCode = _NumbRecCharNotOK;  //set error code
        uart2_SendErrorCode(g_Param.uint8_ErrCode); //call subroutine
    } 
}   //end of cmd_SSPDLIN


/**********************************************************************************************************************
 * Routine:                 cmd_GSPDLIN

 * Description:
 * Verify the received parameters of this command, if all parameters are within the tolerance then
 * send back the actuall LIN communication baud rate. 
 * 
 * Creator:                 A. Staub
 * Date of creation:        19.12.2015
 * Last modification on:    -
 * Modified by:             - 
 * 
 * Input:                   -
 * Output:                  -
***********************************************************************************************************************/
void cmd_GSPDLIN(void)
{
    if(g_CmdChk.uint8_ParamPos == 1)        //number of received characters OK?
    {
        uart2_sendbuffer('E');      //first the letter E
        uart2_sendbuffer(',');      //then the comma
        funct_IntToAscii(g_Param.uint32_LinSpd,_Active);
        uart2_sendbuffer(13);       //then the CR
    }
    else
    {
        g_Param.uint8_ErrCode = _NumbRecCharNotOK;  //set error code
        uart2_SendErrorCode(g_Param.uint8_ErrCode); //call subroutine
    } 
}   //end of cmd_GSPDLIN


/**********************************************************************************************************************
 * Routine:                 cmd_STOLIN

 * Description:
 * Verify the received parameters of this command, if all parameters are within the tolerance then set
 * up the new timeout for the lin bus communication
 * 
 * Creator:                 A. Staub
 * Date of creation:        19.12.2015
 * Last modification on:    -
 * Modified by:             - 
 * 
 * Input:                   -
 * Output:                  -
***********************************************************************************************************************/
void cmd_STOLIN(void)
{
    auto unsigned char uint8_Result = 0;    //local work byte
    
    if(g_CmdChk.uint8_ParamPos == 2)        //number of received characters OK?
    {
        if(g_CmdChk.uint8_GlobalLock == 1)  //global lock active?
        {
            g_Param.uint8_ErrCode = _MotorInRun;        //set error code
            uart2_SendErrorCode(g_Param.uint8_ErrCode); //call subroutine
        }
        else
        {
            //verify the limits if they are inside the tolerance
            uint8_Result += funct_CheckTol(g_CmdChk.uint32_TempPara[1],_LinToMin,_LinToMax);

            if(uint8_Result == 1)       //verify the result
            {
                //store the new timeout
                g_Param.uint16_LinTO = g_CmdChk.uint32_TempPara[1];

                uart2_sendbuffer('E');      //first the letter E
                uart2_sendbuffer(13);       //then the CR
            }
            else
            {
                g_Param.uint8_ErrCode = _OutOfTolSTOLIN;     //set error code
                uart2_SendErrorCode(g_Param.uint8_ErrCode); //call subroutine
            }
        }
    }
    else
    {
        g_Param.uint8_ErrCode = _NumbRecCharNotOK;  //set error code
        uart2_SendErrorCode(g_Param.uint8_ErrCode); //call subroutine
    } 
}   //end of cmd_STOLIN


/**********************************************************************************************************************
 * Routine:                 cmd_GTOLIN

 * Description:
 * Verify the received parameters of this command, if all parameters are within the tolerance then
 * send back the actuall LIN communication timeout. 
 * 
 * Creator:                 A. Staub
 * Date of creation:        19.12.2015
 * Last modification on:    -
 * Modified by:             - 
 * 
 * Input:                   -
 * Output:                  -
***********************************************************************************************************************/
void cmd_GTOLIN(void)
{
    if(g_CmdChk.uint8_ParamPos == 1)        //number of received characters OK?
    {
        uart2_sendbuffer('E');      //first the letter E
        uart2_sendbuffer(',');      //then the comma
        funct_IntToAscii(g_Param.uint16_LinTO,_Active);
        uart2_sendbuffer(13);       //then the CR
    }
    else
    {
        g_Param.uint8_ErrCode = _NumbRecCharNotOK;  //set error code
        uart2_SendErrorCode(g_Param.uint8_ErrCode); //call subroutine
    } 
}   //end of cmd_GTOLIN


/**********************************************************************************************************************
 * Routine:                 cmd_SLIN

 * Description:
 * ...explain
 * 
 * Creator:                 A. Staub
 * Date of creation:        19.12.2015
 * Last modification on:    -
 * Modified by:             - 
 * 
 * Input:                   -
 * Output:                  -
***********************************************************************************************************************/
void cmd_SLIN(void)
{
    auto unsigned char uint8_Result = 0;    //local work byte
    auto unsigned char uint8_WB;            //local work byte
    
    //lin bus communication not busy?
    if(!g_LIN.uint8_LinBusy)
    {
        //number of received characters OK?
        if((g_CmdChk.uint8_ParamPos >= 3) && (g_CmdChk.uint8_ParamPos <= 42))        
        {
            uint8_Result += funct_CheckTol(g_CmdChk.uint32_TempPara[1],_LinLengthMin,_LinLengthMax);
            g_LIN.uint8_SlaveReceiveCounter = g_CmdChk.uint32_TempPara[1];  //store lenght of the lin answer
            if(g_LIN.uint8_SlaveReceiveCounter) //answer from slave requested
            {
                g_LIN.uint8_SlaveAnswerRequested = 1;
            }
            else
            {
                g_LIN.uint8_SlaveAnswerRequested = 0;
            }

            g_LIN.uint8_SlaveAnswerFinish = 0;      //reset variable
            g_LIN.uint8_SlaveTimeout = 0;           //reset variable
            g_UART1rxd.uint8_BufEmpty = 1;          //receive buffer empty
            g_UART1rxd.uint16_Wch = 0;              //write-pointer of the ring buffer at position 0
            g_UART1rxd.uint16_Rch = 0;              //read-pointer of the ring buffer at position 0

            //ToDo:
            //verify each received parameter with the tolerance (start with the 3rd parameter)
            //store directly the characters into the sendbuffer
            //Until:
            //the local work byte has the same size as number of received characters
            uint8_WB = 2;   //start with the first parameter to verify 
            g_LIN.uint8_MasterSendCounter = 1;  //1 because the first parameter is the start break
            
            do
            {
                uint8_Result += funct_CheckTol(g_CmdChk.uint32_TempPara[uint8_WB],_LinParaMin,_LinParaMax);
                uart1_sendbuffer(g_CmdChk.uint32_TempPara[uint8_WB]);
                uint8_WB++; //increment with 1 to take out the next parameter
                g_LIN.uint8_MasterSendCounter++;    //increment the counter
            }
            while(uint8_WB < g_CmdChk.uint8_ParamPos);

            //add the real answer length, like the TxD will be received as well
            g_LIN.uint8_SlaveReceiveCounter += g_LIN.uint8_MasterSendCounter;
            
            //each parameter within the tolerance (uint8_WB - 1), because the 1st parameter is the cmd ID
            if(uint8_Result == (uint8_WB-1))
            {
                g_LIN.uint8_LinBreakToSend = 1;             //enable LIN break to send
                IEC0SET = _IEC0_U1TXIE_MASK;                //enable the send interrupt
                g_LIN.uint8_LinBusy = 1;                    //set busy flag
                //send back a first result
                uart2_sendbuffer('E');      //first the letter E
                uart2_sendbuffer(13);       //then the CR
            }
            else
            {
                g_Param.uint8_ErrCode = _OutOfTolSLIN;      //set error code
                uart2_SendErrorCode(g_Param.uint8_ErrCode); //call subroutine
                g_UART1txd.uint8_BufEmpty = 1;              //send buffer empty
                g_UART1txd.uint16_Rch = 0;                  //read pointer = 0
                g_UART1txd.uint16_Wch = 0;                  //write pointer = 0
            }
        }
        else
        {
            g_Param.uint8_ErrCode = _NumbRecCharNotOK;  //set error code
            uart2_SendErrorCode(g_Param.uint8_ErrCode); //call subroutine
        } 
    }
    else
    {
        g_Param.uint8_ErrCode = _NumbRecCharNotOK;  //set error code
        uart2_SendErrorCode(g_Param.uint8_ErrCode); //call subroutine   
    }
}   //end of cmd_SLIN


/**********************************************************************************************************************
 * Routine:                 cmd_GBIPSTAS

 * Description:
 * Verify the received parameters of this command, if all parameters are within the tolerance.
 * If all is correct, then send back the bipolar status short
 * 
 * Creator:                 A. Staub
 * Date of creation:        16.01.2016
 * Last modification on:    -
 * Modified by:             - 
 * 
 * Input:                   -
 * Output:                  -
***********************************************************************************************************************/
void cmd_GBIPSTAS(void)
{
    if(g_CmdChk.uint8_ParamPos == 1)        //number of received characters OK?
    {
        uart2_sendbuffer('E');      //first the letter E
        uart2_sendbuffer(',');      //then the comma
        funct_IntToAscii(A3981.FAULT1.BITS.FF,_Active);
        uart2_sendbuffer(',');      //then the comma
        funct_IntToAscii(A3981.FAULT1.BITS.TW,_Active);
        uart2_sendbuffer(',');      //then the comma
        funct_IntToAscii(A3981.FAULT1.BITS.OV,_Active);
        uart2_sendbuffer(',');      //then the comma
        funct_IntToAscii(A3981.FAULT1.BITS.UV,_Active);
        uart2_sendbuffer(',');      //then the comma
        funct_IntToAscii(A3981.FAULT1.BITS.ST,_Active);
        uart2_sendbuffer(',');      //then the comma
        funct_IntToAscii(A3981.FAULT1.BITS.OLB,_Active);
        uart2_sendbuffer(',');      //then the comma
        funct_IntToAscii(A3981.FAULT1.BITS.OLA,_Active);
        uart2_sendbuffer(13);      //then the CR
    }
    else
    {
        g_Param.uint8_ErrCode = _NumbRecCharNotOK;  //set error code
        uart2_SendErrorCode(g_Param.uint8_ErrCode); //call subroutine
    } 
}   //end of cmd_GBIPSTAS


/**********************************************************************************************************************
 * Routine:                 cmd_GBIPSTAL

 * Description:
 * Verify the received parameters of this command, if all parameters are within the tolerance.
 * If all is correct, then send back the bipolar status long.
 * 
 * Creator:                 A. Staub
 * Date of creation:        16.01.2016
 * Last modification on:    -
 * Modified by:             - 
 * 
 * Input:                   -
 * Output:                  -
***********************************************************************************************************************/
void cmd_GBIPSTAL(void)
{
    if(g_CmdChk.uint8_ParamPos == 1)        //number of received characters OK?
    {
        uart2_sendbuffer('E');      //first the letter E
        uart2_sendbuffer(',');      //then the comma
        funct_IntToAscii(A3981.FAULT0.BITS.FF,_Active);
        uart2_sendbuffer(',');      //then the comma
        funct_IntToAscii(A3981.FAULT0.BITS.TW,_Active);
        uart2_sendbuffer(',');      //then the comma
        funct_IntToAscii(A3981.FAULT0.BITS.OV,_Active);
        uart2_sendbuffer(',');      //then the comma
        funct_IntToAscii(A3981.FAULT0.BITS.UV,_Active);
        uart2_sendbuffer(',');      //then the comma
        funct_IntToAscii(A3981.FAULT0.BITS.ST,_Active);
        uart2_sendbuffer(',');      //then the comma
        funct_IntToAscii(A3981.FAULT0.BITS.OLB,_Active);
        uart2_sendbuffer(',');      //then the comma
        funct_IntToAscii(A3981.FAULT0.BITS.OLA,_Active);      
        uart2_sendbuffer(',');      //then the comma
        funct_IntToAscii(A3981.FAULT0.BITS.BML,_Active);
        uart2_sendbuffer(',');      //then the comma
        funct_IntToAscii(A3981.FAULT0.BITS.BMH,_Active);
        uart2_sendbuffer(',');      //then the comma
        funct_IntToAscii(A3981.FAULT0.BITS.BPL,_Active);
        uart2_sendbuffer(',');      //then the comma
        funct_IntToAscii(A3981.FAULT0.BITS.BPH,_Active);
        uart2_sendbuffer(',');      //then the comma
        funct_IntToAscii(A3981.FAULT0.BITS.AML,_Active);
        uart2_sendbuffer(',');      //then the comma
        funct_IntToAscii(A3981.FAULT0.BITS.AMH,_Active);
        uart2_sendbuffer(',');      //then the comma
        funct_IntToAscii(A3981.FAULT0.BITS.APL,_Active);
        uart2_sendbuffer(',');      //then the comma
        funct_IntToAscii(A3981.FAULT0.BITS.APH,_Active); 
        uart2_sendbuffer(13);      //then the CR
    }
    else
    {
        g_Param.uint8_ErrCode = _NumbRecCharNotOK;  //set error code
        uart2_SendErrorCode(g_Param.uint8_ErrCode); //call subroutine
    } 
}   //end of cmd_GBIPSTAL


/**********************************************************************************************************************
 * Routine:                 cmd_SBIPPD

 * Description:
 * Verify the received parameters of this command, if all parameters are within the tolerance.
 * If all is correct, then set the bipolar phase decay mode according to the received parameters
 * 
 * Creator:                 A. Staub
 * Date of creation:        16.01.2016
 * Last modification on:    -
 * Modified by:             - 
 * 
 * Input:                   -
 * Output:                  -
***********************************************************************************************************************/
void cmd_SBIPPD(void)
{
    auto unsigned char uint8_Result = 0;    //local work byte
    
    if(g_CmdChk.uint8_ParamPos == 4)        //number of received characters OK?
    {
        if(g_CmdChk.uint8_GlobalLock == 1)  //global lock active?
        {
            g_Param.uint8_ErrCode = _MotorInRun;        //set error code
            uart2_SendErrorCode(g_Param.uint8_ErrCode); //call subroutine
        }
        else
        {
            //verify the limits if they are inside the tolerance
            uint8_Result += funct_CheckTol(g_CmdChk.uint32_TempPara[1],_BipSyrMin,_BipSyrMax);
            uint8_Result += funct_CheckTol(g_CmdChk.uint32_TempPara[2],_BipDcyMin,_BipDcyMax);
            uint8_Result += funct_CheckTol(g_CmdChk.uint32_TempPara[3],_BipHlrMin,_BipHlrMax);

            if(uint8_Result == 3)       //verify the result
            {
                A3981.CONFIG0.BITS.SYR = g_CmdChk.uint32_TempPara[1];
                A3981.RUN.BITS.DCY = g_CmdChk.uint32_TempPara[2];
                A3981.RUN.BITS.HLR = g_CmdChk.uint32_TempPara[3];

                uart2_sendbuffer('E');      //first the letter E
                uart2_sendbuffer(13);       //then the CR
            }
            else
            {
                g_Param.uint8_ErrCode = _BipSBIPPD;         //set error code
                uart2_SendErrorCode(g_Param.uint8_ErrCode); //call subroutine
            }
        }
    }
    else
    {
        g_Param.uint8_ErrCode = _NumbRecCharNotOK;  //set error code
        uart2_SendErrorCode(g_Param.uint8_ErrCode); //call subroutine
    } 
}   //end of cmd_SBIPPD


/**********************************************************************************************************************
 * Routine:                 cmd_GBIPPD

 * Description:
 * Verify the received parameters of this command, if all parameters are within the tolerance.
 * If all is correct, then send back the bipolar phase decay
 * 
 * Creator:                 A. Staub
 * Date of creation:        16.01.2016
 * Last modification on:    -
 * Modified by:             - 
 * 
 * Input:                   -
 * Output:                  -
***********************************************************************************************************************/
void cmd_GBIPPD(void)
{
    if(g_CmdChk.uint8_ParamPos == 1)        //number of received characters OK?
    {
        uart2_sendbuffer('E');      //first the letter E
        uart2_sendbuffer(',');      //then the comma
        funct_IntToAscii(A3981.CONFIG0.BITS.SYR,_Active);
        uart2_sendbuffer(',');      //then the comma
        funct_IntToAscii(A3981.RUN.BITS.DCY,_Active);
        uart2_sendbuffer(',');      //then the comma
        funct_IntToAscii(A3981.RUN.BITS.HLR,_Active);
        uart2_sendbuffer(13);      //then the CR
    }
    else
    {
        g_Param.uint8_ErrCode = _NumbRecCharNotOK;  //set error code
        uart2_SendErrorCode(g_Param.uint8_ErrCode); //call subroutine
    } 
}   //end of cmd_GBIPPD


/**********************************************************************************************************************
 * Routine:                 cmd_SBIPTSC

 * Description:
 * Verify the received parameters of this command, if all parameters are within the tolerance.
 * If all is correct, then set the bipolar overcurrent fault delay according to the received parameter
 * 
 * Creator:                 A. Staub
 * Date of creation:        16.01.2016
 * Last modification on:    -
 * Modified by:             - 
 * 
 * Input:                   -
 * Output:                  -
***********************************************************************************************************************/
void cmd_SBIPTSC(void)
{
    auto unsigned char uint8_Result = 0;    //local work byte
    
    if(g_CmdChk.uint8_ParamPos == 2)        //number of received characters OK?
    {
        if(g_CmdChk.uint8_GlobalLock == 1)  //global lock active?
        {
            g_Param.uint8_ErrCode = _MotorInRun;        //set error code
            uart2_SendErrorCode(g_Param.uint8_ErrCode); //call subroutine
        }
        else
        {
            //verify the limits if they are inside the tolerance
            uint8_Result += funct_CheckTol(g_CmdChk.uint32_TempPara[1],_BipTscMin,_BipTscMax);

            if(uint8_Result == 1)       //verify the result
            {
                A3981.CONFIG1.BITS.TSC = g_CmdChk.uint32_TempPara[1];

                uart2_sendbuffer('E');      //first the letter E
                uart2_sendbuffer(13);       //then the CR
            }
            else
            {
                g_Param.uint8_ErrCode = _BipSBIPTSC;        //set error code
                uart2_SendErrorCode(g_Param.uint8_ErrCode); //call subroutine
            }
        }
    }
    else
    {
        g_Param.uint8_ErrCode = _NumbRecCharNotOK;  //set error code
        uart2_SendErrorCode(g_Param.uint8_ErrCode); //call subroutine
    } 
}   //end of cmd_SBIPTSC


/**********************************************************************************************************************
 * Routine:                 cmd_GBIPTSC

 * Description:
 * Verify the received parameters of this command, if all parameters are within the tolerance.
 * If all is correct, then send back the bipolar overcurrent fault delay
 * 
 * Creator:                 A. Staub
 * Date of creation:        16.01.2016
 * Last modification on:    -
 * Modified by:             - 
 * 
 * Input:                   -
 * Output:                  -
***********************************************************************************************************************/
void cmd_GBIPTSC(void)
{
    if(g_CmdChk.uint8_ParamPos == 1)        //number of received characters OK?
    {
        uart2_sendbuffer('E');      //first the letter E
        uart2_sendbuffer(',');      //then the comma
        funct_IntToAscii(A3981.CONFIG1.BITS.TSC,_Active);
        uart2_sendbuffer(13);      //then the CR
    }
    else
    {
        g_Param.uint8_ErrCode = _NumbRecCharNotOK;  //set error code
        uart2_SendErrorCode(g_Param.uint8_ErrCode); //call subroutine
    } 
}   //end of cmd_GBIPTSC


/**********************************************************************************************************************
 * Routine:                 cmd_SBIPOL

 * Description:
 * Verify the received parameters of this command, if all parameters are within the tolerance.
 * If all is correct, then set the bipolar open load current according to the received parameter
 * 
 * Creator:                 A. Staub
 * Date of creation:        16.01.2016
 * Last modification on:    -
 * Modified by:             - 
 * 
 * Input:                   -
 * Output:                  -
***********************************************************************************************************************/
void cmd_SBIPOL(void)
{
    auto unsigned char uint8_Result = 0;    //local work byte
    
    if(g_CmdChk.uint8_ParamPos == 2)        //number of received characters OK?
    {
        if(g_CmdChk.uint8_GlobalLock == 1)  //global lock active?
        {
            g_Param.uint8_ErrCode = _MotorInRun;        //set error code
            uart2_SendErrorCode(g_Param.uint8_ErrCode); //call subroutine
        }
        else
        {
            //verify the limits if they are inside the tolerance
            uint8_Result += funct_CheckTol(g_CmdChk.uint32_TempPara[1],_BipOlMin,_BipOlMax);

            if(uint8_Result == 1)       //verify the result
            {
                A3981.RUN.BITS.OL = g_CmdChk.uint32_TempPara[1];

                uart2_sendbuffer('E');      //first the letter E
                uart2_sendbuffer(13);       //then the CR
            }
            else
            {
                g_Param.uint8_ErrCode = _BipSBIPOL;         //set error code
                uart2_SendErrorCode(g_Param.uint8_ErrCode); //call subroutine
            }
        }
    }
    else
    {
        g_Param.uint8_ErrCode = _NumbRecCharNotOK;  //set error code
        uart2_SendErrorCode(g_Param.uint8_ErrCode); //call subroutine
    } 
}   //end of cmd_SBIPOL


/**********************************************************************************************************************
 * Routine:                 cmd_GBIPOL

 * Description:
 * Verify the received parameters of this command, if all parameters are within the tolerance.
 * If all is correct, then send back the bipolar open load current 
 * 
 * Creator:                 A. Staub
 * Date of creation:        16.01.2016
 * Last modification on:    -
 * Modified by:             - 
 * 
 * Input:                   -
 * Output:                  -
***********************************************************************************************************************/
void cmd_GBIPOL(void)
{
    if(g_CmdChk.uint8_ParamPos == 1)        //number of received characters OK?
    {
        uart2_sendbuffer('E');      //first the letter E
        uart2_sendbuffer(',');      //then the comma
        funct_IntToAscii(A3981.RUN.BITS.OL,_Active);
        uart2_sendbuffer(13);      //then the CR
    }
    else
    {
        g_Param.uint8_ErrCode = _NumbRecCharNotOK;  //set error code
        uart2_SendErrorCode(g_Param.uint8_ErrCode); //call subroutine
    } 
}   //end of cmd_GBIPOL


/**********************************************************************************************************************
 * Routine:                 cmd_SBIPSTS

 * Description:
 * Verify the received parameters of this command, if all parameters are within the tolerance.
 * If all is correct, then set the bipolar stall detection scheme according to the received parameters
 * 
 * Creator:                 A. Staub
 * Date of creation:        16.01.2016
 * Last modification on:    -
 * Modified by:             - 
 * 
 * Input:                   -
 * Output:                  -
***********************************************************************************************************************/
void cmd_SBIPSTS(void)
{
    auto unsigned char uint8_Result = 0;    //local work byte
    
    if(g_CmdChk.uint8_ParamPos == 3)        //number of received characters OK?
    {
        if(g_CmdChk.uint8_GlobalLock == 1)  //global lock active?
        {
            g_Param.uint8_ErrCode = _MotorInRun;        //set error code
            uart2_SendErrorCode(g_Param.uint8_ErrCode); //call subroutine
        }
        else
        {
            //verify the limits if they are inside the tolerance
            uint8_Result += funct_CheckTol(g_CmdChk.uint32_TempPara[1],_BipStsMin,_BipStsMax);
            uint8_Result += funct_CheckTol(g_CmdChk.uint32_TempPara[2],_BipCdMin,_BipCdMax);

            if(uint8_Result == 2)       //verify the result
            {
                if(g_CmdChk.uint32_TempPara[1] == 2)
                {
                    A3981.TBLLD.BITS.STS = 3;
                }
                else
                {
                    A3981.TBLLD.BITS.STS = g_CmdChk.uint32_TempPara[1];
                }
                A3981.CONFIG1.BITS.CD = g_CmdChk.uint32_TempPara[2];

                uart2_sendbuffer('E');      //first the letter E
                uart2_sendbuffer(13);       //then the CR
            }
            else
            {
                g_Param.uint8_ErrCode = _BipSBIPSTS;        //set error code
                uart2_SendErrorCode(g_Param.uint8_ErrCode); //call subroutine
            }
        }
    }
    else
    {
        g_Param.uint8_ErrCode = _NumbRecCharNotOK;  //set error code
        uart2_SendErrorCode(g_Param.uint8_ErrCode); //call subroutine
    } 
}   //end of cmd_SBIPSTS


/**********************************************************************************************************************
 * Routine:                 cmd_GBIPSTS

 * Description:
 * Verify the received parameters of this command, if all parameters are within the tolerance.
 * If all is correct, then send back the bipolar stall detection scheme  
 * 
 * Creator:                 A. Staub
 * Date of creation:        16.01.2016
 * Last modification on:    -
 * Modified by:             - 
 * 
 * Input:                   -
 * Output:                  -
***********************************************************************************************************************/
void cmd_GBIPSTS(void)
{
    if(g_CmdChk.uint8_ParamPos == 1)        //number of received characters OK?
    {
        uart2_sendbuffer('E');      //first the letter E
        uart2_sendbuffer(',');      //then the comma
        funct_IntToAscii(A3981.TBLLD.BITS.STS,_Active);
        uart2_sendbuffer(',');      //then the comma
        funct_IntToAscii(A3981.CONFIG1.BITS.CD,_Active);
        uart2_sendbuffer(13);      //then the CR
    }
    else
    {
        g_Param.uint8_ErrCode = _NumbRecCharNotOK;  //set error code
        uart2_SendErrorCode(g_Param.uint8_ErrCode); //call subroutine
    } 
}   //end of cmd_GBIPSTS


/**********************************************************************************************************************
 * Routine:                 cmd_SBIPPFD

 * Description:
 * Verify the received parameters of this command, if all parameters are within the tolerance.
 * If all is correct, then set the bipolar fast decay time according to the received parameters
 * 
 * Creator:                 A. Staub
 * Date of creation:        20.01.2016
 * Last modification on:    -
 * Modified by:             - 
 * 
 * Input:                   -
 * Output:                  -
***********************************************************************************************************************/
void cmd_SBIPPFD(void)
{
    auto unsigned char uint8_Result = 0;    //local work byte
    
    if(g_CmdChk.uint8_ParamPos == 2)        //number of received characters OK?
    {
        if(g_CmdChk.uint8_GlobalLock == 1)  //global lock active?
        {
            g_Param.uint8_ErrCode = _MotorInRun;        //set error code
            uart2_SendErrorCode(g_Param.uint8_ErrCode); //call subroutine
        }
        else
        {
            //verify the limits if they are inside the tolerance
            uint8_Result += funct_CheckTol(g_CmdChk.uint32_TempPara[1],_BipPfdMin,_BipPfdMax);

            if(uint8_Result == 1)       //verify the result
            {
                A3981.CONFIG0.BITS.PFD = g_CmdChk.uint32_TempPara[1];

                uart2_sendbuffer('E');      //first the letter E
                uart2_sendbuffer(13);       //then the CR
            }
            else
            {
                g_Param.uint8_ErrCode = _BipSBIPPFD;        //set error code
                uart2_SendErrorCode(g_Param.uint8_ErrCode); //call subroutine
            }
        }
    }
    else
    {
        g_Param.uint8_ErrCode = _NumbRecCharNotOK;  //set error code
        uart2_SendErrorCode(g_Param.uint8_ErrCode); //call subroutine
    } 
}   //end of cmd_SBIPPFD


/**********************************************************************************************************************
 * Routine:                 cmd_GBIPPFD

 * Description:
 * Verify the received parameters of this command, if all parameters are within the tolerance.
 * If all is correct, then send back the bipolar fast decay time
 * 
 * Creator:                 A. Staub
 * Date of creation:        20.01.2016
 * Last modification on:    -
 * Modified by:             - 
 * 
 * Input:                   -
 * Output:                  -
***********************************************************************************************************************/
void cmd_GBIPPFD(void)
{
    if(g_CmdChk.uint8_ParamPos == 1)        //number of received characters OK?
    {
        uart2_sendbuffer('E');      //first the letter E
        uart2_sendbuffer(',');      //then the comma
        funct_IntToAscii(A3981.CONFIG0.BITS.PFD,_Active);
        uart2_sendbuffer(13);      //then the CR
    }
    else
    {
        g_Param.uint8_ErrCode = _NumbRecCharNotOK;  //set error code
        uart2_SendErrorCode(g_Param.uint8_ErrCode); //call subroutine
    } 
}   //end of cmd_GBIPPFD


/**********************************************************************************************************************
 * Routine:                 cmd_SBIPPWM

 * Description:
 * Verify the received parameters of this command, if all parameters are within the tolerance.
 * If all is correct, then set the bipolar PWM mode according to the received parameters
 * 
 * Creator:                 A. Staub
 * Date of creation:        20.01.2016
 * Last modification on:    -
 * Modified by:             - 
 * 
 * Input:                   -
 * Output:                  -
***********************************************************************************************************************/
void cmd_SBIPPWM(void)
{
    auto unsigned char uint8_Result = 0;    //local work byte
    
    if(g_CmdChk.uint8_ParamPos == 3)        //number of received characters OK?
    {
        if(g_CmdChk.uint8_GlobalLock == 1)  //global lock active?
        {
            g_Param.uint8_ErrCode = _MotorInRun;        //set error code
            uart2_SendErrorCode(g_Param.uint8_ErrCode); //call subroutine
        }
        else
        {
            //verify the limits if they are inside the tolerance
            uint8_Result += funct_CheckTol(g_CmdChk.uint32_TempPara[1],_BipTbkMin,_BipTbkMax);
            uint8_Result += funct_CheckTol(g_CmdChk.uint32_TempPara[2],_BipTofMin,_BipTofMax);

            if(uint8_Result == 2)       //verify the result
            {
                A3981.CONFIG0.BITS.TBK = g_CmdChk.uint32_TempPara[1];
                A3981.CONFIG0.BITS.TOF_FRQ = g_CmdChk.uint32_TempPara[2];
                A3981.CONFIG0.BITS.PWM = 0; //mode = fixed off-time (PWM)

                uart2_sendbuffer('E');      //first the letter E
                uart2_sendbuffer(13);       //then the CR
            }
            else
            {
                g_Param.uint8_ErrCode = _BipSBIPPWM;        //set error code
                uart2_SendErrorCode(g_Param.uint8_ErrCode); //call subroutine
            }
        }
    }
    else
    {
        g_Param.uint8_ErrCode = _NumbRecCharNotOK;  //set error code
        uart2_SendErrorCode(g_Param.uint8_ErrCode); //call subroutine
    } 
}   //end of cmd_SBIPPWM


/**********************************************************************************************************************
 * Routine:                 cmd_GBIPPWM

 * Description:
 * Verify the received parameters of this command, if all parameters are within the tolerance.
 * If all is correct, then send back the bipolar PWM mode only if this mode is active.
 * 
 * Creator:                 A. Staub
 * Date of creation:        20.01.2016
 * Last modification on:    -
 * Modified by:             - 
 * 
 * Input:                   -
 * Output:                  -
***********************************************************************************************************************/
void cmd_GBIPPWM(void)
{
    if(g_CmdChk.uint8_ParamPos == 1)        //number of received characters OK?
    {
        if(A3981.CONFIG0.BITS.PWM == 0)     //PWM mode active?
        {
            uart2_sendbuffer('E');      //first the letter E
            uart2_sendbuffer(',');      //then the comma
            funct_IntToAscii(A3981.CONFIG0.BITS.TBK,_Active);
            uart2_sendbuffer(',');      //then the comma
            funct_IntToAscii(A3981.CONFIG0.BITS.TOF_FRQ,_Active);
            uart2_sendbuffer(13);      //then the CR     
        }
        else
        {
            g_Param.uint8_ErrCode = _BipGBIPPWM;        //set error code
            uart2_SendErrorCode(g_Param.uint8_ErrCode); //call subroutine         
        }       
    }
    else
    {
        g_Param.uint8_ErrCode = _NumbRecCharNotOK;  //set error code
        uart2_SendErrorCode(g_Param.uint8_ErrCode); //call subroutine
    } 
}   //end of cmd_GBIPPWM


/**********************************************************************************************************************
 * Routine:                 cmd_SBIPFRQ

 * Description:
 * Verify the received parameters of this command, if all parameters are within the tolerance.
 * If all is correct, then set the bipolar frequency mode according to the received parameters
 * 
 * Creator:                 A. Staub
 * Date of creation:        20.01.2016
 * Last modification on:    -
 * Modified by:             - 
 * 
 * Input:                   -
 * Output:                  -
***********************************************************************************************************************/
void cmd_SBIPFRQ(void)
{
    auto unsigned char uint8_Result = 0;    //local work byte
    
    if(g_CmdChk.uint8_ParamPos == 2)        //number of received characters OK?
    {
        if(g_CmdChk.uint8_GlobalLock == 1)  //global lock active?
        {
            g_Param.uint8_ErrCode = _MotorInRun;        //set error code
            uart2_SendErrorCode(g_Param.uint8_ErrCode); //call subroutine
        }
        else
        {
            //verify the limits if they are inside the tolerance
            uint8_Result += funct_CheckTol(g_CmdChk.uint32_TempPara[1],_BipFrqMin,_BipFrqMax);

            if(uint8_Result == 1)       //verify the result
            {
                A3981.CONFIG0.BITS.TOF_FRQ = g_CmdChk.uint32_TempPara[1];
                A3981.CONFIG0.BITS.PWM = 1; //mode = fixed frequency

                uart2_sendbuffer('E');      //first the letter E
                uart2_sendbuffer(13);       //then the CR
            }
            else
            {
                g_Param.uint8_ErrCode = _BipSBIPFRQ;        //set error code
                uart2_SendErrorCode(g_Param.uint8_ErrCode); //call subroutine
            }
        }
    }
    else
    {
        g_Param.uint8_ErrCode = _NumbRecCharNotOK;  //set error code
        uart2_SendErrorCode(g_Param.uint8_ErrCode); //call subroutine
    } 
}   //end of cmd_SBIPFRQ


/**********************************************************************************************************************
 * Routine:                 cmd_GBIPFRQ

 * Description:
 * Verify the received parameters of this command, if all parameters are within the tolerance.
 * If all is correct, then send back the bipolar frequency mode only if this mode is active.
 * 
 * Creator:                 A. Staub
 * Date of creation:        20.01.2016
 * Last modification on:    -
 * Modified by:             - 
 * 
 * Input:                   -
 * Output:                  -
***********************************************************************************************************************/
void cmd_GBIPFRQ(void)
{
    if(g_CmdChk.uint8_ParamPos == 1)        //number of received characters OK?
    {
        if(A3981.CONFIG0.BITS.PWM == 1)     //Frequency mode active?
        {
            uart2_sendbuffer('E');      //first the letter E
            uart2_sendbuffer(',');      //then the comma
            funct_IntToAscii(A3981.CONFIG0.BITS.TOF_FRQ,_Active);
            uart2_sendbuffer(13);      //then the CR     
        }
        else
        {
            g_Param.uint8_ErrCode = _BipGBIPFRQ;        //set error code
            uart2_SendErrorCode(g_Param.uint8_ErrCode); //call subroutine         
        }   
    }
    else
    {
        g_Param.uint8_ErrCode = _NumbRecCharNotOK;  //set error code
        uart2_SendErrorCode(g_Param.uint8_ErrCode); //call subroutine
    } 
}   //end of cmd_GBIPFRQ


/**********************************************************************************************************************
 * Routine:                 cmd_SBIPSLEW

 * Description:
 * Verify the received parameters of this command, if all parameters are within the tolerance.
 * If all is correct, then set the bipolar slew rate according to the received parameters
 * 
 * Creator:                 A. Staub
 * Date of creation:        20.01.2016
 * Last modification on:    -
 * Modified by:             - 
 * 
 * Input:                   -
 * Output:                  -
***********************************************************************************************************************/
void cmd_SBIPSLEW(void)
{
    auto unsigned char uint8_Result = 0;    //local work byte
    
    if(g_CmdChk.uint8_ParamPos == 2)        //number of received characters OK?
    {
        if(g_CmdChk.uint8_GlobalLock == 1)  //global lock active?
        {
            g_Param.uint8_ErrCode = _MotorInRun;        //set error code
            uart2_SendErrorCode(g_Param.uint8_ErrCode); //call subroutine
        }
        else
        {
            //verify the limits if they are inside the tolerance
            uint8_Result += funct_CheckTol(g_CmdChk.uint32_TempPara[1],_BipSlewMin,_BipSlewMax);

            if(uint8_Result == 1)       //verify the result
            {
                A3981.RUN.BITS.SLEW = g_CmdChk.uint32_TempPara[1];

                uart2_sendbuffer('E');      //first the letter E
                uart2_sendbuffer(13);       //then the CR
            }
            else
            {
                g_Param.uint8_ErrCode = _BipSBIPSLEW;        //set error code
                uart2_SendErrorCode(g_Param.uint8_ErrCode); //call subroutine
            }
        }
    }
    else
    {
        g_Param.uint8_ErrCode = _NumbRecCharNotOK;  //set error code
        uart2_SendErrorCode(g_Param.uint8_ErrCode); //call subroutine
    } 
}   //end of cmd_SBIPSLEW


/**********************************************************************************************************************
 * Routine:                 cmd_GBIPSLEW

 * Description:
 * Verify the received parameters of this command, if all parameters are within the tolerance.
 * If all is correct, then send back the bipolar slew rate 
 * 
 * Creator:                 A. Staub
 * Date of creation:        20.01.2016
 * Last modification on:    -
 * Modified by:             - 
 * 
 * Input:                   -
 * Output:                  -
***********************************************************************************************************************/
void cmd_GBIPSLEW(void)
{
    if(g_CmdChk.uint8_ParamPos == 1)        //number of received characters OK?
    {
        uart2_sendbuffer('E');      //first the letter E
        uart2_sendbuffer(',');      //then the comma
        funct_IntToAscii(A3981.RUN.BITS.SLEW,_Active);
        uart2_sendbuffer(13);      //then the CR
    }
    else
    {
        g_Param.uint8_ErrCode = _NumbRecCharNotOK;  //set error code
        uart2_SendErrorCode(g_Param.uint8_ErrCode); //call subroutine
    } 
}   //end of cmd_GBIPSLEW


/**********************************************************************************************************************
 * Routine:                 cmd_GBIPSST

 * Description:
 * Verify the received parameters of this command, if all parameters are within the tolerance.
 * If all is correct, then send back the bipolar stall status
 * 
 * Creator:                 A. Staub
 * Date of creation:        20.01.2016
 * Last modification on:    -
 * Modified by:             - 
 * 
 * Input:                   -
 * Output:                  -
***********************************************************************************************************************/
void cmd_GBIPSST(void)
{
    if(g_CmdChk.uint8_ParamPos == 1)        //number of received characters OK?
    {
        uart2_sendbuffer('E');      //first the letter E
        uart2_sendbuffer(',');      //then the comma
        funct_IntToAscii(A3981.FAULT1.BITS.ST,_Active);
        uart2_sendbuffer(13);      //then the CR
    }
    else
    {
        g_Param.uint8_ErrCode = _NumbRecCharNotOK;  //set error code
        uart2_SendErrorCode(g_Param.uint8_ErrCode); //call subroutine
    } 
}   //end of cmd_GBIPSST


/**********************************************************************************************************************
 * Routine:                 cmd_GVBAK

 * Description:
 * Send back the measured backup voltage of the button cell. Send the command starting with an X if 
 * the cut-off voltage Vcut is measured
 * 
 * Creator:                 A. Staub
 * Date of creation:        20.01.2016
 * Last modification on:    -
 * Modified by:             - 
 * 
 * Input:                   -
 * Output:                  -
***********************************************************************************************************************/
void cmd_GVBAK(void)
{  
    auto unsigned long int uint32_WB = 0;   //local work register for the result
    
    if(g_CmdChk.uint8_ParamPos == 1)   //number of received characters OK?
    {
        //measure here the voltage or take the last measured parameter and send it back!
        
        //convert the result in mV
        uint32_WB = funct_ADCtoMiliUnit(g_ADC.uint32_Battery,310);
        
        //Backup voltage less than the Vcut "cut-off" level
        //621 = (ADCres / 3.3V) * Vcut = (1024 / 3.3V) * 2.0V
        if(g_ADC.uint32_Battery <= 621)
        {
            uart2_sendbuffer('X');                      //first the letter X
            uart2_sendbuffer(',');                      //add the comma       
        }
        else
        {
            uart2_sendbuffer('E');                      //first the letter E
            uart2_sendbuffer(',');                      //add the comma 
        }
        //send back the needed informations
        funct_IntToAscii(uint32_WB,_Active);            //add the voltage
        uart2_sendbuffer('m');                          //add the m
        uart2_sendbuffer('V');                          //add the V symbol
        uart2_sendbuffer(13);                           //add the CR at the end      
    }
    else
    {
        g_Param.uint8_ErrCode = _NumbRecCharNotOK;  //set error code
        uart2_SendErrorCode(g_Param.uint8_ErrCode); //call subroutine
    }
}   //end of cmd_GVBAK


/**********************************************************************************************************************
 * Routine:                 cmd_SRESLIN

 * Description:
 * Verify the received parameters of this command, if all parameters are within the tolerance then set
 * up the response mode for the lin bus communication
 * 
 * Creator:                 A. Staub
 * Date of creation:        23.01.2016
 * Last modification on:    -
 * Modified by:             - 
 * 
 * Input:                   -
 * Output:                  -
***********************************************************************************************************************/
void cmd_SRESLIN(void)
{
    auto unsigned char uint8_Result = 0;    //local work byte
    
    if(g_CmdChk.uint8_ParamPos == 2)        //number of received characters OK?
    {
        if(g_CmdChk.uint8_GlobalLock == 1)  //global lock active?
        {
            g_Param.uint8_ErrCode = _MotorInRun;        //set error code
            uart2_SendErrorCode(g_Param.uint8_ErrCode); //call subroutine
        }
        else
        {
            //verify the limits if they are inside the tolerance
            uint8_Result += funct_CheckTol(g_CmdChk.uint32_TempPara[1],0,1);

            if(uint8_Result == 1)       //verify the result
            {
                //store the new timeout
                g_Param.uint8_LinRes = g_CmdChk.uint32_TempPara[1];

                uart2_sendbuffer('E');      //first the letter E
                uart2_sendbuffer(13);       //then the CR
            }
            else
            {
                g_Param.uint8_ErrCode = _OutOfTolSRESLIN;   //set error code
                uart2_SendErrorCode(g_Param.uint8_ErrCode); //call subroutine
            }
        }
    }
    else
    {
        g_Param.uint8_ErrCode = _NumbRecCharNotOK;  //set error code
        uart2_SendErrorCode(g_Param.uint8_ErrCode); //call subroutine
    } 
}   //end of cmd_SRESLIN


/**********************************************************************************************************************
 * Routine:                 cmd_GRESLIN

 * Description:
 * Verify the received parameters of this command, if all parameters are within the tolerance.
 * If all is correct, then send back the lin bus response mode
 * 
 * Creator:                 A. Staub
 * Date of creation:        23.01.2016
 * Last modification on:    -
 * Modified by:             - 
 * 
 * Input:                   -
 * Output:                  -
***********************************************************************************************************************/
void cmd_GRESLIN(void)
{
    if(g_CmdChk.uint8_ParamPos == 1)        //number of received characters OK?
    {
        uart2_sendbuffer('E');      //first the letter E
        uart2_sendbuffer(',');      //then the comma
        funct_IntToAscii(g_Param.uint8_LinRes,_Active);
        uart2_sendbuffer(13);      //then the CR
    }
    else
    {
        g_Param.uint8_ErrCode = _NumbRecCharNotOK;  //set error code
        uart2_SendErrorCode(g_Param.uint8_ErrCode); //call subroutine
    } 
}   //end of cmd_GRESLIN


/**********************************************************************************************************************
 * Routine:                 cmd_SFRQBIT

 * Description:
 * Verify the received parameters of this command, if all parameters are within the tolerance then set
 * up the frequency bit
 * 
 * Creator:                 A. Staub
 * Date of creation:        06.02.2016
 * Last modification on:    -
 * Modified by:             - 
 * 
 * Input:                   -
 * Output:                  -
***********************************************************************************************************************/
void cmd_SFRQBIT(void)
{
    auto unsigned char uint8_Result = 0;    //local work byte
    
    if(g_CmdChk.uint8_ParamPos == 2)        //number of received characters OK?
    {
        //verify the limits if they are inside the tolerance
        uint8_Result += funct_CheckTol(g_CmdChk.uint32_TempPara[1],_SFRQbitMin,_SFRQbitMax);
        
        if(uint8_Result == 1)       //verify the result
        {
            //store the new timeout
            g_Param.uint8_FreqBit = g_CmdChk.uint32_TempPara[1];
            
            uart2_sendbuffer('E');      //first the letter E
            uart2_sendbuffer(13);       //then the CR
        }
        else
        {
            g_Param.uint8_ErrCode = _OutOfTolSFRQBIT;   //set error code
            uart2_SendErrorCode(g_Param.uint8_ErrCode); //call subroutine
        }
    }
    else
    {
        g_Param.uint8_ErrCode = _NumbRecCharNotOK;  //set error code
        uart2_SendErrorCode(g_Param.uint8_ErrCode); //call subroutine
    } 
}   //end of cmd_SFRQBIT


/**********************************************************************************************************************
 * Routine:                 cmd_GFRQBIT

 * Description:
 * Verify the received parameters of this command, if all parameters are within the tolerance.
 * If all is correct, then send back the state of the frequency bit
 * 
 * Creator:                 A. Staub
 * Date of creation:        06.02.2016
 * Last modification on:    -
 * Modified by:             - 
 * 
 * Input:                   -
 * Output:                  -
***********************************************************************************************************************/
void cmd_GFRQBIT(void)
{
    if(g_CmdChk.uint8_ParamPos == 1)        //number of received characters OK?
    {
        uart2_sendbuffer('E');      //first the letter E
        uart2_sendbuffer(',');      //then the comma
        funct_IntToAscii(g_Param.uint8_FreqBit,_Active);
        uart2_sendbuffer(13);      //then the CR
    }
    else
    {
        g_Param.uint8_ErrCode = _NumbRecCharNotOK;  //set error code
        uart2_SendErrorCode(g_Param.uint8_ErrCode); //call subroutine
    } 
}   //end of cmd_GFRQBIT


/**********************************************************************************************************************
 * Routine:                 cmd_SRUNBIT

 * Description:
 * Verify the received parameters of this command, if all parameters are within the tolerance then set
 * up the run bit
 * 
 * Creator:                 A. Staub
 * Date of creation:        15.02.2016
 * Last modification on:    -
 * Modified by:             - 
 * 
 * Input:                   -
 * Output:                  -
***********************************************************************************************************************/
void cmd_SRUNBIT(void)
{
    auto unsigned char uint8_Result = 0;    //local work byte
    
    if(g_CmdChk.uint8_ParamPos == 2)        //number of received characters OK?
    {
        //verify the limits if they are inside the tolerance
        uint8_Result += funct_CheckTol(g_CmdChk.uint32_TempPara[1],_SRUNbitMin,_SRUNbitMax);
        
        if(uint8_Result == 1)       //verify the result
        {
            //store the new timeout
            g_Param.uint8_RunBit = g_CmdChk.uint32_TempPara[1];
            
            uart2_sendbuffer('E');      //first the letter E
            uart2_sendbuffer(13);       //then the CR
        }
        else
        {
            g_Param.uint8_ErrCode = _OutOfTolSRUNBIT;   //set error code
            uart2_SendErrorCode(g_Param.uint8_ErrCode); //call subroutine
        }
    }
    else
    {
        g_Param.uint8_ErrCode = _NumbRecCharNotOK;  //set error code
        uart2_SendErrorCode(g_Param.uint8_ErrCode); //call subroutine
    } 
}   //end of cmd_SRUNBIT


/**********************************************************************************************************************
 * Routine:                 cmd_GRUNBIT

 * Description:
 * Verify the received parameters of this command, if all parameters are within the tolerance.
 * If all is correct, then send back the state of the run bit
 * 
 * Creator:                 A. Staub
 * Date of creation:        15.02.2016
 * Last modification on:    -
 * Modified by:             - 
 * 
 * Input:                   -
 * Output:                  -
***********************************************************************************************************************/
void cmd_GRUNBIT(void)
{
    if(g_CmdChk.uint8_ParamPos == 1)        //number of received characters OK?
    {
        uart2_sendbuffer('E');      //first the letter E
        uart2_sendbuffer(',');      //then the comma
        funct_IntToAscii(g_Param.uint8_RunBit,_Active);
        uart2_sendbuffer(13);      //then the CR
    }
    else
    {
        g_Param.uint8_ErrCode = _NumbRecCharNotOK;  //set error code
        uart2_SendErrorCode(g_Param.uint8_ErrCode); //call subroutine
    } 
}   //end of cmd_GRUNBIT


/**********************************************************************************************************************
 * Routine:                 cmd_RTESTIN

 * Description:
 * This subroutine does first check if the test is for bipolar, unipolar or matrix motor. If not then it sends
 * back an error. If true it switches off every supply and measures during 3s the supply Vmot. 
 * If this supply goes under 0.3V then the tests starts. If not it sends back an error. 
 * For the tests:
 * One coil after one is switched on and off to measure the voltage by appling 10mA current source. 
 * This gives us the resistor value at the end. Each coil measure takes around 100ms. 
 * 
 * Creator:                 A. Staub
 * Date of creation:        15.02.2016
 * Last modification on:    -
 * Modified by:             - 
 * 
 * Input:                   -
 * Output:                  -
***********************************************************************************************************************/
void cmd_RTESTIN(void)
{
    volatile unsigned long int uint32_WB;   
    
    if(g_CmdChk.uint8_ParamPos == 2)   //number of received characters OK?
    {
        if(g_CmdChk.uint8_GlobalLock == 1)  //global lock active?
        {
            g_Param.uint8_ErrCode = _MotorInRun;        //set error code
            uart2_SendErrorCode(g_Param.uint8_ErrCode); //call subroutine
        }
        else
        {
            g_CmdChk.uint8_GlobalLock = 1;  //enable global lock
            
            ads1115_SetChannel(_AIN0p_GND,_FS4096mV);   //set channel on Vmot
            
            //test if unipolar, bipolar or matrix?
            if((g_CmdChk.uint32_TempPara[1] == 'U') || (g_CmdChk.uint32_TempPara[1] == 'B') || 
              (g_CmdChk.uint32_TempPara[1] == 'M'))
            {
                oVmotOnOff = 0;                 //switch off the main supply
                oBiEnaVmot = 0;                 //switch off the bipolar supply
                oEnaVLINSupply = 0;             //switch off the lin supply

                g_Timer1.uint8_TimeoutFlag = 1;     //set the timeout flag
                SetTimer(_TIMER1,_ENABLE,0,3000);   //load the timer with an timeout of 3s

                while(g_Timer1.uint8_TimeoutFlag)   //do this until the flag is low
                {
                    adc_LaunchNextMeasure();        //call subroutine 
                    if(g_ADC.uint32_Vmot <= 100)    //voltage smaller than 0.322V
                    {
                        g_Timer1.uint8_TimeoutFlag = 0; //clear timeout flag inside this loop
                    }
                }

                if(g_ADC.uint32_Vmot <= 100)    //test it again
                {
                    oEnaCoilResMeas = 1;        //enable the relay for the resistor     
                    oEnaCurrSource = 1;         //enable the current source

                    ads1115_SetChannel(_AIN0p_GND,_FS4096mV);   //set channel on Vmot

                    if((g_CmdChk.uint32_TempPara[1] == 'U') || (g_CmdChk.uint32_TempPara[1] == 'M'))
                    {
                        uart2_sendbuffer('E');              //first the letter E
                        uart2_sendbuffer(',');              //add the comma

                        //measure coil A1
                        oUniCoilA1 = 1;                     //switch on coil A1
                        g_Timer1.uint8_TimeoutFlag = 1;     //set the timeout flag
                        SetTimer(_TIMER1,_ENABLE,0,100);    //load the timer with 100ms

                        while(g_Timer1.uint8_TimeoutFlag)   //rest in the while until flag is reseted
                        {
                            //do nothing
                        }  
                        uint32_WB = ads1115_read();         //read out Vmot
                        oUniCoilA1 = 0;                     //switch off coil A1

                        if(uint32_WB)   //convert result in mV only if it is not 0
                        {
                            uint32_WB = funct_ADCtoMiliUnit(uint32_WB,8000);                             
                        }
                        funct_MiliVoltToOhm(uint32_WB); //convert into ohm and put it into the sendbuffer
                        uart2_sendbuffer(',');              //add the comma

                        //measure coil A2
                        oUniCoilA2 = 1;                     //switch on coil A2
                        g_Timer1.uint8_TimeoutFlag = 1;     //set the timeout flag
                        SetTimer(_TIMER1,_ENABLE,0,100);    //load the timer with 100ms

                        while(g_Timer1.uint8_TimeoutFlag)   //rest in the while until flag is reseted
                        {
                            //do nothing
                        }           
                        uint32_WB = ads1115_read();         //read out Vmot
                        oUniCoilA2 = 0;                     //switch off coil A2

                        if(uint32_WB)   //convert result in mV only if it is not 0
                        {
                            uint32_WB = funct_ADCtoMiliUnit(uint32_WB,8000);                             
                        }
                        funct_MiliVoltToOhm(uint32_WB); //convert into ohm and put it into the sendbuffer
                        uart2_sendbuffer(',');              //add the comma

                        //measure coil B1
                        oUniCoilB1 = 1;                     //switch on coil B1
                        g_Timer1.uint8_TimeoutFlag = 1;     //set the timeout flag
                        SetTimer(_TIMER1,_ENABLE,0,100);    //load the timer with 100ms

                        while(g_Timer1.uint8_TimeoutFlag)   //rest in the while until flag is reseted
                        {
                            //do nothing
                        }               
                        uint32_WB = ads1115_read();         //read out Vmot
                        oUniCoilB1 = 0;                     //switch off coil B1

                        if(uint32_WB)   //convert result in mV only if it is not 0
                        {
                            uint32_WB = funct_ADCtoMiliUnit(uint32_WB,8000);                             
                        }
                        funct_MiliVoltToOhm(uint32_WB); //convert into ohm and put it into the sendbuffer
                        uart2_sendbuffer(',');              //add the comma

                        //measure coil B2
                        oUniCoilB2 = 1;                     //switch on coil B2
                        g_Timer1.uint8_TimeoutFlag = 1;     //set the timeout flag
                        SetTimer(_TIMER1,_ENABLE,0,100);    //load the timer with 100ms

                        while(g_Timer1.uint8_TimeoutFlag)   //rest in the while until flag is reseted
                        {
                            //do nothing
                        }    
                        uint32_WB = ads1115_read();         //read out Vmot
                        oUniCoilB2 = 0;                     //switch off coil B2

                        if(uint32_WB)   //convert result in mV only if it is not 0
                        {
                            uint32_WB = funct_ADCtoMiliUnit(uint32_WB,8000);                             
                        }
                        funct_MiliVoltToOhm(uint32_WB); //convert into ohm and put it into the sendbuffer
                        uart2_sendbuffer(13);                           //add the CR at the end
                    }
                    else if (g_CmdChk.uint32_TempPara[1] == 'B')
                    {
                        uart2_sendbuffer('E');              //first the letter E
                        uart2_sendbuffer(',');              //add the comma

                        //measure coil A
                        oBiRelayCoilA = 1;                  //switch on relay for coil A
                        g_Timer1.uint8_TimeoutFlag = 1;     //set the timeout flag
                        SetTimer(_TIMER1,_ENABLE,0,100);    //load the timer with 100ms

                        while(g_Timer1.uint8_TimeoutFlag)   //rest in the while until flag is reseted
                        {
                            //do nothing
                        }         
                        uint32_WB = ads1115_read();         //read out Vmot
                        oBiRelayCoilA = 0;                  //switch off relay forcoil A

                        if(uint32_WB)   //convert result in mV only if it is not 0
                        {
                            uint32_WB = funct_ADCtoMiliUnit(uint32_WB,8000);                             
                        }
                        funct_MiliVoltToOhm(uint32_WB); //convert into ohm and put it into the sendbuffer
                        uart2_sendbuffer(',');              //add the comma
                        funct_MiliVoltToOhm(uint32_WB); //convert into ohm and put it into the sendbuffer
                        uart2_sendbuffer(',');              //add the comma

                        //measure coil B
                        oBiRelayCoilB = 1;                  //switch on relay for coil B
                        g_Timer1.uint8_TimeoutFlag = 1;     //set the timeout flag
                        SetTimer(_TIMER1,_ENABLE,0,100);    //load the timer with 100ms

                        while(g_Timer1.uint8_TimeoutFlag)   //rest in the while until flag is reseted
                        {
                            //do nothing
                        } 
                        uint32_WB = ads1115_read();         //read out Vmot
                        oBiRelayCoilB = 0;                  //switch off relay forcoil B

                        if(uint32_WB)   //convert result in mV only if it is not 0
                        {
                            uint32_WB = funct_ADCtoMiliUnit(uint32_WB,8000);                             
                        }
                        funct_MiliVoltToOhm(uint32_WB); //convert into ohm and put it into the sendbuffer
                        uart2_sendbuffer(',');              //add the comma
                        funct_MiliVoltToOhm(uint32_WB); //convert into ohm and put it into the sendbuffer
                        uart2_sendbuffer(13);                           //add the CR at the end
                    }
                    else
                    {
                        //do nothing, error will be send before
                    }

                    oEnaCoilResMeas = 0;        //disable the relay for the resistor
                    oEnaCurrSource = 0;         //disable the current source
                }
                else
                {
                    uart2_sendbuffer('X');                          //first the letter E
                    uart2_sendbuffer(',');                          //then the comma
                    g_Param.uint8_ErrCode = _RTESTINsupply;         //set error code
                    funct_IntToAscii(g_Param.uint8_ErrCode,_Active);
                    uart2_sendbuffer(',');                          //then the comma
                    if(g_ADC.uint32_Vmot)   //convert result in mV only if it is not 0
                    {
                       uint32_WB = funct_ADCtoMiliUnit(g_ADC.uint32_Vmot,18);                             
                    }
                    funct_IntToAscii(uint32_WB,_Active);            //add the voltage
                    uart2_sendbuffer('m');                          //add the m
                    uart2_sendbuffer('V');                          //add the V
                    uart2_sendbuffer(13);                           //add the CR at the end
                }
            }
            else
            {
                g_Param.uint8_ErrCode = _RTESTINnotPossible;    //set error code
                uart2_SendErrorCode(g_Param.uint8_ErrCode);     //call subroutine
            }
            
            g_CmdChk.uint8_GlobalLock = 0;  //disable global lock
        }
    }
    else
    {
        g_Param.uint8_ErrCode = _NumbRecCharNotOK;  //set error code
        uart2_SendErrorCode(g_Param.uint8_ErrCode); //call subroutine
    }
}   //end of cmd_RTESTIN


/**********************************************************************************************************************
 * Routine:                 cmd_SPROD

 * Description:             Writing in EEPROM of all productions information.
 * 
 * Modification 27.02.2016 / A. Staub:
 * Modified the code for that it works. 
 * 
 * Creator:                 J. Rebetez
 * Date of creation:        17.02.2016
 * Last modification on:    27.02.2016
 * Modified by:             A. Staub
 * 
 * Input:                   -
 * Output:                  -
***********************************************************************************************************************/
void cmd_SPROD(void)
{
    volatile unsigned char uint8_Result = 0;    //local work byte 
    volatile unsigned char uint8_WB;            //local work byte
    volatile unsigned long int uint32_WB;       //local work byte
    
    if(g_CmdChk.uint8_ParamPos == 12)       //number of received characters OK?
    {
        if(g_CmdChk.uint8_GlobalLock == 1)  //global lock active?
        {
            g_Param.uint8_ErrCode = _MotorInRun;        //set error code
            uart2_SendErrorCode(g_Param.uint8_ErrCode); //call subroutine
        }
        else
        {
            g_CmdChk.uint8_GlobalLock = 1;  //enable global lock
            
            //control of all received values
            uint8_Result = 1;   //because the serial number is 32bit lenght
            uint8_Result += funct_CheckTol(g_CmdChk.uint32_TempPara[2],0,99);   //year
            uint8_Result += funct_CheckTol(g_CmdChk.uint32_TempPara[3],0,52);   //week
            uint8_Result += funct_CheckTol(g_CmdChk.uint32_TempPara[4],0,1);    //test status
            uint8_Result += funct_CheckTol(g_CmdChk.uint32_TempPara[5],0,256);  //firmware 1. number
            uint8_Result += funct_CheckTol(g_CmdChk.uint32_TempPara[6],0,256);  //firmware 2. number
            uint8_Result += funct_CheckTol(g_CmdChk.uint32_TempPara[7],0,256);  //firmware 3. number
            uint8_Result += funct_CheckTol(g_CmdChk.uint32_TempPara[8],0,256);  //firmware 4. number
            uint8_Result += funct_CheckTol(g_CmdChk.uint32_TempPara[9],0,256);  //firmware 5. number
            uint8_Result += funct_CheckTol(g_CmdChk.uint32_TempPara[10],0,256); //firmware 6. number
            uint8_Result += funct_CheckTol(g_CmdChk.uint32_TempPara[11],65,90); //hardware 'A' - 'Z' OR
            uint8_Result += funct_CheckTol(g_CmdChk.uint32_TempPara[11],97,122); //hardware 'a' - 'z'

            if(uint8_Result == 11)  //all parameters correct?
            {
                //store the parameters into the variables
                g_Prod.uint32_SN = g_CmdChk.uint32_TempPara[1];
                g_Prod.uint8_year = g_CmdChk.uint32_TempPara[2];
                g_Prod.uint8_week = g_CmdChk.uint32_TempPara[3];
                g_Prod.uint8_TS = g_CmdChk.uint32_TempPara[4];
                g_Prod.uint8_FW[0] = g_CmdChk.uint32_TempPara[5];
                g_Prod.uint8_FW[1] = g_CmdChk.uint32_TempPara[6];
                g_Prod.uint8_FW[2] = g_CmdChk.uint32_TempPara[7];
                g_Prod.uint8_FW[3] = g_CmdChk.uint32_TempPara[8];
                g_Prod.uint8_FW[4] = g_CmdChk.uint32_TempPara[9];
                g_Prod.uint8_FW[5] = g_CmdChk.uint32_TempPara[10];
                g_Prod.uint8_HW = g_CmdChk.uint32_TempPara[11];

                //store the parameters into the EEPROM

                //serial number LSB
                uint8_WB = g_Prod.uint32_SN & 0xFF;
                ROM24LC256_WrByte(EE_ADDR_H,EE_ADDR_L_SERIAL_NUMBER1,uint8_WB);

                //serial number part 2
                g_Timer1.uint8_TimeoutFlag = 1;     //set the timeout flag
                SetTimer(_TIMER1,_ENABLE,0,50);     //load the timer with 50ms 
                while(g_Timer1.uint8_TimeoutFlag);  //wait the time
                uint32_WB = g_Prod.uint32_SN >> 8;
                uint8_WB = uint32_WB & 0xFF;
                ROM24LC256_WrByte(EE_ADDR_H,EE_ADDR_L_SERIAL_NUMBER2,uint8_WB);

                //serial number part 3
                g_Timer1.uint8_TimeoutFlag = 1;     //set the timeout flag
                SetTimer(_TIMER1,_ENABLE,0,50);     //load the timer with 50ms
                while(g_Timer1.uint8_TimeoutFlag);  //wait the time
                uint32_WB = g_Prod.uint32_SN >> 16;
                uint8_WB = uint32_WB & 0xFF;
                ROM24LC256_WrByte(EE_ADDR_H,EE_ADDR_L_SERIAL_NUMBER3,uint8_WB);

                //serial number MSB
                g_Timer1.uint8_TimeoutFlag = 1;     //set the timeout flag
                SetTimer(_TIMER1,_ENABLE,0,50);     //load the timer with 50ms
                while(g_Timer1.uint8_TimeoutFlag);  //wait the time
                uint32_WB = g_Prod.uint32_SN >> 24;
                uint8_WB = uint32_WB & 0xFF;
                ROM24LC256_WrByte(EE_ADDR_H,EE_ADDR_L_SERIAL_NUMBER4,uint8_WB);

                //production year
                g_Timer1.uint8_TimeoutFlag = 1;     //set the timeout flag
                SetTimer(_TIMER1,_ENABLE,0,50);     //load the timer with 50ms
                while(g_Timer1.uint8_TimeoutFlag);  //wait the time
                ROM24LC256_WrByte(EE_ADDR_H,EE_ADDR_L_PROD_YEAR,g_Prod.uint8_year);

                //production week
                g_Timer1.uint8_TimeoutFlag = 1;     //set the timeout flag
                SetTimer(_TIMER1,_ENABLE,0,50);     //load the timer with 50ms
                while(g_Timer1.uint8_TimeoutFlag);  //wait the time
                ROM24LC256_WrByte(EE_ADDR_H,EE_ADDR_L_PROD_WEEK,g_Prod.uint8_week);

                //test status
                g_Timer1.uint8_TimeoutFlag = 1;     //set the timeout flag
                SetTimer(_TIMER1,_ENABLE,0,50);     //load the timer with 50ms
                while(g_Timer1.uint8_TimeoutFlag);  //wait the time
                ROM24LC256_WrByte(EE_ADDR_H,EE_ADDR_L_TEST_STATUS,g_Prod.uint8_TS);

                //firmware x.
                g_Timer1.uint8_TimeoutFlag = 1;     //set the timeout flag
                SetTimer(_TIMER1,_ENABLE,0,50);     //load the timer with 50ms
                while(g_Timer1.uint8_TimeoutFlag);  //wait the time
                ROM24LC256_WrByte(EE_ADDR_H,EE_ADDR_L_FW_VERSION_1,g_Prod.uint8_FW[0]);

                //firmware x.x
                g_Timer1.uint8_TimeoutFlag = 1;     //set the timeout flag
                SetTimer(_TIMER1,_ENABLE,0,50);     //load the timer with 50ms
                while(g_Timer1.uint8_TimeoutFlag);  //wait the time
                ROM24LC256_WrByte(EE_ADDR_H,EE_ADDR_L_FW_VERSION_2,g_Prod.uint8_FW[1]);

                //firmware x.xx
                g_Timer1.uint8_TimeoutFlag = 1;     //set the timeout flag
                SetTimer(_TIMER1,_ENABLE,0,50);     //load the timer with 50ms
                while(g_Timer1.uint8_TimeoutFlag);  //wait the time
                ROM24LC256_WrByte(EE_ADDR_H,EE_ADDR_L_FW_VERSION_3,g_Prod.uint8_FW[2]);

                //firmware x.xx.x
                g_Timer1.uint8_TimeoutFlag = 1;     //set the timeout flag
                SetTimer(_TIMER1,_ENABLE,0,50);     //load the timer with 50ms
                while(g_Timer1.uint8_TimeoutFlag);  //wait the time
                ROM24LC256_WrByte(EE_ADDR_H,EE_ADDR_L_FW_VERSION_4,g_Prod.uint8_FW[3]);

                //firmware x.xx.xx
                g_Timer1.uint8_TimeoutFlag = 1;     //set the timeout flag
                SetTimer(_TIMER1,_ENABLE,0,50);     //load the timer with 50ms
                while(g_Timer1.uint8_TimeoutFlag);  //wait the time
                ROM24LC256_WrByte(EE_ADDR_H,EE_ADDR_L_FW_VERSION_5,g_Prod.uint8_FW[4]);

                //firmware x.xx.xxx
                g_Timer1.uint8_TimeoutFlag = 1;     //set the timeout flag
                SetTimer(_TIMER1,_ENABLE,0,50);     //load the timer with 50ms
                while(g_Timer1.uint8_TimeoutFlag);  //wait the time
                ROM24LC256_WrByte(EE_ADDR_H,EE_ADDR_L_FW_VERSION_6,g_Prod.uint8_FW[5]);

                //hardware version
                g_Timer1.uint8_TimeoutFlag = 1;     //set the timeout flag
                SetTimer(_TIMER1,_ENABLE,0,50);     //load the timer with 50ms
                while(g_Timer1.uint8_TimeoutFlag);  //wait the time
                ROM24LC256_WrByte(EE_ADDR_H,EE_ADDR_L_HW_VERSION,g_Prod.uint8_HW);

                //send back the feedback
                uart2_sendbuffer('E');      //first the letter E
                uart2_sendbuffer(13);       //then the CR
            }
            else
            {
                g_Param.uint8_ErrCode = _OutOfTolSPROD;  //set error code
                uart2_SendErrorCode(g_Param.uint8_ErrCode); //call subroutine
            }
            
            g_CmdChk.uint8_GlobalLock = 0;  //disable global lock
        }
    }
    else
    {
        g_Param.uint8_ErrCode = _NumbRecCharNotOK;  //set error code
        uart2_SendErrorCode(g_Param.uint8_ErrCode); //call subroutine
    }
}   


/**********************************************************************************************************************
 * Routine:                 cmd_STAT

 * Description:
 * Verify that the customer send only STAT. If ok send back the status of the SMMA
 * 
 * Creator:                 A. Staub
 * Date of creation:        15.02.2016
 * Last modification on:    -
 * Modified by:             - 
 * 
 * Input:                   -
 * Output:                  -
***********************************************************************************************************************/
void cmd_STAT(void)
{
    if(g_CmdChk.uint8_ParamPos == 1)        //number of received characters OK?
    {
        uart2_sendbuffer('E');      //first the letter E
        uart2_sendbuffer(',');      //then the comma
        funct_IntToAscii(RCONbits.POR,_Active);
        uart2_sendbuffer(',');      //then the comma
        funct_IntToAscii(RCONbits.BOR,_Active);
        uart2_sendbuffer(',');      //then the comma
        funct_IntToAscii(RCONbits.IDLE,_Active);
        uart2_sendbuffer(',');      //then the comma
        funct_IntToAscii(RCONbits.SLEEP,_Active);
        uart2_sendbuffer(',');      //then the comma
        funct_IntToAscii(RCONbits.WDTO,_Active);
        uart2_sendbuffer(',');      //then the comma
        funct_IntToAscii(RCONbits.SWR,_Active);
        uart2_sendbuffer(',');      //then the comma
        funct_IntToAscii(RCONbits.EXTR,_Active);
        uart2_sendbuffer(',');      //then the comma
        funct_IntToAscii(RCONbits.CMR,_Active);
        uart2_sendbuffer(13);      //then the CR
        
        //reset the errors
        RCONbits.POR = 0;
        RCONbits.BOR = 0;
        RCONbits.IDLE = 0;
        RCONbits.SLEEP = 0;
        RCONbits.WDTO = 0;
        RCONbits.SWR = 0;
        RCONbits.EXTR = 0;
        RCONbits.CMR = 0;
    }
    else
    {
        g_Param.uint8_ErrCode = _NumbRecCharNotOK;  //set error code
        uart2_SendErrorCode(g_Param.uint8_ErrCode); //call subroutine
    } 
}   //end of cmd_STAT
