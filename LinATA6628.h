/**********************************************************************************************************************

                                            UBD - Unipolar Bipolar Driver

***********************************************************************************************************************
 * File name:               LinATA6628.h
 * Creation date:           19.12.2015
 * Main creator:            A. Staub
 * Company:                 REFAST GmbH
 *                          Copyright (c) 2015 REFAST GmbH
***********************************************************************************************************************
 * Content overview :       - LINATA6628
***********************************************************************************************************************/

/**********************************************************************************************************************
 * Routine:                 LINATA6628_H

 * Description:
 * ...
 * 
 * Creator:                 A. Staub
 * Date of creation:        19.12.2015
 * Last modification on:    -
 * Modified by:             - 
 * 
 * Input:                   -
 * Output:                  -
***********************************************************************************************************************/

#ifndef LINATA6628_H
#define	LINATA6628_H

extern void LINATA6628_init(void);
extern void LINATA6629_SendBackSlaveAnswer(void);
extern void LINJE_Protocol(void);
extern unsigned char LINJE_ConvertASCII(unsigned char uint8_Character);

typedef struct                  //definitions of multiple variable for LIN
{
    volatile unsigned char uint8_LinBusy;                 //1 = Lin communication busy / 0 = Lin not busy
    volatile unsigned char uint8_LinBreakToSend;          //1 = Lin break to send / 0 = No Lin break to send
    volatile unsigned char uint8_MasterSendCounter;       //counts how much byte was send from the master
    volatile unsigned char uint8_SlaveReceiveCounter;     //counts how much byte will be received from slave 
    volatile unsigned char uint8_SlaveAnswerRequested;    //1 = slave answer requested / 0 = No slave answer requested 
    volatile unsigned char uint8_SlaveAnswerFinish;       //1 = slave answer finish / 0 = slave still sending
    volatile unsigned char uint8_SlaveTimeout;            //1 = timeout occured / 0 = all ok
} SLin;                         //struct name is SLin (S for struct)

typedef struct                  //definitions of multiple variable for JE protocol
{
    volatile unsigned char uint8_JEprotocol;                //1 = true / 0 = false
}SJE;                           //struct name is SJE (S for struct)


#endif	/* LINATA6628_H */

