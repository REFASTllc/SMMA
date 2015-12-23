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

STimer2 g_Timer2;     //global variables for struct


/**********************************************************************************************************************
 * Routine:                 timers_Init

 * Description:
 * Initialization of the timers of the microcontroller.
 * Timer 1: used 
 * Clock source = PBCLK = 80MHz with prescaler 256 = 1 / (80MHz / 256) = 3.2us time base 
 * 
 * Timer 2: used
 * Clock source = PBCLK = 80MHz with prescaler 8 = 1 / (80MHz / 8) = 100ns time base
 * Used for the unipolar driver - DON'T USE THIS TIMER FOR SOMETHING OTHERS!!!
 * 
 * Timer 3: not used
 * 
 * Timer 4: not used
 * 
 * Timer 5: not used
 * 
 * Timer 23 (timer 2 in 32-bit): not used
 * 
 * Timer 45 (timer 4 in 32-bit): not used 
 * 
 * 
 * Creator:                 J. Rebetez
 * Date of creation:        08.08.2015
 * Last modification on:    -
 * Modified by:             - 
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

        TMR2 = 0;
        PR2 = 100;
        
        g_Timer2.uint16_LastTime = 0;       //clear last time
        g_Timer2.uint16_Count = 0;          //reset counter
        g_Timer2.uint16_IntTime = 50000;    //interrupt time = 2.5ms (25000 * 100ns)
        
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
        T2CONbits.TCKPS0 = 0;   // 101 = 1:32 prescale value
                                // 100 = 1:16 prescale value
                                // 011 = 1:8 prescale value
                                // 010 = 1:4 prescale value
                                // 001 = 1:2 prescale value
                                // 000 = 1:1 prescale value

        TMR2 = 0;           // LSB
        TMR3 = 0;           // MSB
        PR2 = 0;            // LSB
        PR3 = 0;            // MSB
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
 * Timer 2: used
 * Priority = 1
 * Subpriority = 3
 * 
 * Timer 3: not used
 * 
 * Timer 4: not used
 * 
 * Timer 5: not used
 * 
 * Timer 23 (timer 2 in 32-bit): not used
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
        IFS0bits.T3IF = 0;
        IPC3bits.T3IP = 1;
        IPC3bits.T3IS = 3;
        if(action == _ENABLE) IEC0bits.T3IE = 1;
        else IEC0bits.T3IE = 0;
    }
    else if(timerx == _TIMER45)
    {
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
 * Timer 1: used
 * 
 * Timer 2: used
 * DON'T USE THIS SUBROUTINE FOR TIMER 2! This timer is used in a special way for the unipolar driver.
 * 
 * Timer 3: not used
 * 
 * Timer 4: not used
 * 
 * Timer 5: not used
 * 
 * Timer 23 (timer 2 in 32-bit): not used
 * 
 * Timer 45 (timer 4 in 32-bit): not used  
 * 
 * Creator:                 J. Rebetez
 * Date of creation:        08.08.2015
 * Last modification on:    -
 * Modified by:             - 
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
	case _TIMER1:	if(status)
        		{
                            TMR1 = valueTMRReg;
                            PR1 = valuePRReg;
                            T1CONbits.ON = 1;
			}
			else T1CONbits.ON = 0;
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