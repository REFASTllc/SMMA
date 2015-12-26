/**********************************************************************************************************************

                                            UBD - Unipolar Bipolar Driver

***********************************************************************************************************************
 * File name:               UART.c
 * Creation date:           03.10.2015
 * Main creator:            Alain Staub
 * Company:                 REFAST GmbH
 *                          Copyright (c) 2015 REFAST GmbH
***********************************************************************************************************************
 * Content overview:        - uart_init
 *                          - uart_set
 *                          - uart_InitInterrupt
 *                          - uart_enable
 *                          - uart_disable
 *                          - uart_SendChar
 *                          - uart_GetChar
 *                          - uart_status
 *                          - uart2_sendbuffer
 *                          - uart2_receivebuffer
 *                          - uart2_SendErrorCode
 *                          - uart1_sendbuffer
 *                          - uart1_receivebuffer
***********************************************************************************************************************/


#include "includes.h"

SUART2txd g_UART2txd;       //global variables for struct  
SUART2rxd g_UART2rxd;       //global variables for struct
SUART1txd g_UART1txd;       //global variables for struct
SUART1rxd g_UART1rxd;       //global variables for struct


/**********************************************************************************************************************
 * Routine:                 uart_init

 * Description:
 * Initialization of the UART module.
 * 
 * Creator:                 J. Rebetez
 * Date of creation:        05.10.2015
 * Last modification on:    -
 * Modified by:             - 
 * 
 * Input:                   -
 * Output:                  -
***********************************************************************************************************************/
void uart_init(unsigned char uartx)
{
    if(uartx == _UART1_)
    {
    /*** U1MODE ***************************************************************/
        U1MODEbits.ON = 0;      // UARTx Enable bit
                                // 1 = UARTx is enabled. UARTx pins are controlled by UARTx as defined by UEN<1:0> and UTXEN control bits
                                // 0 = UARTx is disabled. All UARTx pins are controlled by corresponding bits in the PORTx, TRISx and LATx registers; UARTx power consumption is minimal

        U1MODEbits.SIDL = 1;    // Stop in Idle Mode bit
                                // 1 = Discontinue operation when device enters Idle mode
                                // 0 = Continue operation in Idle mode

        U1MODEbits.IREN = 0;    // IrDA Encoder and Decoder Enable bit
                                // 1 = IrDA is enabled
                                // 0 = IrDA is disabled

        U1MODEbits.RTSMD = 0;   // Mode Selection for UxRTS Pin bit
                                // 1 = UxRTS pin is in Simplex mode
                                // 0 = UxRTS pin is in Flow Control mode

        U1MODEbits.UEN1 = 0;    // UARTx Enable bits
        U1MODEbits.UEN0 = 0;    // 11 = UxTX, UxRX and UxBCLK pins are enabled and used; UxCTS pin is controlled by corresponding bits in the PORTx register
                                // 10 = UxTX, UxRX, UxCTS and UxRTS pins are enabled and used
                                // 01 = UxTX, UxRX and UxRTS pins are enabled and used; UxCTS pin is controlled by corresponding bits in the PORTx register
                                // 00 = UxTX and UxRX pins are enabled and used; UxCTS and UxRTS/UxBCLK pins are controlled by corresponding bits in the PORTx register

        U1MODEbits.WAKE = 0;    // Enable Wake-up on Start bit Detect During Sleep Mode bit
                                // 1 = Wake-up enabled
                                // 0 = Wake-up disabled

        U1MODEbits.LPBACK = 0;  // UARTx Loopback Mode Select bit
                                // 1 = Loopback mode is enabled
                                // 0 = Loopback mode is disabled

        U1MODEbits.ABAUD = 0;   // Auto-Baud Enable bit
                                // 1 = Enable baud rate measurement on the next character ? requires reception of Sync character (0x55); cleared by hardware upon completion
                                // 0 = Baud rate measurement disabled or completed

        U1MODEbits.RXINV = 0;   // Receive Polarity Inversion bit
                                // 1 = UxRX Idle state is "0"
                                // 0 = UxRX Idle state is "1"

        U1MODEbits.BRGH = 0;    // High Baud Rate Enable bit
                                // 1 = High-Speed mode ? 4x baud clock enabled
                                // 0 = Standard Speed mode ? 16x baud clock enabled

        U1MODEbits.PDSEL1 = 0;  // Parity and Data Selection bits
        U1MODEbits.PDSEL0 = 0;  // 11 = 9-bit data, no parity
                                // 10 = 8-bit data, odd parity
                                // 01 = 8-bit data, even parity
                                // 00 = 8-bit data, none parity

        U1MODEbits.STSEL = 0;   // Stop Selection bit
                                // 1 = 2 Stop bits
                                // 0 = 1 Stop bit
    /*** U1STA ****************************************************************/
        U1STAbits.ADM_EN = 0;   // Automatic Address Detect Mode Enable bit
                                // 1 = Automatic Address Detect mode is enabled
                                // 0 = Automatic Address Detect mode is disabled

        U1STAbits.ADDR = 0xff;  // Automatic Address Mask bits
                                // When the ADM_EN bit is "1", this value defines the address character to use for automatic address detection.

        U1STAbits.UTXISEL1 = 0; // UTXISEL<1:0>: TX Interrupt Mode Selection bits(4)
        U1STAbits.UTXISEL0 = 1; // For 4-level deep FIFO UART modules:
                                // 11 = Reserved, do not use
                                // 10 = Interrupt is generated when the transmit buffer becomes empty
                                // 01 = Interrupt is generated when all characters have been transmitted
                                // 00 = Interrupt is generated when the transmit buffer contains at least one empty space
                                // For 8-level deep FIFO UART modules:
                                // 11 = Reserved, do not use
                                // 10 = Interrupt is generated and asserted while the transmit buffer is empty
                                // 01 = Interrupt is generated and asserted when all characters have been transmitted
                                // 00 = Interrupt is generated and asserted while the transmit buffer contains at least one empty space

        U1STAbits.UTXINV = 0;   // Transmit Polarity Inversion bit
                                // If IrDA mode is disabled (i.e., IREN (UxMODE<12>) is "0"):
                                // 1 = UxTX Idle state is "0"
                                // 0 = UxTX Idle state is "1"
                                // If IrDA mode is enabled (i.e., IREN (UxMODE<12>) is "1"):
                                // 1 = IrDA encoded UxTX Idle state is "1"
                                // 0 = IrDA encoded UxTX Idle state is "0"

        U1STAbits.URXEN = 1;    // Receiver Enable bit
                                // 1 = UARTx receiver is enabled. UxRX pin is controlled by UARTx (if ON = 1)
                                // 0 = UARTx receiver is disabled. UxRX pin is ignored by the UARTx module. UxRX pin is controlled by port.

        U1STAbits.UTXBRK = 0;   // Transmit Break bit
                                // 1 = Send Break on next transmission. Start bit followed by twelve ?0? bits, followed by Stop bit; cleared by hardware upon completion
                                // 0 = Break transmission is disabled or completed

        U1STAbits.UTXEN = 1;    // Transmit Enable bit
                                // 1 = UARTx transmitter is enabled. UxTX pin is controlled by UARTx (if ON = 1)
                                // 0 = UARTx transmitter is disabled. Any pending transmission is aborted and buffer is reset. UxTX pin is controlled by port.

        U1STAbits.UTXBF = 0;    // Transmit Buffer Full Status bit (read-only)
                                // 1 = Transmit buffer is full
                                // 0 = Transmit buffer is not full, at least one more character can be written

        U1STAbits.TRMT = 0;     // Transmit Shift Register is Empty bit (read-only)
                                // 1 = Transmit shift register is empty and transmit buffer is empty (the last transmission has completed)
                                // 0 = Transmit shift register is not empty, a transmission is in progress or queued in the transmit buffer

        U1STAbits.URXISEL1 = 0; // Receive Interrupt Mode Selection bit(4)
        U1STAbits.URXISEL0 = 0; // For 4-level deep FIFO UART modules:
                                // 11 = Interrupt flag bit is set when receive buffer becomes full (i.e., has 4 data characters)
                                // 10 = Interrupt flag bit is set when receive buffer becomes 3/4 full (i.e., has 3 data characters)
                                // 0x = Interrupt flag bit is set when a character is received
                                // For 8-level deep FIFO UART modules:
                                // 11 = Reserved; do not use
                                // 10 = Interrupt flag bit is asserted while receive buffer is 3/4 or more full (i.e., has 6 or more data characters)
                                // 01 = Interrupt flag bit is asserted while receive buffer is 1/2 or more full (i.e., has 4 or more data characters)
                                // 00 = Interrupt flag bit is asserted while receive buffer is not empty (i.e., has at least 1 data character)

        U1STAbits.ADDEN = 0;    // Address Character Detect bit (bit 8 of received data = 1)
                                // 1 = Address Detect mode is enabled. If 9-bit mode is not selected, this control bit has no effect.
                                // 0 = Address Detect mode is disabled

        U1STAbits.RIDLE = 0;    // Receiver Idle bit (read-only)
                                // 1 = Receiver is Idle
                                // 0 = Data is being received

        U1STAbits.PERR = 0;     // Parity Error Status bit (read-only)
                                // 1 = Parity error has been detected for the current character
                                // 0 = Parity error has not been detected

        U1STAbits.FERR = 0;     // Framing Error Status bit (read-only)
                                // 1 = Framing error has been detected for the current character
                                // 0 = Framing error has not been detected

        U1STAbits.OERR = 0;     // Receive Buffer Overrun Error Status bit.
                                // This bit is set in hardware and can only be cleared (= 0) in software. Clearing a previously set OERR bit resets the receiver buffer and RSR to empty state.
                                // 1 = Receive buffer has overflowed
                                // 0 = Receive buffer has not overflowed

        U1STAbits.URXDA = 0;    // Receive Buffer Data Available bit (read-only)
                                // 1 = Receive buffer has data, at least one more character can be read
                                // 0 = Receive buffer is empty
    /*** U1TXREG **************************************************************/
        U1TXREG = 0;            // Data bits 8-0 of the character to be transmitted
    /*** U1TXREG **************************************************************/
        U1RXREG = 0;
    /*** U1BRG ****************************************************************/
        U1BRG = 0;

    /*  U1MODEbits.BRGH = 0                  *//*  U1MODEbits.BRGH = 1                  */
    /*  Baud Rate = FPB / (16 * (U1BRG +1))  *//*  Baud Rate = FPB / (4 * (U1BRG +1))   */
    /*  U1BRG = (FPB / (16 * Baud rate)) -1  *//*  U1BRG = (FPB / (4 * Baud rate)) -1   */
    /*                          FPB denotes the PBCLK frequency                         */

    /*  Baud rate error                     */
    /*  (calculated baud rate - desired baud rate) / desired baud rate = xy%            */
    }
    else if(uartx ==_UART2_)
    {
    /*** U2MODE ***************************************************************/
        U2MODEbits.ON = 0;      // UARTx Enable bit
                                // 1 = UARTx is enabled. UARTx pins are controlled by UARTx as defined by UEN<1:0> and UTXEN control bits
                                // 0 = UARTx is disabled. All UARTx pins are controlled by corresponding bits in the PORTx, TRISx and LATx registers; UARTx power consumption is minimal

        U2MODEbits.SIDL = 1;    // Stop in Idle Mode bit
                                // 1 = Discontinue operation when device enters Idle mode
                                // 0 = Continue operation in Idle mode

        U2MODEbits.IREN = 0;    // IrDA Encoder and Decoder Enable bit
                                // 1 = IrDA is enabled
                                // 0 = IrDA is disabled

        U2MODEbits.RTSMD = 0;   // Mode Selection for UxRTS Pin bit
                                // 1 = UxRTS pin is in Simplex mode
                                // 0 = UxRTS pin is in Flow Control mode

        U2MODEbits.UEN1 = 0;    // UARTx Enable bits
        U2MODEbits.UEN0 = 0;    // 11 = UxTX, UxRX and UxBCLK pins are enabled and used; UxCTS pin is controlled by corresponding bits in the PORTx register
                                // 10 = UxTX, UxRX, UxCTS and UxRTS pins are enabled and used
                                // 01 = UxTX, UxRX and UxRTS pins are enabled and used; UxCTS pin is controlled by corresponding bits in the PORTx register
                                // 00 = UxTX and UxRX pins are enabled and used; UxCTS and UxRTS/UxBCLK pins are controlled by corresponding bits in the PORTx register

        U2MODEbits.WAKE = 0;    // Enable Wake-up on Start bit Detect During Sleep Mode bit
                                // 1 = Wake-up enabled
                                // 0 = Wake-up disabled

        U2MODEbits.LPBACK = 0;  // UARTx Loopback Mode Select bit
                                // 1 = Loopback mode is enabled
                                // 0 = Loopback mode is disabled

        U2MODEbits.ABAUD = 0;   // Auto-Baud Enable bit
                                // 1 = Enable baud rate measurement on the next character ? requires reception of Sync character (0x55); cleared by hardware upon completion
                                // 0 = Baud rate measurement disabled or completed

        U2MODEbits.RXINV = 0;   // Receive Polarity Inversion bit
                                // 1 = UxRX Idle state is "0"
                                // 0 = UxRX Idle state is "1"

        U2MODEbits.BRGH = 0;    // High Baud Rate Enable bit
                                // 1 = High-Speed mode ? 4x baud clock enabled
                                // 0 = Standard Speed mode ? 16x baud clock enabled

        U2MODEbits.PDSEL1 = 0;  // Parity and Data Selection bits
        U2MODEbits.PDSEL0 = 0;  // 11 = 9-bit data, no parity
                                // 10 = 8-bit data, odd parity
                                // 01 = 8-bit data, even parity
                                // 00 = 8-bit data, none parity

        U2MODEbits.STSEL = 0;   // Stop Selection bit
                                // 1 = 2 Stop bits
                                // 0 = 1 Stop bit
    /*** U2STA ****************************************************************/
        U2STAbits.ADM_EN = 0;   // Automatic Address Detect Mode Enable bit
                                // 1 = Automatic Address Detect mode is enabled
                                // 0 = Automatic Address Detect mode is disabled

        U2STAbits.ADDR = 0xff;  // Automatic Address Mask bits
                                // When the ADM_EN bit is ?1?, this value defines the address character to use for automatic address detection.

        U2STAbits.UTXISEL1 = 0; // UTXISEL<1:0>: TX Interrupt Mode Selection bits(4)
        U2STAbits.UTXISEL0 = 1; // For 4-level deep FIFO UART modules:
                                // 11 = Reserved, do not use
                                // 10 = Interrupt is generated when the transmit buffer becomes empty
                                // 01 = Interrupt is generated when all characters have been transmitted
                                // 00 = Interrupt is generated when the transmit buffer contains at least one empty space
                                // For 8-level deep FIFO UART modules:
                                // 11 = Reserved, do not use
                                // 10 = Interrupt is generated and asserted while the transmit buffer is empty
                                // 01 = Interrupt is generated and asserted when all characters have been transmitted
                                // 00 = Interrupt is generated and asserted while the transmit buffer contains at least one empty space

        U2STAbits.UTXINV = 0;   // Transmit Polarity Inversion bit
                                // If IrDA mode is disabled (i.e., IREN (UxMODE<12>) is ?0?):
                                // 1 = UxTX Idle state is ?0?
                                // 0 = UxTX Idle state is ?1?
                                // If IrDA mode is enabled (i.e., IREN (UxMODE<12>) is ?1?):
                                // 1 = IrDA encoded UxTX Idle state is ?1?
                                // 0 = IrDA encoded UxTX Idle state is ?0?

        U2STAbits.URXEN = 1;    // Receiver Enable bit
                                // 1 = UARTx receiver is enabled. UxRX pin is controlled by UARTx (if ON = 1)
                                // 0 = UARTx receiver is disabled. UxRX pin is ignored by the UARTx module. UxRX pin is controlled by port.

        U2STAbits.UTXBRK = 0;   // Transmit Break bit
                                // 1 = Send Break on next transmission. Start bit followed by twelve ?0? bits, followed by Stop bit; cleared by hardware upon completion
                                // 0 = Break transmission is disabled or completed

        U2STAbits.UTXEN = 1;    // Transmit Enable bit
                                // 1 = UARTx transmitter is enabled. UxTX pin is controlled by UARTx (if ON = 1)
                                // 0 = UARTx transmitter is disabled. Any pending transmission is aborted and buffer is reset. UxTX pin is controlled by port.

        U2STAbits.UTXBF = 0;    // Transmit Buffer Full Status bit (read-only)
                                // 1 = Transmit buffer is full
                                // 0 = Transmit buffer is not full, at least one more character can be written

        U2STAbits.TRMT = 0;     // Transmit Shift Register is Empty bit (read-only)
                                // 1 = Transmit shift register is empty and transmit buffer is empty (the last transmission has completed)
                                // 0 = Transmit shift register is not empty, a transmission is in progress or queued in the transmit buffer

        U2STAbits.URXISEL1 = 0; // Receive Interrupt Mode Selection bit(4)
        U2STAbits.URXISEL0 = 0; // For 4-level deep FIFO UART modules:
                                // 11 = Interrupt flag bit is set when receive buffer becomes full (i.e., has 4 data characters)
                                // 10 = Interrupt flag bit is set when receive buffer becomes 3/4 full (i.e., has 3 data characters)
                                // 0x = Interrupt flag bit is set when a character is received
                                // For 8-level deep FIFO UART modules:
                                // 11 = Reserved; do not use
                                // 10 = Interrupt flag bit is asserted while receive buffer is 3/4 or more full (i.e., has 6 or more data characters)
                                // 01 = Interrupt flag bit is asserted while receive buffer is 1/2 or more full (i.e., has 4 or more data characters)
                                // 00 = Interrupt flag bit is asserted while receive buffer is not empty (i.e., has at least 1 data character)

        U2STAbits.ADDEN = 0;    // Address Character Detect bit (bit 8 of received data = 1)
                                // 1 = Address Detect mode is enabled. If 9-bit mode is not selected, this control bit has no effect.
                                // 0 = Address Detect mode is disabled

        U2STAbits.RIDLE = 0;    // Receiver Idle bit (read-only)
                                // 1 = Receiver is Idle
                                // 0 = Data is being received

        U2STAbits.PERR = 0;     // Parity Error Status bit (read-only)
                                // 1 = Parity error has been detected for the current character
                                // 0 = Parity error has not been detected

        U2STAbits.FERR = 0;     // Framing Error Status bit (read-only)
                                // 1 = Framing error has been detected for the current character
                                // 0 = Framing error has not been detected

        U2STAbits.OERR = 0;     // Receive Buffer Overrun Error Status bit.
                                // This bit is set in hardware and can only be cleared (= 0) in software. Clearing a previously set OERR bit resets the receiver buffer and RSR to empty state.
                                // 1 = Receive buffer has overflowed
                                // 0 = Receive buffer has not overflowed

        U2STAbits.URXDA = 0;    // Receive Buffer Data Available bit (read-only)
                                // 1 = Receive buffer has data, at least one more character can be read
                                // 0 = Receive buffer is empty
    /*** U2TXREG **************************************************************/
        U2TXREG = 0;            // Data bits 8-0 of the character to be transmitted
    /*** U2TXREG **************************************************************/
        U2RXREG = 0;
    /*** U2BRG ****************************************************************/
        U2BRG = 0;
        
    /*  U2MODEbits.BRGH = 0                  *//*  U2MODEbits.BRGH = 1                  */
    /*  Baud Rate = FPB / (16 * (U2BRG +1))  *//*  Baud Rate = FPB / (4 * (U2BRG +1))   */
    /*  U2BRG = (FPB / (16 * Baud rate)) -1  *//*  U2BRG = (FPB / (4 * Baud rate)) -1   */
    /*                          FPB denotes the PBCLK frequency                         */

    /*  Baud rate error                     */
    /*  (calculated baud rate - desired baud rate) / desired baud rate = xy%            */
    }
    
//initialize the variables for UART2
    g_UART2txd.uint8_BufEmpty = 1;        //send buffer empty
    g_UART2txd.uint16_Wch = 0;            //write-pointer of the ring buffer at position 0
    g_UART2txd.uint16_Rch = 0;            //read-pointer of the ring buffer at position 0
    g_UART2txd.uint8_Bufptr = g_UART2txd.uint8_Buffer;  //pointer on buffer
  
    g_UART2rxd.uint8_BufEmpty = 1;         //receive buffer empty
    g_UART2rxd.uint16_Wch = 0;             //write-pointer of the ring buffer at position 0
    g_UART2rxd.uint16_Rch = 0;             //read-pointer of the ring buffer at position 0
    g_UART2rxd.uint8_Bufptr = g_UART2rxd.uint8_Buffer;   //pointer on buffer 
    
    
//initialize the variables for UART1
    g_UART1txd.uint8_BufEmpty = 1;        //send buffer empty
    g_UART1txd.uint16_Wch = 0;            //write-pointer of the ring buffer at position 0
    g_UART1txd.uint16_Rch = 0;            //read-pointer of the ring buffer at position 0
    g_UART1txd.uint8_Bufptr = g_UART1txd.uint8_Buffer;  //pointer on buffer
  
    g_UART1rxd.uint8_BufEmpty = 1;         //receive buffer empty
    g_UART1rxd.uint16_Wch = 0;             //write-pointer of the ring buffer at position 0
    g_UART1rxd.uint16_Rch = 0;             //read-pointer of the ring buffer at position 0
    g_UART1rxd.uint8_Bufptr = g_UART1rxd.uint8_Buffer;   //pointer on buffer 
}


/**********************************************************************************************************************
 * Routine:                 uart_set

 * Description:
 * Setting of the UART module.
 * 
 * Creator:                 J. Rebetez
 * Date of creation:        05.10.2015
 * Last modification on:    -
 * Modified by:             - 
 * 
 * Input:                   uartx (choose of the module)
 *                          parity (choose of the parity)
 *                          stopBit (length of a stop bit)
 *                          polarity (polarity of the lines) 
 *                          baudrate (baud rate of the UART (0 if auto)
 *                          autobaud (auto bauding on the UART)
 * Output:                  -
***********************************************************************************************************************/
void uart_set(unsigned char uartx, unsigned char parity, unsigned char stopBit, unsigned char polarity, unsigned char autobaud, unsigned long int baudrate)
{
    /*  UxMODEbits.BRGH = 0                  *//*  UxMODEbits.BRGH = 1                  */
    /*  Baud Rate = FPB / (16 * (UxBRG +1))  *//*  Baud Rate = FPB / (4 * (UxBRG +1))   */
    /*  UxBRG = (FPB / (16 * Baud rate)) -1  *//*  UxBRG = (FPB / (4 * Baud rate)) -1   */
    /*                          FPB denotes the PBCLK frequency                         */

    /*  Baud rate error                     */
    /*  (calculated baud rate - desired baud rate) / desired baud rate = xy%            */

//--- Choose of the UART to set ---//
    if(uartx == _UART1_)
    {
    //--- Set the parity ---//
        switch(parity)
        {
            case _NONE: U1MODEbits.PDSEL = 0;
                        break;
            case _EVEN: U1MODEbits.PDSEL = 1;
                        break;
            case _ODD:  U1MODEbits.PDSEL = 2;
                        break;
        }
    //--- Set the stop bit length ---//
        U1MODEbits.STSEL = stopBit;
    //--- Set the polarity of the UART ---//
        if(polarity == _NON_INVERTED)
        {
            U1MODEbits.RXINV = 0;
            U1STAbits.UTXINV = 0;
        }
        else
        {
            U1MODEbits.RXINV = 1;
            U1STAbits.UTXINV = 1;
        }
    //--- Set of the baud rate (if not in autobaud mode) ---//
        if(autobaud == _NO_AUTOBAUD)
        {
            U1MODEbits.ABAUD = 0;
            U1MODEbits.BRGH = 0;
            U1BRG = (_FREQ_OSC / (16 * baudrate)) - 1;
        }
        else U1MODEbits.ABAUD = 1;
    }
    else if(uartx == _UART2_)
    {
    //--- Set the parity ---//
        switch(parity)
        {
            case _NONE: U2MODEbits.PDSEL = 0;
                        break;
            case _EVEN: U2MODEbits.PDSEL = 1;
                        break;
            case _ODD:  U2MODEbits.PDSEL = 2;
                        break;
        }
    //--- Set the stop bit length ---//
        U2MODEbits.STSEL = stopBit;
    //--- Set the polarity of the UART ---//
        if(polarity == _NON_INVERTED)
        {
            U2MODEbits.RXINV = 0;
            U2STAbits.UTXINV = 0;
        }
        else
        {
            U2MODEbits.RXINV = 1;
            U2STAbits.UTXINV = 1;
        }
    //--- Set of the baud rate (if not in autobaud mode) ---//
        if(autobaud == _NO_AUTOBAUD)
        {
            U2MODEbits.ABAUD = 0;
            U2MODEbits.BRGH = 1;
            U2BRG = (_FREQ_OSC / (4 * baudrate)) - 1;
        }
        else U2MODEbits.ABAUD = 1;
    }
}


/**********************************************************************************************************************
 * Routine:                 uart_InitInterrupt

 * Description:
 * Initialization of the interrupts of the UART module.
 * Mod. 10.10.2015:
 * Bit "U2TXIE" is always set to 0, because this is controlled outside this rutine. 
 * 
 * Creator:                 J. Rebetez
 * Date of creation:        05.10.2015
 * Last modification on:    10.10.2015
 * Modified by:             A. Staub 
 * 
 * Input:                   uartx (choose of the module)
 *                          action (enable / disable interrupt)
 * Output:                  -
***********************************************************************************************************************/
void uart_InitInterrupt(unsigned char uartx, unsigned char action)
{
    if(uartx == _UART1_)
    {
        IFS0bits.U1RXIF = 0;
        IFS0bits.U1TXIF = 1;    //this must be set for the first time - don't set it to 0
        IFS0bits.U1EIF = 0;
        if(action == _ENABLE)
        {
            IPC6bits.U1IP = 2;
            IPC6bits.U1IS = 3;
            IEC0bits.U1RXIE = 0;    //set back later
            IEC0bits.U1TXIE = 0;    //this bit is controlled externally - don't set it 
            IEC0bits.U1EIE = 0;     //set back later
        }
        else
        {
            IEC0bits.U1RXIE = 0;
            IEC0bits.U1TXIE = 0;
            IEC0bits.U1EIE = 0;
        }
    }
    else if(uartx == _UART2_)
    {
        IFS1bits.U2RXIF = 0;
        IFS1bits.U2TXIF = 1;    //this must be set for the first time - don't set it to 0
        IFS1bits.U2EIF = 0;
        if(action == _ENABLE)
        {
            IPC8bits.U2IP = 2;
            IPC8bits.U2IS = 3;
            IEC1bits.U2RXIE = 1;
            IEC1bits.U2TXIE = 0;  //this bit is controlled externally - don't set it
            IEC1bits.U2EIE = 1;
        }
        else
        {
            IEC1bits.U2RXIE = 0;
            IEC1bits.U2TXIE = 0;
            IEC1bits.U2EIE = 0;
        }
    }
}


/**********************************************************************************************************************
 * Routine:                 uart_enable

 * Description:
 * Enable of UART module.
 * 
 * Creator:                 J. Rebetez
 * Date of creation:        05.10.2015
 * Last modification on:    -
 * Modified by:             - 
 * 
 * Input:                   uartx (choose of the module)
 * Output:                  -
***********************************************************************************************************************/
void uart_enable(unsigned char uartx)
{
    if(uartx == _UART1_)
    {
        U1STAbits.UTXEN = 1;
        U1STAbits.URXEN = 1;
        U1MODEbits.ON = 1;
    }
    else if(uartx == _UART2_)
    {
        U2STAbits.UTXEN = 1;  
        U2STAbits.URXEN = 1;
        U2MODEbits.ON = 1;
    }
}


/**********************************************************************************************************************
 * Routine:                 uart_disable

 * Description:
 * Disable of UART module.
 * 
 * Creator:                 J. Rebetez
 * Date of creation:        05.10.2015
 * Last modification on:    -
 * Modified by:             - 
 * 
 * Input:                   uartx (choose of the module)
 * Output:                  -
***********************************************************************************************************************/
void uart_disable(unsigned char uartx)
{
    if(uartx == _UART1_) U1MODEbits.ON = 1;
    else if(uartx == _UART2_) U2MODEbits.ON = 1;
}


/**********************************************************************************************************************
 * Routine:                 uart_SendChar

 * Description:
 * Send a character over UART. 
 * 
 * Creator:                 J. Rebetez
 * Date of creation:        05.10.2015
 * Last modification on:    -
 * Modified by:             - 
 * 
 * Input:                   uartx (choose of the module)
 *                          character (char to be send)
 * Output:                  -
***********************************************************************************************************************/
void uart_SendChar(unsigned char uartx, unsigned char character)
{
    if(uartx == _UART1_)
    {
        if(!U1STAbits.UTXEN)
        {
            U1STAbits.UTXEN = 1;
            while(!U1STAbits.UTXEN);
        }
        while(!U1STAbits.TRMT);
        U1TXREG = character;
    }
    else if(uartx == _UART2_)
    {
        if(!U2STAbits.UTXEN)
        {
            U2STAbits.UTXEN = 1;
            while(!U2STAbits.UTXEN);
        }
        while(!U2STAbits.TRMT);
        U2TXREG = character;
    }
}


/**********************************************************************************************************************
 * Routine:                 uart_GetChar

 * Description:
 * Get a character from UART. 
 * 
 * Creator:                 J. Rebetez
 * Date of creation:        05.10.2015
 * Last modification on:    -
 * Modified by:             - 
 * 
 * Input:                   -
 * Output:                  Character readed 
***********************************************************************************************************************/
unsigned char uart_GetChar(void)
{
    if(IFS0bits.U1RXIF)
    {
       IFS0bits.U1RXIF = 0;
       return U1RXREG;
    }
    else if(IFS1bits.U2RXIF)
    {
        IFS1bits.U2RXIF = 0;
        return U2RXREG;
    }
}


/**********************************************************************************************************************
 * Routine:                 uart_status

 * Description:
 * Get the status of the UART.
 * 
 * Creator:                 J. Rebetez
 * Date of creation:        05.10.2015
 * Last modification on:    -
 * Modified by:             - 
 * 
 * Input:                   uartx (choose of the UART)
 * Output:                  error code:
 *                              - bit 0: Overrun error 
 *                              - bit 1: Framing error
 *                              - bit 2: Parity error
***********************************************************************************************************************/
unsigned char uart_status(unsigned char uartx)
{
    unsigned char temp = 0;
    if(uartx == _UART1_)
    {
        if(U1STAbits.OERR) temp |= 0x01;
        if(U1STAbits.FERR) temp |= 0x02;
        if(U1STAbits.PERR) temp |= 0x04;
    }
    else if(uartx == _UART2_)
    {
        if(U2STAbits.OERR) temp |= 0x01;
        if(U2STAbits.FERR) temp |= 0x02;
        if(U2STAbits.PERR) temp |= 0x04;
    }
    return temp;
}


/**********************************************************************************************************************
 * Routine:                 uart2_sendbuffer

 * Description:
 * Store the data byte that is given to this routine into the send buffer. 
 * Increment the write-pointer of the send buffer.
 * Set the write-pointer of the send buffer to 0 if it is on the end position.
 * Clear BufEmpty to 0 --> Send buffer not more empty.
 * 
 * Creator:                 A. Staub
 * Date of creation:        07.10.2015
 * Last modification on:    -
 * Modified by:             - 
 * 
 * Input:                   uint8_DataByte
 * Output:                  -
***********************************************************************************************************************/
void uart2_sendbuffer(unsigned char uint8_DataByte)  
{
    //store data byte into send buffer
    *(g_UART2txd.uint8_Bufptr + g_UART2txd.uint16_Wch) = uint8_DataByte;
  
    //increment the write-pointer of the ring buffer
    g_UART2txd.uint16_Wch++;
  
    //verify if write-pointer is at the end of ring buffer
    g_UART2txd.uint16_Wch = g_UART2txd.uint16_Wch % _TxD2_BUFSIZE;
  
    //send buffer not empty
    g_UART2txd.uint8_BufEmpty = 0;    
}   //end of uart2_sendbuffer


/**********************************************************************************************************************
 * Routine:                 uart2_receivebuffer

 * Description:
 * Verify if the receive buffer is not empty. Buffer not empty:
 * Read out one byte from the receive buffer
 * Increment the read-pointer of the receive buffer
 * Set the read-pointer of the receive buffer to 0 if it is on the end position.
 * If the read-pointer is equal to write-pointer of the receive buffer, then set BufEmpty to 1.
 * Buffer empty:
 * Send back 'return' the value 0.  
 * 
 * Creator:                 A. Staub
 * Date of creation:        07.10.2015
 * Last modification on:    -
 * Modified by:             - 
 * 
 * Input:                   -
 * Output:                  uint8_DataByte
***********************************************************************************************************************/
unsigned char uart2_receivebuffer(void) 
{
    auto unsigned char uint8_DataByte;  //local variable uint8_DataByte
  
    if(!g_UART2rxd.uint8_BufEmpty)      //receive buffer not empty?
    {
        //read out one byte from the receive buffer
        uint8_DataByte = g_UART2rxd.uint8_Buffer[g_UART2rxd.uint16_Rch];
    
        //increment the read-pointer of the ring buffer
        g_UART2rxd.uint16_Rch++;
    
        //verify if read-pointer is at the end of ring buffer
        g_UART2rxd.uint16_Rch = g_UART2rxd.uint16_Rch % _RxD2_BUFSIZE;
    
        //verify if read-pointer and write-pointer of the ring buffer are equal
        if(g_UART2rxd.uint16_Rch == g_UART2rxd.uint16_Wch)
        {
            g_UART2rxd.uint8_BufEmpty = 1;  //receive buffer = empty
        }
        else
        {
            //do nothing
        }   
        return uint8_DataByte;            //return the value from the receive buffer        
    }
    else
    {
        return 0;                         //return value 0
    } 
}   //end of uart2_receivebuffer


/**********************************************************************************************************************
 * Routine:                 uart2_SendErrorCode

 * Description:
 * This routine sends back the error code on the UART 2 interface. 
 * The content is always the same, starting with an X followed by a <comma>, the error code
 * and at the end the <carriage return>. 
 * 
 * Creator:                 A. Staub
 * Date of creation:        10.10.2015
 * Last modification on:    -
 * Modified by:             - 
 * 
 * Input:                   uint8_ErrorCode
 * Output:                  -
***********************************************************************************************************************/
void uart2_SendErrorCode(unsigned char uint8_ErrorCode)
{ 
    //store the error code into the buffer
    uart2_sendbuffer('X');                          //first the letter X
    uart2_sendbuffer(',');                          //then the comma
    funct_IntToAscii(uint8_ErrorCode,_Active);      //the error code
    uart2_sendbuffer(13);                           //with CR at the end
}   //end of uart2_SendErrorCode


/**********************************************************************************************************************
 * Routine:                 uart1_sendbuffer

 * Description:
 * Store the data byte that is given to this routine into the send buffer. 
 * Increment the write-pointer of the send buffer.
 * Set the write-pointer of the send buffer to 0 if it is on the end position.
 * Clear BufEmpty to 0 --> Send buffer not more empty.
 * 
 * Creator:                 A. Staub
 * Date of creation:        17.12.2015
 * Last modification on:    -
 * Modified by:             - 
 * 
 * Input:                   uint8_DataByte
 * Output:                  -
***********************************************************************************************************************/
void uart1_sendbuffer(unsigned char uint8_DataByte)  
{
    //store data byte into send buffer
    *(g_UART1txd.uint8_Bufptr + g_UART1txd.uint16_Wch) = uint8_DataByte;
  
    //increment the write-pointer of the ring buffer
    g_UART1txd.uint16_Wch++;
  
    //verify if write-pointer is at the end of ring buffer
    g_UART1txd.uint16_Wch = g_UART1txd.uint16_Wch % _TxDRxD_BUFSIZE;
  
    //send buffer not empty
    g_UART1txd.uint8_BufEmpty = 0;    
}   //end of uart1_sendbuffer


/**********************************************************************************************************************
 * Routine:                 uart1_receivebuffer

 * Description:
 * Verify if the receive buffer is not empty. Buffer not empty:
 * Read out one byte from the receive buffer
 * Increment the read-pointer of the receive buffer
 * Set the read-pointer of the receive buffer to 0 if it is on the end position.
 * If the read-pointer is equal to write-pointer of the receive buffer, then set BufEmpty to 1.
 * Buffer empty:
 * Send back 'return' the value 0.  
 * 
 * Creator:                 A. Staub
 * Date of creation:        17.12.2015
 * Last modification on:    -
 * Modified by:             - 
 * 
 * Input:                   -
 * Output:                  uint8_DataByte
***********************************************************************************************************************/
unsigned char uart1_receivebuffer(void) 
{
    auto unsigned char uint8_DataByte;  //local variable uint8_DataByte
  
    if(!g_UART1rxd.uint8_BufEmpty)      //receive buffer not empty?
    {
        //read out one byte from the receive buffer
        uint8_DataByte = g_UART1rxd.uint8_Buffer[g_UART1rxd.uint16_Rch];
    
        //increment the read-pointer of the ring buffer
        g_UART1rxd.uint16_Rch++;
    
        //verify if read-pointer is at the end of ring buffer
        g_UART1rxd.uint16_Rch = g_UART1rxd.uint16_Rch % _TxDRxD_BUFSIZE;
    
        //verify if read-pointer and write-pointer of the ring buffer are equal
        if(g_UART1rxd.uint16_Rch == g_UART1rxd.uint16_Wch)
        {
            g_UART1rxd.uint8_BufEmpty = 1;  //receive buffer = empty
        }
        else
        {
            //do nothing
        }   
        return uint8_DataByte;            //return the value from the receive buffer        
    }
    else
    {
        return 0;                         //return value 0
    } 
}   //end of uart1_receivebuffer