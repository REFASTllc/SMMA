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

typedef struct
{
    unsigned int CONFIG0;
    unsigned int CONFIG1;
    unsigned int RUN;
    unsigned int TBLLD;
    unsigned int FAULT0;
    unsigned int FAULT1;
} T_A3981;

typedef struct
{
    unsigned int FaultFlag : 1;
    unsigned int Temp1Diag : 1;
    unsigned int Temp2Diag : 1;
    unsigned int OverVoltVBB : 1;
    unsigned int StallDetect : 1;
    unsigned int OpenLoadPhB : 1;
    unsigned int OpenLoadPhA : 1;
    unsigned int OverCurrBM_L : 1;
    unsigned int OverCurrBM_H : 1;
    unsigned int OverCurrBP_L : 1;
    unsigned int OverCurrBP_H : 1;
    unsigned int OverCurrAM_L : 1;
    unsigned int OverCurrAM_H : 1;
    unsigned int OverCurrAP_L : 1;
    unsigned int OverCurrAP_H : 1;
} T_FAULT0;

typedef struct
{
    unsigned int FaultFlag : 1;
    unsigned int Temp1Diag : 1;
    unsigned int Temp2Diag : 1;
    unsigned int OverVoltVBB : 1;
    unsigned int StallDetect : 1;
    unsigned int OpenLoadPhB : 1;
    unsigned int OpenLoadPhA : 1;
    unsigned int NotUsed : 2;
    unsigned int StepAngleNumber : 6;
} T_FAULT1;

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

#endif	/* A3981_H */
