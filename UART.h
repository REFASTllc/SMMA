/**********************************************************************************************************************

                                            UBD - Unipolar Bipolar Driver

***********************************************************************************************************************
 * File name:               uart.h
 * Creation date:           03.10.2015
 * Main creator:            J. Rebetez
 * Company:                 REFAST GmbH
 *                          Copyright (c) 2015 REFAST GmbH
***********************************************************************************************************************
 * Content overview :       - UART
***********************************************************************************************************************/

/**********************************************************************************************************************
 * Routine:                 UART_H

 * Description:
 * ...
 * 
 * Creator:                 J. Rebetez
 * Date of creation:        05.10.2015
 * Last modification on:    -
 * Modified by:             - 
 * 
 * Input:                   -
 * Output:                  -
 * Global variable:         -
***********************************************************************************************************************/

#ifndef UART_H
#define	UART_H


extern void uart_init(unsigned char uartx);
extern void uart_set(unsigned char uartx, unsigned char parity, unsigned char stopBit, unsigned char polarity, unsigned char autobaud, unsigned int baudrate);
extern void uart_InitInterrupt(unsigned char uartx, unsigned char action);
extern void uart_enable(unsigned char uartx);
extern void uart_disable(unsigned char uartx);
extern void uart_SendChar(unsigned char uartx, unsigned char character);
extern unsigned char uart_GetChar(void);
extern unsigned char uart_status(unsigned char uartx);
extern void uart2_sendbuffer(unsigned char uint8_DataByte);
extern unsigned char uart2_receivebuffer(void);
extern void uart2_SendErrorCode(unsigned char uint8_ErrorCode);
//extern void uart2_error(void);


#define _UART1_         1
#define _UART2_         2
#define _ENABLE         1
#define _DISABLE        0
#define _NONE           0
#define _EVEN           1
#define _ODD            2
#define _1_STOP         0
#define _2_STOP         1
#define _NON_INVERTED   0
#define _INVERTED       1
#define _NO_AUTOBAUD    0
#define _AUTOBAUD       1
#define _FREQ_OSC       80000000.0


//uart2 - txd
#define _TxD2_BUFSIZE 560         //size of send buffer from uart2 

typedef struct                    //definitions of multiple variable for uart2 txd
{
  volatile unsigned int uint16_Wch;                   //write-pointer of the ring buffer
  volatile unsigned int uint16_Rch;                   //read-pointer of the ring buffer
  volatile unsigned char uint8_BufEmpty;              //1: send buffer empty / 0: send buffer not empty
  volatile unsigned char *uint8_Bufptr;               //pointer for the ring buffer
  volatile unsigned char uint8_Buffer[_TxD2_BUFSIZE]; //buffer for characters
} SUART2txd;                      //struct name is SUART1txd (S for struct)
extern SUART2txd    g_UART2txd;   //extern name is g_UART1txd (g for global)


//uart2 - rxd
#define _RxD2_BUFSIZE 560          //size of receive buffer from uart2

typedef struct                    //definitions of multiple variable for uart2 rxd
{
  volatile unsigned int uint16_Wch;                   //write-pointer of the ring buffer
  volatile unsigned int uint16_Rch;                   //read-pointer of the ring buffer
  volatile unsigned char uint8_BufEmpty;              //1: receive buffer empty / 0: receive buffer not empty
  volatile unsigned char *uint8_Bufptr;               //pointer for the ring buffer
  volatile unsigned char uint8_Buffer[_RxD2_BUFSIZE];  //buffer for characters
  //volatile unsigned char uint8_FE;                    //1: framing error detected / 0: no framing error detected
  //volatile unsigned char uint8_PE;                    //1: parity error detected / 0: no parity error detected
} SUART2rxd;                      //struct name is SUART1rxd (S for struct)
extern SUART2rxd    g_UART2rxd;   //extern name is g_UART1rxd (g for global)

#endif	/* UART_H */