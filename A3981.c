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
//--- Configuration register 0 ---//
    A3981.CONFIG0.BITS.ADR = 0;         // Register address
    A3981.CONFIG0.BITS.SYR = 0;         // Synchronous rectification
    A3981.CONFIG0.BITS.MS = _HALF_STEP; // Microstep mode for external STEP input control
    A3981.CONFIG0.BITS.MX = 3;          // Max phase current as a percentage of ISMAX
    A3981.CONFIG0.BITS.PFD = 4;         // Fast decay time for mixed decay
    A3981.CONFIG0.BITS.TBK = 1;         // Blank Time
    A3981.CONFIG0.BITS.TOF_FRQ = 6;     // Off time (when PWM bit = 0) / Frequency (when PWM bit = 1)
    A3981.CONFIG0.BITS.PWM = 0;         // PWM configuration

//--- Configuration register 1 ---//
    A3981.CONFIG1.BITS.ADR = 1;         // Register address
    A3981.CONFIG1.BITS.OSC = 1;         // Selects clock source
    A3981.CONFIG1.BITS.TSC = 2;         // Overcurrent fault delay
    A3981.CONFIG1.BITS.CD = 8;          // PWM count difference for ST detection
    A3981.CONFIG1.BITS.DIAG = 1;        // Selects signal routed to DIAG output
    
//--- Configuration register run ---//
    A3981.RUN.BITS.ADR = 2;             // Register address
    A3981.RUN.BITS.EN = 0;              // Phase current enable
    A3981.RUN.BITS.OL = 1;              // Open load current threshold as a percentage of maximum current defined by ISMAX and MX
    A3981.RUN.BITS.HLR = 0;             // Selects slow decay and brake recirculation path
    A3981.RUN.BITS.SLEW = 1;            // Slew rate control
    A3981.RUN.BITS.BRK = 0;             // Brake enable
    A3981.RUN.BITS.DCY = 1;             // Decay mode selection
    A3981.RUN.BITS.SC = 0;              // Step change number
            
//--- Configuration Phase Table Load Register ---//
    A3981.TBLLD.BITS.ADR = 3;           // Register address
    A3981.TBLLD.BITS.STS = 3;           // Selects stall detection scheme
    A3981.TBLLD.BITS.PTP = 1;           // Parity bit (odd parity)
    A3981.TBLLD.BITS.PT = 5;            // Phase Table Value
}


/********************************************************************************************************************/
/*  Name of the function:       SetA3981									    
/*  Purpose of the function:    Set a register of the A3981 chip						    
/*  Parameters:													    
/*      IN:                     value: value to be written in the register										    
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

/********************************************************************************************************************/
/*  Name of the function:       checckA3981									    
/*  Purpose of the function:    Check the status of the A3981					    
/*  Parameters:													    
/*      IN:                     -										    
/*      OUT:                    1: function ok
/*                              -1: error during the function 
/*														    
/*  Remark:                     This function will update both FAULT register of the driver.		
/*                              This function should be preceded by function initA3981. 							    
/*														    
/*  Creator:                    julien_rebetez									    
/*  Date of creation:           02.01.2016								    
/*														    
/*  Last modified on:           -										    
/*  Modified by:                -										    
/*  Version:                    -										    
/*														    
/*  Remark:                     -										    
/********************************************************************************************************************/
signed char checkA3981(void)
{
    SendOneDataSPI1(A3981.CONFIG0.REG);
    A3981.FAULT0.REG = GetLastDataSPI1();
    SendOneDataSPI1(A3981.CONFIG1.REG);
    A3981.FAULT1.REG = GetLastDataSPI1();
    if(A3981.FAULT0.REG != 0 || ((A3981.FAULT1.REG & 0xf0) !=0))
        return -1;
    else
        return 1;
}