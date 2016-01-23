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

typedef struct
{
    unsigned short int firstMeasure;
    unsigned short int secondMeasure;
    unsigned short int thirdMeasure;
    unsigned short int timeHigh;
    unsigned short int timeLow;
    unsigned short int timeTotal;
    float frequency;
} S_MEAS_PWM;


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

#endif	/* INPUTCAPTURE_H */
