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
//initialize the variables of the unipolar actuator
    g_Bipol.uint32_RealPos = 0;               //set the real position to 0
    g_Bipol.uint32_GoalPos = 0;               //set the goal position to 0
    g_Bipol.uint16_RunFreq = 0;               //set the run frequency to 0
    
    A3981.RUN.REG = 0b1000101001000000;
    A3981.CONFIG0.REG = 0b0010011100011100;
    A3981.CONFIG1.REG = 0b0111000000100000;
    A3981.TBLLD.REG = 0b1111000001000101;
    
    g_Bipol.uint8_Status = 0b00000001;        //status byte - set bit 'FS' and clear the other bits
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
    g_Bipol.uint1_IntTimeExpiredFlag = 1;       //force the interrupt routine to load the new time
}   //end of uni_init


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
    if(g_Bipol.uint8_Status & 0x01)               //is this the first step?
    {
        if(g_Bipol.uint8_Status & 0x10)           //and is the next step allowed?
        {
            g_Bipol.uint8_Status &= 0xEF;         //then clear the bit 'NS - next step'
            bi_CheckCalc();                    //subroutine to verify all parameters and calculate different pos.       
            g_Bipol.uint8_Status &= 0xFE;         //clear the bit 'FS - first step'
      
            if(!(g_Bipol.uint8_Status & 0x80))    //no error detected during the check from the subroutine
            {
                if(g_Bipol.uint1_NextStepIsRamp == 1) //then verify as next if the acceleration ramp is active?
                {
                    g_Bipol.uint8_Status |= 0x04; //set bit 'ACCEL - acceleration'
                }
                else
                {
                    g_Bipol.uint8_Status &= 0xF3; //if not active then clear for security the bit 'ACC and DEC'
                }
            }
            else
            {
                g_Bipol.uint32_GoalPos = g_Bipol.uint32_RealPos;    //otherwise set the goal to the real position       
                g_Bipol.uint8_Status |= 0x01;     //set the bit 'FS - first step'
        
                //and stop the timer4 
                T4CONbits.ON = 0;               //switch off timer 4
                TMR4 = 0;                       //reset LSB counter
                TMR5 = 0;                       //reset MSB counter
                PR4 = 400;                      //load LSB register with start condition
                PR5 = 0;                        //load MSB register with 0 
                g_Bipol.uint1_IntTimeExpiredFlag = 1;   //force the interrupt routine to load the new time
                A3981.RUN.BITS.EN = 0;
                SendOneDataSPI1(A3981.RUN.REG);
                g_Bipol.uint1_IsBipolEnabled = 0;
            }     
        }
        else
        {
        //It is a must have to load the modulo timer already with a number. This number must be higher then 0, and 
        //should be not to small. Is this time is to short, then the interrupt will be called too much time before it 
        //has time to load the new switch on delay. The delay should be 400 * 25ns (timebase) that give us 10us
        //this means on a switch on wait time of 1ms, the real time is 1.01ms. Now it is important to know, that you 
        //cannot load this modulo timer in this else case, because this case could be executed more then 1 time 
        //(depending on main charge routine). Writing more then 1 time on this modulo timer inhibits the interrupt bit. 
        //So you have to load this modulo timer during the initialization phase and to set it again at the end 
        //of the move! PR4 = 400;
      
        //load the first switch on delay
        g_Bipol.uint32_IntTime = g_Bipol.uint32_SwOnTime;
      
        SendOneDataSPI1(A3981.RUN.REG);
      
        g_Bipol.uint8_Status &= 0x7F;             //clear error
      
        T4CONbits.ON = 1;                       //enable the timer 4
        }
    }
    else
    {
        if(g_Bipol.uint8_Status & 0x20)         //otherwise verify if the motor arrived at his goal position
        {
            if(g_Bipol.uint8_Status & 0x10)     //if true - motor arrived at his goal position, 
                                                //verify if the next step is allowed
            {
                g_Bipol.uint8_Status &= 0xEF;     //then clear the bit 'NS - next step'
        
                if(g_Bipol.uint8_Status & 0x02)   //is this the last step?
                {
                    //then stop the timer 
                    g_Bipol.uint1_IsBipolEnabled = 0;
                    T4CONbits.ON = 0;               //switch off the timer
                    TMR4 = 0;                       //reset LSB counter
                    TMR5 = 0;                       //reset MSB counter
                    PR4 = 400;                      //load LSB register with start condition
                    PR5 = 0;                        //load MSB register with 0
                    g_Bipol.uint1_IntTimeExpiredFlag = 1;    //force the interrupt routine to load the new time
          
                    if(g_Bipol.uint1_CurrInCoilAtTheEnd == 1) //coils current active after move?
                    {
                        //then nothing is to do
                    }
                    else
                    {
                        //otherwise switch off all outputs
                        A3981.RUN.BITS.EN = 0;
                        SendOneDataSPI1(A3981.RUN.REG);
                    }
                 
                    g_Bipol.uint8_Status &= 0xC1;     //clear 'NS', 'DEC', 'ACC', 'GOAL' and 'LS' bit
                    g_Bipol.uint8_Status |= 0x01;     //set the bit 'FS'
                    
                    if(g_Param.uint8_Acknowledge)           //verify if we have to send back a second ack
                    {
                        uart2_sendbuffer('E');              //first the letter E
                        uart2_sendbuffer(13);               //with CR at the end
                    }
                    else
                    {
                        //nothing to send back
                    }               
                }
                else
                {
                    g_Bipol.uint8_Status |= 0x02;     //otherwise set the bit 'LS'
                }
            }
            else
            {
                //otherwise load the last switch off delay 
                g_Bipol.uint32_IntTime = g_Bipol.uint32_SwOffTime;
            }
        }
        else
        {
            //not arrived at the goal position, so verify if the acceleration ramp is still active
            if(g_Bipol.uint8_Status & 0x04)           
            {  
                bi_acc();                      //then call the subroutine acceleration
            }
            //or if the deceleration ramp is still active
            else if(g_Bipol.uint8_Status & 0x08)
            {
                bi_dec();                     //then call the subroutine deceleration
            }
            else
            {
                bi_run();                     //otherwise call the subroutine run
            }
        }
    }  
}   //end of unipolar_move


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
  
    if(g_Bipol.uint8_Status & 0x10)       //is the next step is allowed?
    {
        g_Bipol.uint8_Status &= 0xEF;     //then clear the bit 'NS - next step'
        g_Bipol.uint16_AccNumbStep--;     //decrement the number of steps
    
        //verify if real position is equal to acceleration stop position
        if(g_Bipol.uint32_RealPos == g_Bipol.uint32_AccStop)
        {
            g_Bipol.uint8_Status &= 0xFB; //then clear bit 'ACCEL - acceleration'
        }
        //or if real position is equal to deceleration start position
        else if(g_Bipol.uint32_RealPos == g_Bipol.uint32_DecStart)
        {
            g_Bipol.uint8_Status |= 0x08; //then set bit 'DECEL - deceleration'
        }
        //or if real position is equal to goal position
        else if(g_Bipol.uint32_RealPos == g_Bipol.uint32_GoalPos)
        {
            g_Bipol.uint8_Status |= 0x20; //then set bit 'GOAL'
        }
        else
        {
        //nothing is to do, because it was a normal step
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
        else
        {
            //otherwise do nothing, because the timer loads automatically 
            //again the same time from the already defined variable g_Bipol.uint32_IntTime
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
    if(g_Bipol.uint8_Status & 0x10)           //if the next step is allowed?
    {
        g_Bipol.uint8_Status &= 0xEF;         //then clear the bit 'NS - next step'
        
        //verify if deceleration has to start if deceleration ramp is active
        if((g_Bipol.uint32_RealPos == g_Bipol.uint32_DecStart) && (g_Bipol.uint1_NextStepIsRamp == 1))
        {
            g_Bipol.uint8_Status |= 0x08;     //then set bit 'DECEL - deceleration'
        }
        //or if real position is equal to goal position
        else if(g_Bipol.uint32_RealPos == g_Bipol.uint32_GoalPos)
        {
            g_Bipol.uint8_Status |= 0x20;     //then set bit 'GOAL'
        }
        else
        {
            //nothing is to do, because it was a normal step
        }     
    }
    else
    { 
        //otherwise load always the run time
        g_Bipol.uint32_IntTime = g_Bipol.uint32_RunTime;    
    }   
}   //end of uni_run


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
  
    if(g_Bipol.uint8_Status & 0x10)       //if the next step is allowed?
    {
        g_Bipol.uint8_Status &= 0xEF;     //then clear the bit 'NS - next step'
        g_Bipol.uint16_DecNumbStep--;     //decrement the number of steps
    
        //verify if real position is equal to goal position
        if(g_Bipol.uint32_RealPos == g_Bipol.uint32_GoalPos)
        {
            g_Bipol.uint8_Status |= 0x20; //then set bit 'GOAL'
        }
        else
        {
            //nothing is to do, because it was a normal step
        }     
    }
    else
    {
        //othewise, if there are all steps done with the actually frequency?
        if(g_Bipol.uint16_DecNumbStep == 0)
        {
            //then load the new number of steps from the array
            g_Bipol.uint16_DecNumbStep = funct_ReadRamp(_Dec,_Step,g_Bipol.uint8_DecArrPos);
      
            //load the new DEC time, convert and store it
            uint16_Freq = funct_ReadRamp(_Dec,_Freq,g_Bipol.uint8_DecArrPos);
            g_Bipol.uint32_IntTime = funct_FreqToTimer23(uint16_Freq);
            
            //decrement the array for the next parameter
            g_Bipol.uint8_DecArrPos--;       
        }
        else
        {
            //otherwise do nothing, because the timer loads automatically 
            //again the same time from the already defined variable g_Bipol.uint32_IntTime
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
                g_Bipol.uint8_Status |= 0x80;
                //leave the for loop
                uint8_loop = g_Param.uint8_AccNumbRamp;
            }
            //is it the first value in the array and this frequency is higher than the run frequency?
            else if((uint8_loop == 0) && (uint16_Freq > g_Bipol.uint16_RunFreq))
            {
                //then signal an error, because ramp is active but the first frequency is to high
                g_Bipol.uint8_Status |= 0x80;
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
            else if(uint16_Freq > _BiFreqMax)
            {
                //then signal an error
                g_Bipol.uint8_Status |= 0x80;
                //leave the for loop
                uint8_loop = g_Param.uint8_AccNumbRamp;
            }
            //is the actually frequency higher than the run frequency?
            else if(uint16_Freq > g_Bipol.uint16_RunFreq)
            {
                //then we have nothing more to read out and we can leave the for loop
                uint8_loop = g_Param.uint8_AccNumbRamp;
            }
            else
            {
                //otherwise all parameters are right, so add the number of steps
                //into acceleration stop position
                g_Bipol.uint32_AccStop += uint16_NumbStep;
            }
        }
    //-----------------------------DECELERATION-----------------------------
        //start the array position with 255, because 0 is already a place in the array
        g_Bipol.uint8_DecArrPos = 255;   
    
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
                    g_Bipol.uint8_Status |= 0x80;
                    //leave the for loop
                    uint8_loop = g_Param.uint8_DecNumbRamp;
                }   
                //is it the first value in the array and this frequency is higher than the run frequency?
                else if((uint8_loop == 0) && (uint16_Freq > g_Bipol.uint16_RunFreq))
                {
                    //then signal an error, because ramp is active but the first frequency is to high
                    g_Bipol.uint8_Status |= 0x80;
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
                else if(uint16_Freq > _BiFreqMax)
                {
                    //then signal an error
                    g_Bipol.uint8_Status |= 0x80;
                    //leave the for loop
                    uint8_loop = g_Param.uint8_DecNumbRamp;
                }
                //is the actually frequency higher than the run frequency?
                else if(uint16_Freq > g_Bipol.uint16_RunFreq)
                {
                    //then we have nothing more to read out and we can leave the for loop
                    uint8_loop = g_Param.uint8_DecNumbRamp;
                }
                else
                {
                    //otherwise all parameters are right, so add the number of steps
                    //into deceleration start position
                    g_Bipol.uint32_DecStart += uint16_NumbStep;
                    //increment the start position of the deceleration array, to know where we have to start
                    g_Bipol.uint8_DecArrPos++;
                }
            }
        }
        else
        {
            //otherwise do nothing
        }
    
        //-------------------------DEFINE THE POSITIONS-------------------------
        if(!(g_Bipol.uint8_Status & 0x80))        //all parameters are correct for the move?
        {
            //then calculate the positions
            //acceleration stop position was calculated in the first for loop; use for this 'g_Bipol.uint32_AccStop'
            //define deceleration start position but first copy the number of steps that we have to do, 
            //for the plausibility check
            uint32_PlausCheck = g_Bipol.uint32_DecStart;
            g_Bipol.uint32_DecStart = g_Bipol.uint32_GoalPos - g_Bipol.uint32_DecStart;
            //to know where to start in the deceleration array use this 'g_Bipol.uint32_DecStart'    
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
    if(g_Bipol.uint16_RunFreq > _BiFreqMax)  //verify if the frequency is higger than allowed?
    {
        g_Bipol.uint8_Status |= 0x80;         //set error - run frequency to fast
    }
    else
    {
        //otherwise do nothing
    }
   
    //-------------------------PLAUSIBILITY CHECK-------------------------
     //do it only if there is not already an error
    if(!(g_Bipol.uint8_Status & 0x80))
    {
        //start plausibility check which means:
        //start position is always 0
        //goal position is the total of steps that we have to do
        //AccStop is the maximal steps that we have to do for the acceleration
        //goal position - DecStart is the maximal steps that we have to do for the deceleration
        //we have to do at minimum 1 cylce in run mode that means 4 steps in full step
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
        if(uint32_PlausCheck > g_Bipol.uint32_GoalPos)
        {
            //then signale an error and send back the error code
            g_Bipol.uint8_Status |= 0x80;
            g_Param.uint8_ErrCode = _BipPlausiCheck;    //set error code
            uart2_SendErrorCode(g_Param.uint8_ErrCode); //call subroutine
        }
        else
        {
            //otherwise all is correct, so verify if the ACC ramp is active 
            //to load the first time, otherwise load the run time
            if(g_Bipol.uint1_NextStepIsRamp == 1)
            {
                //load the first ACC time, convert and store it
                uint16_Freq = funct_ReadRamp(_Acc,_Freq,0);
                g_Bipol.uint32_IntTime = funct_FreqToTimer23(uint16_Freq);
        
                //load the number of steps that are to do with this frequency
                g_Bipol.uint16_AccNumbStep = funct_ReadRamp(_Acc,_Step,0);
        
                //increment the position for the next read out in the array
                g_Bipol.uint8_AccArrPos++;
            }
            else
            {
                //otherwise convert the run time and store it
                g_Bipol.uint32_RunTime = funct_FreqToTimer23(g_Bipol.uint16_RunFreq);
                g_Bipol.uint32_IntTime = g_Bipol.uint32_RunTime;
            }
            
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