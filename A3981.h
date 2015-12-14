/********************************************************************************************************************/
/*  Name of the file:       A3981.h                                                                                 */
/*  Purpose of the file:    Headers of all functions related to the motor driver A3981                              */
/*  Creator:                julien_rebetez                                                                          */
/*  Date of creation:       12 septembre 2015                                                                       */
/*                                                                                                                  */
/*  Last modification on:   -                                                                                       */
/*  Modified by:            -                                                                                       */
/*  Version:                -                                                                                       */
/*                                                                                                                  */
/*  List of functions:      -                                                                                       */
/* ******************************************************************************************************************/

#ifndef A3981_H
#define	A3981_H

#define _CONFIG0    0
#define _CONFIG1    1
#define _RUN        2
#define _TBLLD      3

typedef union
{
    unsigned short int REG;
    struct
    {
        unsigned short int FaultFlag : 1;
        unsigned short int Temp1Diag : 1;
        unsigned short int Temp2Diag : 1;
        unsigned short int OverVoltVBB : 1;
        unsigned short int StallDetect : 1;
        unsigned short int OpenLoadPhB : 1;
        unsigned short int OpenLoadPhA : 1;
        unsigned short int OverCurrBM_L : 1;
        unsigned short int OverCurrBM_H : 1;
        unsigned short int OverCurrBP_L : 1;
        unsigned short int OverCurrBP_H : 1;
        unsigned short int OverCurrAM_L : 1;
        unsigned short int OverCurrAM_H : 1;
        unsigned short int OverCurrAP_L : 1;
        unsigned short int OverCurrAP_H : 1;
    } BITS;
} U_FAULT0;

typedef union
{
    unsigned short int REG;
    struct
    {
        unsigned short int FaultFlag : 1;
        unsigned short int Temp1Diag : 1;
        unsigned short int Temp2Diag : 1;
        unsigned short int OverVoltVBB : 1;
        unsigned short int StallDetect : 1;
        unsigned short int OpenLoadPhB : 1;
        unsigned short int OpenLoadPhA : 1;
        unsigned short int NotUsed : 2;
        unsigned short int StepAngleNumber : 6;
    } BITS;
} U_FAULT1;

typedef struct
{
    unsigned short int CONFIG0;
    unsigned short int CONFIG1;
    unsigned short int RUN;
    unsigned short int TBLLD;
    U_FAULT0 FAULT0;
    U_FAULT1 FAULT1;
} T_A3981;

/********************************************************************************************************************/
/*  Purpose of the function:    Header of the function InitA3981                                                    */
/*  Parameters:                                                                                                     */
/*      IN:                     -                                                                                   */
/*      OUT:                    -                                                                                   */
/*                                                                                                                  */
/*  Used global variables:      -                                                                                   */
/*                                                                                                                  */
/*  Creator:                    julien_rebetez                                                                      */
/*  Date of creation:           12 septembre 2015                                                                   */
/*                                                                                                                  */
/*  Last modified on:           -                                                                                   */
/*  Modified by:                -                                                                                   */
/*  Version:                    -                                                                                   */
/*                                                                                                                  */
/*  Remark:                     -                                                                                   */
/********************************************************************************************************************/
void InitA3981(void);

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
signed char SetA3981(unsigned short int value);

#endif	/* A3981_H */
