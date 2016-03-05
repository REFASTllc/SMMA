/**********************************************************************************************************************

                                            UBD - Unipolar Bipolar Driver

***********************************************************************************************************************
 * File name:               bipolar.c
 * Creation date:           28.09.2015
 * Main creator:            Alain Staub
 * Company:                 REFAST GmbH
 *                          Copyright (c) 2015 REFAST GmbH
***********************************************************************************************************************
 * Content overview:        - bi_init
 *                          - bi_move
 *                          - bi_acc
 *                          - bi_run
 *                          - bi_dec
 *                          - bi_CheckCalc
***********************************************************************************************************************/


#include "includes.h"

Sbipol g_Bipol;                 //global variables for struct
extern SParam g_Param;
extern T_A3981 A3981;
extern SCmdChk g_CmdChk;

/**********************************************************************************************************************
 * Routine:                 bi_init

 * Description:
 * This routine initialize the basic things.
 * 
 * Creator:                 A. Staub / J. Rebetez
 * Date of creation:        21.12.2015
 * Last modification on:    -
 * Modified by:             - 
 * 
 * Input:                   -
 * Output:                  -
***********************************************************************************************************************/
void bi_init(void)
{
//initialize the variables of the bipolar actuator
    g_Bipol.uint32_RealPos = 0;               //set the real position to 0
    g_Bipol.uint32_GoalPos = 0;               //set the goal position to 0
    g_Bipol.uint16_RunFreq = 0;               //set the run frequency to 0    
    g_Bipol.status.REG = 0b00000001;        //status byte - set bit 'FS' and clear the other bits
    //                     ||||||||
    //                     |||||||+- 0: FS    - First step; 1: active / 0: not active 
    //                     ||||||+-- 1: LS    - Last step; 1: active / 0: not active
    //                     |||||+--- 2: ACC   - Acceleration; 1: active / 0: not active
    //                     ||||+---- 3: DEC   - Deceleration; 1: active / 0: not active
    //                     |||+----- 4: NS    - Next step; 1: allowed / 0: not allowed
    //                     ||+------ 5: GOAL  - 1: Real- = Goalposition / 0: Real- != Goalposition
    //                     |+------- 6:       - not used / free       
    //                     +-------- 7: Error - Error; 1: active / 0: not active 
    
    g_Bipol.uint32_IntTime = 0;                 //clear effective interrupt time
    g_Bipol.uint32_RunTime = 0;                 //clear run time for interrupt
    g_Bipol.uint32_SwOnTime = 0;                //clear switch on time for interrupt
    g_Bipol.uint32_SwOffTime = 0;               //clear switch off time for interrupt
    g_Bipol.uint8_AccArrPos = 0;                //clear the acceleration array position
    g_Bipol.uint16_AccNumbStep = 0;             //clear the acceleration number of steps
    g_Bipol.uint32_AccStop = 0;                 //clear the acceleration stop position
    g_Bipol.uint8_DecArrPos = 0;                //clear the deceleration array position
    g_Bipol.uint16_DecNumbStep = 0;             //clear the deceleration number of steps
    g_Bipol.uint32_DecStart = 0;                //clear the deceleration start position
    g_Bipol.uint1_IntTimeExpiredFlag = 0;       //force the interrupt routine to load the new time
}   //end of bi_init


/**********************************************************************************************************************
 * Routine:                 bi_move

 * Description:
 * This subroutine decide which subroutine or step is to do as next, with follow priority:
 * 1. First step
 * 2. Motor position = Goal Position
 * 3. Acceleration active
 * 4. Deceleration active
 * 5. Run active
 * 
 * Modification (23.12.2015 / A. Staub)
 * Timer23 is now used. Changed the register PR2 to 400 and clear the register PR3.
 * 
 * Creator:                 A. Staub / J. Rebetez
 * Date of creation:        21.12.2015
 * Last modification on:    23.12.2015
 * Modified by:             A. Staub 
 * 
 * Input:                   -
 * Output:                  -
***********************************************************************************************************************/
void bi_move(void)
{
    if(g_Bipol.status.BITS.firstStepIsActived)               //is this the first step?
    {
        if(g_Bipol.status.BITS.nextStepIsAllowed)           //and is the next step allowed?
        {
            g_Bipol.status.BITS.nextStepIsAllowed = 0;         //then clear the bit 'NS - next step'
            bi_CheckCalc();                    //subroutine to verify all parameters and calculate different pos.       
            g_Bipol.status.BITS.firstStepIsActived = 0;         //clear the bit 'FS - first step'
      
            if(!g_Bipol.status.BITS.error)    //no error detected during the check from the subroutine
            {
                if(g_Bipol.uint1_NextStepIsRamp == 1) //then verify as next if the acceleration ramp is active?
                    g_Bipol.status.BITS.accelerationIsActived = 1; //set bit 'ACCEL - acceleration'
                else
                {
                    g_Bipol.status.BITS.accelerationIsActived = 0; //if not active then clear for security the bit 'ACC and DEC'
                    g_Bipol.status.BITS.decelerationIsActived = 0;
                }
            }
            else
            {
                g_Bipol.uint32_GoalPos = g_Bipol.uint32_RealPos;    //otherwise set the goal to the real position       
                g_Bipol.status.BITS.firstStepIsActived = 1;     //set the bit 'FS - first step'
        
                //and stop the timer45 
                T4CON &= 0x7FFF;    //disable interrupt module
                
                TMR4 = 0x0;         //clear contents of the TMR4 and TMR5
                PR4 = 400;          //load the timer with 10us
                IFS0CLR = _IFS0_T5IF_MASK;  //clear interrupt flag
                
                g_Bipol.uint1_IntTimeExpiredFlag = 0;   //force the interrupt routine to load the new time
                A3981.RUN.BITS.EN = 0;
                SendOneDataSPI1(A3981.RUN.REG);
                g_Bipol.uint1_IsBipolEnabled = 0;
                
                if(g_Param.uint8_RunBit)
                {
                    oSinkSource0 = 0;
                }
                
                g_CmdChk.uint8_GlobalLock = 0;  //disable global lock
            }     
        }
        else
        {
            //it is a must have to load the modulo timer already with a number. This number must be higher then 0, and 
            //should be not to small. Is this time is to short, then the interrupt will be called too much time before it 
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

            SendOneDataSPI1(A3981.RUN.REG);
            
            g_Bipol.uint32_IntTime = g_Bipol.uint32_SwOnTime;

            g_Bipol.status.BITS.error = 0;             //clear error

            T4CON |= 0x8000;            //enable timer
        }
    }
    else
    {
        if(g_Bipol.status.BITS.goalIsReached)         //otherwise verify if the motor arrived at his goal position
        {
            if(g_Bipol.status.BITS.nextStepIsAllowed)     //if true - motor arrived at his goal position, 
                                                //verify if the next step is allowed
            {
                g_Bipol.status.BITS.nextStepIsAllowed = 0;     //then clear the bit 'NS - next step'
        
                if(g_Bipol.status.BITS.lastStepIsActived)   //is this the last step?
                {
                    //then stop the timer 
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
                    
                    if(g_Param.uint8_Acknowledge)           //verify if we have to send back a second ack
                    {
                        uart2_sendbuffer('E');              //first the letter E
                        uart2_sendbuffer(13);               //with CR at the end
                    }      
                    
                    g_CmdChk.uint8_GlobalLock = 0;  //disable global lock
                }
                else
                    g_Bipol.status.BITS.lastStepIsActived = 1;     //otherwise set the bit 'LS'
            }
            else //otherwise load the last switch off delay 
                g_Bipol.uint32_IntTime = g_Bipol.uint32_SwOffTime;
        }
        else
        {
            if(g_Bipol.status.BITS.accelerationIsActived)
            {
                bi_acc();       //call subroutine acceleration
            }
            else
            {
                //deceleration still active
                if(g_Bipol.status.BITS.decelerationIsActived)
                {
                    bi_dec();   //call subroutine deceleration
                }
                else
                {
                    bi_run();   //call subroutine run
                }
            }
        }
    }  
}   //end of bi_move


/**********************************************************************************************************************
 * Routine:                 bi_acc

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
 * Creator:                 A. Staub / J. Rebetez
 * Date of creation:        21.12.2015
 * Last modification on:    -
 * Modified by:             - 
 * 
 * Input:                   -
 * Output:                  -
***********************************************************************************************************************/
void bi_acc(void)
{
    auto unsigned short int uint16_Freq;      //local variable for the frequency
  
    if(g_Bipol.status.BITS.nextStepIsAllowed)       //is the next step is allowed?
    {
        g_Bipol.status.BITS.nextStepIsAllowed = 0;     //then clear the bit 'NS - next step'
        g_Bipol.uint16_AccNumbStep--;     //decrement the number of steps
        
        //verify if real position is equal to acceleration stop position
        if(g_Bipol.uint32_RealPos == g_Bipol.uint32_AccStop)
        {
            g_Bipol.status.BITS.accelerationIsActived = 0;  //clear bit acceleration
        }
        else
        {
            //do nothing
        }
        
        //verify if real position is equal to deceleration start position
        if(g_Bipol.uint32_RealPos == g_Bipol.uint32_DecStart)
        {
            g_Bipol.status.BITS.decelerationIsActived = 1;  //set bit deceleration
            g_Bipol.uint16_DecNumbStep = 0; //for security reason set this to 0
        }
        else
        {
            //do nothing
        }
        
        //verify if real position is equal to goal position
        if(g_Bipol.uint32_RealPos == g_Bipol.uint32_GoalPos)
        {
            g_Bipol.status.BITS.goalIsReached = 1;  //set bit goal 
        }
        else
        {
            //do nothing
        }
           
    }
    else
    {
        //otherwise, if there are all steps done with the actually frequency?
        if(g_Bipol.uint16_AccNumbStep == 0)
        {
            //then load the new number of steps from the array
            g_Bipol.uint16_AccNumbStep = funct_ReadRamp(_Acc,_Step,g_Bipol.uint8_AccArrPos);
      
            //load the new ACC time, convert and store it
            uint16_Freq = funct_ReadRamp(_Acc,_Freq,g_Bipol.uint8_AccArrPos);
            g_Bipol.uint32_IntTime = funct_FreqToTimer23(uint16_Freq);
                       
            //increment position for the array
            g_Bipol.uint8_AccArrPos++;       
        }   
    } 
}   //end of bi_acc


/**********************************************************************************************************************
 * Routine:                 bi_run

 * Description:
 * First we verify if the next step is allowed to execute.
 * If true:
 * then we disable the bit next step. Then we have to verify if the real position is on the deceleration start 
 * position, or on the goal position. If not one of these two were true, then it was a normal step. 
 * If false:
 * we only load the timer with the run timing defintion.
 * 
 * Creator:                 A. Staub / J. Rebetez
 * Date of creation:        21.12.2015
 * Last modification on:    -
 * Modified by:             - 
 * 
 * Input:                   -
 * Output:                  -
***********************************************************************************************************************/
void bi_run(void)
{
    if(g_Bipol.status.BITS.nextStepIsAllowed)           //if the next step is allowed?
    {
        g_Bipol.status.BITS.nextStepIsAllowed = 0;         //then clear the bit 'NS - next step'
        
        if((g_Bipol.uint32_RealPos == g_Bipol.uint32_DecStart) && (g_Bipol.uint1_NextStepIsRamp == 1))
        {
            g_Bipol.status.BITS.decelerationIsActived = 1;  //set bit deceleration
            g_Bipol.uint16_DecNumbStep = 0; //for security reason set this variable to 0
        }
        else
        {
            //nothing is done
        }
        
        //verify if real position is equal to goal position
        if(g_Bipol.uint32_RealPos == g_Bipol.uint32_GoalPos)
        {
            g_Bipol.status.BITS.goalIsReached = 1;  //set bit goal
        }
        else
        {
            //nothing is done
        }   
    }
    else //otherwise load always the run time
    {
        g_Bipol.uint32_IntTime = g_Bipol.uint32_RunTime;        
    }    
}   //end of bi_run


/**********************************************************************************************************************
 * Routine:                 bi_dec

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
 * Creator:                 A. Staub / J. Rebetez
 * Date of creation:        21.12.2015
 * Last modification on:    -
 * Modified by:             - 
 * 
 * Input:                   -
 * Output:                  -
***********************************************************************************************************************/
void bi_dec(void)
{
    auto unsigned short int uint16_Freq;      //local variable for the frequency
  
    if(g_Bipol.status.BITS.nextStepIsAllowed)       //if the next step is allowed?
    {
        g_Bipol.status.BITS.nextStepIsAllowed = 0;     //then clear the bit 'NS - next step'
        
        g_Bipol.uint16_DecNumbStep--;     //decrement the number of steps
    
        //verify if real position is equal to goal position
        if(g_Bipol.uint32_RealPos == g_Bipol.uint32_GoalPos)
        {
            g_Bipol.status.BITS.goalIsReached = 1; //then set bit 'GOAL'
        }
        else
        {
            //nothing to do
        }
    }
    else
    {
        //otherwise, if there are all steps done with the actually frequency?
        if(g_Bipol.uint16_DecNumbStep == 0)
        {
            //then load the new number of steps from the array
            g_Bipol.uint16_DecNumbStep = funct_ReadRamp(_Dec,_Step,g_Bipol.uint8_DecArrPos);
      
            //load the new DEC time, convert and store it
            uint16_Freq = funct_ReadRamp(_Dec,_Freq,g_Bipol.uint8_DecArrPos);
            g_Bipol.uint32_IntTime = funct_FreqToTimer23(uint16_Freq);
            
            //increment the array for the next parameter
            g_Bipol.uint8_DecArrPos++;       
        }
    }
}   //end of bi_dec


/**********************************************************************************************************************
 * Routine:                 bi_CheckCalc

 * Description:
 * The idea behind this subroutine is to check and calculate all before start the move, for that we have during the 
 * move not so much to verify and of course not to start a move if we see that there is already a problem with the 
 * parameters.
 * A move will be not executed if we have one of these situations:
 * - the run or ramp frequency is higher then allowed (see definiton in bipolar.h)
 * - ramp active but first parameter is not correct
 * - ramp active but frequency is higher then allowed (see definition in bipolar.h)
 * - are there enough steps to execute the ramp(s) (if enable) and at the minimum one cycle that means 4 steps 
 *   in fullstep and 8 steps in halfsteps (run mode)
 * So first we check if the ramps are enabled and if true we verify each parameter. If there are 20 parameters and 
 * for example the frequency in the 10th parameter is higher then the run frequency then we stop with the ramp at 
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
 * Creator:                 A. Staub / J. Rebetez
 * Date of creation:        21.12.2015
 * Last modification on:    -
 * Modified by:             - 
 * 
 * Input:                   -
 * Output:                  -
***********************************************************************************************************************/
void bi_CheckCalc(void)
{
    auto unsigned short int uint16_NumbStep = 0;      //local work integer for the number of steps
    auto unsigned short int uint16_Freq = 0;          //local work integer for the frequency
    auto unsigned char uint8_loop;              //local work byte for both for loops
    auto unsigned long int uint32_PlausCheck = 0;   //local work register for the plausibility check
  
    g_Bipol.uint32_RealPos = 0;       //real position starts always by 0
    //goal position is already known and defined in the command.c by receiving the command
      
    //reset when we have to start and stop the deceleration
    g_Bipol.uint32_AccStop = 0;
    g_Bipol.uint32_DecStart = 0;
    //reset the position of the acceleration array
    g_Bipol.uint8_AccArrPos = 0; 
    //reset the position in the array where we have to start with the deceleration
    g_Bipol.uint8_DecArrPos = 0;

    if(g_Bipol.uint1_NextStepIsRamp == 1)     //is the ramp function active
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
                g_Bipol.status.BITS.error = 1;
                //leave the for loop
                uint8_loop = g_Param.uint8_AccNumbRamp;
            }
            //is it the first value in the array and this frequency is higher than the run frequency?
            else if((uint8_loop == 0) && (uint16_Freq > g_Bipol.uint16_RunFreq))
            {
                //then signal an error, because ramp is active but the first frequency is to high
                g_Bipol.status.BITS.error = 1;
                //leave the for loop
                uint8_loop = g_Param.uint8_AccNumbRamp;
            }
            //is another value in the array for the number of steps = 0?
            else if(uint16_NumbStep == 0) //then we have nothing more to read out and we can leave the for loop
                uint8_loop = g_Param.uint8_AccNumbRamp;
            //is the frequency higher then allowed?
            else if(uint16_Freq > _BiFreqMax)
            {
                //then signal an error
                g_Bipol.status.BITS.error = 1;
                //leave the for loop
                uint8_loop = g_Param.uint8_AccNumbRamp;
            }
            //is the actually frequency higher than the run frequency?
            else if(uint16_Freq > g_Bipol.uint16_RunFreq) //then we have nothing more to read out and we can leave the for loop
                uint8_loop = g_Param.uint8_AccNumbRamp;
            else //otherwise all parameters are right, so add the number of steps into acceleration stop position
                g_Bipol.uint32_AccStop += uint16_NumbStep;
        }
    //-----------------------------DECELERATION-----------------------------
        //start the array position with 255, because 0 is already a place in the array
        //verify the deceleration ramp if enable
        if(g_Bipol.uint1_IsDecNeeded == 1)
        {
            for(uint8_loop = 0; uint8_loop < g_Param.uint8_DecNumbRamp; uint8_loop++)
            {
                uint16_NumbStep = funct_ReadRamp(_Dec,_Step,uint8_loop);    //read out temporary the number of steps
                uint16_Freq = funct_ReadRamp(_Dec,_Freq,uint8_loop);        //read out temporary the frequency
            
                //is it the first value in the array and this number of steps has the value = 0?
                if((uint8_loop == 0) && (uint16_NumbStep == 0))
                {
                    //then signal an error, because ramp is active but there is nothing in the buffer
                    g_Bipol.status.BITS.error = 1;
                    //leave the for loop
                    uint8_loop = g_Param.uint8_DecNumbRamp;
                }   
                //is it the first value in the array and this frequency is higher than the run frequency?
                else if((uint8_loop == 0) && (uint16_Freq > g_Bipol.uint16_RunFreq))
                {
                    //then signal an error, because ramp is active but the first frequency is to high
                    g_Bipol.status.BITS.error = 1;
                    //leave the for loop
                    uint8_loop = g_Param.uint8_DecNumbRamp;
                }
                //is another value in the array for the number of steps = 0?
                else if(uint16_NumbStep == 0) //then we have nothing more to read out and we can leave the for loop
                    uint8_loop = g_Param.uint8_DecNumbRamp;
                //is the frequency higher then allowed?
                else if(uint16_Freq > _BiFreqMax)
                {
                    //then signal an error
                    g_Bipol.status.BITS.error = 1;
                    //leave the for loop
                    uint8_loop = g_Param.uint8_DecNumbRamp;
                }
                //is the actually frequency higher than the run frequency?
                else if(uint16_Freq > g_Bipol.uint16_RunFreq) //then we have nothing more to read out and we can leave the for loop
                    uint8_loop = g_Param.uint8_DecNumbRamp;
                else
                {
                    //otherwise all parameters are right, so add the number of steps
                    //into deceleration start position
                    g_Bipol.uint32_DecStart += uint16_NumbStep;
                }
            }
        }
        //-------------------------DEFINE THE POSITIONS-------------------------
        if(!g_Bipol.status.BITS.error)        //all parameters are correct for the move?
        {
            //then calculate the positions
            //acceleration stop position was calculated in the first for loop; use for this 'g_Bipol.uint32_AccStop'
            //define deceleration start position but first copy the number of steps that we have to do, 
            //for the plausibility check
            uint32_PlausCheck = g_Bipol.uint32_DecStart;
            g_Bipol.uint32_DecStart = g_Bipol.uint32_GoalPos - g_Bipol.uint32_DecStart;
            //to know where to start in the deceleration array use this 'g_Bipol.uint32_DecStart'    
        }
    }
    //-------------------------VERIFY THE RUN MODE------------------------
    if(g_Bipol.uint16_RunFreq > _BiFreqMax)  //verify if the frequency is higher than allowed?
    {
        g_Bipol.status.BITS.error = 1;         //set error - run frequency to fast
    }
    //-------------------------PLAUSIBILITY CHECK-------------------------
     //do it only if there is not already an error
    if(!g_Bipol.status.BITS.error)
    {
        //start plausibility check which means:
        //start position is always 0
        //goal position is the total of steps that we have to do
        //AccStop is the maximal steps that we have to do for the acceleration
        //goal position - DecStart is the maximal steps that we have to do for the deceleration
        //we have to do at minimum 1 cycle in run mode that means 4 steps in full step
        //and 8 steps in half step
    
        //deceleration steps are already stored or 0 if not active -> add the acceleration steps
        uint32_PlausCheck = uint32_PlausCheck + g_Bipol.uint32_AccStop;
    
        //mode = fullstep or halfstep?
        switch(A3981.CONFIG0.BITS.MS)
        {
            case 0: //mode = fullstep --> add 4 steps
                    uint32_PlausCheck += 4;
                    break;
            case 1: //mode = halfstep --> add 8 steps
                    uint32_PlausCheck += 8;
                    break;
            case 2: //mode = quarter step --> add 4 steps
                    uint32_PlausCheck += 4;
                    break;
            case 3: //mode = sixteenth step --> add 16 steps
                    uint32_PlausCheck += 16;
                    break;
            default:
                break;
        }
    
        //number of steps that we have to do greater then goal position?
        //if(uint32_PlausCheck > g_Bipol.uint32_GoalPos)
        //re-use the code from above to use the plausibility check
        //for the moment it is disabled
        if(0)
        {
            //then signal an error and send back the error code
            g_Bipol.status.BITS.error = 1;
//            g_Param.uint8_ErrCode = _BipPlausiCheck;    //set error code
//            uart2_SendErrorCode(g_Param.uint8_ErrCode); //call subroutine
        }
        else
        {
            //convert the run frequency to have the value ready
            g_Bipol.uint32_RunTime = funct_FreqToTimer23(g_Bipol.uint16_RunFreq);
            
            //send back the OK
            uart2_sendbuffer('E');              //first the letter E
            uart2_sendbuffer(13);               //with CR at the end
        }   
    }
    else
    {
        //send back the error code
        g_Param.uint8_ErrCode = _BipPlausiCheck;    //set error code
        uart2_SendErrorCode(g_Param.uint8_ErrCode); //call subroutine
    } 
}   //end of bi_CheckCalc