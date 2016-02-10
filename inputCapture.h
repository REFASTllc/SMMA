/********************************************************************************************************************/
/*  Name of the file:       inputCapture.h                                                              
/*  Purpose of the file:    Headers of all functions related to the input capture module                                         
/*  Creator:                julien_rebetez                                                                      
/*  Date of creation:       January 18, 2016                                                                       
/*                                                                                                                  
/*  Last modification on:   -                                                                                       
/*  Modified by:            -                                                                                       
/*  Version:                -                                                                                       
/*                                                                                                                  
/*  List of functions:      -                                                                                       
/* ******************************************************************************************************************/

#ifndef INPUTCAPTURE_H
#define	INPUTCAPTURE_H


#define _IC_1       1
#define _IC_2       2   
#define _IC_3       3
#define _IC_4       4
#define _IC_5       5

#define _ENABLE     1
#define _DISABLE    0

#define _MEAS_PWM   3
#define _MEAS_FREQ  2

typedef struct
{
    unsigned long timeHigh;
    unsigned long periodeTime;
    unsigned long frequency;
    unsigned short dutyCycle;
    unsigned char timeoutMeas;
} S_IC;


/********************************************************************************************************************/
/*  Purpose of the function:    Header of the function InitInputCapturexModule                                                   
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
void InitInputCapture1Module(void);
void InitInputCapture2Module(void);
void InitInputCapture3Module(void);
void InitInputCapture4Module(void);
void InitInputCapture5Module(void);

/********************************************************************************************************************/
/*  Name of the function:       InitInterruptInputCaptureModule									    
/*  Purpose of the function:    Header of the function InitInputCapturexModule
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
void InitInterruptInputCaptureModule(unsigned char module, unsigned char status);

/********************************************************************************************************************/
/*  Name of the function:       ResetInputCaptureModule									    
/*  Purpose of the function:    Reset of the module in case of buffer overflow
/*  Parameters:													    
/*      IN:                     - module: select the module to initialize	    
/*      OUT:                    -			
/* 							    See the .h file for the list of all defines.
/*														    
/*  Used global variables:      -										    
/*														    
/*  Creator:                    julien_rebetez								    
/*  Date of creation:           January 23, 2016								    
/*														    
/*  Last modified on:           -										    
/*  Modified by:                -										    
/*  Version:                    -										    
/*														    
/*  Remark:                     -										    
/********************************************************************************************************************/
void ResetInputCaptureModule(unsigned char module);

/********************************************************************************************************************/
/*  Name of the function:       FormatBufToRealValues									    
/*  Purpose of the function:    Formatting the values returned by the interrupt in a timing values
/*  Parameters:													    
/*      IN:                     - 	    
/*      OUT:                    -			
/* 							    See the .h file for the list of all defines.
/*														    
/*  Used global variables:      -										    
/*														    
/*  Creator:                    julien_rebetez								    
/*  Date of creation:           06.02.2016								    
/*														    
/*  Last modified on:           -										    
/*  Modified by:                -										    
/*  Version:                    -										    
/*														    
/*  Remark:                     -										    
/********************************************************************************************************************/
void FormatBufToRealValues(S_IC *data, unsigned char typeMeasure);

#endif	/* INPUTCAPTURE_H */
