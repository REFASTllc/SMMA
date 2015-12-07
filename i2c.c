/**********************************************************************************************************************

                                            UBD - Unipolar Bipolar Driver

***********************************************************************************************************************
 * File name:               i2c.c
 * Creation date:           05.12.2015
 * Main creator:            A. Staub
 * Company:                 REFAST GmbH
 *                          Copyright (c) 2015 REFAST GmbH
***********************************************************************************************************************
 * Content overview:        - i2c_init
 *                          - i2c_enable
 *                          - i2c_disable
 *                          - i2c_InitInterrupt
 *                          - i2c_StartTransfer
 *                          - i2c_SendBufWr
 *                          - i2c_SendBufRd
 *                          - i2c_ReceiveBufWr
 *                          - i2c_ReceiveBufRd
***********************************************************************************************************************/


#include "includes.h"

Si2c1 g_i2c1;     //global variables for struct 


/**********************************************************************************************************************
 * Routine:                 i2c_init

 * Description:
 * Initialization of the i2c module. 
 * 
 * Creator:                 A. Staub
 * Date of creation:        05.05.2015
 * Last modification on:    -
 * Modified by:             - 
 * 
 * Input:                   uin8_i2cx
 * Output:                  -
***********************************************************************************************************************/
void i2c_init(unsigned char uint8_i2cx)
{
    if(uint8_i2cx == _i2c1)
    {
        //I2C1CON
        I2C1CONbits.ON = 0;             //i2c enable bit
                                        //0 = disable the i2c module
                                        //1 = enable the i2c module 
        
        I2C1CONbits.I2CSIDL = 1;        //stop in idle mode bit
                                        //0 = continue module operation in idle mode
                                        //1 = discontinue module operation when device enters idle mode
        
        I2C1CONbits.SCLREL = 1;         //scl release control bit in i2c slave mode only
                                        //if STREN = 0:
                                        //0 = force clock low (clock) stretch
                                        //1 = release clock
                                        //if STREN = 1;
                                        //0 = holds clock low (clock) stretch
                                        //1 = release clock
        
        I2C1CONbits.STRICT = 0;         //strict i2c reserved address rule enable bit
                                        //0 = strict i2c reserved address rule not enabled
                                        //1 = strict reserved addressing is enforced
                                        //    device doesn't respond to reserved address space or generate 
                                        //    addresses in reserved address space
        
        I2C1CONbits.A10M = 0;           //10-bit slave address flag bit
                                        //0 = I2CxADD is a 7-bit slave address
                                        //1 = I2CxADD is a 10-bit slave address
        
        I2C1CONbits.DISSLW = 0;         //slew rate control disable bit
                                        //0 = slew rate control enabled for high-speed mode (400kHz)
                                        //1 = slew rate control disabled for standard speed mode (100kHz) & (1MHz) too
        
        I2C1CONbits.SMEN = 0;           //SMBus input levels disable bit
                                        //0 = disable SMBus specific inputs
                                        //1 = enable input logic so that thresholds are compliant with SMBus spec.
        
        I2C1CONbits.GCEN = 0;           //general call enable bit
                                        //0 = general call address disabled
                                        //1 = enable interrupt when a general call address is received in I2CSR
        
        I2C1CONbits.STREN = 0;          //scl clock stretch enable bit (slave mode only)
                                        //0 = disable clock stretching
                                        //1 = enable clock stretching
        
        I2C1CONbits.ACKDT = 0;          //acknowledge data bit
                                        //0 = a ACK (invers) is sent
                                        //1 = a NACK is sent
        
        I2C1CONbits.ACKEN = 0;          //acknowledge sequence enable bit (master mode only)
                                        //0 = acknowledge sequency idle
                                        //1 = initiate acknowledge sequence on SDA & SCL pins and transmit ACKDT 
                                        //    data bit; cleared by module
        
        I2C1CONbits.RCEN = 0;           //receive enable bit
                                        //0 = receive sequence not in progress
                                        //1 = enables receive mode for i2c automatically cleared by module at 
                                        //    the end of 8-bit receive data byte
        
        I2C1CONbits.PEN = 0;            //stop condition enable bit
                                        //0 = stop condition idle
                                        //1 = initiate stop condition on SDA and SCL pins; cleared by module
        
        I2C1CONbits.RSEN = 0;           //restart condition enable bit
                                        //0 = restart condition idle
                                        //1 = initiate restart condition on SDA & SCL pins, cleared by module
        
        I2C1CONbits.SEN = 0;            //start condition enable bit
                                        //0 = start condition idle
                                        //1 = initiate start condition on SDA & SCL pins, cleared by module
        
        //I2C1MSK
        I2C1MSK = 0;                    //i2c address mask register
                                        //0 = address bit position must match the incoming i2c address match seq.
                                        //1 = forces a don't care in the particular bit position on the incoming
                                        //    address match sequence
        
        //I2C1BRG 
        I2C1BRG = 98;                   //i2c baud rate generator register
                                        //baud rate = [PBCLK / (FSCL x 2)] - 2
                                        //PBCLK = 80 MHz
                                        //FSCL = desired i2c bus speed = 400kHz 
        
        //init variables
        g_i2c1.uint8_RScount = 0;       //reset repeat start condition
        g_i2c1.uint8_ErrACK = 0;        //reset error
        //g_i2c1.uint8_Transfer = 0;      //reset transfer
        g_i2c1.uint8_RDcount = 0;       //reset counter 
        g_i2c1.uint8_RdWr = 0;          //set command on write
        g_i2c1.uint8_CurrDir = 0;       //direction = write
        g_i2c1.uint8_LastNACKsend = 0;  //reset variable
        g_i2c1.uint8_BusCollCount = 0;  //reset buss collision counter
        
        g_i2c1.uint8_TxBufEmpty = 1;    //reset buffer empty status
        g_i2c1.uint8_TxWch = 0;         //set write-pointer of the send ring buffer to 0
        g_i2c1.uint8_TxRch = 0;         //set read-pointer of the send ring buffer to 0
        g_i2c1.uint8_TxBufPtr = g_i2c1.uint8_TxBuf; //pointer on send buffer
        
        g_i2c1.uint8_RxBufEmpty = 1;    //reset buffer empty status
        g_i2c1.uint8_RxWch = 0;         //set write-pointer of the receive ring buffer to 0
        g_i2c1.uint8_RxRch = 0;         //set read-pointer of the receive ring buffer to 0
        g_i2c1.uint8_RxBufPtr = g_i2c1.uint8_RxBuf; //pointer on receive buffer      
    }
    else if(uint8_i2cx == _i2c2)
    {
        //I2C2CON
        I2C2CONbits.ON = 0;             //i2c enable bit
                                        //0 = disable the i2c module
                                        //1 = enable the i2c module 
        
        I2C2CONbits.I2CSIDL = 1;        //stop in idle mode bit
                                        //0 = continue module operation in idle mode
                                        //1 = discontinue module operation when device enters idle mode
        
        I2C2CONbits.SCLREL = 1;         //scl release control bit in i2c slave mode only
                                        //if STREN = 0:
                                        //0 = force clock low (clock) stretch
                                        //1 = release clock
                                        //if STREN = 1;
                                        //0 = holds clock low (clock) stretch
                                        //1 = release clock
        
        I2C2CONbits.STRICT = 0;         //strict i2c reserved address rule enable bit
                                        //0 = strict i2c reserved address rule not enabled
                                        //1 = strict reserved addressing is enforced
                                        //    device doesn't respond to reserved address space or generate 
                                        //    addresses in reserved address space
        
        I2C2CONbits.A10M = 0;           //10-bit slave address flag bit
                                        //0 = I2CxADD is a 7-bit slave address
                                        //1 = I2CxADD is a 10-bit slave address
        
        I2C2CONbits.DISSLW = 0;         //slew rate control disable bit
                                        //0 = slew rate control enabled for high-speed mode (400kHz)
                                        //1 = slew rate control disabled for standard speed mode (100kHz) & (1MHz) too
        
        I2C2CONbits.SMEN = 0;           //SMBus input levels disable bit
                                        //0 = disable SMBus specific inputs
                                        //1 = enable input logic so that thresholds are compliant with SMBus spec.
        
        I2C2CONbits.GCEN = 0;           //general call enable bit
                                        //0 = general call address disabled
                                        //1 = enable interrupt when a general call address is received in I2CSR
        
        I2C2CONbits.STREN = 0;          //scl clock stretch enable bit (slave mode only)
                                        //0 = disable clock stretching
                                        //1 = enable clock stretching
        
        I2C2CONbits.ACKDT = 0;          //acknowledge data bit
                                        //0 = a ACK (invers) is sent
                                        //1 = a NACK is sent
        
        I2C2CONbits.ACKEN = 0;          //acknowledge sequence enable bit (master mode only)
                                        //0 = acknowledge sequency idle
                                        //1 = initiate acknowledge sequence on SDA & SCL pins and transmit ACKDT 
                                        //    data bit; cleared by module
        
        I2C2CONbits.RCEN = 0;           //receive enable bit
                                        //0 = receive sequence not in progress
                                        //1 = enables receive mode for i2c automatically cleared by module at 
                                        //    the end of 8-bit receive data byte
        
        I2C2CONbits.PEN = 0;            //stop condition enable bit
                                        //0 = stop condition idle
                                        //1 = initiate stop condition on SDA and SCL pins; cleared by module
        
        I2C2CONbits.RSEN = 0;           //restart condition enable bit
                                        //0 = restart condition idle
                                        //1 = initiate restart condition on SDA & SCL pins, cleared by module
        
        I2C2CONbits.SEN = 0;            //start condition enable bit
                                        //0 = start condition idle
                                        //1 = initiate start condition on SDA & SCL pins, cleared by module
        
        //I2C2MSK
        I2C2MSK = 0;                    //i2c address mask register
                                        //0 = address bit position must match the incoming i2c address match seq.
                                        //1 = forces a don't care in the particular bit position on the incoming
                                        //    address match sequence
        
        //I2C2BRG 
        I2C2BRG = 98;                   //i2c baud rate generator register
                                        //baud rate = [PBCLK / (FSCL x 2)] - 2
                                        //PBCLK = 80 MHz
                                        //FSCL = desired i2c bus speed = 400kHz
    }
    else
    {
        //do nothing
    }
}   //end of i2c_init


/**********************************************************************************************************************
 * Routine:                 i2c_enable

 * Description:
 * Enable the i2c module. 
 * 
 * Creator:                 A. Staub
 * Date of creation:        05.05.2015
 * Last modification on:    -
 * Modified by:             - 
 * 
 * Input:                   uin8_i2cx
 * Output:                  -
***********************************************************************************************************************/
void i2c_enable(unsigned char uint8_i2cx)
{
    if(uint8_i2cx == _i2c1)
    {
        I2C1CONbits.ON = 1;             //i2c enable bit
                                        //0 = disable the i2c module
                                        //1 = enable the i2c module
    }
    else if (uint8_i2cx == _i2c2)
    {
        I2C2CONbits.ON = 1;             //i2c enable bit
                                        //0 = disable the i2c module
                                        //1 = enable the i2c module
    }
    else
    {
        //do nothing
    }
}   //end of i2c_enable


/**********************************************************************************************************************
 * Routine:                 i2c_disable

 * Description:
 * Disable the i2c module. 
 * 
 * Creator:                 A. Staub
 * Date of creation:        05.05.2015
 * Last modification on:    -
 * Modified by:             - 
 * 
 * Input:                   uin8_i2cx
 * Output:                  -
***********************************************************************************************************************/
void i2c_disable(unsigned char uint8_i2cx)
{
    if(uint8_i2cx == _i2c1)
    {
        I2C1CONbits.ON = 0;             //i2c enable bit
                                        //0 = disable the i2c module
                                        //1 = enable the i2c module
    }
    else if (uint8_i2cx == _i2c2)
    {
        I2C2CONbits.ON = 0;             //i2c enable bit
                                        //0 = disable the i2c module
                                        //1 = enable the i2c module
    }
    else
    {
        //do nothing
    }
}   //end of i2c_disable


/**********************************************************************************************************************
 * Routine:                 i2c_InitInterrupt

 * Description:
 * Init the i2c module interrupts. 
 * 
 * Creator:                 A. Staub
 * Date of creation:        05.05.2015
 * Last modification on:    -
 * Modified by:             - 
 * 
 * Input:                   uin8_i2cx
 *                          uint8_action
 * Output:                  -
***********************************************************************************************************************/
void i2c_InitInterrupt(unsigned char uint8_i2cx, unsigned char uint8_action, unsigned char uint8_mode)
{
    if(uint8_i2cx == _i2c1)
    {
        if(uint8_action)                //enable?
        {
            IFS0bits.I2C1MIF = 0;       //I2C1 master interrupt request flag bit
            IFS0bits.I2C1SIF = 0;       //I2C1 slave interrupt request flag bit
            IFS0bits.I2C1BIF = 0;       //I2C1 bus collection interrupt request flag bit
            
            IPC6bits.I2C1IP = 4;        //interrupt priority = 4
            IPC6bits.I2C1IS = 3;        //interrupt subpriority = 3
            
            if(uint8_mode)              //master?
            {
                IEC0bits.I2C1MIE = 1;   //master interrupt is enabled
                IEC0bits.I2C1SIE = 0;   //slave interrupt is enabled
                IEC0bits.I2C1BIE = 1;   //bus collision interrupt enabled
            }
            else                        //slave?
            {
                IEC0bits.I2C1MIE = 0;   //master interrupt is disabled
                IEC0bits.I2C1SIE = 1;   //slave interrupt is enabled
                IEC0bits.I2C1BIE = 0;   //bus collision interrupt disabled
            }
        }
        else                            //disable?
        {
            IEC0bits.I2C1MIE = 0;       //master interrupt is disabled
            IEC0bits.I2C1SIE = 0;       //slave interrupt is disabled
            IEC0bits.I2C1BIE = 0;       //bus collision interrupt disabled
        }
    }
    else if (uint8_i2cx == _i2c2)
    {
        if(uint8_action)                //enable?
        {
            IFS1bits.I2C2MIF = 0;       //I2C1 master interrupt request flag bit
            IFS1bits.I2C2SIF = 0;       //I2C1 slave interrupt request flag bit
            IFS1bits.I2C2BIF = 0;       //I2C1 bus collection interrupt request flag bit
            
            IPC8bits.I2C2IP = 4;        //interrupt priority = 4
            IPC8bits.I2C2IS = 3;        //interrupt subpriority = 3
            
            if(uint8_mode)              //master?
            {
                IEC1bits.I2C2MIE = 1;   //master interrupt is enabled
                IEC1bits.I2C2SIE = 0;   //slave interrupt is enabled
                IEC1bits.I2C2BIE = 1;   //bus collision interrupt enabled
            }
            else                        //slave?
            {
                IEC1bits.I2C2MIE = 0;   //master interrupt is disabled
                IEC1bits.I2C2SIE = 1;   //slave interrupt is enabled
                IEC1bits.I2C2BIE = 0;   //bus collision interrupt disabled
            }
        }
        else                            //disable?
        {
            IEC1bits.I2C2MIE = 0;       //master interrupt is disabled
            IEC1bits.I2C2SIE = 0;       //slave interrupt is disabled
            IEC1bits.I2C2BIE = 0;       //bus collision interrupt disabled
        }
    }
    else
    {
        //do nothing
    }
}   //end of i2c_InitInterrupt


/**********************************************************************************************************************
 * Routine:                 i2c_StartTransfer

 * Description:
 * Use this subroutine to launch the I2C communication. Please note that the buffer and all needed variables
 * must be set/defined before.   
 * 
 * Creator:                 A. Staub
 * Date of creation:        05.05.2015
 * Last modification on:    -
 * Modified by:             - 
 * 
 * Input:                   uin8_i2cx
 * Output:                  -
***********************************************************************************************************************/
void i2c_StartTransfer(unsigned char uint8_i2cx)
{
    if(uint8_i2cx == _i2c1)
    {
        i2c_enable(_i2c1);              //enable interrupt
        g_i2c1.uint8_LastNACKsend = 0;  //reset variable
        g_i2c1.uint8_CurrDir = 0;       //current direction = write
        I2C1CONbits.RCEN = 0;           //receive sequence not in progress
        I2C1CONbits.ACKEN = 0;          //acknowledge sequence disable
        I2C1CONbits.SEN = 1;            //send start condition - launch interrupt routine
    }
    else if(uint8_i2cx == _i2c2)
    {
        //not programmed yet
    }
    else
    {
        //nothing is done
    }
}   //end of i2c_StartTransfer


/**********************************************************************************************************************
 * Routine:                 i2c_SendBufWr

 * Description:
 * First the data byte which is given with the call of the subroutine is stored into the sendbuffer and the 
 * write pointer is incremented. If the write pointer is at the end of the buffer, then it is set automatic
 * to the position 0. At the end the 'uint8_TxBufEmpty' is set to 0 --> Buffer not more empty. 
 * 
 * Creator:                 A. Staub
 * Date of creation:        05.05.2015
 * Last modification on:    -
 * Modified by:             - 
 * 
 * Input:                   uint8_i2cx
 *                          uint8_DataByte
 * Output:                  -
***********************************************************************************************************************/
void i2c_SendBufWr(unsigned char uint8_i2cx, unsigned char uint8_DataByte)
{
    if(uint8_i2cx == _i2c1)
    {
        //store data byte into send buffer
        *(g_i2c1.uint8_TxBufPtr + g_i2c1.uint8_TxWch) = uint8_DataByte;
        
        //increment the write pointer
        g_i2c1.uint8_TxWch++;
        
        //verify if write pointer is at the end of ring buffer
        g_i2c1.uint8_TxWch = g_i2c1.uint8_TxWch % _i2c1TxRxBuffer;
        
        //set buffer not empty
        g_i2c1.uint8_TxBufEmpty = 0;
    }
    else if(uint8_i2cx == _i2c2)
    {
        //not programmed yet
    }
    else
    {
        //nothing is done
    }
}   //end of i2c_SendBufWr


/**********************************************************************************************************************
 * Routine:                 i2c_SendBufRd

 * Description:
 * First the send buffer will be verified if it is empty. If the buffer is empty, then the return value is 0.
 * If not, then one character is read out from the send buffer, and the read pointer of the send buffer is 
 * incremented. If the read pointer is at the end of the buffer, then it is set automaticaly to the position 0.
 * If the read pointer is equal to the write pointer of the send buffer, then the uint8_TxBufEmpty will be set to 1.
 * 
 * 
 * Creator:                 A. Staub
 * Date of creation:        05.05.2015
 * Last modification on:    -
 * Modified by:             - 
 * 
 * Input:                   uint8_i2cx
 * Output:                  uint8_WB
***********************************************************************************************************************/
unsigned char i2c_SendBufRd(unsigned char uint8_i2cx)
{
    auto unsigned char uint8_WB;    //local work byte
    
    if(uint8_i2cx == _i2c1)
    {
        if(!g_i2c1.uint8_TxBufEmpty)     //send buffer not empty?
        {
            //read out one byte from the send buffer
            uint8_WB = g_i2c1.uint8_TxBuf[g_i2c1.uint8_TxRch];
            
            //increment the read pointer
            g_i2c1.uint8_TxRch++;
            
            //verify if read pointer and write pointer of the ring buffer are equal
            if(g_i2c1.uint8_TxRch == g_i2c1.uint8_TxWch)
            {
                g_i2c1.uint8_TxBufEmpty = 1;        //send buffer empty
            }
            else
            {
                //nothing is done
            }
            
            return uint8_WB;    //return the value from the buffer
        }
        else
        {
            return 0;           //return value 0
        }
    }
    else if(uint8_i2cx == _i2c2)
    {
        //not programmed yet
    }
    else
    {
        //nothing is done
    }
}   //end of i2c_SendBufRd


/**********************************************************************************************************************
 * Routine:                 i2c_ReceiveBufWr

 * Description:
 * First the data byte which is given with the call of the subroutine is stored into the receive buffer and the 
 * write pointer is incremented. If the write pointer is at the end of the buffer, then it is set automatic
 * to the position 0. At the end the 'uint8_RxBufEmpty' is set to 0 --> Buffer not more empty. 
 * 
 * Creator:                 A. Staub
 * Date of creation:        05.05.2015
 * Last modification on:    -
 * Modified by:             - 
 * 
 * Input:                   uint8_i2cx
 *                          uint8_DataByte
 * Output:                  -
***********************************************************************************************************************/
void i2c_ReceiveBufWr(unsigned char uint8_i2cx, unsigned char uint8_DataByte)
{
    if(uint8_i2cx == _i2c1)
    {
        //store data byte into send buffer
        *(g_i2c1.uint8_RxBufPtr + g_i2c1.uint8_RxWch) = uint8_DataByte;
        
        //increment the write pointer
        g_i2c1.uint8_RxWch++;
        
        //verify if write pointer is at the end of ring buffer
        g_i2c1.uint8_RxWch = g_i2c1.uint8_RxWch % _i2c1TxRxBuffer;
        
        //set buffer not empty
        g_i2c1.uint8_RxBufEmpty = 0;
    }
    else if(uint8_i2cx == _i2c2)
    {
        //not programmed yet
    }
    else
    {
        //nothing is done
    }
}   //end of i2c_ReceiveBufWr


/**********************************************************************************************************************
 * Routine:                 i2c_ReceiveBufRd

 * Description:
 * First the receiver buffer will be verified if it is empty. If the buffer is empty, then the return value is 0.
 * If not, then one character is read out from the send buffer, and the read pointer of the send buffer is 
 * incremented. If the read pointer is at the end of the buffer, then it is set automaticaly to the position 0.
 * If the read pointer is equal to the write pointer of the send buffer, then the uint8_RxBufEmpty will be set to 1.
 * 
 * 
 * Creator:                 A. Staub
 * Date of creation:        05.05.2015
 * Last modification on:    -
 * Modified by:             - 
 * 
 * Input:                   uint8_i2cx
 * Output:                  uint8_WB
***********************************************************************************************************************/
unsigned char i2c_ReceiveBufRd(unsigned char uint8_i2cx)
{
    auto unsigned char uint8_WB;    //local work byte
    
    if(uint8_i2cx == _i2c1)
    {
        if(!g_i2c1.uint8_RxBufEmpty)    //send buffer not empty?
        {
            //read out one byte from the send buffer
            uint8_WB = g_i2c1.uint8_RxBuf[g_i2c1.uint8_RxRch];
            
            //increment the read pointer
            g_i2c1.uint8_RxRch++;
            
            //verify if read pointer and write pointer of the ring buffer are equal
            if(g_i2c1.uint8_RxRch == g_i2c1.uint8_RxWch)
            {
                g_i2c1.uint8_RxBufEmpty = 1;        //send buffer empty
            }
            else
            {
                //nothing is done
            }
            
            return uint8_WB;    //return the value from the buffer
        }
        else
        {
            return 0;           //return value 0
        }
    }
    else if(uint8_i2cx == _i2c2)
    {
        //not programmed yet
    }
    else
    {
        //nothing is done
    }
}   //end of i2c_ReceiveBufRd