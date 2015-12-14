/********************************************************************************************************************/
/*  Name of the file:       A3981.c										    
/*  Purpose of the file:    Definitions of all functions related to the motor driver A3981			    
/*  Creator:                julien_rebetez									    
/*  Date of creation:       12 septembre 2015									    
/*														    
/*  Last modification on:   -											    
/*  Modified by:            -											    
/*  Version:                -											    
/*														    
/*  List of functions:      -											    
/* ******************************************************************************************************************/

#include "includes.h" // File which contain all includes files

T_A3981 A3981;

/********************************************************************************************************************/
/*  Name of the function:       InitA3981									    
/*  Purpose of the function:    Header of the function InitA3981						    
/*  Parameters:													    
/*      IN:                     -										    
/*      OUT:                    -										    
/*														    
/*  Used global variables:      -										    
/*														    
/*  Creator:                    julien_rebetez									    
/*  Date of creation:           12 septembre 2015								    
/*														    
/*  Last modified on:           -										    
/*  Modified by:                -										    
/*  Version:                    -										    
/*														    
/*  Remark:                     -										    
/********************************************************************************************************************/
void InitA3981(void)
{   
    oBiResetSignal = 1;
//--- Configuration register 0 ---//
    A3981.CONFIG0 = 0b0010011100011100;
                //	 |||||||||||||||+---	PWM:  PWM configuration
                //	 ||||||||||||||+----	TOF0/FRQ0: Off time (only valid when PWM bit = 0)
                //	 |||||||||||||+-----	TOF1/FRQ1: Frequency (only valid when PWM bit = 1)
                //	 ||||||||||||+------	TOF2/FRQ2:
                //	 |||||||||||+-------	TBK0: Blank Time
                //	 ||||||||||+--------	TBK1:
                //	 |||||||||+---------	PFD0: Fast decay time for mixed decay
                //	 ||||||||+----------	PFD1:
                //	 |||||||+-----------	PFD2:
                //	 ||||||+------------	MXI0: Max phase current as a percentage of ISMAX
                //	 |||||+-------------	MXI1:
                //	 ||||+--------------	MS0:  Microstep mode for external STEP input control
                //	 |||+---------------	MS1:
                //	 ||+----------------	SYR:  Synchronous rectification
                //	 |+----------------- Register address
                //	 +------------------ Register address
//--- Configuration register 1 ---//
    A3981.CONFIG1 = 0b0111000000100000;
                //	 |||||||||||||||+---	DIAG0: Selects signal routed to DIAG output
                //	 ||||||||||||||+----	DIAG1:
                //	 |||||||||||||+-----	CD0:   PWM count difference for ST detection
                //	 ||||||||||||+------	CD1:   Default value: 8
                //	 |||||||||||+-------	CD2:
                //	 ||||||||||+--------	CD3:
                //	 |||||||||+---------	CD4:
                //	 ||||||||+----------	CD5:
                //	 |||||||+-----------	CD6:
                //	 ||||||+------------	CD7:
                //	 |||||+-------------	Empty (0)
                //	 ||||+--------------	TSC0:  Overcurrent fault delay
                //	 |||+---------------	TSC1:
                //	 ||+----------------	OSC:   Selects clock source
                //	 |+----------------- Register address
                //	 +------------------ Register address
//--- Configuration register run ---//
    A3981.RUN = 0b1011101001000001;
            //	  |||||||||||||||+---	SC0:  Step change number
            //	  ||||||||||||||+----	SC1:  2?s complement format
            //	  |||||||||||||+-----	SC2:  Positive value increases Step Angle Number
            //	  ||||||||||||+------	SC3:  Negative value decreases Step Angle Number
            //	  |||||||||||+-------	SC4:
            //	  ||||||||||+--------	SC5:
            //	  |||||||||+---------	DCY0: Decay mode selection
            //	  ||||||||+----------	DCY1:
            //	  |||||||+-----------	BRK:  Brake enable
            //	  ||||||+------------	SLEW: Slew rate control
            //	  |||||+-------------	HLR:  Selects slow decay and brake recirculation path
            //	  ||||+--------------	OL0:  Open load current threshold as a percentage of -
            //	  |||+---------------	OL1:  maximum current defined by ISMAX and MXI[1..0]
            //	  ||+----------------	EN:   Phase current enable OR with ENABLE pin
            //	  |+----------------- Register address
            //	  +------------------ Register address
//--- Configuration Phase Table Load Register ---//
    A3981.TBLLD = 0b1100000001000101;
                //	|||||||||||||||+---	PT0: Phase Table Value
                //	||||||||||||||+----	PT1:
                //	|||||||||||||+-----	PT2:
                //	||||||||||||+------	PT3:
                //	|||||||||||+-------	PT4:
                //	||||||||||+--------	PTP: Parity bit (odd parity)
                //	|||||||||+---------	Empty (0)
                //	||||||||+----------	Empty (0)
                //	|||||||+-----------	Empty (0)
                //	||||||+------------	Empty (0)
                //	|||||+-------------	Empty (0)
                //	||||+--------------	Empty (0)
                //	|||+---------------	STS0: Selects stall detection scheme
                //	||+----------------	STS1:
                //	|+----------------- Register address
                //	+------------------ Register address
//--- Send the configuration to the chip ---//
    SendOneDataSPI1(A3981.CONFIG0);
    A3981.FAULT0.REG = GetLastDataSPI1();
    SendOneDataSPI1(A3981.CONFIG1);
    A3981.FAULT1.REG = GetLastDataSPI1();
    SendOneDataSPI1(A3981.RUN);
    SendOneDataSPI1(A3981.TBLLD);
}


/********************************************************************************************************************/
/*  Name of the function:       SetA3981									    
/*  Purpose of the function:    Set a register of the A3981 chip						    
/*  Parameters:													    
/*      IN:                     value: value to be writted in the register										    
/*      OUT:                    status  -1: error during trying to write the configuration
/*                                       1: no error  
/*                              See .h file to find the related defined values
/*														    
/*  Used global variables:      -										    
/*														    
/*  Creator:                    julien_rebetez									    
/*  Date of creation:           12.12.2015								    
/*														    
/*  Last modified on:           -										    
/*  Modified by:                -										    
/*  Version:                    -										    
/*														    
/*  Remark:                     -										    
/********************************************************************************************************************/
signed char SetA3981(unsigned short int value)
{
    unsigned short int faultReg;
    
    SendOneDataSPI2(value);         // then send the data to the chip
    faultReg = GetLastDataSPI1();   // Get the corresponding fault register
}