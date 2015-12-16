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
***********************************************************************************************************************/


#include "includes.h" 

extern SCmdChk g_CmdChk;
extern SFunct g_Funct;
extern Si2c1 g_i2c1;
extern SParam g_Param;
extern STimer2 g_Timer2;
extern SUni g_Uni;
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
  
    if(g_CmdChk.uint8_ParamPos == 6)   //number of received characters OK?
    {
        if(g_Uni.uint8_Settings & 0x01)             //is unipolar motor in run mode?
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
    if(g_CmdChk.uint8_ParamPos == 1)   //number of received characters OK?
    {
        if(g_Uni.uint8_Settings & 0x01)             //is unipolar motor in run mode?
        {
            g_Param.uint8_ErrCode = _MotorInRun;        //set error code
            uart2_SendErrorCode(g_Param.uint8_ErrCode); //call subroutine
        }
        else
        {
            if(g_Param.uint8_MotTyp == 'L')         //motor type = Lin?
            {
                g_Param.uint8_ErrCode = _LinETESTIN;        //set error code
                uart2_SendErrorCode(g_Param.uint8_ErrCode); //call subroutine            
            }
            else if(g_Param.uint8_MotTyp == 'B')    //motor type = Bipolar?
            {
                //starts the measure
            
                //convert the result
            
                //verify if within tolerance
            
                //send back the result
                g_Param.uint8_ErrCode = _BipETESTIN;        //set error code
                uart2_SendErrorCode(g_Param.uint8_ErrCode); //call subroutine
            }
            else if((g_Param.uint8_MotTyp == 'U') || (g_Param.uint8_MotTyp == 'M'))    //motor type = Unipolar or matrix?
            {
                //starts the measure
            
                //convert the result
            
                //verify if within tolerance
            
                //send back the result
                g_Param.uint8_ErrCode = _UniETESTIN;        //set error code
                uart2_SendErrorCode(g_Param.uint8_ErrCode); //call subroutine
            }
            else
            {
                g_Param.uint8_ErrCode = _UnknownMotTyp;        //set error code
                uart2_SendErrorCode(g_Param.uint8_ErrCode); //call subroutine
            }
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
            if(g_Uni.uint8_Settings & 0x01)         //is motor in run mode?
            {
                g_Param.uint8_ErrCode = _MotorInRun;        //set error code
                uart2_SendErrorCode(g_Param.uint8_ErrCode); //call subroutine
            }
            else
            {
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
            if(g_Uni.uint8_Settings & 0x01)     //is motor in run mode?
            {
                g_Param.uint8_ErrCode = _MotorInRun;        //set error code
                uart2_SendErrorCode(g_Param.uint8_ErrCode); //call subroutine
            }
            else
            {
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
 * unipolar driver this is still not possible. 
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
 * ...
 * 
 * Creator:                 A. Staub
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
    
    if(g_CmdChk.uint8_ParamPos == 15)  //number of received characters OK?
    {
        if(g_Uni.uint8_Settings & 0x01)         //is motor in run mode?
        {
            g_Param.uint8_ErrCode = _MotorInRun;        //set error code
            uart2_SendErrorCode(g_Param.uint8_ErrCode); //call subroutine
        }
        else
        {
            uint8_Result += funct_CheckTol(g_CmdChk.uint32_TempPara[2],_StepModeUniMatMin,_StepModeUniMatMax);
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
                }
                //type of motor = unipolar or matrix?
                else if((g_CmdChk.uint32_TempPara[1] == 'U') || (g_CmdChk.uint32_TempPara[1] == 'M'))
                {
                    //to decide what is to do when motor type changes
                    g_Param.uint8_MotTyp = g_CmdChk.uint32_TempPara[1] & 0xFF;    
                           
                    g_Uni.uint8_Settings |= 0x01;   //enable run bit
          
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
                        g_Uni.uint8_Settings = 0;   //erase settings
                    }
          
                    //define switch ON time - first convert and then store it
                    funct_msToTimer2(g_Param.uint16_AccOnDelay,g_Timer2.uint16_IntTime);
                    g_Uni.uint16_SwOnLastTime = g_Uni.uint16_LastTime;
                    g_Uni.uint16_SwOnCount = g_Uni.uint16_Count;
          
                    //define switch OFF time - first convert and then store it
                    funct_msToTimer2(g_Param.uint16_DecOffDelay,g_Timer2.uint16_IntTime);
                    g_Uni.uint16_SwOffLastTime = g_Uni.uint16_LastTime;
                    g_Uni.uint16_SwOffCount = g_Uni.uint16_Count;
          
                    if(g_Uni.uint8_Settings)    //verify if settings are 0, this means there was something wrong
                    {
                        //nothing wrong, nothing to do
                    }
                    else
                    {
                        g_Param.uint8_ErrCode = _OutOfTolRUN;       //set error code
                        uart2_SendErrorCode(g_Param.uint8_ErrCode); //call subroutine
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
            g_Param.uint8_ErrCode = _BipSPHC;           //set error code
            uart2_SendErrorCode(g_Param.uint8_ErrCode); //call subroutine
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
        RV30xx_TempMeas();
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
        if(g_Uni.uint8_Settings & 0x01)                 //is motor in run mode?
        {
            g_Param.uint8_ErrCode = _MotorInRun;        //set error code
            uart2_SendErrorCode(g_Param.uint8_ErrCode); //call subroutine
        }
        else
        {
            //send back a first quittance
            uart2_sendbuffer('E');
            uart2_sendbuffer(13);
        
            //set all back to the default value
        
            //switch off the output's, etc.
        
            //send back the device information's
            funct_LoadDeviceInfo();                     //call subroutine      
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
 * Only allowed if the motor is not in run. Before the system changes to another driver (switching on the
 * output/supply) it verifies if the set limits are within the tolerances. If one parameter/limit is not
 * within the tolerance, the system changes nothing and send back the corresponding error. 
 * If all is correct, it sends back an E<CR>. 
 * 
 * Creator:                 A. Staub
 * Date of creation:        19.10.2015
 * Last modification on:    -
 * Modified by:             - 
 * 
 * Input:                   -
 * Output:                  -
***********************************************************************************************************************/
void cmd_SMTYP(void)
{
    auto unsigned char uint8_Result = 0;        //local work byte for the result 
  
    if(g_CmdChk.uint8_ParamPos == 2)   //number of received characters OK?
    {
        if(g_Uni.uint8_Settings & 0x01)             //is unipolar motor in run mode?
        {
            g_Param.uint8_ErrCode = _MotorInRun;        //set error code
            uart2_SendErrorCode(g_Param.uint8_ErrCode); //call subroutine
        }
        else
        {
            uint8_Result = funct_CheckCmdSMTYP();   //call subroutine
            
            if(uint8_Result == 5)   //each parameter within the tolerance?
            {
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
 * Perform a self-test sequence only if possible if there is nothing activated, running, etc. 
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
        //to define!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    
        uart2_sendbuffer('E');                      //first the letter E
        uart2_sendbuffer(13);                       //add the CR at the end      
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
  
        //convert the minimal ON current into an ascii and store it into the send buffer
        funct_IntToAscii(g_Param.uint16_Imin,_Active);
        uart2_sendbuffer(',');                  //add the comma
  
        //convert the maximal ON current into an ascii and store it into the send buffer
        funct_IntToAscii(g_Param.uint16_Imax,_Active);
        uart2_sendbuffer(',');                  //add the comma
  
        //convert the test voltage into an ascii and store it into the send buffer
        funct_IntToAscii(g_Param.uint16_Umot,_Active);
        uart2_sendbuffer(',');                  //add the comma
  
        //convert the short-circuit current limit into an ascii and store it into the send buffer
        funct_IntToAscii(g_Param.uint16_SCiLevel,_Active);
        uart2_sendbuffer(',');                  //add the comma
  
        //convert the short-circuit wait time into an ascii and store it into the send buffer
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
        if(g_Uni.uint8_Settings & 0x01) //is motor in run mode?
        {
            g_Param.uint8_ErrCode = _MotorInRun;        //set error code
            uart2_SendErrorCode(g_Param.uint8_ErrCode); //call subroutine
        }
        else
        {
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
    
                    //convert the number of steps into an ascii and store it into the send buffer
                    funct_IntToAscii(g_Param.uint16_AccNumbStep[uint8_WB],_Active);
                    uart2_sendbuffer(',');              //add the comma
    
                    //convert the frequency into an ascii and store it into the send buffer
                    funct_IntToAscii(g_Param.uint16_AccFreq[uint8_WB],_Active);        
                }
            }
            uart2_sendbuffer(13);                       //add the CR at the end       
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
 * - uint16_DecFreq max character lenght = 4 x 50 different frequency = 200
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
        if(g_Uni.uint8_Settings & 0x01) //is motor in run mode?
        {
            g_Param.uint8_ErrCode = _MotorInRun;        //set error code
            uart2_SendErrorCode(g_Param.uint8_ErrCode); //call subroutine
        }
        else
        {
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
    
                    //convert the number of steps into an ascii and store it into the send buffer
                    funct_IntToAscii(g_Param.uint16_DecNumbStep[uint8_WB],_Active);
                    uart2_sendbuffer(',');              //add the comma
    
                    //convert the frequency into an ascii and store it into the send buffer
                    funct_IntToAscii(g_Param.uint16_DecFreq[uint8_WB],_Active);        
                }
            }
            uart2_sendbuffer(13);                       //add the CR at the end       
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
  
        //convert the step mode of the actuator into an ascii and store it into the send buffer
        funct_IntToAscii(g_Param.uint8_StepMode,_Active);
        uart2_sendbuffer(',');                      //add the comma
  
        //convert the step count of the actuator into an ascii and store it into the send buffer
        funct_IntToAscii(g_Param.uint32_StepCount,_Active);
        uart2_sendbuffer(',');                      //add the comma 
  
        //convert the speed into an ascii and store it into the send buffer
        funct_IntToAscii(g_Param.uint16_RunFreq,_Active);
        uart2_sendbuffer(',');                      //add the comma
  
        //convert the direction into an ascii and store it into the send buffer
        funct_IntToAscii(g_Param.uint8_Direction,_Active);
        uart2_sendbuffer(',');                      //add the comma
  
        //convert the coil state into an ascii and store it into the send buffer
        funct_IntToAscii(g_Param.uint8_CoilState,_Active);
        uart2_sendbuffer(',');                      //add the comma
  
        //convert the current during move into an ascii and store it into the send buffer
        funct_IntToAscii(g_Param.uint16_BipRunI,_Active);
        uart2_sendbuffer(',');                      //add the comma
  
        //convert the "holding" current into an ascii and store it into the send buffer
        funct_IntToAscii(g_Param.uint16_BipHoldI,_Active);
        uart2_sendbuffer(',');                      //add the comma
  
        //convert the selected ramp into an ascii and store it into the send buffer
        funct_IntToAscii(g_Param.uint8_SelectRamp,_Active);
        uart2_sendbuffer(',');                      //add the comma
  
        //convert the curremt for acceleration into an ascii and store it into the send buffer
        funct_IntToAscii(g_Param.uint16_BipAccI,_Active);
        uart2_sendbuffer(',');                      //add the comma
  
        //convert the curremt for deceleration into an ascii and store it into the send buffer
        funct_IntToAscii(g_Param.uint16_BipDecI,_Active);
        uart2_sendbuffer(',');                      //add the comma
 
        //convert the time into an ascii and store it into the send buffer
        funct_IntToAscii(g_Param.uint16_AccOnDelay,_Active);
        uart2_sendbuffer(',');                      //add the comma
  
        //convert the time into an ascii and store it into the send buffer
        funct_IntToAscii(g_Param.uint16_DecOffDelay,_Active);
        uart2_sendbuffer(',');                      //add the comma
  
        //convert the protocol quittance into an ascii and store it into the send buffer
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
 * Creator:                 A. Staub
 * Date of creation:        26.10.2015
 * Last modification on:    -
 * Modified by:             - 
 * 
 * Input:                   -
 * Output:                  -
***********************************************************************************************************************/
void cmd_BREAK(void)
{  
    if(g_CmdChk.uint8_ParamPos == 1)   //number of received characters OK?
    {
        //add here the conditions!!!!!
        
        //send back the needed informations
        uart2_sendbuffer('E');                  //first the letter E
        uart2_sendbuffer(13);                   //add the CR at the end
        
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
    if(g_CmdChk.uint8_ParamPos == 1)   //number of received characters OK?
    {
        //measure here the voltage or take the last measured parameter and send it back!
    
        //send back the needed informations
        uart2_sendbuffer('E');                          //first the letter E
        uart2_sendbuffer(',');                          //add the comma
        funct_IntToAscii(g_Param.uint16_Ue,_Active);    //add the voltage
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
        if(g_Uni.uint8_Settings & 0x01) //is motor in run mode?
        {
            g_Param.uint8_ErrCode = _MotorInRun;        //set error code
            uart2_SendErrorCode(g_Param.uint8_ErrCode); //call subroutine
        }
        else
        {
            //verify the limits if they are inside the tolerance
            uint8_WB = g_CmdChk.uint32_TempPara[1];
            if((uint8_WB == 'N') || (uint8_WB == 'B') || (uint8_WB == 'M') || (uint8_WB == 'U'))
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
                    //switch off/on the inputs here
                }
                //type of motor = unipolar or matrix?
                else if((g_CmdChk.uint32_TempPara[1] == 'U') || (g_CmdChk.uint32_TempPara[1] == 'M'))
                {
                    //switch off/on the inputs here
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
        if(g_Uni.uint8_Settings & 0x01)             //is unipolar motor in run mode?
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
 * Creator:                 A. Staub
 * Date of creation:        28.11.2015
 * Last modification on:    -
 * Modified by:             - 
 * 
 * Input:                   -
 * Output:                  -
***********************************************************************************************************************/
void cmd_SBIT(void)
{
    auto unsigned char uint8_Result = 0;    //local work byte
    
    if(g_CmdChk.uint8_ParamPos == 2)           //number of received characters OK?
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
 * Creator:                 A. Staub
 * Date of creation:        28.11.2015
 * Last modification on:    -
 * Modified by:             - 
 * 
 * Input:                   -
 * Output:                  -
***********************************************************************************************************************/
void cmd_CBIT(void)
{
    auto unsigned char uint8_Result = 0;    //local work byte
    
    if(g_CmdChk.uint8_ParamPos == 2)           //number of received characters OK?
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
            if(g_Uni.uint8_Settings & 0x01) //is motor in run mode?
            {
                g_Param.uint8_ErrCode = _MotorInRun;        //set error code
                uart2_SendErrorCode(g_Param.uint8_ErrCode);  //call subroutine
            }
            else
            {
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
        if(g_Uni.uint8_Settings & 0x01) //is motor in run mode?
        {
            g_Param.uint8_ErrCode = _MotorInRun;        //set error code
            uart2_SendErrorCode(g_Param.uint8_ErrCode);  //call subroutine
        }
        else 
        {
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
        }
    }
    else
    {
        g_Param.uint8_ErrCode = _NumbRecCharNotOK;  //set error code
        uart2_SendErrorCode(g_Param.uint8_ErrCode); //call subroutine
    }
}   //end of cmd_GMCRSTP cmd_SSWLIM


/**********************************************************************************************************************
 * Routine:                 cmd_SSWLIM

 * Description:
 * Defines the position limits for switch or hall-sensor switch. 
 * 
 * Creator:                 A. Staub
 * Date of creation:        30.11.2015
 * Last modification on:    -
 * Modified by:             - 
 * 
 * Input:                   -
 * Output:                  -
***********************************************************************************************************************/
void cmd_SSWLIM(void)
{
    auto unsigned char uint8_Result = 0;    //local work byte
    
    if(g_CmdChk.uint8_ParamPos == 5)   //number of received characters OK?
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
    else
    {
        g_Param.uint8_ErrCode = _NumbRecCharNotOK;  //set error code
        uart2_SendErrorCode(g_Param.uint8_ErrCode); //call subroutine
    }
}   //end of cmd_SSWLIM 

/**********************************************************************************************************************
 * Routine:                 cmd_GSWLIM

 * Description:
 * Get the position limits for switch or hall-sensor switch. 
 * 
 * Creator:                 J. Rebetez
 * Date of creation:        30.11.2015
 * Last modification on:    -
 * Modified by:             - 
 * 
 * Input:                   -
 * Output:                  -
***********************************************************************************************************************/
void cmd_GSWLIM(void)
{    
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
}   //end of cmd_GSWLIM 

/**********************************************************************************************************************
 * Routine:                 cmd_GSWPOS

 * Description:
 * Get the currently switch-position for switch or hall-sensor. 
 * 
 * Creator:                 J. Rebetez
 * Date of creation:        30.11.2015
 * Last modification on:    -
 * Modified by:             - 
 * 
 * Input:                   -
 * Output:                  -
***********************************************************************************************************************/
void cmd_GSWPOS(void)
{    
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
}   //end of cmd_GSWPOS 

/**********************************************************************************************************************
 * Routine:                 cmd_SPWMLIM

 * Description:
 * Set PWM limits for linear hall-detector
 * 
 * Creator:                 J. Rebetez
 * Date of creation:        30.11.2015
 * Last modification on:    -
 * Modified by:             - 
 * 
 * Input:                   -
 * Output:                  -
***********************************************************************************************************************/
void cmd_SPWMLIM(void)
{
    auto unsigned char uint8_Result = 0;    //local work byte
    
    if(g_CmdChk.uint8_ParamPos == 3)   //number of received characters OK?
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
    else
    {
        g_Param.uint8_ErrCode = _NumbRecCharNotOK;  //set error code
        uart2_SendErrorCode(g_Param.uint8_ErrCode); //call subroutine
    }
}   //end of cmd_SPWMLIM 

/**********************************************************************************************************************
 * Routine:                 cmd_GPWMLIM

 * Description:
 * Get PWM limits for linear hall-detector. 
 * 
 * Creator:                 J. Rebetez
 * Date of creation:        30.11.2015
 * Last modification on:    -
 * Modified by:             - 
 * 
 * Input:                   -
 * Output:                  -
***********************************************************************************************************************/
void cmd_GPWMLIM(void)
{    
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
}   //end of cmd_GPWMLIM

/**********************************************************************************************************************
 * Routine:                 cmd_GPWMPOS

 * Description:
 * Get position for linear hall-detector. 
 * 
 * Creator:                 J. Rebetez
 * Date of creation:        30.11.2015
 * Last modification on:    -
 * Modified by:             - 
 * 
 * Input:                   -
 * Output:                  -
***********************************************************************************************************************/
void cmd_GPWMPOS(void)
{    
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
void cmd_GPWMVAL(void)
{    
    if(g_CmdChk.uint8_ParamPos == 1)   //number of received characters OK?
    {
        if(funct_CheckTol(g_Param.uint16_SwPWMval,470,530))
        {
            uart2_sendbuffer('E');          //first the letter E
        }
        else
        {
            uart2_sendbuffer('X');  //add the comma
            g_Param.uint8_ErrCode = _OutOfTolGPWMVAL;  //set error code
            uart2_sendbuffer(',');  //add the comma
            funct_IntToAscii(g_Param.uint8_ErrCode,_Active);
        }   
        uart2_sendbuffer(',');  //add the comma
        //convert the parameter and store it into the send buffer
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
        uart2_sendbuffer('E');
        uart2_sendbuffer(',');  //add the comma
        funct_IntToAscii(g_Param.uint32_SwFrqVal,_Inactive);
        do{
            g_Funct.uint8_ArrAsciiPos--;
            uart2_sendbuffer(g_Funct.uint8_ArrAscii[g_Funct.uint8_ArrAsciiPos]);
            if(g_Funct.uint8_ArrAsciiPos == 1)
            {
                uart2_sendbuffer('.');
            }
        }while(g_Funct.uint8_ArrAsciiPos);
        uart2_sendbuffer(13);
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
    else
    {
        g_Param.uint8_ErrCode = _NumbRecCharNotOK;  //set error code
        uart2_SendErrorCode(g_Param.uint8_ErrCode); //call subroutine
    } 
}   //end of cmd_GTIME