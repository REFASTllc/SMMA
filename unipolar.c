/**********************************************************************************************************************

                                            UBD - Unipolar Bipolar Driver

***********************************************************************************************************************
 * File name:               unipolar.c
 * Creation date:           28.09.2015
 * Main creator:            Alain Staub
 * Company:                 REFAST GmbH
 *                          Copyright (c) 2015 REFAST GmbH
***********************************************************************************************************************
 * Content overview:        - uni_init
 *                          - uni_move
 *                          - uni_acc
 *                          - uni_run
 *                          - uni_dec
 *                          - uni_CheckCalc
***********************************************************************************************************************/


#include "includes.h"

SUni g_Uni;                 //global variables for struct
extern SParam g_Param;
extern SCmdChk g_CmdChk;

/**********************************************************************************************************************
 * Routine:                 uni_init

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
void uni_init(void)
{
//initialize the variables of the unipolar actuator
    g_Uni.uint32_RealPos = 0;               //set the real position to 0
    g_Uni.uint32_GoalPos = 0;               //set the goal position to 0
    g_Uni.uint16_RunFreq = 0;               //set the run frequency to 0
    
    g_Uni.uint8_Status = 0b00000001;        //status byte - set bit 'FS' and clear the other bits
    //                     ||||||||
    //                     |||||||+- 0: FS    - First step; 1: active / 0: not active 
    //                     ||||||+-- 1: LS    - Last step; 1: active / 0: not active
    //                     |||||+--- 2: ACC   - Acceleration; 1: active / 0: not active
    //                     ||||+---- 3: DEC   - Deceleration; 1: active / 0: not active
    //                     |||+----- 4: NS    - Next step; 1: allowed / 0: not allowed
    //                     ||+------ 5: GOAL  - 1: Real- = Goalposition / 0: Real- != Goalposition
    //                     |+------- 6:       - not used / free       
    //                     +-------- 7: Error - Error; 1: active / 0: not active 
    
    g_Uni.uint8_Settings = 0b00000000;      //set settings to default value
    //                       ||||||||
    //                       |||||||+- 0: RUN    - Motor; 1: running / 0: not running 
    //                       ||||||+-- 1: RAMP   - Ramp; 1: active / 0: not active
    //                       |||||+--- 2: SMODE  - Step mode; 1: Halfstep / 0: Fullstep
    //                       ||||+---- 3: CC     - Coils current; 1: active / 0: not active
    //                       |||+----- 4: CW_CCW - Direction; 1: CW / 0: CCW
    //                       ||+------ 5: SRAMP  - Select ramp; 1: both ACC&DEC / 0: only ACC
    //                       |+------- 6: PMODE  - Phase mode; 1: two phase ON / 0: one phase ON
    //                       +-------- 7:        - not used / free
    
    g_Uni.uint8_FsHsCount = 0;              //clear the full or half step counter
    g_Uni.uint32_IntTime = 0;                 //clear effective interrupt time
    g_Uni.uint32_RunTime = 0;                 //clear run time for interrupt
    g_Uni.uint32_SwOnTime = 0;                //clear switch on time for interrupt
    g_Uni.uint32_SwOffTime = 0;               //clear switch off time for interrupt
    g_Uni.uint8_AccArrPos = 0;              //clear the acceleration array position
    g_Uni.uint16_AccNumbStep = 0;           //clear the acceleration number of steps
    g_Uni.uint32_AccStop = 0;               //clear the acceleration stop position
    g_Uni.uint8_DecArrPos = 0;              //clear the deceleration array position
    g_Uni.uint16_DecNumbStep = 0;           //clear the deceleration number of steps
    g_Uni.uint32_DecStart = 0;              //clear the deceleration start position
    
//define start position
    g_Uni.uint8_PhA1 = _UniPhOFF;           //status PhA1 = off
    g_Uni.uint8_PhA2 = _UniPhON;            //status PhA2 = on
    g_Uni.uint8_PhB1 = _UniPhOFF;           //status PhB1 = off
    g_Uni.uint8_PhB2 = _UniPhON;            //status PhB1 = on
 
//define coil outputs
    oUniCoilA1 = _UniPhOFF;                 //output PhA1 = off
    oUniCoilA2 = _UniPhOFF;                 //output PhA2 = off
    oUniCoilB1 = _UniPhOFF;                 //output PhB1 = off
    oUniCoilB2 = _UniPhOFF;                 //output PhB2 = off  
}   //end of uni_init


/**********************************************************************************************************************
 * Routine:                 uni_move

 * Description:
 * This subroutine decide which subroutine or step is to do as next, with follow priority:
 * 1. First step
 * 2. Motor position = Goal Position
 * 3. Acceleration active
 * 4. Deceleration active
 * 5. Run active
 * 
 * Creator:                 A. Staub
 * Date of creation:        30.09.2015
 * Last modification on:    -
 * Modified by:             - 
 * 
 * Input:                   -
 * Output:                  -
***********************************************************************************************************************/
void uni_move(void)
{
    if(g_Uni.uint8_Status & 0x01)               //is this the first step?
    {
        if(g_Uni.uint8_Status & 0x10)           //and is the next step allowed?
        {
            g_Uni.uint8_Status &= 0xEF;         //then clear the bit 'NS - next step'
            uni_CheckCalc();                    //subroutine to verify all parameters and calculate different pos.       
            g_Uni.uint8_Status &= 0xFE;         //clear the bit 'FS - first step'
      
            if(!(g_Uni.uint8_Status & 0x80))    //no error detected during the check from the subroutine
            {
                if(g_Uni.uint8_Settings & 0x02) //then verify as next if the acceleration ramp is active?
                {
                    g_Uni.uint8_Status |= 0x04; //set bit 'ACCEL - acceleration'
                }
                else
                {
                    g_Uni.uint8_Status &= 0xF3; //if not active then clear for security the bit 'ACC and DEC'
                }
            }
            else
            {
                //and stop the timer45 
                T4CON &= 0x7FFF;    //disable interrupt module
                
                TMR4 = 0x0;         //clear contents of the TMR4 and TMR5
                PR4 = 400;          //load the timer with 10us
                IFS0CLR = _IFS0_T5IF_MASK;  //clear interrupt flag
                
                g_Uni.uint32_GoalPos = g_Uni.uint32_RealPos;    //otherwise set the goal to the real position
                g_Uni.uint8_Settings = 0;       //erase settings         
                g_Uni.uint8_Status |= 0x01;     //set the bit 'FS - first step' 
                
                if(g_Param.uint8_RunBit)
                {
                    oSinkSource0 = 0;
                }
                
                //and switch off all outputs
                oUniCoilA1 = _UniPhOFF;                 //output PhA1 = off
                oUniCoilA2 = _UniPhOFF;                 //output PhA2 = off
                oUniCoilB1 = _UniPhOFF;                 //output PhB1 = off
                oUniCoilB2 = _UniPhOFF;                 //output PhB2 = off
                
                g_CmdChk.uint8_GlobalLock = 0;  //disable global lock
            }     
        }
        else
        {
            //it is a must have to load the modulo timer already with a number. This number must be higher then 0, and 
            //should be not to small. Is this time is to short, then the interrupt will be called to much time before it 
            //has time to load the new switch on delay. The delay should be 400 * 25ns (timebase) that give us 10us
            //this means on a switch on wait time of 1ms, the real time is 1.01ms. Now it is important to know, that you 
            //cannot load this modulo timer in this else case, because this case could be executed more then 1 time 
            //(depending on main charge routine). Writing more then 1 time on this modulo timer inhibits the interrupt bit. 
            //So you have to load this modulo timer during the initialization phase and to set it again at the end 
            //of the move! PR4 = 400;   
            
            if(g_Param.uint8_RunBit)
            {
                oSinkSource0 = 1;
            }
      
            //prepare the start sequency
            oUniCoilA1 = g_Uni.uint8_PhA1;
            oUniCoilA2 = g_Uni.uint8_PhA2;
            oUniCoilB1 = g_Uni.uint8_PhB1;
            oUniCoilB2 = g_Uni.uint8_PhB2;

            g_Uni.uint8_Status &= 0x7F;             //clear error
            
            g_Uni.uint32_IntTime = g_Uni.uint32_SwOnTime;

            T4CON |= 0x8000;            //enable timer
        }
    }
    else
    {
        if(g_Uni.uint8_Status & 0x20)           //otherwise verify if the motor arrived at his goal position
        {
            if(g_Uni.uint8_Status & 0x10)       //if true - motor arrived at his goal position, 
                                                //verify if the next step is allowed
            {
                g_Uni.uint8_Status &= 0xEF;     //then clear the bit 'NS - next step'
        
                if(g_Uni.uint8_Status & 0x02)   //is this the last step?
                {
                    //then stop the timer 
                    T4CON &= 0x7FFF;    //disable interrupt module
                
                    TMR4 = 0x0;         //clear contents of the TMR4 and TMR5
                    PR4 = 400;          //load the timer with 10us
                    IFS0CLR = _IFS0_T5IF_MASK;  //clear interrupt flag
          
                    if(g_Uni.uint8_Settings & 0x08) //coils current active after move?
                    {
                        //then nothing is to do
                    }
                    else
                    {
                        //otherwise switch off all outputs
                        oUniCoilA1 = _UniPhOFF;     //output PhA1 = off
                        oUniCoilA2 = _UniPhOFF;     //output PhA2 = off
                        oUniCoilB1 = _UniPhOFF;     //output PhB1 = off
                        oUniCoilB2 = _UniPhOFF;     //output PhB2 = off
                    }
                    
                    if(g_Param.uint8_RunBit)
                    {
                        oSinkSource0 = 0;
                    }
                 
                    g_Uni.uint8_Settings &= 0xFE;   //clear the 'RUN' bit
                    g_Uni.uint8_Status &= 0xC1;     //clear 'NS', 'DEC', 'ACC', 'GOAL' and 'LS' bit
                    g_Uni.uint8_Status |= 0x01;     //set the bit 'FS'
                    
                    if(g_Param.uint8_Acknowledge)           //verify if we have to send back a second ack
                    {
                        uart2_sendbuffer('E');              //first the letter E
                        uart2_sendbuffer(13);               //with CR at the end
                    }
                    else
                    {
                        //nothing to send back
                    }    
                    
                    g_CmdChk.uint8_GlobalLock = 0;  //global lock disable
                }
                else
                {
                    g_Uni.uint8_Status |= 0x02;     //otherwise set the bit 'LS'
                }
            }
            else
            {
                //otherwise load the last switch off delay 
                g_Uni.uint32_IntTime = g_Uni.uint32_SwOffTime;
            }
        }
        else
        {
            if(g_Uni.uint8_Status & 0x04)
            {
                uni_acc();  //call the acceleration
            }
            else
            {
                //or if the deceleration ramp is still active
                if(g_Uni.uint8_Status & 0x08)
                {
                    uni_dec();  //call the deceleration
                }
                else
                {
                    uni_run();  //call subroutine
                }
            }
        }
    }  
}   //end of unipolar_move


/**********************************************************************************************************************
 * Routine:                 uni_acc

 * Description:
 * First we verify if the next step is allowed to execute. 
 * If true:
 * then we disable the bit next step and decrement the number of steps. Then we have to verify if the real position 
 * is on the acceleration stop position, or deceleration start position or on the goal position.
 * if not one of these three was true, then it was a normal step. 
 * If false:
 * we verify if the number of steps are 0, then in this case we have to load the new number of steps and frequency 
 * from the array. Otherwise we have to do nothing, because the TPM1 timer loads automatically the same time from 
 * the already defined time variables. 
 * 
 * Creator:                 A. Staub
 * Date of creation:        30.09.2015
 * Last modification on:    -
 * Modified by:             - 
 * 
 * Input:                   -
 * Output:                  -
***********************************************************************************************************************/
void uni_acc(void)
{
    auto unsigned short int uint16_Freq;      //local variable for the frequency
  
    if(g_Uni.uint8_Status & 0x10)       //is the next step is allowed?
    {
        g_Uni.uint8_Status &= 0xEF;     //then clear the bit 'NS - next step'
        g_Uni.uint16_AccNumbStep--;     //decrement the number of steps
    
        if(g_Uni.uint32_RealPos == g_Uni.uint32_AccStop)
        {
            g_Uni.uint8_Status &= 0xFB; //then clear bit 'ACCEL - acceleration'
        }
        else
        {
            //do nothing
        }
        
        //if real position is equal to deceleration start position
        if(g_Uni.uint32_RealPos == g_Uni.uint32_DecStart)
        {
            g_Uni.uint8_Status |= 0x08; //then set bit 'DECEL - deceleration'
            g_Uni.uint16_DecNumbStep = 0;   // for security reason set this to 0
        }
        else
        {
            //do nothing
        }
        
        //if real position is equal to goal position
        if(g_Uni.uint32_RealPos == g_Uni.uint32_GoalPos)
        {
            g_Uni.uint8_Status |= 0x20; //then set bit 'GOAL'
        }
        else
        {
            //do nothing
        }
    }
    else
    {
        //othewise, if there are all steps done with the actually frequency?
        if(g_Uni.uint16_AccNumbStep == 0)
        {
            //then load the new number of steps from the array
            g_Uni.uint16_AccNumbStep = funct_ReadRamp(_Acc,_Step,g_Uni.uint8_AccArrPos);
            
            //load the new ACC time, convert and store it
            uint16_Freq = funct_ReadRamp(_Acc,_Freq,g_Uni.uint8_AccArrPos);
            g_Uni.uint32_IntTime = funct_FreqToTimer23(uint16_Freq);
      
            //increment position for the array
            g_Uni.uint8_AccArrPos++;       
        }
        else
        {
            //otherwise do nothing, because the timer loads automatically 
            //again the same time from the already defined variable g_Bipol.uint32_IntTime
        }      
    } 
}   //end of uni_acc


/**********************************************************************************************************************
 * Routine:                 uni_run

 * Description:
 * First we verify if the next step is allowed to execute.
 * If true:
 * then we disable the bit next step. Then we have to verify if the real position is on the deceleration start 
 * position, or on the goal position. If not one of these two were true, then it was a normal step. 
 * If false:
 * we only load the timer with the run timing defintion.
 * 
 * Creator:                 A. Staub
 * Date of creation:        30.09.2015
 * Last modification on:    -
 * Modified by:             - 
 * 
 * Input:                   -
 * Output:                  -
***********************************************************************************************************************/
void uni_run(void)
{
    if(g_Uni.uint8_Status & 0x10)           //if the next step is allowed?
    {
        g_Uni.uint8_Status &= 0xEF;         //then clear the bit 'NS - next step'
        
        if((g_Uni.uint32_RealPos == g_Uni.uint32_DecStart) && (g_Uni.uint8_Settings & 0x20))
        {
            g_Uni.uint8_Status |= 0x08;     //then set bit 'DECEL - deceleration'
            g_Uni.uint16_DecNumbStep = 0;   //for security reason set this to 0
        }
        else
        {
            //do nothing
        }
        
        //if real position is equal to goal position
        if(g_Uni.uint32_RealPos == g_Uni.uint32_GoalPos)
        {
            g_Uni.uint8_Status |= 0x20;     //then set bit 'GOAL'
        }
        else
        {
            //do nothing
        }
    }
    else
    { 
        //otherwise load always the run time
        g_Uni.uint32_IntTime = g_Uni.uint32_RunTime;     
    }   
}   //end of uni_run


/**********************************************************************************************************************
 * Routine:                 uni_dec

 * Description:
 * First we verify if the next step is allowed to execute.
 * If true:
 * then we disable the bit next step and decrement the number of steps. Then we have to verify if the real position 
 * is on the goal position. If this is not the case, then it was a normal step. 
 * If false:
 * we verify if the number of steps are 0, then in this case we have to load the new number of steps and frequency 
 * from the array. Otherwise we have to do nothing, because the TPM1 timer loads automatically the same time from 
 * the already defined time variables. 
 * 
 * Creator:                 A. Staub
 * Date of creation:        30.09.2015
 * Last modification on:    -
 * Modified by:             - 
 * 
 * Input:                   -
 * Output:                  -
***********************************************************************************************************************/
void uni_dec(void)
{
    auto unsigned short int uint16_Freq;      //local variable for the frequency
  
    if(g_Uni.uint8_Status & 0x10)       //if the next step is allowed?
    {
        g_Uni.uint8_Status &= 0xEF;     //then clear the bit 'NS - next step'
        g_Uni.uint16_DecNumbStep--;     //decrement the number of steps
    
        //verify if real position is equal to goal position
        if(g_Uni.uint32_RealPos == g_Uni.uint32_GoalPos)
        {
            g_Uni.uint8_Status |= 0x20; //then set bit 'GOAL'
        }
        else
        {
            //nothing is to do, because it was a normal step
        }     
    }
    else
    {
        //othewise, if there are all steps done with the actually frequency?
        if(g_Uni.uint16_DecNumbStep == 0)
        {
            //then load the new number of steps from the array
            g_Uni.uint16_DecNumbStep = funct_ReadRamp(_Dec,_Step,g_Uni.uint8_DecArrPos);
            
            //load the new DEC time, convert and store it
            uint16_Freq = funct_ReadRamp(_Dec,_Freq,g_Uni.uint8_DecArrPos);
            g_Uni.uint32_IntTime = funct_FreqToTimer23(uint16_Freq);
      
            //increment the array for the next parameter
            g_Uni.uint8_DecArrPos++;       
        }
        else
        {
            //otherwise do nothing, because the timer loads automatically 
            //again the same time from the already defined variable g_Bipol.uint32_IntTime
        }    
    }
}   //end of uni_dec


/**********************************************************************************************************************
 * Routine:                 uni_CheckCalc

 * Description:
 * The idea behind this subroutine is to check and calculate all before start the move, for that we have during the 
 * move not so much to verify and of course not to start a move if we see that there is already a problem with the 
 * parameters.
 * A move will be not executed if we have one of these situations:
 * - the run or ramp frequency is higger then allowed (see definiton in unipolar.h)
 * - ramp active but first parameter is not correct
 * - ramp active but frequency is higger then allowed (see definition in unipolar.h)
 * - are there enough steps to execute the ramp(s) (if enable) and at the minimum one cycle that means 4 steps 
 *   in fullstep and 8 steps in halfsteps (run mode)
 * So first we check if the ramps are enabled and if true we verify each parameter. If there are 20 parameters and 
 * for example the frequency in the 10th parameter is higger then the run frequency then we stop with the ramp at 
 * this position. This is a protection to ensure for that we jump not for example from 800 Hz ramp frequency to a 
 * 400 Hz run frequency. 
 * 
 * In a second step we verify if the run frequency is under the maximum frequency. If not we set the error bit 
 * otherwise we convert run frequency to a time.      
 * 
 * In a third step, we make the plausibility check, that means:
 * - Start position is always 0 (real position)
 * - Goal position is the total of steps that we have to do
 * - Acceleration stop is the maximum steps that we have to do for the acceleration
 * - Goal position - deceleration start is the maximal steps that we have to do for the deceleration
 * - We have to do at minimum 1 cycle in run mode that means 4 steps in full step and 8 steps in half step
 * All this conditions must be true/possible for that we can start the move. 
 * 
 * Creator:                 A. Staub
 * Date of creation:        30.09.2015
 * Last modification on:    -
 * Modified by:             - 
 * 
 * Input:                   -
 * Output:                  -
***********************************************************************************************************************/
void uni_CheckCalc(void)
{
    auto unsigned short int uint16_NumbStep = 0;      //local work integer for the number of steps
    auto unsigned short int uint16_Freq = 0;          //local work integer for the frequency
    auto unsigned char uint8_loop;              //local work byte for both for loops
    auto unsigned long int uint32_PlausCheck = 0;   //local work register for the plausibility check
  
    g_Uni.uint32_RealPos = 0;       //real position starts always by 0
    //goal position is already known and defined in the command.c by receiving the command
      
    //reset when we have to start and stop the deceleration
    g_Uni.uint32_AccStop = 0;
    g_Uni.uint32_DecStart = 0;
    //reset the position of the acceleration array
    g_Uni.uint8_AccArrPos = 0; 
    //reset the position in the array where we have to start with the deceleration
    g_Uni.uint8_DecArrPos = 0;

    if(g_Uni.uint8_Settings & 0x02)     //is the ramp function active
    {
    //-----------------------------ACCELERATION-----------------------------
        //then verify first the acceleration ramp
        for(uint8_loop = 0; uint8_loop < g_Param.uint8_AccNumbRamp; uint8_loop++)
        {
            uint16_NumbStep = funct_ReadRamp(_Acc,_Step,uint8_loop);    //read out temporary the number of steps
            uint16_Freq = funct_ReadRamp(_Acc,_Freq,uint8_loop);        //read out temporary the frequency
      
            //is the first value in the array for the number of steps = 0?
            if((uint8_loop == 0) && (uint16_NumbStep == 0))
            {
                //then signal an error, because ramp is active but there is nothing in the buffer
                g_Uni.uint8_Status |= 0x80;
                //leave the for loop
                uint8_loop = g_Param.uint8_AccNumbRamp;
            }
            //is it the first value in the array and this frequency is higher than the run frequency?
            else if((uint8_loop == 0) && (uint16_Freq > g_Uni.uint16_RunFreq))
            {
                //then signal an error, because ramp is active but the first frequency is to high
                g_Uni.uint8_Status |= 0x80;
                //leave the for loop
                uint8_loop = g_Param.uint8_AccNumbRamp;
            }
            //is another value in the array for the number of steps = 0?
            else if(uint16_NumbStep == 0)
            {
                //then we have nothing more to read out and we can leave the for loop
                uint8_loop = g_Param.uint8_AccNumbRamp;
            }
            //is the frequency higger then allowed?
            else if(uint16_Freq > _UniFreqMax)
            {
                //then signal an error
                g_Uni.uint8_Status |= 0x80;
                //leave the for loop
                uint8_loop = g_Param.uint8_AccNumbRamp;
            }
            //is the actually frequency higher than the run frequency?
            else if(uint16_Freq > g_Uni.uint16_RunFreq)
            {
                //then we have nothing more to read out and we can leave the for loop
                uint8_loop = g_Param.uint8_AccNumbRamp;
            }
            else
            {
                //otherwise all parameters are right, so add the number of steps
                //into acceleration stop position
                g_Uni.uint32_AccStop += uint16_NumbStep;
            }
        }
    //-----------------------------DECELERATION-----------------------------
        //start the array position with 255, because 0 is already a place in the array
        //verify the deceleration ramp if enable
        if(g_Uni.uint8_Settings & 0x20)
        {
            for(uint8_loop = 0; uint8_loop < g_Param.uint8_DecNumbRamp; uint8_loop++)
            {
                uint16_NumbStep = funct_ReadRamp(_Dec,_Step,uint8_loop);    //read out temporary the number of steps
                uint16_Freq = funct_ReadRamp(_Dec,_Freq,uint8_loop);        //read out temporary the frequency
            
                //is it the first value in the array and this number of steps has the value = 0?
                if((uint8_loop == 0) && (uint16_NumbStep == 0))
                {
                    //then signal an error, because ramp is active but there is nothing in the buffer
                    g_Uni.uint8_Status |= 0x80;
                    //leave the for loop
                    uint8_loop = g_Param.uint8_DecNumbRamp;
                }   
                //is it the first value in the array and this frequency is higher than the run frequency?
                else if((uint8_loop == 0) && (uint16_Freq > g_Uni.uint16_RunFreq))
                {
                    //then signal an error, because ramp is active but the first frequency is to high
                    g_Uni.uint8_Status |= 0x80;
                    //leave the for loop
                    uint8_loop = g_Param.uint8_DecNumbRamp;
                }
                //is another value in the array for the number of steps = 0?
                else if(uint16_NumbStep == 0)
                {
                    //then we have nothing more to read out and we can leave the for loop
                    uint8_loop = g_Param.uint8_DecNumbRamp;
                }
                //is the frequency higger then allowed?
                else if(uint16_Freq > _UniFreqMax)
                {
                    //then signal an error
                    g_Uni.uint8_Status |= 0x80;
                    //leave the for loop
                    uint8_loop = g_Param.uint8_DecNumbRamp;
                }
                //is the actually frequency higher than the run frequency?
                else if(uint16_Freq > g_Uni.uint16_RunFreq)
                {
                    //then we have nothing more to read out and we can leave the for loop
                    uint8_loop = g_Param.uint8_DecNumbRamp;
                }
                else
                {
                    //otherwise all parameters are right, so add the number of steps
                    //into deceleration start position
                    g_Uni.uint32_DecStart += uint16_NumbStep;
                }
            }
        }
        else
        {
            //otherwise do nothing
        }
    
        //-------------------------DEFINE THE POSITIONS-------------------------
        if(!(g_Uni.uint8_Status & 0x80))        //all parameters are correct for the move?
        {
            //then calculate the positions
            //acceleration stop position was calculated in the first for loop; use for this 'g_Uni.uint32_AccStop'
            //define deceleration start position but first copy the number of steps that we have to do, 
            //for the plausibility check
            uint32_PlausCheck = g_Uni.uint32_DecStart;
            g_Uni.uint32_DecStart = g_Uni.uint32_GoalPos - g_Uni.uint32_DecStart;
            //to know where to start in the deceleration array use this 'g_Uni.uint32_DecStart'    
        }
        else
        {
            //otherwise there is nothing to do
        }
    } 
    else
    {
        //otherwise there is nothing to do
    }
  
    //-------------------------VERIFY THE RUN MODE------------------------
    if(g_Uni.uint16_RunFreq > _UniFreqMax)  //verify if the frequency is higger than allowed?
    {
        g_Uni.uint8_Status |= 0x80;         //set error - run frequency to fast
    }
    else
    {
        //otherwise do nothing
    }
   
    //-------------------------PLAUSIBILITY CHECK-------------------------
     //do it only if there is not already an error
    if(!(g_Uni.uint8_Status & 0x80))
    {
        //start plausibility check which means:
        //start position is always 0
        //goal position is the total of steps that we have to do
        //AccStop is the maximal steps that we have to do for the acceleration
        //goal position - DecStart is the maximal steps that we have to do for the deceleration
        //we have to do at minimum 1 cylce in run mode that means 4 steps in full step
        //and 8 steps in half step
    
        //deceleration steps are already stored or 0 if not active -> add the acceleration steps
        uint32_PlausCheck = uint32_PlausCheck + g_Uni.uint32_AccStop;
    
        //mode = fullstep or halfstep?
        if(g_Uni.uint8_Settings & 0x04)
        {
            //mode = halfstep --> add 8 steps
            uint32_PlausCheck += 8;
        }
        else
        {
            //mode = fullstep --> add 4 steps
            uint32_PlausCheck += 4;
        }
    
        //number of steps that we have to do greater then goal position?
        //if(uint32_PlausCheck > g_Uni.uint32_GoalPos)
        //re-use the code from above to use the plausibility check
        //for the moment it is disabled
        if(0)
        {
            //then signale an error and send back the error code
            g_Uni.uint8_Status |= 0x80;
//            g_Param.uint8_ErrCode = _UniPlausiCheck;    //set error code
//            uart2_SendErrorCode(g_Param.uint8_ErrCode); //call subroutine
        }
        else
        {
            //convert the run frequency to have the value ready
            g_Uni.uint32_RunTime = funct_FreqToTimer23(g_Uni.uint16_RunFreq);
            
            //send back the OK
            uart2_sendbuffer('E');              //first the letter E
            uart2_sendbuffer(13);               //with CR at the end
        }   
    }
    else
    {
        //send back the error code
        g_Param.uint8_ErrCode = _UniPlausiCheck;    //set error code
        uart2_SendErrorCode(g_Param.uint8_ErrCode); //call subroutine
    } 
}   //end of unipolar_checkcalc