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
 * Routine:                 timers_Init

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
 * 
 * Timer 45 (timer 4 in 32-bit): not used 
 * 
 * Modification (23.12.2015):
 * Timer 2 and 3 combined to an 32-bit timer. Time base changed to 25ns. 
 * 
 * 
 * Creator:                 J. Rebetez
 * Date of creation:        08.08.2015
 * Last modification on:    23.12.2015
 * Modified by:             A. Staub
 * 
 * Input:                   timerx  (selected timer)
 * Output:                  -
***********************************************************************************************************************/
void timers_Init(unsigned char timerx)
{
    if(timerx == _TIMER1)
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
    else if(timerx == _TIMER2)
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
        T2CONbits.TCKPS1 = 1;   // 110 = 1:64 prescale value
        T2CONbits.TCKPS0 = 1;   // 101 = 1:32 prescale value
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
        PR2 = 100;              //set comperator to 10us (100 * 100ns)
    }
    else if(timerx == _TIMER3)
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
    else if(timerx == _TIMER4)
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
    else if(timerx == _TIMER5)
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
    else if(timerx == _TIMER23)
    {
        T2CONbits.T32 = 1;  // Timer in 32 bits mode
        T2CONbits.ON = 0;   // Timer is OFF
        T3CON = 0;          // Has no effect in 32 bits mode

        T2CONbits.TCKPS2 = 0;   // 111 = 1:256 prescale value
        T2CONbits.TCKPS1 = 0;   // 110 = 1:64 prescale value
        T2CONbits.TCKPS0 = 1;   // 101 = 1:32 prescale value
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
    else if(timerx == _TIMER45)
    {
        T4CONbits.T32 = 1;  // Timer in 32 bits mode
        T4CONbits.ON = 0;   // Timer is OFF
        T5CON = 0;          // Has no effect in 32 bits mode

        T4CONbits.TCKPS2 = 0;   // 111 = 1:256 prescale value
        T4CONbits.TCKPS1 = 0;   // 110 = 1:64 prescale value
        T4CONbits.TCKPS0 = 0;   // 101 = 1:32 prescale value
                                // 100 = 1:16 prescale value
                                // 011 = 1:8 prescale value
                                // 010 = 1:4 prescale value
                                // 001 = 1:2 prescale value
                                // 000 = 1:1 prescale value

        TMR4 = 0;           // LSB
        TMR5 = 0;           // MSB
        PR4 = 0;            // LSB
        PR5 = 0;            // MSB
    }
}

/**********************************************************************************************************************
 * Routine:                 timers_SetInterrupt

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
 * Timer 23 (timer 2 in 32-bit): used for bipolar and unipolar actuator
 * Priority = 1
 * Subpriority = 3
 * 
 * Timer 45 (timer 4 in 32-bit): not used 
 * 
 * Creator:                 J. Rebetez
 * Date of creation:        08.08.2015
 * Last modification on:    -
 * Modified by:             - 
 * 
 * Input:                   timerx  (selected timer)
 *                          action  (enable / disable interrupt)
 * Output:                  -
***********************************************************************************************************************/
void timers_SetInterrupt(unsigned char timerx, unsigned char action)
{
    if(timerx == _TIMER1)
    {
        IFS0bits.T1IF = 0;
        IPC1bits.T1IP = 5;
        IPC1bits.T1IS = 3;
        if(action == _ENABLE) IEC0bits.T1IE = 1;
        else IEC0bits.T1IE = 0;
    }
    else if(timerx == _TIMER2)
    {
        IFS0bits.T2IF = 0;
        IPC2bits.T2IP = 1;
        IPC2bits.T2IS = 3;
        if(action == _ENABLE) IEC0bits.T2IE = 1;
        else IEC0bits.T2IE = 0;
    }
    else if(timerx == _TIMER3)
    {
        IFS0bits.T3IF = 0;
        IPC3bits.T3IP = 1;
        IPC3bits.T3IS = 3;
        if(action == _ENABLE) IEC0bits.T3IE = 1;
        else IEC0bits.T3IE = 0;
    }
    else if(timerx == _TIMER4)
    {
        IFS0bits.T4IF = 0;
        IPC4bits.T4IP = 7;
        IPC4bits.T4IS = 3;
        if(action == _ENABLE) IEC0bits.T4IE = 1;
        else IEC0bits.T4IE = 0;
    }
    else if(timerx == _TIMER5)
    {
        IFS0bits.T5IF = 0;
        IPC5bits.T5IP = 7;
        IPC5bits.T5IS = 3;
        if(action == _ENABLE) IEC0bits.T5IE = 1;
        else IEC0bits.T5IE = 0;
    }
    else if(timerx == _TIMER23)
    {
        IFS0bits.T2IF = 0;  //just to be safe
        IFS0bits.T3IF = 0;
        IPC3bits.T3IP = 1;
        IPC3bits.T3IS = 3;
        if(action == _ENABLE) IEC0bits.T3IE = 1;
        else IEC0bits.T3IE = 0;
    }
    else if(timerx == _TIMER45)
    {
        IFS0bits.T4IF = 0;  //just to be safe
        IFS0bits.T5IF = 0;
        IPC5bits.T5IP = 7;
        IPC5bits.T5IS = 3;
        if(action == _ENABLE) IEC0bits.T5IE = 1;
        else IEC0bits.T5IE = 0;
    }

}


/**********************************************************************************************************************
 * Routine:                 timers_Set

 * Description:
 * Start / stop and choose time for each 16bits timers.
 * Timer 1: used with a time base of 3.2us
 * 
 * Timer 2: used in combination; see Timer 23
 * DON'T USE THIS SUBROUTINE FOR TIMER 2! This timer is used in a special way for the unipolar driver.
 * 
 * Timer 3: used in combination; see Timer 23
 * DON'T USE THIS SUBROUTINE FOR TIMER 2! This timer is used in a special way for the unipolar driver.
 * 
 * Timer 4: not used
 * 
 * Timer 5: not used
 * 
 * Timer 23 (timer 2 in 32-bit): used for bipolar and unipolar driver
 * DON'T USE THIS SUBROUTINE FOR TIMER 23! This timer is used in a special way for the unipolar driver.
 * 
 * Timer 45 (timer 4 in 32-bit): not used  
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
 * Creator:                 J. Rebetez
 * Date of creation:        08.08.2015
 * Last modification on:    26.12.2015
 * Modified by:             A. Staub
 * 
 * Input:                   timer (choose of the timer)
 *                          status (enable or disable)
 *                          valueTMRReg (init value of TMRx reg)
 *                          valuePRReg (init value of PRx reg)
 * Output:                  -
***********************************************************************************************************************/
void timers_Set(unsigned char timer, unsigned char status, unsigned long int valueTMRReg, unsigned long int valuePRReg)
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
	case _TIMER2:	if(status)
			{
                            TMR2 = valueTMRReg;
                            PR2 = valuePRReg;
                            T2CONbits.ON = 1;
			}
			else T2CONbits.ON = 0;
			break;
	case _TIMER3:	if(status)
        		{
                            TMR3 = valueTMRReg;
                            PR3 = valuePRReg;
                            T3CONbits.ON = 1;
			}
			else T3CONbits.ON = 0;
			break;
	case _TIMER4:	if(status)
			{
                            TMR4 = valueTMRReg;
                            PR4 = valuePRReg;
                            T4CONbits.ON = 1;
			}
			else T4CONbits.ON = 0;
			break;
	case _TIMER5:	if(status)
			{
                            TMR5 = valueTMRReg;
                            PR5 = valuePRReg;
                            T5CONbits.ON = 1;
			}
			else T5CONbits.ON = 0;
			break;
        case _TIMER23:	if(status)
			{
                            TMR2 = valueTMRReg;
                            PR2 = valuePRReg;
                            T2CONbits.ON = 1;
			}
			else T2CONbits.ON = 0;
			break;
	case _TIMER45:	if(status)
			{
                            TMR4 = valueTMRReg;
                            PR4 = valuePRReg;
                            T4CONbits.ON = 1;
			}
			else T4CONbits.ON = 0;
			break;
    }
}