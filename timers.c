/**********************************************************************************************************************

                                            UBD - Unipolar Bipolar Driver

***********************************************************************************************************************
 * File name:               timers.c
 * Creation date:           08.08.2015
 * Main creator:            Julien Rebetez
 * Company:                 REFAST GmbH
 *                          Copyright (c) 2015 REFAST GmbH
***********************************************************************************************************************
 * Content overview:        - timers_Set
 *                          - timers_SetInterrupt
 *                          - timers_Set
***********************************************************************************************************************/


#include "includes.h" // File which contain all includes files

STimer1 g_Timer1;     //global variables for struct


/**********************************************************************************************************************
 * Routine:                 InitTimerX

 * Description:
 * Initialization of the timers of the microcontroller.
 * Timer 1: used 
 * Clock source = PBCLK = 80MHz with prescaler 256 = 1 / (80MHz / 256) = 3.2us time base 
 * 
 * Timer 2: used in combination; see Timer 23
 * Clock source = PBCLK = 80MHz with prescaler 8 = 1 / (80MHz / 8) = 100ns time base
 * Used for the unipolar driver - DON'T USE THIS TIMER FOR SOMETHING OTHERS!!!
 * See modification from (23.12.2015) at the bottom. 
 * Since this mod. the time base is = 1 / (80MHz / 2) = 25ns. 
 * See case "_TIMER23"
 * 
 * Timer 3: used in combination; see Timer 23
 * 
 * Timer 4: not used
 * 
 * Timer 5: not used
 * 
 * Timer 23 (timer 2 in 32-bit): not used
 * Clock source = PBCLK = 80MHz with prescaler 2 = 1 / (80MHz / 2) = 25ns time base
 * Used for the unipolar driver - DON'T USE THIS TIMER FOR SOMETHING OTHERS!!!
 * See modification (29.12.2015)
 * 
 * Timer 45 (timer 4 in 32-bit): used 
 * Clock source = PBCLK = 80MHz with prescaler 2 = 1 / (80MHz / 2) = 25ns time base
 * Used for the unipolar driver - DON'T USE THIS TIMER FOR SOMETHING OTHERS!!!
 * 
 * 
 * Modification (23.12.2015 / A. Staub):
 * Timer 2 and 3 combined to an 32-bit timer. Time base changed to 25ns. 
 * 
 * Modification (29.12.2015 / A. Staub):
 * Timer 2 and 3 changed to timer 4 and 5, because of the input capture issue. 
 * 
 * Modification (27.01.2016 / J. Rebetez):
 * Function InitTimer has been splited in different InitTimerX functions to be more memory efficient. 
 * 
 * Creator:                 J. Rebetez
 * Date of creation:        08.08.2015
 * Last modification on:    29.12.2015
 * Modified by:             A. Staub
 * 
 * Input:                   -
 * Output:                  -
***********************************************************************************************************************/
void InitTimer1(void)
{
    T1CONbits.ON = 0;       // 1 = Timer is enabled
                            // 0 = Timer is disabled

    T1CONbits.SIDL = 1;     // 1 = Discontinue operation when device enters Idle mode
                            // 0 = Continue operation when device enters Idle mode

    T1CONbits.TWDIS = 0;    // 1 = Writes to TMR1 are ignored until pending write operation completes
                            // 0 = Back-to-back writes are enabled (Legacy Asynchronous Timer functionality)

    T1CONbits.TWIP = 1;     // In Asynchronous Timer mode:
                            //    1 = Asynchronous write to TMR1 register in progress
                            //    0 = Asynchronous write to TMR1 register complete
                            // In Synchronous Timer mode: This bit is read as ?0?.

    T1CONbits.TGATE = 0;    // When TCS = 1: This bit is ignored.
                            // When TCS = 0:
                            //    1 = Gated time accumulation is enabled
                            //    0 = Gated time accumulation is disabled

    T1CONbits.TCKPS1 = 1;   // 11 = 1:256 prescale value
    T1CONbits.TCKPS0 = 1;   // 10 = 1:64 prescale value
                            // 01 = 1:8 prescale value
                            // 00 = 1:1 prescale value

    T1CONbits.TSYNC = 0;    // When TCS = 1:
                            //    1 = External clock input is synchronized
                            //    0 = External clock input is not synchronized
                            // When TCS = 0: This bit is ignored.

    T1CONbits.TCS = 0;      // 1 = External clock from TxCKI pin
                            // 0 = Internal peripheral clock

    TMR1 = 0;
    PR1 = 0;
    g_Timer1.uint8_Timer1SafFlag = 0;   //reset safety flag
}

/**********************************************************************************************************************
 * Routine:                 InitTimer2
 *
 * Description:             Initialization of the timer 2 (16 bits) of the microcontroller.
 *
 * Creator:                 J. Rebetez
 * Date of creation:        27.01.2016
 * Last modification on:    -
 * Modified by:             -
 * 
 * Input:                   -
 * Output:                  -
***********************************************************************************************************************/
void InitTimer2(void)
{
    T2CONbits.ON = 0;       // 1 = Module is enabled
                            // 0 = Module is disabled

    T2CONbits.SIDL = 0;     // 1 = Discontinue operation when device enters Idle mode
                            // 0 = Continue operation when device enters Idle mode

    T2CONbits.TGATE = 0;    // When TCS = 1: This bit is ignored and is read as ?0?.
                            // When TCS = 0:
                            //    1 = Gated time accumulation is enabled
                            //    0 = Gated time accumulation is disabled

    T2CONbits.TCKPS2 = 0;   // 111 = 1:256 prescale value
    T2CONbits.TCKPS1 = 0;   // 110 = 1:64 prescale value
    T2CONbits.TCKPS0 = 0;   // 101 = 1:32 prescale value
                            // 100 = 1:16 prescale value
                            // 011 = 1:8 prescale value
                            // 010 = 1:4 prescale value
                            // 001 = 1:2 prescale value
                            // 000 = 1:1 prescale value

    T2CONbits.T32 = 0;      // 1 = TMRx and TMRy form a 32-bit timer
                            // 0 = TMRx and TMRy form separate 16-bit timer

    T2CONbits.TCS = 0;      // 1 = External clock from TxCK pin
                            // 0 = Internal peripheral clock

    TMR2 = 0;               //clear counter
    PR2 = 0xFFFF;           //PR2 register at maximum, timer2 free-running
}

/**********************************************************************************************************************
 * Routine:                 InitTimer3
 *
 * Description:             Initialization of the timer 3 (16 bits) of the microcontroller.
 *
 * Creator:                 J. Rebetez
 * Date of creation:        27.01.2016
 * Last modification on:    -
 * Modified by:             -
 * 
 * Input:                   -
 * Output:                  -
***********************************************************************************************************************/
void InitTimer3(void)
{
    T3CON = 0x8020;
    PR3 = 40000;
    TMR3 = 0;
    /*
    T3CONbits.ON = 0;       // 1 = Module is enabled
                            // 0 = Module is disabled

    T3CONbits.SIDL = 1;     // 1 = Discontinue operation when device enters Idle mode
                            // 0 = Continue operation when device enters Idle mode

    T3CONbits.TGATE = 1;    // When TCS = 1: This bit is ignored and is read as ?0?.
                            // When TCS = 0:
                            //    1 = Gated time accumulation is enabled
                            //    0 = Gated time accumulation is disabled

    T3CONbits.TCKPS2 = 0;   // 111 = 1:256 prescale value
    T3CONbits.TCKPS1 = 0;   // 110 = 1:64 prescale value
    T3CONbits.TCKPS0 = 0;   // 101 = 1:32 prescale value
                            // 100 = 1:16 prescale value
                            // 011 = 1:8 prescale value
                            // 010 = 1:4 prescale value
                            // 001 = 1:2 prescale value
                            // 000 = 1:1 prescale value

    T3CONbits.TCS = 0;      // 1 = External clock from TxCK pin
                            // 0 = Internal peripheral clock

    TMR3 = 0;
    PR3 = 0;*/
}

/**********************************************************************************************************************
 * Routine:                 InitTimer4
 *
 * Description:             Initialization of the timer 4 (16 bits) of the microcontroller.
 *
 * Creator:                 J. Rebetez
 * Date of creation:        27.01.2016
 * Last modification on:    -
 * Modified by:             -
 * 
 * Input:                   -
 * Output:                  -
***********************************************************************************************************************/
void InitTimer4(void)
{
    T4CONbits.ON = 0;       // 1 = Module is enabled
                            // 0 = Module is disabled

    T4CONbits.SIDL = 1;     // 1 = Discontinue operation when device enters Idle mode
                            // 0 = Continue operation when device enters Idle mode

    T4CONbits.TGATE = 1;    // When TCS = 1: This bit is ignored and is read as ?0?.
                            // When TCS = 0:
                            //    1 = Gated time accumulation is enabled
                            //    0 = Gated time accumulation is disabled

    T4CONbits.TCKPS2 = 0;   // 111 = 1:256 prescale value
    T4CONbits.TCKPS1 = 0;   // 110 = 1:64 prescale value
    T4CONbits.TCKPS0 = 0;   // 101 = 1:32 prescale value
                            // 100 = 1:16 prescale value
                            // 011 = 1:8 prescale value
                            // 010 = 1:4 prescale value
                            // 001 = 1:2 prescale value
                            // 000 = 1:1 prescale value

    T4CONbits.T32 = 0;      // 1 = TMRx and TMRy form a 32-bit timer
                            // 0 = TMRx and TMRy form separate 16-bit timer

    T4CONbits.TCS = 0;      // 1 = External clock from TxCK pin
                            // 0 = Internal peripheral clock

    TMR4 = 0;
    PR4 = 0;
}

/**********************************************************************************************************************
 * Routine:                 InitTimer5
 *
 * Description:             Initialization of the timer 5 (16 bits) of the microcontroller.
 *
 * Creator:                 J. Rebetez
 * Date of creation:        27.01.2016
 * Last modification on:    -
 * Modified by:             -
 * 
 * Input:                   -
 * Output:                  -
***********************************************************************************************************************/
void InitTimer5(void)
{
    T5CONbits.ON = 0;       // 1 = Module is enabled
                            // 0 = Module is disabled

    T5CONbits.SIDL = 1;     // 1 = Discontinue operation when device enters Idle mode
                            // 0 = Continue operation when device enters Idle mode

    T5CONbits.TGATE = 1;    // When TCS = 1: This bit is ignored and is read as ?0?.
                            // When TCS = 0:
                            //    1 = Gated time accumulation is enabled
                            //    0 = Gated time accumulation is disabled

    T5CONbits.TCKPS2 = 0;   // 111 = 1:256 prescale value
    T5CONbits.TCKPS1 = 0;   // 110 = 1:64 prescale value
    T5CONbits.TCKPS0 = 0;   // 101 = 1:32 prescale value
                            // 100 = 1:16 prescale value
                            // 011 = 1:8 prescale value
                            // 010 = 1:4 prescale value
                            // 001 = 1:2 prescale value
                            // 000 = 1:1 prescale value

    T5CONbits.TCS = 0;      // 1 = External clock from TxCK pin
                            // 0 = Internal peripheral clock

    TMR5 = 0;
    PR5 = 0;
}

/**********************************************************************************************************************
 * Routine:                 InitTimer23
 *
 * Description:             Initialization of the timer 23 (32 bits) of the microcontroller.
 *
 * Creator:                 J. Rebetez
 * Date of creation:        27.01.2016
 * Last modification on:    -
 * Modified by:             -
 * 
 * Input:                   -
 * Output:                  -
***********************************************************************************************************************/
void InitTimer23(void)
{
    T2CONbits.T32 = 1;  // Timer in 32 bits mode
    T2CONbits.ON = 0;   // Timer is OFF
    T3CON = 0;          // Has no effect in 32 bits mode

    T2CONbits.TCKPS2 = 0;   // 111 = 1:256 prescale value
    T2CONbits.TCKPS1 = 0;   // 110 = 1:64 prescale value
    T2CONbits.TCKPS0 = 0;   // 101 = 1:32 prescale value
                            // 100 = 1:16 prescale value
                            // 011 = 1:8 prescale value
                            // 010 = 1:4 prescale value
                            // 001 = 1:2 prescale value
                            // 000 = 1:1 prescale value

    TMR2 = 0;           // LSB --> clear counter
    TMR3 = 0;           // MSB --> clear counter
    PR2 = 400;          // LSB --> set to 10us
    PR3 = 0;            // MSB --> (400 * 25ns)
}

/**********************************************************************************************************************
 * Routine:                 InitTimer45
 *
 * Description:             Initialization of the timer 45 (32 bits) of the microcontroller.
 *
 * Creator:                 J. Rebetez
 * Date of creation:        27.01.2016
 * Last modification on:    -
 * Modified by:             -
 * 
 * Input:                   -
 * Output:                  -
***********************************************************************************************************************/
void InitTimer45(void)
{
    T4CON = 0x0;        //clear settings of timer 4
    T5CON = 0x0;        //clear settings of timer 5
    
    T4CON = 0x0018;     //configure timer 4
                        //timer in 32 bit mode
                        //use internal peripheral clock
                        //prescale value = 1:2 = 40MHz
                        //timer 4 is disabled

    TMR4 = 0x0;         //clear contents of the TMR4 and TMR5
    PR4 = 400;          //load PR4 and PR5 with 10us
}

/**********************************************************************************************************************
 * Routine:                 InitInterruptTimer

 * Description:
 * Initialization of the interrupts of the selected timer.
 * Timer 1: used
 * Priority = 5
 * Subpriority = 3
 * 
 * Timer 2: used in combination; see Timer 23
 * Priority = 1
 * Subpriority = 3
 * 
 * Timer 3: used in combination; see Timer 23
 * 
 * Timer 4: not used
 * 
 * Timer 5: not used
 * 
 * Timer 23 (timer 2 in 32-bit): not used
 * 
 * Timer 45 (timer 4 in 32-bit): used for bipolar and unipolar actuator
 * Priority = 1
 * Subpriority = 3
 * 
 * 
 * Modification (29.12.2015 / A. Staub):
 * Changed timer 2 and 3 to 4 and 5
 * 
 * Modification (30.12.2015 / A. Staub):
 * Change priority of the timer 45. It was only on priority 1 and all others are higher but this 
 * one should have more priority. Changed to 6. 
 * 
 * Creator:                 J. Rebetez
 * Date of creation:        08.08.2015
 * Last modification on:    30.12.2015
 * Modified by:             A. Staub
 * 
 * Input:                   timerx  (selected timer)
 *                          action  (enable / disable interrupt)
 * Output:                  -
***********************************************************************************************************************/
void InitInterruptTimer(unsigned char timerx, unsigned char action)
{
    if(timerx == _TIMER1)
    {
        IFS0CLR = _IFS0_T1IF_MASK;
        IPC1bits.T1IP = 1;
        IPC1bits.T1IS = 3;
        if(action == _ENABLE) 
            IEC0SET = _IEC0_T1IE_MASK;
        else
            IEC0CLR = _IEC0_T1IE_MASK;
    }
    else if(timerx == _TIMER2)
    {
        IFS0CLR = _IFS0_T2IF_MASK;
        IPC2bits.T2IP = 2;
        IPC2bits.T2IS = 3;
        if(action == _ENABLE)
            IEC0SET = _IEC0_T2IE_MASK;
        else
            IEC0CLR = _IEC0_T2IE_MASK;
    }
    else if(timerx == _TIMER3)
    {
        IFS0CLR = _IFS0_T3IF_MASK;
        IPC3bits.T3IP = 1;
        IPC3bits.T3IS = 3;
        if(action == _ENABLE)
            IEC0SET = _IEC0_T3IE_MASK;
        else
            IEC0CLR = _IEC0_T3IE_MASK;
    }
    else if(timerx == _TIMER4)
    {
        IFS0CLR = _IFS0_T4IF_MASK;
        IPC4bits.T4IP = 7;
        IPC4bits.T4IS = 3;
        if(action == _ENABLE)
            IEC0SET = _IEC0_T4IE_MASK;
        else
            IEC0CLR = _IEC0_T4IE_MASK;
    }
    else if(timerx == _TIMER5)
    {
        IFS0CLR = _IFS0_T5IF_MASK;
        IPC5bits.T5IP = 7;
        IPC5bits.T5IS = 3;
        if(action == _ENABLE)
            IEC0SET = _IEC0_T5IE_MASK;
        else
            IEC0CLR = _IEC0_T5IE_MASK;
    }
    else if(timerx == _TIMER23)
    {
        IFS0CLR = _IFS0_T2IF_MASK;
        IFS0CLR = _IFS0_T3IF_MASK;
        IPC3bits.T3IP = 1;
        IPC3bits.T3IS = 3;
        if(action == _ENABLE)
            IEC0SET = _IEC0_T3IE_MASK;
        else
            IEC0CLR = _IEC0_T3IE_MASK;
    }
    else if(timerx == _TIMER45)
    {
        IFS0CLR = _IFS0_T4IF_MASK;
        IFS0CLR = _IFS0_T5IF_MASK;
        IPC5bits.T5IP = 6;
        IPC5bits.T5IS = 3;
        if(action == _ENABLE)
            IEC0SET = _IEC0_T5IE_MASK;
        else
            IEC0CLR = _IEC0_T5IE_MASK;
    }

}


/**********************************************************************************************************************
 * Routine:                 SetTimer

 * Description:
 * Start / stop and choose time for each 16bits timers.
 * Timer 1: used with a time base of 3.2us
 * 
 * Timer 2: used for PWM and frequency measure (Input capture module)
 * 
 * Timer 3: not used
 * 
 * Timer 4: used in combination; see Timer 45
 * DON'T USE THIS SUBROUTINE FOR TIMER 2! This timer is used in a special way for the unipolar driver.
 * 
 * Timer 5: used in combination; see Timer 45
 * DON'T USE THIS SUBROUTINE FOR TIMER 2! This timer is used in a special way for the unipolar driver.
 * 
 * Timer 23 (timer 2 in 32-bit): not used
 * 
 * Timer 45 (timer 4 in 32-bit): used for bipolar and unipolar driver
 * DON'T USE THIS SUBROUTINE FOR TIMER 23! This timer is used in a special way for the unipolar driver.  
 * 
 * Modification (26.12.2015 / A. Staub):
 * We use the timer 1 to apply waiting times in ms. So the given value "valuePRReg" has to be in ms.
 * It doesn't matter what you send for valueTMRReg, this
 * register is always set to 0 by using timer 1. 
 * For the waiting time; first we look how much time we can divide the wait time (ms) with 200ms
 * to know how much time we have to wait this time of 200ms. 
 * Then in a second step we load the rest of the time into the PR1, multiply it with 312 to have the time 
 * in ms and this time will be the first interrupt time. 
 * Important to know; once the safety flag is set the configuration can't be change, because this waiting
 * time have the first priority. This flag is reset to zero automatic if the waiting time occured 
 * (inside the interrupt routine).
 * 
 * Modification (29.12.2015 / A. Staub):
 * Timer 2 and 3 changed to timer 4 and 5. 
 * 
 * Creator:                 J. Rebetez
 * Date of creation:        08.08.2015
 * Last modification on:    29.12.2015
 * Modified by:             A. Staub
 * 
 * Input:                   timer (choose of the timer)
 *                          status (enable or disable)
 *                          valueTMRReg (init value of TMRx reg)
 *                          valuePRReg (init value of PRx reg)
 * Output:                  -
***********************************************************************************************************************/
void SetTimer(unsigned char timer, unsigned char status, unsigned long int valueTMRReg, unsigned long int valuePRReg)
{
    switch(timer)
    {
	case _TIMER1:
        if(!g_Timer1.uint8_Timer1SafFlag)   //safety flag not set?
        {
            if(status)
            {
                g_Timer1.uint8_200msCount = valuePRReg / 200;  //divide the wait time (ms) with 200 ms 
                PR1 = valuePRReg % 200; //load the PR1 with the rest of the time
                if(PR1 == 0)            //rest time = 0?
                {
                    PR1 = 312 * 200;    //load PR1 with 312 (1ms) * 200 to have a wait time of 200ms
                    g_Timer1.uint8_200msCount--;  //decrement counter                    
                }
                else
                {
                    PR1 = PR1 * 312;    //multiply with 312 to have the time in ms
                }
                TMR1 = 0;               //start by 0 with counting 
                T1CONbits.ON = 1;       //enable timer 1
            }
            else 
            {
                T1CONbits.ON = 0;       //disable timer 1
            } 
        }
        else
        {
            //do nothing because if the safety flag is set then it is not allowed to change 
            //this interrupt configuration 
        }
    break;
	case _TIMER2:	
        if(status)
        {
            TMR2 = valueTMRReg;
            PR2 = valuePRReg;
            T2CONbits.ON = 1;
        }
        else T2CONbits.ON = 0;
	break;
	case _TIMER3:	
        if(status)
        {
            TMR3 = valueTMRReg;
            PR3 = valuePRReg;
            T3CONbits.ON = 1;
        }
        else T3CONbits.ON = 0;
    break;
	case _TIMER4:	
        if(status)
        {
            TMR4 = valueTMRReg;
            PR4 = valuePRReg;
            T4CONbits.ON = 1;
        }
        else T4CONbits.ON = 0;
    break;
	case _TIMER5:	
        if(status)
        {
            TMR5 = valueTMRReg;
            PR5 = valuePRReg;
            T5CONbits.ON = 1;
        }
        else T5CONbits.ON = 0;
    break;
    case _TIMER23:	
        if(status)
        {
            TMR2 = valueTMRReg;
            PR2 = valuePRReg;
            T2CONbits.ON = 1;
        }
        else T2CONbits.ON = 0;
    break;
	case _TIMER45:	
        if(status)
        {
            TMR4 = valueTMRReg;
            PR4 = valuePRReg;
            T4CONbits.ON = 1;
        }
        else T4CONbits.ON = 0;
    break;
    }
}