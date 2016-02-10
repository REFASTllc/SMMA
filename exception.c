/********************************************************************************************************************/
/*  Name of the file:       exception.c								    
/*  Purpose of the file:    Definitions of all functions related to ...						    
/*  Creator:                julien_rebetez									    
/*  Date of creation:       February 10, 2016									    
/*														    
/*  Last modification on:   -											    
/*  Modified by:            -											    
/*  Version:                -											    
/*														    
/*  List of functions:      -											    
/* ******************************************************************************************************************/

#include "includes.h" // File which contain all includes files

/******************************************************************************
 * Function:        void _general_exception_handler(void)
 *
 * PreCondition:    None
 *
 * Input:           None
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        This function overrides the normal _weak_ _generic_exception_handler
 *                  which is defined in the XC32 User's Guide.
 *
 * Note:
 *
 *****************************************************************************/

/** VARIABLES *****************************************************************/

int *error_1;       // pointer variable used to trigger illegal address exception
int error_2;        // variable used to trigger trap (divide-by-zero) exception

/* Exception Handler Variables */
/* Static in case exception condition would stop auto variable from being created */
static unsigned int _excep_code;
static unsigned int _excep_addr;

void __attribute__((nomips16)) _general_exception_handler(void)
{
    // Mask off the ExcCode Field from the Cause Register
    // Refer to FRM Section-2 CPU - DS61113
    _excep_code=(_CP0_GET_CAUSE() & 0x0000007C) >> 2;
    _excep_addr=_CP0_GET_EPC();

    // Disable Interrupts
    _CP0_SET_STATUS(_CP0_GET_STATUS()&0xFFFFFFE); 

    
    oTestLed1 = 0;
    oTestLed2 = 0;
    
    // Examine _excep_code to identify the type of exception
    // Examine _excep_addr to find the address that caused the exception
    // Perform an indication of the exception
    switch(_excep_code)
    {
        case EXCCODE_ADDRESS_ERROR_STORE:
            oTestLed1 = 1;
            break;
            
        case EXCCODE_TRAP_EXCEPTION:
            oTestLed2 = 1;
            break;
            
        default:
            oTestLed1 = 1;
            oTestLed2 = 1;                    
            break;
    }
    
    while(1);       // Set a breakpoint here
                    // Examine _excep_code to identify the type of exception
                    // Examine _excep_addr to find the address that caused the exception
                    // Example LEDs for indication

                    // In a real application, you would perform a useful indication
                    // of the exception, as well as a recovery mechanism
}