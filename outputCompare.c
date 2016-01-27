/****************************************************************************/
/*  Name of the file:       outputCompare.c                                 */
/*  Purpose of the file:    Functions related to output compare and PWM     */
/*                          module                                          */
/*  Creator:                julien_rebetez                                  */
/*  Date of creation:       24 août 2015, 21:57                             */
/*                                                                          */
/*  Last modification on:   -                                               */
/*  Modified by:            -                                               */
/*  Version:                -                                               */
/*                                                                          */
/*  List of functions:      InitPWM                                         */
/*                          SetPWM                                          */
/*                          SetPWMInterrupt                                 */
/*                          SetPWMFrequency                                 */
/*                          SetPWMDutyCycle                                 */
/* **************************************************************************/

#include "includes.h" // File which contain all includes files

/****************************************************************************/
/*  Name of the function:       InitPWM                                     */
/*  Purpose of the function:    Initialization of the output compare module */
/*                              as a PWM output                             */
/*  Parameters:                                                             */
/*      IN:                     pwmx (selection of the PWM output)          */
/*                              See .h for define definitions.              */
/*      OUT:                    -                                           */
/*                                                                          */
/*  Used global variables:      -                                           */
/*                                                                          */
/*  Creator:                    julien_rebetez                              */
/*  Date of creation:           24 août 2015, 21:57                         */
/*                                                                          */
/*  Last modified on:           -                                           */
/*  Modified by:                -                                           */
/*  Version:                    -                                           */
/*                                                                          */
/*  Remark:                     -                                           */
/****************************************************************************/
void InitPWM(unsigned char pwmx)
{
/***************** H O W   T O   U S E   P W M   M O D U L E ****************/
/*  1.  PWM module use timer 3 or timer 2/3 (32-bit)                        */
/*  2.  If interrupt is needed, interrupt of related timer must be used     */
/****************************************************************************/

/*********** C a l c u l a t i n g   t h e   P W M   P e r i o d ************/
/*      PWM Period = [(PR + 1) * TPB * (TMR Prescale Value)]                */
/*                                                                          */
/*      PWM Frequency = 1/[PWM Period]                                      */
/****************************************************************************/
    if(pwmx == _PWM1)
    {
    //------ OC1R --------------------------------------------------------------//
        OC1R = 0x0;             // 16-bit compare value. 32-bit compare value if OC32 = 1.
                                // Read-only in PWM mode
    //------ OC1RS -------------------------------------------------------------//
        OC1RS = 0x0;            // 16-bit compare value. 32-bit compare value if OC32 = 1.
    //------ OC1CON ------------------------------------------------------------//
        OC1CONbits.ON = 0;      // Output Compare Peripheral On bit(1)
                                // 1 = Output compare peripheral is enabled
                                // 0 = Output compare peripheral is disabled and not drawing current.
                                // SFR modifications are allowed.
                                // The status of other bits in this register are not affected by setting or clearing this bit

        OC1CONbits.SIDL = 0;    // Stop in Idle Mode bit
                                // 1 = Discontinue operation when CPU enters Idle mode
                                // 0 = Continue operation in Idle mode

        OC1CONbits.OC32 = 1;    // 32-bit Compare Mode bit
                                // 1 = OCxR<31:0> and/or OCxRS<31:0> is used for comparisions to the 32-bit timer source
                                // 0 = OCxR<15:0> and OCxRS<15:0> are used for comparisons to the 16-bit timer source

        OC1CONbits.OCFLT = 0;   // PWM Fault Condition Status bit
                                // (This bit is only used when OCM<2:0> = 111 and reads as ?0? in modes other than PWM mode.)
                                // 1 = PWM Fault condition has occurred (cleared in hardware only)
                                // 0 = No PWM Fault condition has occurred

        OC1CONbits.OCTSEL = 0;  // Output Compare Timer Select bit
                                // 1 = Timer3 is the clock source for this Output Compare module
                                // 0 = Timer2 is the clock source for this Output Compare module

        OC1CONbits.OCM2 = 1;    // Output Compare Mode Select bits
        OC1CONbits.OCM1 = 1;    // 111 = PWM mode on OCx; Fault pin enabled
        OC1CONbits.OCM0 = 0;    // 110 = PWM mode on OCx; Fault pin disabled
                                // 101 = Initialize OCx pin low; generate continuous output pulses on OCx pin
                                // 100 = Initialize OCx pin low; generate single output pulse on OCx pin
                                // 011 = Compare event toggles OCx pin
                                // 010 = Initialize OCx pin high; compare event forces OCx pin low
                                // 001 = Initialize OCx pin low; compare event forces OCx pin high
                                // 000 = Output compare peripheral is disabled but continues to draw current
    //--- If PWM mode on OCx; Fault pin enabled is selected, pin OCFA must be defined as input ---//
        if(OC1CONbits.OCM == 7) TRISBbits.TRISB6 = 1;
    //--- Once the PWM module initialized, related timer is also initialized. ---//
    //--- If PWM is in 32-bit initialized, timer is also in 32-bit initialized. ---//
        if(OC1CONbits.OC32)
        {
            InitTimer23();
            PR2 = 0;
        }
        else if(OC1CONbits.OCTSEL)
        {
            InitTimer3();
            PR3 = 0;
        }
        else
        {
            InitTimer2();
            PR2 = 0;
        }
    }
    else if(pwmx == _PWM2)
    {
    //------ OC2R --------------------------------------------------------------//
        OC2R = 0;               // 16-bit compare value. 32-bit compare value if OC32 = 1.
                                // Read-only in PWM mode
    //------ OC2RS -------------------------------------------------------------//
        OC2RS = 0;              // 16-bit compare value. 32-bit compare value if OC32 = 1.
    //------ OC2CON ------------------------------------------------------------//
        OC2CONbits.ON = 0;      // Output Compare Peripheral On bit(1)
                                // 1 = Output compare peripheral is enabled
                                // 0 = Output compare peripheral is disabled and not drawing current.
                                // SFR modifications are allowed.
                                // The status of other bits in this register are not affected by setting or clearing this bit

        OC2CONbits.SIDL = 0;    // Stop in Idle Mode bit
                                // 1 = Discontinue operation when CPU enters Idle mode
                                // 0 = Continue operation in Idle mode

        OC2CONbits.OC32 = 1;    // 32-bit Compare Mode bit
                                // 1 = OCxR<31:0> and/or OCxRS<31:0> is used for comparisions to the 32-bit timer source
                                // 0 = OCxR<15:0> and OCxRS<15:0> are used for comparisons to the 16-bit timer source

        OC2CONbits.OCFLT = 0;   // PWM Fault Condition Status bit
                                // (This bit is only used when OCM<2:0> = 111 and reads as ?0? in modes other than PWM mode.)
                                // 1 = PWM Fault condition has occurred (cleared in hardware only)
                                // 0 = No PWM Fault condition has occurred

        OC2CONbits.OCTSEL = 0;  // Output Compare Timer Select bit
                                // 1 = Timer3 is the clock source for this Output Compare module
                                // 0 = Timer2 is the clock source for this Output Compare module

        OC2CONbits.OCM2 = 1;    // Output Compare Mode Select bits
        OC2CONbits.OCM1 = 1;    // 111 = PWM mode on OCx; Fault pin enabled
        OC2CONbits.OCM0 = 0;    // 110 = PWM mode on OCx; Fault pin disabled
                                // 101 = Initialize OCx pin low; generate continuous output pulses on OCx pin
                                // 100 = Initialize OCx pin low; generate single output pulse on OCx pin
                                // 011 = Compare event toggles OCx pin
                                // 010 = Initialize OCx pin high; compare event forces OCx pin low
                                // 001 = Initialize OCx pin low; compare event forces OCx pin high
                                // 000 = Output compare peripheral is disabled but continues to draw current
    //--- If PWM mode on OCx; Fault pin enabled is selected, pin OCFA must be defined as input ---//
        if(OC2CONbits.OCM == 7) TRISBbits.TRISB6 = 1;
    //--- Once the PWM module initialized, related timer is also initialized. ---//
    //--- If PWM is in 32-bit initialized, timer is also in 32-bit initialized. ---//
        if(OC2CONbits.OC32)
        {
            InitTimer23();
            PR2 = 0;
        }
        else if(OC2CONbits.OCTSEL)
        {
            InitTimer3();
            PR3 = 0;
        }
        else
        {
            InitTimer2();
            PR2 = 0;
        }
    }
    else if(pwmx == _PWM3)
    {
    //------ OC3R --------------------------------------------------------------//
        OC3R = 0;               // 16-bit compare value. 32-bit compare value if OC32 = 1.
                                // Read-only in PWM mode
    //------ OC3RS -------------------------------------------------------------//
        OC3RS = 0;              // 16-bit compare value. 32-bit compare value if OC32 = 1.
    //------ OC3CON ------------------------------------------------------------//
        OC3CONbits.ON = 0;      // Output Compare Peripheral On bit(1)
                                // 1 = Output compare peripheral is enabled
                                // 0 = Output compare peripheral is disabled and not drawing current.
                                // SFR modifications are allowed.
                                // The status of other bits in this register are not affected by setting or clearing this bit

        OC3CONbits.SIDL = 0;    // Stop in Idle Mode bit
                                // 1 = Discontinue operation when CPU enters Idle mode
                                // 0 = Continue operation in Idle mode

        OC3CONbits.OC32 = 1;    // 32-bit Compare Mode bit
                                // 1 = OCxR<31:0> and/or OCxRS<31:0> is used for comparisions to the 32-bit timer source
                                // 0 = OCxR<15:0> and OCxRS<15:0> are used for comparisons to the 16-bit timer source

        OC3CONbits.OCFLT = 0;   // PWM Fault Condition Status bit
                                // (This bit is only used when OCM<2:0> = 111 and reads as ?0? in modes other than PWM mode.)
                                // 1 = PWM Fault condition has occurred (cleared in hardware only)
                                // 0 = No PWM Fault condition has occurred

        OC3CONbits.OCTSEL = 0;  // Output Compare Timer Select bit
                                // 1 = Timer3 is the clock source for this Output Compare module
                                // 0 = Timer2 is the clock source for this Output Compare module

        OC3CONbits.OCM2 = 1;    // Output Compare Mode Select bits
        OC3CONbits.OCM1 = 1;    // 111 = PWM mode on OCx; Fault pin enabled
        OC3CONbits.OCM0 = 0;    // 110 = PWM mode on OCx; Fault pin disabled
                                // 101 = Initialize OCx pin low; generate continuous output pulses on OCx pin
                                // 100 = Initialize OCx pin low; generate single output pulse on OCx pin
                                // 011 = Compare event toggles OCx pin
                                // 010 = Initialize OCx pin high; compare event forces OCx pin low
                                // 001 = Initialize OCx pin low; compare event forces OCx pin high
                                // 000 = Output compare peripheral is disabled but continues to draw current
    //--- If PWM mode on OCx; Fault pin enabled is selected, pin OCFA must be defined as input ---//
        if(OC3CONbits.OCM == 7) TRISBbits.TRISB6 = 1;
    //--- Once the PWM module initialized, related timer is also initialized. ---//
    //--- If PWM is in 32-bit initialized, timer is also in 32-bit initialized. ---//
        if(OC3CONbits.OC32)
        {
            InitTimer23();
            PR2 = 0;
        }
        else if(OC3CONbits.OCTSEL)
        {
            InitTimer3();
            PR3 = 0;
        }
        else
        {
            InitTimer2();
            PR2 = 0;
        }
    }
    else if(pwmx == _PWM4)
    {
    //------ OC4R --------------------------------------------------------------//
        OC4R = 0;               // 16-bit compare value. 32-bit compare value if OC32 = 1.
                                // Read-only in PWM mode
    //------ OC4RS -------------------------------------------------------------//
        OC4RS = 0;              // 16-bit compare value. 32-bit compare value if OC32 = 1.
    //------ OC4CON ------------------------------------------------------------//
        OC4CONbits.ON = 0;      // Output Compare Peripheral On bit(1)
                                // 1 = Output compare peripheral is enabled
                                // 0 = Output compare peripheral is disabled and not drawing current.
                                // SFR modifications are allowed.
                                // The status of other bits in this register are not affected by setting or clearing this bit

        OC4CONbits.SIDL = 0;    // Stop in Idle Mode bit
                                // 1 = Discontinue operation when CPU enters Idle mode
                                // 0 = Continue operation in Idle mode

        OC4CONbits.OC32 = 1;    // 32-bit Compare Mode bit
                                // 1 = OCxR<31:0> and/or OCxRS<31:0> is used for comparisions to the 32-bit timer source
                                // 0 = OCxR<15:0> and OCxRS<15:0> are used for comparisons to the 16-bit timer source

        OC4CONbits.OCFLT = 0;   // PWM Fault Condition Status bit
                                // (This bit is only used when OCM<2:0> = 111 and reads as ?0? in modes other than PWM mode.)
                                // 1 = PWM Fault condition has occurred (cleared in hardware only)
                                // 0 = No PWM Fault condition has occurred

        OC4CONbits.OCTSEL = 0;  // Output Compare Timer Select bit
                                // 1 = Timer3 is the clock source for this Output Compare module
                                // 0 = Timer2 is the clock source for this Output Compare module
        OC4CONbits.OCM2 = 1;    // Output Compare Mode Select bits
        OC4CONbits.OCM1 = 1;    // 111 = PWM mode on OCx; Fault pin enabled
        OC4CONbits.OCM0 = 1;    // 110 = PWM mode on OCx; Fault pin disabled
                                // 101 = Initialize OCx pin low; generate continuous output pulses on OCx pin
                                // 100 = Initialize OCx pin low; generate single output pulse on OCx pin
                                // 011 = Compare event toggles OCx pin
                                // 010 = Initialize OCx pin high; compare event forces OCx pin low
                                // 001 = Initialize OCx pin low; compare event forces OCx pin high
                                // 000 = Output compare peripheral is disabled but continues to draw current
    //--- If PWM mode on OCx; Fault pin enabled is selected, pin OCFA must be defined as input ---//
        if(OC4CONbits.OCM == 7) TRISBbits.TRISB6 = 1;
    //--- Once the PWM module initialized, related timer is also initialized. ---//
    //--- If PWM is in 32-bit initialized, timer is also in 32-bit initialized. ---//
        if(OC4CONbits.OC32)
        {
            InitTimer23();
            PR2 = 0;
        }
        else if(OC4CONbits.OCTSEL)
        {
            InitTimer3();
            PR3 = 0;
        }
        else
        {
            InitTimer2();
            PR2 = 0;
        }
    }
    else if(pwmx == _PWM5)
    {
    //------ OC5R --------------------------------------------------------------//
        OC5R = 0;               // 16-bit compare value. 32-bit compare value if OC32 = 1.
                                // Read-only in PWM mode
    //------ OC5RS -------------------------------------------------------------//
        OC5RS = 0;              // 16-bit compare value. 32-bit compare value if OC32 = 1.
    //------ OC5CON ------------------------------------------------------------//
        OC5CONbits.ON = 0;      // Output Compare Peripheral On bit(1)
                                // 1 = Output compare peripheral is enabled
                                // 0 = Output compare peripheral is disabled and not drawing current.
                                // SFR modifications are allowed.
                                // The status of other bits in this register are not affected by setting or clearing this bit

        OC5CONbits.SIDL = 0;    // Stop in Idle Mode bit
                                // 1 = Discontinue operation when CPU enters Idle mode
                                // 0 = Continue operation in Idle mode

        OC5CONbits.OC32 = 1;    // 32-bit Compare Mode bit
                                // 1 = OCxR<31:0> and/or OCxRS<31:0> is used for comparisions to the 32-bit timer source
                                // 0 = OCxR<15:0> and OCxRS<15:0> are used for comparisons to the 16-bit timer source

        OC5CONbits.OCFLT = 0;   // PWM Fault Condition Status bit
                                // (This bit is only used when OCM<2:0> = 111 and reads as ?0? in modes other than PWM mode.)
                                // 1 = PWM Fault condition has occurred (cleared in hardware only)
                                // 0 = No PWM Fault condition has occurred

        OC5CONbits.OCTSEL = 0;  // Output Compare Timer Select bit
                                // 1 = Timer3 is the clock source for this Output Compare module
                                // 0 = Timer2 is the clock source for this Output Compare module

        OC5CONbits.OCM2 = 1;    // Output Compare Mode Select bits
        OC5CONbits.OCM1 = 1;    // 111 = PWM mode on OCx; Fault pin enabled
        OC5CONbits.OCM0 = 1;    // 110 = PWM mode on OCx; Fault pin disabled
                                // 101 = Initialize OCx pin low; generate continuous output pulses on OCx pin
                                // 100 = Initialize OCx pin low; generate single output pulse on OCx pin
                                // 011 = Compare event toggles OCx pin
                                // 010 = Initialize OCx pin high; compare event forces OCx pin low
                                // 001 = Initialize OCx pin low; compare event forces OCx pin high
                                // 000 = Output compare peripheral is disabled but continues to draw current
    //--- If PWM mode on OCx; Fault pin enabled is selected, pin OCFB must be defined as input ---//
        if(OC5CONbits.OCM == 7) TRISBbits.TRISB15 = 1;
    //--- Once the PWM module initialized, related timer is also initialized. ---//
    //--- If PWM is in 32-bit initialized, timer is also in 32-bit initialized. ---//
        if(OC5CONbits.OC32)
        {
            InitTimer23();
            PR2 = 0;
        }
        else if(OC5CONbits.OCTSEL)
        {
            InitTimer3();
            PR3 = 0;
        }
        else
        {
            InitTimer2();
            PR2 = 0;
        }
    }
}

/****************************************************************************/
/*  Name of the function:       SetPWM                                      */
/*  Purpose of the function:    Enable/Disable the PWM module               */
/*  Parameters:                                                             */
/*      IN:                     pwmx (selection of the PWM output)          */
/*                              action (Enable or disable PWM)              */
/*                              See .h for define definitions.              */
/*      OUT:                    -                                           */
/*                                                                          */
/*  Used global variables:      -                                           */
/*                                                                          */
/*  Creator:                    julien_rebetez                              */
/*  Date of creation:           26 août 2015, 21:01                         */
/*                                                                          */
/*  Last modified on:           -                                           */
/*  Modified by:                -                                           */
/*  Version:                    -                                           */
/*                                                                          */
/*  Remark:                     -                                           */
/****************************************************************************/
void SetPWM(unsigned char pwmx, unsigned char action)
{
    if(pwmx == _PWM1)
    {
        if(action == _ENABLE)
        {
            OC1CONbits.ON = 1;
            if(OC1CONbits.OC32 || !OC1CONbits.OCTSEL) T2CONbits.ON = 1;
            else T3CONbits.ON = 1;
        }
        else
        {
            OC1CONbits.ON = 0;
            if(OC1CONbits.OC32 || !OC1CONbits.OCTSEL) T2CONbits.ON = 0;
            else T3CONbits.ON = 0;
        }
    }
    else if(pwmx == _PWM2)
    {
        if(action == _ENABLE)
        {
            OC2CONbits.ON = 1;
            if(OC2CONbits.OC32 || !OC2CONbits.OCTSEL) T2CONbits.ON = 1;
            else T3CONbits.ON = 1;
        }
        else
        {
            OC2CONbits.ON = 0;
            if(OC2CONbits.OC32 || !OC2CONbits.OCTSEL) T2CONbits.ON = 0;
            else T3CONbits.ON = 0;
        }
    }
    else if(pwmx == _PWM3)
    {
        if(action == _ENABLE)
        {
            OC3CONbits.ON = 1;
            if(OC3CONbits.OC32 || !OC3CONbits.OCTSEL) T2CONbits.ON = 1;
            else T3CONbits.ON = 1;
        }
        else
        {
            OC3CONbits.ON = 0;
            if(OC3CONbits.OC32 || !OC3CONbits.OCTSEL) T2CONbits.ON = 0;
            else T3CONbits.ON = 0;
        }
    }
    else if(pwmx == _PWM4)
    {
        if(action == _ENABLE)
        {
            OC4CONbits.ON = 1;
            if(OC4CONbits.OC32 || !OC4CONbits.OCTSEL) T2CONbits.ON = 1;
            else if(OC4CONbits.OCTSEL) T3CONbits.ON = 1;
        }
        else
        {
            OC4CONbits.ON = 0;
            if(OC4CONbits.OC32 || !OC4CONbits.OCTSEL) T2CONbits.ON = 0;
            else T3CONbits.ON = 0;
        }
    }
    else if(pwmx == _PWM5)
    {
        if(action == _ENABLE)
        {
            OC5CONbits.ON = 1;
            if(OC5CONbits.OC32 || !OC5CONbits.OCTSEL) T2CONbits.ON = 1;
            else T3CONbits.ON = 1;
        }
        else
        {
            OC5CONbits.ON = 0;
            if(OC5CONbits.OC32 || !OC5CONbits.OCTSEL) T2CONbits.ON = 0;
            else T3CONbits.ON = 0;
        }
    }
}

/****************************************************************************/
/*  Name of the function:       SetPWMInterrupt                             */
/*  Purpose of the function:    Enable/Disable PWM module interrupt         */
/*  Parameters:                                                             */
/*      IN:                     pwmx (selection of the PWM output)          */
/*                              action (Enable or disable PWM)              */
/*                              See .h for define definitions.              */
/*      OUT:                    -                                           */
/*                                                                          */
/*  Used global variables:      -                                           */
/*                                                                          */
/*  Creator:                    julien_rebetez                              */
/*  Date of creation:           26 août 2015, 21:26                         */
/*                                                                          */
/*  Last modified on:           -                                           */
/*  Modified by:                -                                           */
/*  Version:                    -                                           */
/*                                                                          */
/*  Remark:                     -                                           */
/****************************************************************************/
void SetPWMInterrupt(unsigned char pwmx, unsigned char action)
{
    if(pwmx == _PWM1)
    {
        if(action == _ENABLE)
        {
            if(OC1CONbits.OCTSEL || OC1CONbits.OC32)
                InitInterruptTimer(_TIMER3, _ENABLE);
            else 
                InitInterruptTimer(_TIMER2, _ENABLE);
        }
        else
        {
            if(OC1CONbits.OCTSEL || OC1CONbits.OC32) 
                InitInterruptTimer(_TIMER3, _DISABLE);
            else 
                InitInterruptTimer(_TIMER2, _DISABLE);
        }
    }
    else if(pwmx == _PWM2)
    {
        if(action == _ENABLE)
        {
            if(OC2CONbits.OCTSEL || OC2CONbits.OC32) 
                InitInterruptTimer(_TIMER3, _ENABLE);
            else 
                InitInterruptTimer(_TIMER2, _ENABLE);
        }
        else
        {
            if(OC2CONbits.OCTSEL || OC2CONbits.OC32) 
                InitInterruptTimer(_TIMER3, _DISABLE);
            else 
                InitInterruptTimer(_TIMER2, _DISABLE);
        }
    }
    else if(pwmx == _PWM3)
    {
        if(action == _ENABLE)
        {
            if(OC3CONbits.OCTSEL || OC3CONbits.OC32) 
                InitInterruptTimer(_TIMER3, _ENABLE);
            else 
                InitInterruptTimer(_TIMER2, _ENABLE);
        }
        else
        {
            if(OC3CONbits.OCTSEL || OC3CONbits.OC32) 
                InitInterruptTimer(_TIMER3, _DISABLE);
            else 
                InitInterruptTimer(_TIMER2, _DISABLE);
        }
    }
    else if(pwmx == _PWM4)
    {
        if(action == _ENABLE)
        {
            if(OC4CONbits.OCTSEL || OC4CONbits.OC32) 
                InitInterruptTimer(_TIMER3, _ENABLE);
            else 
                InitInterruptTimer(_TIMER2, _ENABLE);
        }
        else
        {
            if(OC4CONbits.OCTSEL || OC4CONbits.OC32) 
                InitInterruptTimer(_TIMER3, _DISABLE);
            else 
                InitInterruptTimer(_TIMER2, _DISABLE);
        }
    }
    else if(pwmx == _PWM5)
    {
        if(action == _ENABLE)
        {
            if(OC5CONbits.OCTSEL || OC5CONbits.OC32) 
                InitInterruptTimer(_TIMER3, _ENABLE);
            else 
                InitInterruptTimer(_TIMER2, _ENABLE);
        }
        else
        {
            if(OC5CONbits.OCTSEL || OC5CONbits.OC32) 
                InitInterruptTimer(_TIMER3, _DISABLE);
            else 
                InitInterruptTimer(_TIMER2, _DISABLE);
        }
    }
}

/****************************************************************************/
/*  Name of the function:       SetPWMFrequency                             */
/*  Purpose of the function:    Set PWM module frequency                    */
/*  Parameters:                                                             */
/*      IN:                     pwmx (selection of the PWM output)          */
/*                              frequency                                   */
/*      OUT:                    -                                           */
/*                              See .h for define definitions.              */
/*                                                                          */
/*  Used global variables:      -                                           */
/*                                                                          */
/*  Creator:                    julien_rebetez                              */
/*  Date of creation:           26 août 2015, 21:53                         */
/*                                                                          */
/*  Last modified on:           -                                           */
/*  Modified by:                -                                           */
/*  Version:                    -                                           */
/*                                                                          */
/*  Remark:                     -                                           */
/****************************************************************************/
void SetPWMFrequency(unsigned char pwmx, float frequency)
{
    float temp, temp2;

    temp = (1/frequency)/((1/80000000.0) * 4294967296);
    if(pwmx == _PWM1)
    {
    //--- 32-bit mode ---/
        if(OC1CONbits.OC32)
            PR2 = temp*0xffffffff;
    //--- 16-bit mode ---//
        else
            PR3 = temp*0xffff;
    }
    else if(pwmx == _PWM2)
    {
    //--- 32-bit mode ---/
        if(OC2CONbits.OC32)
            PR2 = temp*0xffffffff;
    //--- 16-bit mode ---//
        else
            PR3 = temp*0xffff;
    }
    else if(pwmx == _PWM3)
    {
    //--- 32-bit mode ---/
        if(OC3CONbits.OC32)
            PR2 = temp*0xffffffff;
    //--- 16-bit mode ---//
        else
            PR3 = temp*0xffff;
    }
    else if(pwmx == _PWM4)
    {
    //--- 32-bit mode ---/
        if(OC4CONbits.OC32)
            PR2 = temp*0xffffffff;
    //--- 16-bit mode ---//
        else
            PR3 = temp*0xffff;
    }
    else if(pwmx == _PWM5)
    {
    //--- 32-bit mode ---/
        if(OC5CONbits.OC32)
            PR2 = temp*0xffffffff;
    //--- 16-bit mode ---//
        else
            PR3 = temp*0xffff;
    }
}

/****************************************************************************/
/*  Name of the function:       SetPWMDutyCycle                             */
/*  Purpose of the function:    Set PWM module duty cycle                   */
/*  Parameters:                                                             */
/*      IN:                     pwmx (selection of the PWM output)          */
/*                              dutyCycle                                   */
/*      OUT:                    -                                           */
/*                              See .h for define definitions.              */
/*                                                                          */
/*  Used global variables:      -                                           */
/*                                                                          */
/*  Creator:                    julien_rebetez                              */
/*  Date of creation:           04 september 2015                           */
/*                                                                          */
/*  Last modified on:           -                                           */
/*  Modified by:                -                                           */
/*  Version:                    -                                           */
/*                                                                          */
/*  Remark:                     -                                           */
/****************************************************************************/
void SetPWMDutyCycle(unsigned char pwmx, unsigned int dutyCycle)
{
    if(pwmx == _PWM1)
    {
        if(OC1CONbits.OC32) 
            OC1RS = (PR2 / 100) * dutyCycle;
        else 
            OC1RS = (PR3 / 100) * dutyCycle;
    }
    else if(pwmx == _PWM2)
    {
        if(OC2CONbits.OC32) 
            OC2RS = (PR2 / 100) * dutyCycle;
        else 
            OC2RS = (PR3 / 100) * dutyCycle;
    }
    else if(pwmx == _PWM3)
    {
        if(OC3CONbits.OC32) 
            OC3RS = (PR2 / 100) * dutyCycle;
        else 
            OC3RS = (PR3 / 100) * dutyCycle;
    }
    else if(pwmx == _PWM4)
    {
        if(OC4CONbits.OC32) 
            OC4RS = (PR2 / 100) * dutyCycle;
        else 
            OC4RS = (PR3 / 100) * dutyCycle;
    }
    else if(pwmx == _PWM5)
    {
        if(OC5CONbits.OC32) 
            OC5RS = (PR2 / 100) * dutyCycle;
        else 
            OC5RS = (PR3 / 100) * dutyCycle;
    }
}