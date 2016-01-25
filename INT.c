/**********************************************************************************************************************

                                            UBD - Unipolar Bipolar Driver

***********************************************************************************************************************
 * File name:               INT.c
 * Creation date:           08.08.2015
 * Main creator:            Julien Rebetez
 * Company:                 REFAST GmbH
 *                          Copyright (c) 2015 REFAST GmbH
***********************************************************************************************************************
 * Content overview:        - INT_init
 *                          - __IntUart2Handler
 *                          - __IntSPI1Handler
 *                          - __IntRTCCHandler
 *                          - __IntTimer45Handler
 *                          - __IntPWM1Handler
 *                          - __IntI2cHandler
 *                          - __IntINT2handler
 *                          - __IntTimer1Handler
 *                          - __IntUart1Handler
 *                          - __IntADCHandler
 * //_ADC_VECTOR
***********************************************************************************************************************/


#include "includes.h" // File which contain all includes files

S_MEAS_PWM DataPWM, DataFreq;
extern Si2c1 g_i2c1;
extern STimer1 g_Timer1;
extern SUART2txd g_UART2txd;
extern SUART2rxd g_UART2rxd;
extern SUni g_Uni;
extern SLin g_LIN; 
extern Sbipol g_Bipol;
extern T_A3981 A3981;
extern SParam g_Param;
extern SLin g_LIN; 
extern SUART1txd g_UART1txd;      
extern SUART1rxd g_UART1rxd;
extern SADC g_ADC;

/**********************************************************************************************************************
 * Routine:                 INT_init

 * Description:
 * ...
 * 
 * Creator:                 J. Rebetez
 * Date of creation:        08.08.2015
 * Last modification on:    -
 * Modified by:             - 
 * 
 * Input:                   -
 * Output:                  -
***********************************************************************************************************************/
void INT_init(void)
{
    INTCONbits.SS0 = 0;     // Single Vector Shadow Register Set bit
                            // 1 = Single vector is presented with a shadow register set
                            // 0 = Single vector is not presented with a shadow register set

    INTCONbits.MVEC = 1;    // Multi Vector Configuration bit
                            // 1 = Interrupt controller configured for multi vectored mode
                            // 0 = Interrupt controller configured for single vectored mode

    INTCONbits.TPC = 0;     // Interrupt Proximity Timer Control bits
                            // 111 = Interrupts of group priority 7 or lower start the Interrupt Proximity timer
                            // 110 = Interrupts of group priority 6 or lower start the Interrupt Proximity timer
                            // 101 = Interrupts of group priority 5 or lower start the Interrupt Proximity timer
                            // 100 = Interrupts of group priority 4 or lower start the Interrupt Proximity timer
                            // 011 = Interrupts of group priority 3 or lower start the Interrupt Proximity timer
                            // 010 = Interrupts of group priority 2 or lower start the Interrupt Proximity timer
                            // 001 = Interrupts of group priority 1 start the Interrupt Proximity timer 
                            // 000 = Disables Interrupt Proximity timer

    INTCONbits.INT4EP = 1;  // External Interrupt 4 Edge Polarity Control bit
                            // 1= Risingedge 
                            // 0= Fallingedge

    INTCONbits.INT3EP = 1;  // External Interrupt 3 Edge Polarity Control bit
                            // 1= Risingedge 
                            // 0= Fallingedge

    INTCONbits.INT2EP = 1;  // External Interrupt 2 Edge Polarity Control bit
                            // 1= Risingedge 
                            // 0= Fallingedge

    INTCONbits.INT1EP = 1;  // External Interrupt 1 Edge Polarity Control bit
                            // 1= Risingedge 
                            // 0= Fallingedge

    INTCONbits.INT0EP = 1;  // External Interrupt 0 Edge Polarity Control bit
                            // 1= Risingedge
                            // 0= Fallingedge

    //INTCONbits.FRZ = ?;

    if(!INTCONbits.MVEC)
    {
        INTSTATbits.SRIPL = 0;  // Requested Priority Level bits for Single Vector Mode bits
                                // 000-111 = The priority level of the latest interrupt presented to the CPU

        INTSTATbits.VEC = 0;    // Interrupt Vector bits
                                // 11111-00000 = The interrupt vector that is presented to the CPU
    }
}


/**********************************************************************************************************************
 * Routine:                 __IntUart2Handler

 * Description:
 * Manage Rx and Tx interrupts on UART2 
 * Rx:
 * Store received data byte into the receive buffer. Set the write-pointer of the receive buffer to 0 if it is 
 * on the end position. Clear BufEmpty to 0 --> Receive buffer not more empty. Otherwise check out 'uart1_error'
 * 
 * Tx:
 * If bit U2TXIF are set, then verify if the send buffer is empty. Buffer empty:
 * Disable the interrupt
 * Buffer not empty:
 * Read out one character from the send buffer.
 * Send this character over the TxD-Line.
 * Increment the read-pointer of the send buffer.
 * Set the read-pointer of the send buffer to 0 if it is on the end position.
 * If the read-pointer is equal to write-pointer of the send buffer, then set BufEmpty to 1. 
 *  
 * Creator:                 J. Rebetez
 * Date of creation:        08.08.2015
 * Last modification on:    07.10.2015
 * Modified by:             A. Staub
 * 
 * Input:                   -
 * Output:                  -
***********************************************************************************************************************/
void __ISR(_UART_2_VECTOR, IPL2AUTO) __IntUart2Handler(void)
{
//--- Is this an RX interrupt? ---//
    if(IFS1bits.U2RXIF)
    {
        IFS1bits.U2RXIF = 0;        //clear interrupt bit
        
        //store received data byte into receive buffer
        *(g_UART2rxd.uint8_Bufptr + g_UART2rxd.uint16_Wch) = U2RXREG;
       
        //increment the write-pointer of the ring buffer
        g_UART2rxd.uint16_Wch++;
    
        //verify if write-pointer is at the end of ring buffer
        g_UART2rxd.uint16_Wch = g_UART2rxd.uint16_Wch % _RxD2_BUFSIZE;
    
        //receive buffer not empty
        g_UART2rxd.uint8_BufEmpty = 0;        
    }
    
//--- Is this an Tx interrupt? ---//    
    if(IFS1bits.U2TXIF)
    {
        IFS1bits.U2TXIF = 0;        //clear interrupt bit
        
        if(!g_UART2txd.uint8_BufEmpty)      //send buffer not empty?
        {
            //read out one byte from the send buffer and send it directly
            U2TXREG = g_UART2txd.uint8_Buffer[g_UART2txd.uint16_Rch];
            
            //increment the read-pointer of the ring buffer
            g_UART2txd.uint16_Rch++;
      
            //verify if read-pointer is at the end of ring buffer
            g_UART2txd.uint16_Rch = g_UART2txd.uint16_Rch % _TxD2_BUFSIZE;
            
            //verify if read-pointer and write-pointer of the ring buffer are equal
            if(g_UART2txd.uint16_Rch == g_UART2txd.uint16_Wch)
            {
                g_UART2txd.uint8_BufEmpty = 1;  //send buffer = empty
            }
            else
            {
                //do nothing
            }
        }
        else
        {
            IEC1bits.U2TXIE = 0;    //disable the send interrupt
            IFS1bits.U2TXIF = 1;    //enable interrupt flag for the next time
        }     
    }
    
//--- Is this an error request interrupt? ---//
    if(IFS1bits.U2EIF)
    {
        IFS1bits.U2EIF = 0;
    }
}   //end of __IntUart2Handler

#ifdef SPI_H

extern T_SPI SPI1;
/****************************************************************************/
/*  Purpose of the INT routine:  Manage SPI 1 Rx interrupts                 */
/****************************************************************************/
void __ISR(_SPI_1_VECTOR, IPL4AUTO) __IntSPI1Handler(void)
{
    if(IFS0bits.SPI1RXIF)
    {
    //--- Save the new char in the buffer ---//
        SPI1.RxSPIbuffer[SPI1.RxIndex++] = SPI1BUF;
    //--- If index bigger than 10, then set the error flag and reset index reg ---//
        if(SPI1.RxIndex >= 10)
        {
            SPI1.RxIndex = 0;
            SPI1.bufferFull = 1;
        }
        SPI1.lastRxWrong = 0;
        IFS0bits.SPI1RXIF = 0;
    }
    else if(IFS0bits.SPI1TXIF)
        IFS0bits.SPI1TXIF = 0;
    else if(IFS0bits.SPI1EIF)
    {
        SPI1.lastRxWrong = 1;
        IFS0bits.SPI1EIF = 0;
    }
}
#endif

/**********************************************************************************************************************
 * Routine:                 __IntTimer45Handler

 * Description:
 * -old text deleted- 
 * 
 * Modification (23.12.2015 / A. Staub):
 * Timer changed to a combination of timer 2 and 3; name changed to __IntTimer23Handler.
 * Interrupt flag changed to timer 3.
 * 
 * Modification (24.12.2015 / A. Staub):
 * Old describtion complete erased, because we change the whole code. New description:
 * First this routine disble the interrupt for timer 23 and reset the interrupt flag and counter. 
 * Then it verifies if the motor type is set to unipolar / matrix or bipolar.
 * Unipolar to do's:
 * - load the counter with the new time
 * - set allow next step flag
 * - if it is the first, or last or an error is present or real = goal position:
 *      then nothing will be done
 * - if this is not the case, then we increment the real position and check which step mode is active to switch
 *   the outputs correct
 * 
 * Bipolar:
 * The important things to understand for the bipolar is that we have to generate an impuls on the step input
 * of the driver for that it executes a next step and this impuls has to be at least 1us long. 
 * So inside this routine we need two different times:
 * - the effective time that we have to wait between two impuls (step executes on the rising edge)
 * - the time to switch off the output step after >1us. 
 * That is the reason why we use the flag "g_Bipol.uint1_IntTimeExpiredFlag". 
 * If this flag is true then we have to load the new effective time but in the same time we subtracte
 * 400 (25ns * 400 = 10us) from the counter, because this was the time that we already waited for the impuls. 
 * If this flag is false then we load the time for the switch off of the output step at the beginning,
 * set the flag allow next step, increment the real position and switch on the output step as well. 
 * We also have a condition for the first, or last, or error or real = goal position.  
 * 
 * 
 * Modification (29.12.2015 / A. Staub)
 * Timer changed from timer 2 and 3 to 4 and 5. 
 * 
 * Creator:                 J. Rebetez
 * Date of creation:        08.08.2015
 * Last modification on:    29.12.2015
 * Modified by:             A. Staub
 * 
 * Input:                   -
 * Output:                  -
***********************************************************************************************************************/
void __ISR(_TIMER_5_VECTOR, IPL6AUTO) __IntTimer45Handler(void)
{
    auto unsigned char uint8_WB1;       //local variabel 'WorkByte1'
    
    T4CONbits.ON = 0;           //disable interrupt module
    IFS0bits.T5IF = 0;          //clear interrupt flag of timer 5
    TMR4 = 0;                   //reset LSB counter
    TMR5 = 0;                   //reset MSB counter
    
    if((g_Param.uint8_MotTyp == 'U') || (g_Param.uint8_MotTyp == 'M'))
    {
        //load the new interrupt time
        PR4 = g_Uni.uint32_IntTime & 0x0000FFFF;  //first the LSB
        PR5 = g_Uni.uint32_IntTime >> 16;         //second the MSB

        g_Uni.uint8_Status |= 0x10;     //allow next step

        //for security - be sure that the motor is in run
        if(g_Uni.uint8_Settings & 0x01)
        {
            //then verify if it is the first or last step or motor error = true or real = goal position
            if((g_Uni.uint8_Status & 0x01) || (g_Uni.uint8_Status & 0x02) ||
            (g_Uni.uint8_Status & 0x80) || (g_Uni.uint32_RealPos == g_Uni.uint32_GoalPos))
            {
                //then do nothing
            }
            else    //otherwise do...
            {
                g_Uni.uint32_RealPos++;     //increment real position with 1      

                if(g_Uni.uint8_Settings & 0x10)     //verify the direction
                {
                    //direction = CW
                    g_Uni.uint8_FsHsCount++;    //increment counter
                }
                else
                {
                    //direction = CCW
                    g_Uni.uint8_FsHsCount--;    //decrement counter
                }

                if(g_Uni.uint8_Settings & 0x04) //verify step mode if full or half
                {
                    //step mode = half step
                    uint8_WB1 = g_Uni.uint8_FsHsCount % 8;  //counter modulo 8

                    switch(uint8_WB1)
                    {
                        case (0):
                            g_Uni.uint8_PhA1 = _UniPhOFF;
                            g_Uni.uint8_PhA2 = _UniPhON;
                            g_Uni.uint8_PhB1 = _UniPhOFF;
                            g_Uni.uint8_PhB2 = _UniPhON;
                            break;

                        case (1):
                            g_Uni.uint8_PhA1 = _UniPhOFF;
                            g_Uni.uint8_PhA2 = _UniPhON;
                            g_Uni.uint8_PhB1 = _UniPhOFF;
                            g_Uni.uint8_PhB2 = _UniPhOFF;
                            break;

                        case (2):
                            g_Uni.uint8_PhA1 = _UniPhOFF;
                            g_Uni.uint8_PhA2 = _UniPhON;
                            g_Uni.uint8_PhB1 = _UniPhON;
                            g_Uni.uint8_PhB2 = _UniPhOFF;
                            break;

                        case (3):
                            g_Uni.uint8_PhA1 = _UniPhOFF;
                            g_Uni.uint8_PhA2 = _UniPhOFF;
                            g_Uni.uint8_PhB1 = _UniPhON;
                            g_Uni.uint8_PhB2 = _UniPhOFF;
                            break;

                        case (4):
                            g_Uni.uint8_PhA1 = _UniPhON;
                            g_Uni.uint8_PhA2 = _UniPhOFF;
                            g_Uni.uint8_PhB1 = _UniPhON;
                            g_Uni.uint8_PhB2 = _UniPhOFF;
                            break;

                        case (5):
                            g_Uni.uint8_PhA1 = _UniPhON;
                            g_Uni.uint8_PhA2 = _UniPhOFF;
                            g_Uni.uint8_PhB1 = _UniPhOFF;
                            g_Uni.uint8_PhB2 = _UniPhOFF;
                            break;

                        case (6):
                            g_Uni.uint8_PhA1 = _UniPhON;
                            g_Uni.uint8_PhA2 = _UniPhOFF;
                            g_Uni.uint8_PhB1 = _UniPhOFF;
                            g_Uni.uint8_PhB2 = _UniPhON;
                            break;

                        case (7):
                            g_Uni.uint8_PhA1 = _UniPhOFF;
                            g_Uni.uint8_PhA2 = _UniPhOFF;
                            g_Uni.uint8_PhB1 = _UniPhOFF;
                            g_Uni.uint8_PhB2 = _UniPhON;
                            break;

                        default:
                            //nothing is done
                            break;
                    }   
                }
                else
                {
                    //step mode = full step
                    uint8_WB1 = g_Uni.uint8_FsHsCount % 4;  //counter modulo 4

                    //define which coil(s) should be switched on and in the same time do not
                    //switch on some coils if the full step is in one phase on. 
                    switch(uint8_WB1)       
                    {
                        case (0):
                            g_Uni.uint8_PhA1 = _UniPhOFF;
                            g_Uni.uint8_PhA2 = _UniPhON;
                            g_Uni.uint8_PhB1 = _UniPhON && (g_Uni.uint8_Settings & 0x40);
                            g_Uni.uint8_PhB2 = _UniPhOFF;
                            break;

                        case (1):
                            g_Uni.uint8_PhA1 = _UniPhON && (g_Uni.uint8_Settings & 0x40);
                            g_Uni.uint8_PhA2 = _UniPhOFF;
                            g_Uni.uint8_PhB1 = _UniPhON;
                            g_Uni.uint8_PhB2 = _UniPhOFF;
                            break;

                        case (2):
                            g_Uni.uint8_PhA1 = _UniPhON;
                            g_Uni.uint8_PhA2 = _UniPhOFF;
                            g_Uni.uint8_PhB1 = _UniPhOFF;
                            g_Uni.uint8_PhB2 = _UniPhON && (g_Uni.uint8_Settings & 0x40);
                            break;

                        case (3):
                            g_Uni.uint8_PhA1 = _UniPhOFF;
                            g_Uni.uint8_PhA2 = _UniPhON && (g_Uni.uint8_Settings & 0x40);
                            g_Uni.uint8_PhB1 = _UniPhOFF;
                            g_Uni.uint8_PhB2 = _UniPhON;
                            break;

                        default:
                            //nothing is done
                            break;
                    }
                }       
                //write the new values to the outputs
                oUniCoilA1 = g_Uni.uint8_PhA1;
                oUniCoilA2 = g_Uni.uint8_PhA2;
                oUniCoilB1 = g_Uni.uint8_PhB1;
                oUniCoilB2 = g_Uni.uint8_PhB2;
            }
        }
        else
        {
            //otherwise load the modulo timer with around 0.5s 
            //for that it executes not too much <-- error
            PR4 = 0xFFFF;
            PR5 = 0x00FF;
        }
    }
    else    //motor is a bipolar
    {
        if(g_Bipol.uint1_IntTimeExpiredFlag)    //interrupt time expired?
        {
            g_Bipol.uint1_IntTimeExpiredFlag = 0;   //clear the flag
            oBiStepSignal = 0;                      //reset output
            
            //load the new interrupt time
            PR4 = g_Bipol.uint32_IntTime & 0x0000FFFF;  //first the LSB
            PR5 = g_Bipol.uint32_IntTime >> 16;         //second the MSB
            PR4 -=8000;  //to correct the already waited time of 10us from the step impuls (output)
        }
        else
        {
            PR4 = 8000;  //load interrupt time with 10us
            //force the interrupt routine to load the correct time (next time)
            g_Bipol.uint1_IntTimeExpiredFlag = 1;   
                
            g_Bipol.status.BITS.nextStepIsAllowed = 1;   //allow next step
            
            //for security - be sure that the motor is in run
            if(g_Bipol.uint1_IsBipolEnabled == 1)
            {
                //if it is the first or last step or motor error = true or real = goal position?
                if((g_Bipol.status.BITS.firstStepIsActived) || (g_Bipol.status.BITS.lastStepIsActived) ||
                (g_Bipol.status.BITS.error) || (g_Bipol.uint32_RealPos == g_Bipol.uint32_GoalPos))
                {
                    //then do nothing
                }
                else        //otherwise do...
                {
                    g_Bipol.uint32_RealPos++;       //increment real position with 1      
                    oBiStepSignal = 1;              //execute one step
                }
            }
            else
            {
                //otherwise load the modulo timer with around 0.5s 
                //for that it executes not too much <-- error
                PR4 = 0xFFFF;
                PR5 = 0x00FF;
            }
        }
    }
    
    T4CONbits.ON = 1;           //enable interrupt module    
}   //end of __IntTimer2Handler

/**********************************************************************************************************************
 * Routine:                 __IntI2cHandler

 * Description:
 * Manage I2C modul 1 interrupt
 * 
 * Master interrupt request:
 * This routine will be called if one of the follow occurs:
 * - Start condition executed
 * - Repeated start condition executed
 * - Stop condition executed
 * - Data transfer byte received
 * - During SEND ACK (invers) sequence
 * - Data transfer byte transmitted
 * - During a slave-detected stop
 * Write explication:
 * The external subroutine "i2c_StartTransfer" launchs the interrupt by executing a start condition on the line.
 * So first it goes into the SEND routine, where it checks that there is no transmission on the bus and the current
 * direction will be set to write inside the external routine "i2c_StartTransfer". 
 * It verifies if an ACK was received from the slave, otherwise it set the error ErrACK and reset all. 
 * A stop condition will be executed as well. If an ACK was received then it takes the next byte to send 
 * from the array. Once there are not anymore bytes to send and like the command was a write, it send the 
 * stop condition. 
 * This stop condition launch again this interrupt and the first if case will be executed where we disable the 
 * interrupt routine and reset all needed variables. 
 * Read explication:
 * It does exactly the same like a write at the beginning. But once the send array is empty and the command was
 * a read, it enables the receive mode and change the current direction variable by setting to 1.
 * The next time enter into this subroutine use now the RECEIVE routine. 
 * Like we know in advance how much bytes we will receive by using the counter "uint8_RDcount" we know how 
 * to react inside this case. On every received byte we send out an ACK (invers) until we received the last
 * byte, because there we send an NACK for that the slave knows that we don't need more bytes. 
 * We can't send the NACK and stop condition at the same time, that is the reason why we use the variable
 * "uint8_LastNACKsend". We send the NACK and set this variable to true for that the next time we enter
 * this routine it use the if case to send again the stop condition. 
 * Write with Read explication:
 * For this the variable "g_i2c1.uint8_RScount" is used. This variable you have to define before launching
 * the I2C interrupt. This is used to generate a repeat start condition. 
 * If you set this variable to 0 then nothing will be made, means a repeat start condition will 
 * not generate. If for example you send the follow:
 * Address + Data1 + Data2 --> Generate repeat start condition --> Address + RecData1 + RecData2
 * then your variable "g_i2c1.uint8_RScount" must be set to 1. Because the master sends 4 bytes:
 * Address + Data1 + Data2 + Address and before the second time sending the Address you want to generate
 * this repeat start condition. That is the position 1. 
 * 
 * 
 * Slave interrupt request:
 * Not programmed yet and not used for this project
 * 
 * Bus collision interrupt request:
 * At the moment there is just a counter that can count until 255 bus collisions. 
 *  
 * Creator:                 A. Staub
 * Date of creation:        05.12.2015
 * Last modification on:    
 * Modified by:             
 * 
 * Input:                   -
 * Output:                  -
***********************************************************************************************************************/
void __ISR(_I2C_1_VECTOR, IPL4AUTO) __IntI2cHandler(void)
{
    
//--- Is this a master interrupt request? ---//
    if(IFS0bits.I2C1MIF)
    {
        IFS0bits.I2C1MIF = 0;       //clear interrupt bit
        
        if(I2C1STATbits.P)          //stop condition detected?
        {
            i2c_disable(_i2c1);             //disable interrupt
            I2C1CONbits.RCEN = 0;           //disable receive mode
            g_i2c1.uint8_LastNACKsend = 0;  //reset variable
            g_i2c1.uint8_Busy = 0;          //i2c finished the transfer of the datas
        }
        else
        {
            //do nothing
        }

        //last NACK was send
        if(g_i2c1.uint8_LastNACKsend)
        {
            I2C1CONbits.PEN = 1;    //initiate stop condition ond SDA & SCL pins, cleared by module
        }
        else
        {
            //do nothing
        }
        
        //SEND routine
        //transmit not anymore in progress (test both of them) and receive mode not enable
        if((!I2C1STATbits.TRSTAT) && (!I2C1STATbits.TBF) && (!g_i2c1.uint8_CurrDir))   
        {  
            if(!I2C1STATbits.ACKSTAT)   //acknowledge received?
            {
                if(!g_i2c1.uint8_TxBufEmpty)    //send buffer not empty
                { 
                    if(g_i2c1.uint8_RScount == 0)   //repeat start condition not used?
                    {
                        I2C1TRN = i2c_SendBufRd(_i2c1); //take out one byte from the buffer and send it
                    }
                    else if(g_i2c1.uint8_RScount == 1)  //sent repeat start condition?
                    {
                        g_i2c1.uint8_RScount--;         //decrement counter
                        I2C1CONbits.RSEN = 1;           //sent repeat start condition
                    }
                    else
                    {
                        g_i2c1.uint8_RScount--;         //decrement counter
                        I2C1TRN = i2c_SendBufRd(_i2c1); //take out one byte from the buffer and send it
                    }
                }
                else                                //send buffer empty, so send stop condition
                {
                    if(g_i2c1.uint8_RdWr)       //was the command a read command?
                    {
                        I2C1CONbits.RCEN = 1;   //enables receive mode, automatic cleared 
                                                //by module at the end of 8-bit receive data byte
                        g_i2c1.uint8_CurrDir = 1; //direction = read
                    }
                    else                        //command was a write command
                    {
                        I2C1CONbits.PEN = 1;    //initiate stop condition ond SDA & SCL pins, cleared by module                        
                    }
                }
            }
            else
            {
                //error
                I2C1CONbits.PEN = 1;            //initiate stop condition ond SDA & SCL pins, cleared by module
                g_i2c1.uint8_ErrACK = 1;        //set ack error
                
                //clear buffer
                g_i2c1.uint8_TxRch = g_i2c1.uint8_TxWch;
                g_i2c1.uint8_TxBufEmpty = 1;             
            }
        }
        else
        {
            //wait...
        }
        
        //RECEIVE routine
        if(I2C1STATbits.RBF)            //receive buffer full/complete
        {
            i2c_ReceiveBufWr(_i2c1,I2C1RCV);    //store received byte into receive buffer
            
            g_i2c1.uint8_RDcount--;             //decrement the counter how many bytes are to read
            
            if(g_i2c1.uint8_RDcount == 0)       //last byte was read
            {
                I2C1CONbits.ACKDT = 1;          //NACK will be send
                I2C1CONbits.ACKEN = 1;          //send NACK
                g_i2c1.uint8_LastNACKsend = 1;  //last NACK send
            }
            else if(g_i2c1.uint8_RDcount == 1)  //2nd last byte was read
            {
                I2C1CONbits.ACKDT = 0;          //ACK will be send
                I2C1CONbits.ACKEN = 1;          //send ACK
            }
            else
            {
                //do nothing
            }
        }
        else
        {
            //wait...
        }
        
        //acknowledge received and current direction in read?
        if((!I2C1STATbits.ACKSTAT) && (g_i2c1.uint8_CurrDir))
        {
            I2C1CONbits.RCEN = 1;   //enables receive mode, automatic cleared 
                                    //by module at the end of 8-bit receive data byte        
        }
    }
    
//--- Is this a slave interrupt request? ---//    
    if(IFS0bits.I2C1SIF)
    {
        IFS0bits.I2C1SIF  = 0;      //clear interrupt bit
        
        //not programmed yet   
    }
    
//--- Is this a BUS collision interrupt request? ---//
    if(IFS0bits.I2C1BIF)
    {
        IFS0bits.I2C1BIF = 0;       //clear interrupt bit
        g_i2c1.uint8_BusColl = 1;   //set bus collision flag
        g_i2c1.uint8_Busy = 0;      //i2c finished the transfer, because there was a coll. err. 
        
        if(g_i2c1.uint8_BusCollCount == 255)    //max. of the counter
        {
            //do nothing
        }
        else
        {
            g_i2c1.uint8_BusCollCount++;        //increment the counter
        }       
    }
}   //end of __IntI2cHandler


/**********************************************************************************************************************
 * Routine:                 __IntINT2handler

 * Description:
 * This interrupt occurs if the external output signal from the RV30xx changes from high to low. 
 * This means that a system reset occured, because of an internal error.
 * This routine tries to start a re-initialization of the chip. 
 * At the moment the this error is not stored somewhere. That we should to, still an open point and noted
 * into the PCDA list. 
 *   
 * Creator:                 A. Staub
 * Date of creation:        17.12.2015
 * Last modification on:    
 * Modified by:             
 * 
 * Input:                   -
 * Output:                  -
***********************************************************************************************************************/
void __ISR(_EXTERNAL_2_VECTOR, IPL3AUTO) __IntINT2handler(void)
{
    IFS0bits.INT2IF = 0;    //clear the interrupt flag
    
    //add here an alarm!!!
    
//first enable again the RV30xx chip because the interrupt signaled a "system reset detection"
    //fill out the buffer
    RV30xx_release();       //call subroutine
    RV30xx_init();          //call subroutine
    RV30xx_InitInterrupt(_Rv30xxENABLE);    //call subroutine
    
    //clear the SRF bit from the control_int flag register
    i2c_SendBufWr(_i2c1,_RV30xxAddr);
    i2c_SendBufWr(_i2c1,_RegControlIntFlag);
    i2c_SendBufWr(_i2c1,0);                 //clear all flag

    //define some important variables for the transfer
    g_i2c1.uint8_RdWr = 0;                  //command will be a write
    g_i2c1.uint8_RScount = 0;               //repeat start condition not used
    
    i2c_StartTransfer(_i2c1);   //launch the transfer
    
    do
    {
        //do nothing...
    }
    while(g_i2c1.uint8_Busy);   //until the transfer is finished
    
    
}   //end of __IntINT2handler


/**********************************************************************************************************************
 * Routine:                 __IntTimer1Handler

 * Description:
 * First we reset the interrupt flag and disable the interrupt as well.
 * If the counter (how much time we have to wait the 200ms is not empty then we load again 200ms for the
 * interrupt and enable it. 
 * Once the counter is empty (0) we disable the interrupt and the safety flag. 
 * 
 * Creator:                 A. Staub
 * Date of creation:        18.12.2015
 * Last modification on:    
 * Modified by:             
 * 
 * Input:                   -
 * Output:                  -
***********************************************************************************************************************/
void __ISR(_TIMER_1_VECTOR, IPL5AUTO) __IntTimer1Handler(void)
{ 
    T1CONbits.ON = 0;       //disable timer 1
    TMR1 = 0;               //reset counter
    IFS0bits.T1IF = 0;      //clear interrupt flag
    
    switch (g_Timer1.uint8_200msCount)  //verify the value of how much time we have to wait the 200ms
    {
        case (0):   //time expired?
            g_Timer1.uint8_Timer1SafFlag = 0;   //disable the safety flag
            T1CONbits.ON = 0;                   //disable timer 1
            
//add here some flags / register for your application (LIN, etc. )
            //LIN:
            g_LIN.uint8_SlaveTimeout = 1;   //timeout occured
            
            
            break;
            
        default:
            PR1 = 312 * 200;    //load PR1 with 312 (1ms) * 200 to have a wait time of 200ms
            g_Timer1.uint8_200msCount--;    //decrement the counter for the 200ms 
            T1CONbits.ON = 1;               //enable timer 1
            break;
    }
}   //end of __IntTimer1Handler


/**********************************************************************************************************************
 * Routine:                 __IntUart1Handler

 * Description:
 * Manage Rx and Tx interrupts on UART2 
 * Rx:
 * Store received data byte into the receive buffer. Set the write-pointer of the receive buffer to 0 if it is 
 * on the end position. Clear BufEmpty to 0 --> Receive buffer not more empty. Otherwise check out 'uart1_error'
 * 
 * Tx:
 * If bit U2TXIF are set, then verify if the send buffer is empty. Buffer empty:
 * Disable the interrupt
 * Buffer not empty:
 * Read out one character from the send buffer.
 * Send this character over the TxD-Line.
 * Increment the read-pointer of the send buffer.
 * Set the read-pointer of the send buffer to 0 if it is on the end position.
 * If the read-pointer is equal to write-pointer of the send buffer, then set BufEmpty to 1. 
 *  
 * Creator:                 A. Staub
 * Date of creation:        18.12.2015
 * Last modification on:    
 * Modified by:             
 * 
 * Input:                   -
 * Output:                  -
***********************************************************************************************************************/
void __ISR(_UART_1_VECTOR, IPL2AUTO) __IntUart1Handler(void)
{
//--- Is this an RX interrupt? ---//
    if(IFS0bits.U1RXIF)
    {
        IFS0bits.U1RXIF = 0;        //clear interrupt bit
        
        //store received data byte into receive buffer
        *(g_UART1rxd.uint8_Bufptr + g_UART1rxd.uint16_Wch) = U1RXREG;
       
        //increment the write-pointer of the ring buffer
        g_UART1rxd.uint16_Wch++;
    
        //verify if write-pointer is at the end of ring buffer
        g_UART1rxd.uint16_Wch = g_UART1rxd.uint16_Wch % _TxDRxD_BUFSIZE;
    
        //receive buffer not empty
        g_UART1rxd.uint8_BufEmpty = 0;  
        
        g_LIN.uint8_SlaveReceiveCounter--;  //decrement counter
        if(g_LIN.uint8_SlaveReceiveCounter) //not all bytes received?
        {
            g_LIN.uint8_SlaveAnswerFinish = 0;      //slave still sending
        }
        else
        {
            g_LIN.uint8_SlaveAnswerFinish = 1;      //slave answer finish
        }
    }
    
//--- Is this an Tx interrupt? ---//    
    if(IFS0bits.U1TXIF)
    {
        IFS0bits.U1TXIF = 0;        //clear interrupt bit
        
        if(!g_UART1txd.uint8_BufEmpty)  //send buffer is not empty
        {
            if(g_LIN.uint8_LinBreakToSend)  //lin break to send
            {
                U1STAbits.UTXBRK = 1;       //set transmit break bit
                U1TXREG = 0;                //load register with dummy character (value is ignored)
                g_LIN.uint8_LinBreakToSend = 0; //reset the flag for the lin break to send
            }
            else    //normal characters to send
            {
                //read out one byte from the send buffer and send it directly
                U1TXREG = g_UART1txd.uint8_Buffer[g_UART1txd.uint16_Rch];
                
                //increment the read-pointer of the ring buffer
                g_UART1txd.uint16_Rch++;
                
                //verify if read-pointer is at the end of ring buffer
                g_UART1txd.uint16_Rch = g_UART1txd.uint16_Rch % _TxDRxD_BUFSIZE;
                
                //verify if read-pointer and write-pointer of the ring buffer are equal
                if(g_UART1txd.uint16_Rch == g_UART1txd.uint16_Wch)
                {
                    g_UART1txd.uint8_BufEmpty = 1;  //send buffer = empty
                }
                else
                {
                    //do nothing
                }
            }
        }
        else        //send buffer is empty
        {
            IEC0bits.U1TXIE = 0;    //disable the send interrupt
            IFS0bits.U1TXIF = 1;    //enable interrupt flag for the next time
            //enable timeout for slave answer if needed
            if(g_LIN.uint8_SlaveAnswerRequested)
            {
                SetTimer(_TIMER1,_ENABLE,0,g_Param.uint16_LinTO);
            }
            else
            {
                //do nothing
            }
        }
    }
    
//--- Is this an error request interrupt? ---//
    if(IFS0bits.U1EIF)
    {
        IFS0bits.U1EIF = 0;
        //at the moment we do nothing by an error, just clear the interrupt flag
    }
}   //end of __IntUart1Handler


/**********************************************************************************************************************
 * Routine:                 __IntADCHandler

 * Description:
 * ...
 * 
 * Creator:                 A. Staub
 * Date of creation:        29.12.2015
 * Last modification on:    
 * Modified by:             
 * 
 * Input:                   -
 * Output:                  -
***********************************************************************************************************************/
void __ISR(_ADC_VECTOR, IPL2AUTO) __IntADCHandler(void)
{ 
    IEC1bits.AD1IE = 0;     //interrupt disable
    IFS1bits.AD1IF = 0;     //clear interrupt flag 
    oTestLed2 = 0;
       
    switch(g_ADC.uint8_ChannelSelect)
    {
        case (0):   //results are for the first scan 
            g_ADC.uint16_UniIcoilA2 = ADC1BUF0;
            g_ADC.uint16_UniIcoilA1 = ADC1BUF1;
            g_ADC.uint16_UniIcoilB2 = ADC1BUF2;
            g_ADC.uint16_UniIcoilB1 = ADC1BUF3;
            g_ADC.uint16_BipIcoilB = ADC1BUF4;
            g_ADC.uint16_BipIcoilA = ADC1BUF5;           
            break;
                
        case (1):   //results are for the second scan
            g_ADC.uint16_BipVref = ADC1BUF0;
            g_ADC.uint16_Vmot = ADC1BUF1;
            g_ADC.uint16_Imot = ADC1BUF2;
            g_ADC.uint16_Battery = ADC1BUF3;        
            break;
                
        default:    //do nothing
            break;
    }  
    AD1CON1bits.ON = 0;     //disable ADC module because we will change later the configuration
                            //for the scan and supply reference
    g_ADC.uint8_ConvStarted = 0;    //signal that conversion is done
}   //end of __IntADCHandler

/**********************************************************************************************************************
 * Routine:                 __IntInputCapture1Handler

 * Description:
 * ...
 * 
 * Creator:                 J. Rebetez
 * Date of creation:        18.01.2016
 * Last modification on:    
 * Modified by:             
 * 
 * Input:                   -
 * Output:                  -
***********************************************************************************************************************/
void __ISR(_INPUT_CAPTURE_1_VECTOR, IPL2AUTO) __IntInputCapture1Handler(void)
{
    IFS0bits.IC1IF = 0;

}

/**********************************************************************************************************************
 * Routine:                 __IntInputCapture2Handler

 * Description:
 * ...
 * 
 * Creator:                 J. Rebetez
 * Date of creation:        18.01.2016
 * Last modification on:    
 * Modified by:             
 * 
 * Input:                   -
 * Output:                  -
***********************************************************************************************************************/
void __ISR(_INPUT_CAPTURE_2_VECTOR, IPL2AUTO) __IntInputCapture2Handler(void)
{
    static unsigned char nbreMeasure = 0, index = 0;
    IFS0bits.IC2IF = 0;
    
    if(IC2CONbits.ICBNE)
    {
        if(!nbreMeasure) // Measure of high state time
        {
            DataFreq.timeHigh[index] = (long)IC2BUF;
            nbreMeasure = !nbreMeasure;
            IC2CONbits.ON = 0;
            IC2CONbits.ICM = 3; // Detection of rising edges
            TMR2 = 0;
            IC2CONbits.ON = 1;
        }
        else // Measure of period time
        {
            DataFreq.timeTotal[index] = (long)IC2BUF;
            DataFreq.frequency[index] = 1 / DataFreq.timeTotal[index];
            nbreMeasure = !nbreMeasure;
            IC2CONbits.ON = 0;
            IC2CONbits.ICM = 6; // Detection of rising and falling edges. Starting with rising edge.
            TMR2 = 0;
            IC2CONbits.ON = 1;
        }
        if(index == 9)
            index = 0;
        else if(!nbreMeasure)
            index ++;
    }
    if(IC2CONbits.ICOV)
        ResetInputCaptureModule(_IC_2);
}