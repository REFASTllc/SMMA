/**********************************************************************************************************************

                                            UBD - Unipolar Bipolar Driver

***********************************************************************************************************************
 * File name:               i2c.h
 * Creation date:           05.12.2015
 * Main creator:            A. Staub
 * Company:                 REFAST GmbH
 *                          Copyright (c) 2015 REFAST GmbH
***********************************************************************************************************************
 * Content overview :       - I2C
***********************************************************************************************************************/

/**********************************************************************************************************************
 * Routine:                 I2C_H

 * Description:
 * ...
 * 
 * Creator:                 A. Staub
 * Date of creation:        05.12.2015
 * Last modification on:    -
 * Modified by:             - 
 * 
 * Input:                   -
 * Output:                  -
***********************************************************************************************************************/

#ifndef I2C_H
#define	I2C_H


extern void i2c_init(unsigned char uint8_i2cx);
extern void i2c_enable(unsigned char uint8_i2cx);
extern void i2c_disabe(unsigned char uint8_i2cx);
extern void i2c_InitInterrupt(unsigned char uint8_i2cx, unsigned char uint8_action, unsigned char uint8_mode);
extern void i2c_StartTransfer(unsigned char uint8_i2cx);
extern void i2c_SendBufWr(unsigned char uint8_i2cx, unsigned char uint8_DataByte);
extern unsigned char i2c_SendBufRd(unsigned char uint8_i2cx);
extern void i2c_ReceiveBufWr(unsigned char uint8_i2cx, unsigned char uint8_DataByte);
extern unsigned char i2c_ReceiveBufRd(unsigned char uint8_i2cx);


#define _i2c1               1       //definition for the i2c module 1
#define _i2c2               2       //definition for the i2c module 2
#define _i2cENABLE          1       //definition for the uint8_action
#define _i2cDISABLE         0       //definition for the uint8_action
#define _i2cMASTER          1       //definition for the uint8_mode
#define _i2cSLAVE           0       //definition for the uint8_mode
#define _i2c1TxRxBuffer      20     //size of send and receive buffer of I2C module 1


typedef struct                      //definitions of multiple variable for function
{
    //volatile unsigned char uint8_StartCondt;    //1= start condition send / 0= start condition not send
    volatile unsigned char uint8_RScount;       //counter for repeat start condtion - set to 0 if not used (not used)
    volatile unsigned char uint8_RDcount;       //how much byte(s) to read
    volatile unsigned char uint8_RdWr;          //1= read command / 0= write command
    volatile unsigned char uint8_CurrDir;       //1= current direction = read / 0= current direction write
    volatile unsigned char uint8_LastNACKsend;  //1= last NACK send / 0= last NACK not send
    volatile unsigned char uint8_BusCollCount;  //bus collision counter
    volatile unsigned char uint8_ErrACK;        //1= error acknowledge / 0= no acknowledge error
    //volatile unsigned char uint8_Transfer;      //1= transfer started / 0= bus is busy / transfer finish
    volatile unsigned char uint8_TxRch;         //read-pointer of the send ring buffer
    volatile unsigned char uint8_TxWch;         //write-pointer of the send ring buffer
    volatile unsigned char *uint8_TxBufPtr;     //pointer for the send ring buffer
    volatile unsigned char uint8_TxBuf[_i2c1TxRxBuffer];    //buffer for send characters
    volatile unsigned char uint8_TxBufEmpty;    //1= send buffer empty / 0= send buffer not empty
    volatile unsigned char uint8_RxRch;         //read-pointer of the send ring buffer
    volatile unsigned char uint8_RxWch;         //write-pointer of the send ring buffer
    volatile unsigned char *uint8_RxBufPtr;     //pointer for the send ring buffer
    volatile unsigned char uint8_RxBuf[_i2c1TxRxBuffer];    //buffer for receive characters
    volatile unsigned char uint8_RxBufEmpty;    //1= receive buffer empty / 0= receive buffer not empty
} Si2c1;                                //struct name is Si2c1 (S for struct)
extern Si2c1    g_i2c1;                 //extern name is g_i2c1 (g for global)

#endif	/* I2C_H */


