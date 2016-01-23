/********************************************************************************************************************/
/*  Name of the file:       inputCapture.								    
/*  Purpose of the file:    Definitions of all functions related to the input capture module						    
/*  Creator:                julien_rebetez									    
/*  Date of creation:       January 18, 2016									    
/*														    
/*  Last modification on:   -											    
/*  Modified by:            -											    
/*  Version:                -											    
/*														    
/*  List of functions:      -											    
/* ******************************************************************************************************************/

#include "includes.h" // File which contain all includes files

/********************************************************************************************************************/
/*  Name of the function:       InitInputCapture1Module									    
/*  Purpose of the function:    Initialization of the input capture module 1							    
/*  Parameters:													    
/*      IN:                     -										    
/*      OUT:                    -										    
/*														    
/*  Used global variables:      -										    
/*														    
/*  Creator:                    julien_rebetez								    
/*  Date of creation:           January 18, 2016								    
/*														    
/*  Last modified on:           -										    
/*  Modified by:                -										    
/*  Version:                    -										    
/*														    
/*  Remark:                     -										    
/********************************************************************************************************************/
void InitInputCapture1Module(void)
{
    IC1CONbits.ON = 0;      // Input Capture Module Enable bit
                            // 1 = Module enabled
                            // 0 = Disable and reset module, disable clocks, disable interrupt generation and allow SFR
                            // modifications
    
    IC1CONbits.SIDL = 1;    // Stop in Idle Control bit
                            // 1 = Halt in CPU Idle mode
                            // 0 = Continue to operate in CPU Idle mode
    
    IC1CONbits.FEDGE = 1;   // First Capture Edge Select bit (only used in mode 6, ICM<2:0> = 110)
                            // 1 = Capture rising edge first
                            // 0 = Capture falling edge first
    
    IC1CONbits.C32 = 0;     // 32-bit Capture Select bit
                            // 1 = 32-bit timer resource capture
                            // 0 = 16-bit timer resource capture
    
    IC1CONbits.ICTMR = 1;   // Timer Select bit (Does not affect timer selection when C32 (ICxCON<8>) is ?1?)
                            // 0 = Timer3 is the counter source for capture
                            // 1 = Timer2 is the counter source for capture

    IC1CONbits.ICI = 3;     // Interrupt Control bits
                            // 11 = Interrupt on every fourth capture event
                            // 10 = Interrupt on every third capture event
                            // 01 = Interrupt on every second capture event
                            // 00 = Interrupt on every capture event
    
    IC1CONbits.ICOV = 0;    // Input Capture Overflow Status Flag bit (read-only)
                            // 1 = Input capture overflow occurred
                            // 0 = No input capture overflow occurred

    IC1CONbits.ICBNE = 0;   // Input Capture Buffer Not Empty Status bit (read-only)
                            // 1 = Input capture buffer is not empty; at least one more capture value can be read
                            // 0 = Input capture buffer is empty
    
    IC1CONbits.ICM = 6;     // Input Capture Mode Select bits
                            // 111 = Interrupt-Only mode (only supported while in Sleep mode or Idle mode)
                            // 110 = Simple Capture Event mode ? every edge, specified edge first and every edge thereafter
                            // 101 = Prescaled Capture Event mode ? every sixteenth rising edge
                            // 100 = Prescaled Capture Event mode ? every fourth rising edge
                            // 011 = Simple Capture Event mode ? every rising edge
                            // 010 = Simple Capture Event mode ? every falling edge
                            // 001 = Edge Detect mode ? every edge (rising and falling)
                            // 000 = Capture Disable mode
    
    TRISDbits.TRISD8 = 1;   // Set PIN RD8 (IC1) as input
}

/********************************************************************************************************************/
/*  Name of the function:       InitInputCapture2Module									    
/*  Purpose of the function:    Initialization of the input capture module 12						    
/*  Parameters:													    
/*      IN:                     -										    
/*      OUT:                    -										    
/*														    
/*  Used global variables:      -										    
/*														    
/*  Creator:                    julien_rebetez								    
/*  Date of creation:           January 18, 2016								    
/*														    
/*  Last modified on:           -										    
/*  Modified by:                -										    
/*  Version:                    -										    
/*														    
/*  Remark:                     -										    
/********************************************************************************************************************/
void InitInputCapture2Module(void)
{
    IC2CONbits.ON = 0;      // Input Capture Module Enable bit
                            // 1 = Module enabled
                            // 0 = Disable and reset module, disable clocks, disable interrupt generation and allow SFR
                            // modifications
    
    IC2CONbits.SIDL = 1;    // Stop in Idle Control bit
                            // 1 = Halt in CPU Idle mode
                            // 0 = Continue to operate in CPU Idle mode
    
    IC2CONbits.FEDGE = 1;   // First Capture Edge Select bit (only used in mode 6, ICM<2:0> = 110)
                            // 1 = Capture rising edge first
                            // 0 = Capture falling edge first
    
    IC2CONbits.C32 = 0;     // 32-bit Capture Select bit
                            // 1 = 32-bit timer resource capture
                            // 0 = 16-bit timer resource capture
    
    IC2CONbits.ICTMR = 1;   // Timer Select bit (Does not affect timer selection when C32 (ICxCON<8>) is ?1?)
                            // 0 = Timer3 is the counter source for capture
                            // 1 = Timer2 is the counter source for capture

    IC2CONbits.ICI = 3;     // Interrupt Control bits
                            // 11 = Interrupt on every fourth capture event
                            // 10 = Interrupt on every third capture event
                            // 01 = Interrupt on every second capture event
                            // 00 = Interrupt on every capture event
    
    IC2CONbits.ICOV = 0;    // Input Capture Overflow Status Flag bit (read-only)
                            // 1 = Input capture overflow occurred
                            // 0 = No input capture overflow occurred

    IC2CONbits.ICBNE = 0;   // Input Capture Buffer Not Empty Status bit (read-only)
                            // 1 = Input capture buffer is not empty; at least one more capture value can be read
                            // 0 = Input capture buffer is empty
    
    IC2CONbits.ICM = 3;     // Input Capture Mode Select bits
                            // 111 = Interrupt-Only mode (only supported while in Sleep mode or Idle mode)
                            // 110 = Simple Capture Event mode ? every edge, specified edge first and every edge thereafter
                            // 101 = Prescaled Capture Event mode ? every sixteenth rising edge
                            // 100 = Prescaled Capture Event mode ? every fourth rising edge
                            // 011 = Simple Capture Event mode ? every rising edge
                            // 010 = Simple Capture Event mode ? every falling edge
                            // 001 = Edge Detect mode ? every edge (rising and falling)
                            // 000 = Capture Disable mode
    
    TRISDbits.TRISD9 = 1;   // Set PIN RD9 (IC2) as input
}

/********************************************************************************************************************/
/*  Name of the function:       InitInputCapture3Module									    
/*  Purpose of the function:    Initialization of the input capture module 3
/*  Parameters:													    
/*      IN:                     -										    
/*      OUT:                    -										    
/*														    
/*  Used global variables:      -										    
/*														    
/*  Creator:                    julien_rebetez								    
/*  Date of creation:           January 18, 2016								    
/*														    
/*  Last modified on:           -										    
/*  Modified by:                -										    
/*  Version:                    -										    
/*														    
/*  Remark:                     -										    
/********************************************************************************************************************/
void InitInputCapture3Module(void)
{
    IC3CONbits.ON = 0;      // Input Capture Module Enable bit
                            // 1 = Module enabled
                            // 0 = Disable and reset module, disable clocks, disable interrupt generation and allow SFR
                            // modifications
    
    IC3CONbits.SIDL = 1;    // Stop in Idle Control bit
                            // 1 = Halt in CPU Idle mode
                            // 0 = Continue to operate in CPU Idle mode
    
    IC3CONbits.FEDGE = 1;   // First Capture Edge Select bit (only used in mode 6, ICM<2:0> = 110)
                            // 1 = Capture rising edge first
                            // 0 = Capture falling edge first
    
    IC3CONbits.C32 = 0;     // 32-bit Capture Select bit
                            // 1 = 32-bit timer resource capture
                            // 0 = 16-bit timer resource capture
    
    IC3CONbits.ICTMR = 1;   // Timer Select bit (Does not affect timer selection when C32 (ICxCON<8>) is ?1?)
                            // 0 = Timer3 is the counter source for capture
                            // 1 = Timer2 is the counter source for capture

    IC3CONbits.ICI = 3;     // Interrupt Control bits
                            // 11 = Interrupt on every fourth capture event
                            // 10 = Interrupt on every third capture event
                            // 01 = Interrupt on every second capture event
                            // 00 = Interrupt on every capture event
    
    IC3CONbits.ICOV = 0;    // Input Capture Overflow Status Flag bit (read-only)
                            // 1 = Input capture overflow occurred
                            // 0 = No input capture overflow occurred

    IC3CONbits.ICBNE = 0;   // Input Capture Buffer Not Empty Status bit (read-only)
                            // 1 = Input capture buffer is not empty; at least one more capture value can be read
                            // 0 = Input capture buffer is empty
    
    IC3CONbits.ICM = 3;     // Input Capture Mode Select bits
                            // 111 = Interrupt-Only mode (only supported while in Sleep mode or Idle mode)
                            // 110 = Simple Capture Event mode ? every edge, specified edge first and every edge thereafter
                            // 101 = Prescaled Capture Event mode ? every sixteenth rising edge
                            // 100 = Prescaled Capture Event mode ? every fourth rising edge
                            // 011 = Simple Capture Event mode ? every rising edge
                            // 010 = Simple Capture Event mode ? every falling edge
                            // 001 = Edge Detect mode ? every edge (rising and falling)
                            // 000 = Capture Disable mode
    
    TRISDbits.TRISD10 = 1;  // Set PIN RD10 (IC3) as input
}

/********************************************************************************************************************/
/*  Name of the function:       InitInputCapture4Module									    
/*  Purpose of the function:    Initialization of the input capture module 4	    
/*  Parameters:													    
/*      IN:                     -										    
/*      OUT:                    -										    
/*														    
/*  Used global variables:      -										    
/*														    
/*  Creator:                    julien_rebetez								    
/*  Date of creation:           January 18, 2016								    
/*														    
/*  Last modified on:           -										    
/*  Modified by:                -										    
/*  Version:                    -										    
/*														    
/*  Remark:                     -										    
/********************************************************************************************************************/
void InitInputCapture4Module(void)
{
    IC4CONbits.ON = 0;      // Input Capture Module Enable bit
                            // 1 = Module enabled
                            // 0 = Disable and reset module, disable clocks, disable interrupt generation and allow SFR
                            // modifications
    
    IC4CONbits.SIDL = 1;    // Stop in Idle Control bit
                            // 1 = Halt in CPU Idle mode
                            // 0 = Continue to operate in CPU Idle mode
    
    IC4CONbits.FEDGE = 1;   // First Capture Edge Select bit (only used in mode 6, ICM<2:0> = 110)
                            // 1 = Capture rising edge first
                            // 0 = Capture falling edge first
    
    IC4CONbits.C32 = 0;     // 32-bit Capture Select bit
                            // 1 = 32-bit timer resource capture
                            // 0 = 16-bit timer resource capture
    
    IC4CONbits.ICTMR = 1;   // Timer Select bit (Does not affect timer selection when C32 (ICxCON<8>) is ?1?)
                            // 0 = Timer3 is the counter source for capture
                            // 1 = Timer2 is the counter source for capture

    IC4CONbits.ICI = 3;     // Interrupt Control bits
                            // 11 = Interrupt on every fourth capture event
                            // 10 = Interrupt on every third capture event
                            // 01 = Interrupt on every second capture event
                            // 00 = Interrupt on every capture event
    
    IC4CONbits.ICOV = 0;    // Input Capture Overflow Status Flag bit (read-only)
                            // 1 = Input capture overflow occurred
                            // 0 = No input capture overflow occurred

    IC4CONbits.ICBNE = 0;   // Input Capture Buffer Not Empty Status bit (read-only)
                            // 1 = Input capture buffer is not empty; at least one more capture value can be read
                            // 0 = Input capture buffer is empty
    
    IC4CONbits.ICM = 3;     // Input Capture Mode Select bits
                            // 111 = Interrupt-Only mode (only supported while in Sleep mode or Idle mode)
                            // 110 = Simple Capture Event mode ? every edge, specified edge first and every edge thereafter
                            // 101 = Prescaled Capture Event mode ? every sixteenth rising edge
                            // 100 = Prescaled Capture Event mode ? every fourth rising edge
                            // 011 = Simple Capture Event mode ? every rising edge
                            // 010 = Simple Capture Event mode ? every falling edge
                            // 001 = Edge Detect mode ? every edge (rising and falling)
                            // 000 = Capture Disable mode
    
    TRISDbits.TRISD11 = 1;  // Set PIN RD11 (IC4) as input
}

/********************************************************************************************************************/
/*  Name of the function:       InitInputCapture5Module									    
/*  Purpose of the function:    Initialization of the input capture module 5					    
/*  Parameters:													    
/*      IN:                     -										    
/*      OUT:                    -										    
/*														    
/*  Used global variables:      -										    
/*														    
/*  Creator:                    julien_rebetez								    
/*  Date of creation:           January 18, 2016								    
/*														    
/*  Last modified on:           -										    
/*  Modified by:                -										    
/*  Version:                    -										    
/*														    
/*  Remark:                     -										    
/********************************************************************************************************************/
void InitInputCapture5Module(void)
{
    IC5CONbits.ON = 0;      // Input Capture Module Enable bit
                            // 1 = Module enabled
                            // 0 = Disable and reset module, disable clocks, disable interrupt generation and allow SFR
                            // modifications
    
    IC5CONbits.SIDL = 1;    // Stop in Idle Control bit
                            // 1 = Halt in CPU Idle mode
                            // 0 = Continue to operate in CPU Idle mode
    
    IC5CONbits.FEDGE = 1;   // First Capture Edge Select bit (only used in mode 6, ICM<2:0> = 110)
                            // 1 = Capture rising edge first
                            // 0 = Capture falling edge first
    
    IC5CONbits.C32 = 0;     // 32-bit Capture Select bit
                            // 1 = 32-bit timer resource capture
                            // 0 = 16-bit timer resource capture
    
    IC5CONbits.ICTMR = 1;   // Timer Select bit (Does not affect timer selection when C32 (ICxCON<8>) is ?1?)
                            // 0 = Timer3 is the counter source for capture
                            // 1 = Timer2 is the counter source for capture

    IC5CONbits.ICI = 3;     // Interrupt Control bits
                            // 11 = Interrupt on every fourth capture event
                            // 10 = Interrupt on every third capture event
                            // 01 = Interrupt on every second capture event
                            // 00 = Interrupt on every capture event
    
    IC5CONbits.ICOV = 0;    // Input Capture Overflow Status Flag bit (read-only)
                            // 1 = Input capture overflow occurred
                            // 0 = No input capture overflow occurred

    IC5CONbits.ICBNE = 0;   // Input Capture Buffer Not Empty Status bit (read-only)
                            // 1 = Input capture buffer is not empty; at least one more capture value can be read
                            // 0 = Input capture buffer is empty
    
    IC5CONbits.ICM = 3;     // Input Capture Mode Select bits
                            // 111 = Interrupt-Only mode (only supported while in Sleep mode or Idle mode)
                            // 110 = Simple Capture Event mode ? every edge, specified edge first and every edge thereafter
                            // 101 = Prescaled Capture Event mode ? every sixteenth rising edge
                            // 100 = Prescaled Capture Event mode ? every fourth rising edge
                            // 011 = Simple Capture Event mode ? every rising edge
                            // 010 = Simple Capture Event mode ? every falling edge
                            // 001 = Edge Detect mode ? every edge (rising and falling)
                            // 000 = Capture Disable mode
    
    TRISDbits.TRISD12 = 1;  // Set PIN RD12 (IC5) as input
}

/********************************************************************************************************************/
/*  Name of the function:       InitInterruptInputCaptureModule									    
/*  Purpose of the function:    Initialization of the interrupt of the input capture modules
/*  Parameters:													    
/*      IN:                     - module: select the module to initialize
/* 								- status: enable or disable of the module	    
/*      OUT:                    -			
/* 							    See the .h file for the list of all defines.
/*														    
/*  Used global variables:      -										    
/*														    
/*  Creator:                    julien_rebetez								    
/*  Date of creation:           January 18, 2016								    
/*														    
/*  Last modified on:           -										    
/*  Modified by:                -										    
/*  Version:                    -										    
/*														    
/*  Remark:                     -										    
/********************************************************************************************************************/
void InitInterruptInputCaptureModule(unsigned char module, unsigned char status)
{
    if(module == _IC_1)
    {
        if(status == _ENABLE)
        {
            IFS0bits.IC1IF = 0;
            IPC1bits.IC1IP = 2;
            IPC1bits.IC1IS = 3;
            IEC0bits.IC1IE = 1;
        }
        else
        {
            IEC0bits.IC1IE = 0;
            IFS0bits.IC1IF = 0;
        }
    }
    else if(module == _IC_2)
    {
        if(status == _ENABLE)
        {
            IFS0bits.IC2IF = 0;
            IPC2bits.IC2IP = 2;
            IPC2bits.IC2IS = 3;
            IEC0bits.IC2IE = 1;
        }
        else
        {
            IEC0bits.IC2IE = 0;
            IFS0bits.IC2IF = 0;
        }
    }
    else if(module == _IC_3)
    {
        if(status == _ENABLE)
        {
            IFS0bits.IC3IF = 0;
            IPC3bits.IC3IP = 2;
            IPC3bits.IC3IS = 3;
            IEC0bits.IC3IE = 1;
        }
        else
        {
            IEC0bits.IC3IE = 0;
            IFS0bits.IC3IF = 0;
        }
    }   
    else if(module == _IC_4)
    {
        if(status == _ENABLE)
        {
            IFS0bits.IC4IF = 0;
            IPC4bits.IC4IP = 2;
            IPC4bits.IC4IS = 3;
            IEC0bits.IC4IE = 1;
        }
        else
        {
            IEC0bits.IC4IE = 0;
            IFS0bits.IC4IF = 0;
        }
    }
    else if(module == _IC_5)
    {
        if(status == _ENABLE)
        {
            IFS0bits.IC5IF = 0;
            IPC5bits.IC5IP = 2;
            IPC5bits.IC5IS = 3;
            IEC0bits.IC5IE = 1;
        }
        else
        {
            IEC0bits.IC5IE = 0;
            IFS0bits.IC5IF = 0;
        }
    }
}