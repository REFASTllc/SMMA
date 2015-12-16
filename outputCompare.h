/****************************************************************************/
/*  Name of the file:       outputCompare.h                                 */
/*  Purpose of the file:    Headers of all functions related to output      */
/*                          compare and PWM.                                */
/*  Creator:                julien_rebetez                                  */
/*  Date of creation:       24 août 2015, 21:55                             */
/*                                                                          */
/*  Last modification on:   -                                               */
/*  Modified by:            -                                               */
/*  Version:                -                                               */
/*                                                                          */
/*  List of functions:      InitPWM                                         */
/*                          SetPWM                                          */
/*                          SetPWMInterrupt                                 */
/*                          SetPWMFrequency                                 */
/* **************************************************************************/

#ifndef OUTPUTCOMPARE_H
#define	OUTPUTCOMPARE_H


#define _PWM1       1
#define _PWM2       2
#define _PWM3       3
#define _PWM4       4
#define _PWM5       5

#define _ENABLE     1
#define _DISABLE    0

/****************************************************************************/
/*  Purpose of the function:    Header of the function InitPWM              */
/*  Parameters:                                                             */
/*      IN:                     pwmx (selection of the PWM output)          */
/*                              See .h for define definitions.              */
/*      OUT:                    -                                           */
/*                                                                          */
/*  Used global variables:      -                                           */
/*                                                                          */
/*  Creator:                    julien_rebetez                              */
/*  Date of creation:           24 août 2015, 21:55                         */
/*                                                                          */
/*  Last modified on:           -                                           */
/*  Modified by:                -                                           */
/*  Version:                    -                                           */
/*                                                                          */
/*  Remark:                     -                                           */
/****************************************************************************/
void InitPWM(unsigned char pwmx);

/****************************************************************************/
/*  Purpose of the function:    Header of the function SetPWM               */
/*  Parameters:                                                             */
/*      IN:                     pwmx (selection of the PWM output)          */
/*                              action (enable or disable the module)       */
/*                              See .h for define definitions.              */
/*      OUT:                    -                                           */
/*                                                                          */
/*  Used global variables:      -                                           */
/*                                                                          */
/*  Creator:                    julien_rebetez                              */
/*  Date of creation:           26 août 2015, 21:17                         */
/*                                                                          */
/*  Last modified on:           -                                           */
/*  Modified by:                -                                           */
/*  Version:                    -                                           */
/*                                                                          */
/*  Remark:                     -                                           */
/****************************************************************************/
void SetPWM(unsigned char pwmx, unsigned char action);

/****************************************************************************/
/*  Purpose of the function:    Header of the function SetPWMInterrupt      */
/*  Parameters:                                                             */
/*      IN:                     pwmx (selection of the PWM output)          */
/*                              action (enable or disable the module)       */
/*                              See .h for define definitions.              */
/*      OUT:                    -                                           */
/*                                                                          */
/*  Used global variables:      -                                           */
/*                                                                          */
/*  Creator:                    julien_rebetez                              */
/*  Date of creation:           26 août 2015, 21:17                         */
/*                                                                          */
/*  Last modified on:           -                                           */
/*  Modified by:                -                                           */
/*  Version:                    -                                           */
/*                                                                          */
/*  Remark:                     -                                           */
/****************************************************************************/
void SetPWMInterrupt(unsigned char pwmx, unsigned char action);

/****************************************************************************/
/*  Purpose of the function:    Header of the function SetPWMFrequency      */
/*  Parameters:                                                             */
/*      IN:                     pwmx (selection of the PWM output)          */
/*                              frequency                                   */
/*      OUT:                    -                                           */
/*                              See .h for define definitions.              */
/*                                                                          */
/*  Used global variables:      -                                           */
/*                                                                          */
/*  Creator:                    julien_rebetez                              */
/*  Date of creation:           26 août 2015, 22:11                         */
/*                                                                          */
/*  Last modified on:           -                                           */
/*  Modified by:                -                                           */
/*  Version:                    -                                           */
/*                                                                          */
/*  Remark:                     -                                           */
/****************************************************************************/
void SetPWMFrequency(unsigned char pwmx, float frequency);

/****************************************************************************/
/*  Purpose of the function:    Header of the function SetPWMDutyCycle      */
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
void SetPWMDutyCycle(unsigned char pwmx, unsigned int dutyCycle);

#endif	/* OUTPUTCOMPARE_H */