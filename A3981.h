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

#define _FULL_STEP      0
#define _HALF_STEP      1
#define _QUART_STEP     2
#define _SIXTH_STEP     3

typedef union
{
    unsigned short int REG;
    struct
    {
        unsigned short int APH : 1;
        unsigned short int APL : 1;
        unsigned short int AMH : 1;
        unsigned short int AML : 1;
        unsigned short int BPH : 1;
        unsigned short int BPL : 1;
        unsigned short int BMH : 1;
        unsigned short int BML : 1;
        unsigned short int OLA : 1;
        unsigned short int OLB : 1;
        unsigned short int ST : 1;
        unsigned short int UV : 1;
        unsigned short int OV : 1;
        unsigned short int TW : 2;
        unsigned short int FF : 1;
    } BITS;
} U_FAULT0;

typedef union
{
    unsigned short int REG;
    struct
    {
        unsigned short int SA : 6;
        unsigned short int nu : 2;
        unsigned short int OLA : 1;
        unsigned short int OLB : 1;
        unsigned short int ST : 1;
        unsigned short int UV : 1;
        unsigned short int OV : 1;
        unsigned short int TW : 2;
        unsigned short int FF : 1;
    } BITS;
} U_FAULT1;

typedef union
{
    unsigned short int REG;
    struct
    {
        unsigned short int PWM:1;
        unsigned short int TOF_FRQ:3;
        unsigned short int TBK:2;
        unsigned short int PFD:3;
        unsigned short int MX:2;
        unsigned short int MS:2;
        unsigned short int SYR:1;
        unsigned short int ADR:2;
    } BITS;
} T_CONFIG0;

typedef union
{
    unsigned short int REG;
    struct
    {
        unsigned short int DIAG:2;
        unsigned short int CD:8;
        unsigned short int nu:1;
        unsigned short int TSC:2;
        unsigned short int OSC:1;
        unsigned short int ADR:2;
    } BITS;
} T_CONFIG1;

typedef union
{
    unsigned short int REG;
    struct
    {
        signed short int SC:6;
        unsigned short int DCY:2;
        unsigned short int BRK:1;
        unsigned short int SLEW:1;
        unsigned short int HLR:1;
        unsigned short int OL:2;
        unsigned short int EN:1;
        unsigned short int ADR:2;
    } BITS;
} T_RUN;

typedef union
{
    unsigned short int REG;
    struct
    {
        unsigned short int PT:6;
        unsigned short int PTP:1;
        unsigned short int nu:5;
        unsigned short int STS:2;
        unsigned short int ADR:2;
    } BITS;
} T_TBLLD;

typedef struct
{
    T_CONFIG0 CONFIG0;
    T_CONFIG1 CONFIG1;
    T_RUN RUN;
    T_TBLLD TBLLD;
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
