/**********************************************************************************************************************

                                            UBD - Unipolar Bipolar Driver

***********************************************************************************************************************
 * File name:               command.h
 * Creation date:           03.10.2015
 * Main creator:            A. Staub
 * Company:                 REFAST GmbH
 *                          Copyright (c) 2015 REFAST GmbH
***********************************************************************************************************************
 * Content overview :       - COMMAND
***********************************************************************************************************************/

/**********************************************************************************************************************
 * Routine:                 COMMAND_H

 * Description:
 * ...
 * 
 * Creator:                 A. Staub
 * Date of creation:        02.10.2015
 * Last modification on:    -
 * Modified by:             - 
 * 
 * Input:                   -
 * Output:                  -
***********************************************************************************************************************/

#ifndef COMMAND_H
#define	COMMAND_H

extern void cmd_SILIM(void);
extern void cmd_ETESTIN(void);
extern void cmd_SRACC(void);
extern void cmd_SRDEC(void);
extern void cmd_RUN(void);
extern void cmd_SPHC(void);
extern void cmd_GVER(void);
extern void cmd_RAZ(void);
extern void cmd_SMTYP(void);
extern void cmd_GMTYP(void);
extern void cmd_STEST(void);
extern void cmd_GILIM(void);
extern void cmd_GPHC(void);
extern void cmd_GRACC(void);
extern void cmd_GRDEC(void);
extern void cmd_GRUN(void);
extern void cmd_BREAK(void);
extern void cmd_MUMOT(void);
extern void cmd_SCOILON(void);
extern void cmd_SSMOD(void);
extern void cmd_GSMOD(void);
extern void cmd_SBIT(void);
extern void cmd_CBIT(void);
extern void cmd_GBIT(void);
extern void cmd_SOUT(void);
extern void cmd_GOUT(void);
extern void cmd_GINP(void);
extern void cmd_SMCRSTP(void);
extern void cmd_GMCRSTP(void);
extern void cmd_SSWLIM(void);
extern void cmd_GSWLIM(void);
extern void cmd_GSWPOS(void);
extern void cmd_SPWMLIM(void);
extern void cmd_GPWMLIM(void);
extern void cmd_GPWMPOS(void);
extern void cmd_GPWMVAL(void);
extern void cmd_SSWTYP(void);
extern void cmd_GSWTYP(void);
extern void cmd_GFRQVAL(void);
//new commands:
extern void cmd_GINPB(void);
extern void cmd_SROM(void);
extern void cmd_RROM(void);
extern void cmd_GTEMP(void);
extern void cmd_SSEC(void);
extern void cmd_GSEC(void);
extern void cmd_SMIN(void);
extern void cmd_GMIN(void);
extern void cmd_SHRS(void);
extern void cmd_GHRS(void);
extern void cmd_SDAY(void);
extern void cmd_GDAY(void);
extern void cmd_SWDAY(void);
extern void cmd_GWDAY(void);
extern void cmd_SMONTH(void);
extern void cmd_GMONTH(void);
extern void cmd_SYEAR(void);
extern void cmd_GYEAR(void);
extern void cmd_GTIME(void);
extern void cmd_GSTALIN(void);
extern void cmd_SSPDLIN(void);
extern void cmd_GSPDLIN(void);
extern void cmd_STOLIN(void);
extern void cmd_GTOLIN(void);
extern void cmd_SLIN(void);
extern void cmd_GBIPSTAS(void);
extern void cmd_GBIPSTAL(void);
extern void cmd_SBIPPD(void);
extern void cmd_GBIPPD(void);
extern void cmd_SBIPTSC(void);
extern void cmd_GBIPTSC(void);
extern void cmd_SBIPOL(void);
extern void cmd_GBIPOL(void);
extern void cmd_SBIPSTS(void);
extern void cmd_GBIPSTS(void);
extern void cmd_SBIPPFD(void);
extern void cmd_GBIPPFD(void);
extern void cmd_SBIPPWM(void);
extern void cmd_GBIPPWM(void);
extern void cmd_SBIPFRQ(void);
extern void cmd_GBIPFRQ(void);
extern void cmd_SBIPSLEW(void);
extern void cmd_GBIPSLEW(void);
extern void cmd_GBIPSST(void);
extern void cmd_GVBAK(void);
extern void cmd_SRESLIN(void);
extern void cmd_GRESLIN(void);
extern void cmd_SFRQBIT(void);
extern void cmd_GFRQBIT(void);
extern void cmd_SRUNBIT(void);
extern void cmd_GRUNBIT(void);
extern void cmd_RTESTIN(void);
extern void cmd_SPROD(void);
extern void cmd_STAT(void);
extern void cmd_ERUN(void);

typedef union
{
    long REG;
    struct
    {
        long eFlagEEPROM        :1;     // bit 0
        long eFlagTemp          :1;     // bit 1
        long eFlagButtCell      :1;     // bit 2
        long eFlagVref          :1;     // bit 3
        long eFlagCommBipolar   :1;     // bit 4
        long eFlagCoilMeasBip   :1;     // bit 5
        long eFlagRunUnipolar   :1;     // bit 6
        long eFlagCoilMeasUni   :1;     // bit 7
        long eFlagRunLin        :1;     // bit 8
        long eFlagInput0        :1;     // bit 9
        long eFlagInput1        :1;     // bit 10
        long eFlagInput2        :1;     // bit 11
        long eFlagInput3PWM     :1;     // bit 12
        long eFlagInput4        :1;     // bit 13
        long eFlagInput5        :1;     // bit 14
        long eFlagInput6Freq    :1;     // bit 15
        long eFlagInput7        :1;     // bit 16
        long eFlagOutput0       :1;     // bit 17
        long eFlagOutput1       :1;     // bit 18
        long eFlagOutput2       :1;     // bit 19
        long eFlagOutput3       :1;     // bit 20
        long eFlagOutput4       :1;     // bit 21
        long eFlagOutput5       :1;     // bit 22
        long eFlagOutput6       :1;     // bit 23
        long eFlagOutput7       :1;     // bit 24
        long notUsed            :7;
    } BITS;
} U_STEST;

#endif	/* COMMAND_H */
