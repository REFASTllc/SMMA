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
 *                          - __IntTimer2Handler
 *                          - __IntUart2Handler
***********************************************************************************************************************/


#include "includes.h" // File which contain all includes files


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
 * Input:                   timerx  (selected timer)
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
        //IEC1bits.U2TXIE = 0;
        
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
    
//--- Is this an ??? interrupt? ---//
    if(IFS1bits.U2EIF)
    {
        IFS1bits.U2EIF = 0;
    }
}

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
/****************************************************************************/
/*  Purpose of the INT routine:  Manage RTCC alarm interrupts               */
/****************************************************************************/
void __ISR(_RTCC_VECTOR, IPL3AUTO) __IntRTCCHandler(void)
{
    IFS1bits.RTCCIF = 0;
}

/****************************************************************************/
/*  Purpose of the INT routine:  Manage TIMER1 overflow interrupts          */
/****************************************************************************/
void __ISR(_TIMER_1_VECTOR, IPL1AUTO) __IntTimer1Handler(void)
{
    TMR1 = 0;
    IFS0bits.T1IF = 0;
}


/**********************************************************************************************************************
 * Routine:                 __IntTimer2Handler

 * Description:
 * First reset the interrupt flag doing this in two steps, first read it and then set it to 0. 
 * Then switch off the clock source to update the modulo value and reset the counter. The switch-case verifies the 
 * value of the counter - if this is:
 * 0:
 * Then the waiting time is expired and the new value must be loaded. It is a must have to verify if the counter has 
 * the value 1, because this means that the last time can be used directly. So the needed time is under the defined 
 * interrupt time. Otherwise we have to load the defined interrupt time as next. 
 * 1:
 * This is the case to load the last waiting time. 
 * default (2-65535):
 * If we have not to use the last time, then this case will be executed and the time will be the defined 
 * interrupt time.
 * 
 * At the end we decrement the counter by 1 and switch on again the clock source. The idea to have different modulo 
 * value is that this interrupt will be not called each 50us or each 10us, etc. The idea was to call this interrupt 
 * as less as possible. And this time is fixed with the variable g_Timer2.uint16_IntTime in 100ns steps.
 * With this solution we execute the interrupt not so much and in the same time we can use the resolution of 100ns.
 * 
 * Creator:                 J. Rebetez
 * Date of creation:        08.08.2015
 * Last modification on:    01.10.2015
 * Modified by:             A. Staub
 * 
 * Input:                   -
 * Output:                  -
***********************************************************************************************************************/
void __ISR(_TIMER_2_VECTOR, IPL1AUTO) __IntTimer2Handler(void)
{
    auto unsigned char uint8_WB1;       //local variabel 'WorkByte1'
    
    T2CONbits.ON = 0;           //disable interrupt module
    IFS0bits.T2IF = 0;          //clear interrupt flag
    TMR2 = 0;                   //reset counter 
    
    switch (g_Timer2.uint16_Count)      //verify the value of the counter
    {
        case (0): //time expired 
            //load the new time for the next step - how many times to wait the interrupt time
            g_Timer2.uint16_Count = g_Uni.uint16_Count;
      
            //load the last time for the next step
            g_Timer2.uint16_LastTime = g_Uni.uint16_LastTime;
       
            //verify if the counter is 1, because in this case the last time must be directly used 
            if(g_Timer2.uint16_Count == 1)
            {
                //then load PR2 with the last time
                PR2 = g_Timer2.uint16_LastTime;
            }
            else
            {
                //otherwise load it with the interrupt time
                PR2 = g_Timer2.uint16_IntTime;
            } 
        
            //allow next step
            g_Uni.uint8_Status |= 0x10;
            
            //for security - be sure that the motor is in run
            if(g_Uni.uint8_Settings & 0x01)
            {
                //then verify if it is the first or last step or motor error = true or real = goal position
                if((g_Uni.uint8_Status & 0x01) || (g_Uni.uint8_Status & 0x02) ||
                (g_Uni.uint8_Status & 0x80) || (g_Uni.uint32_RealPos == g_Uni.uint32_GoalPos))
                {
                    //then do nothing
                }
                else
                {
                    //otherwise do the follow
                    g_Uni.uint32_RealPos++;     //increment real position with 1      
          
                    //verify the direction
                    if(g_Uni.uint8_Settings & 0x10)
                    {
                        //direction = CW
                        g_Uni.uint8_FsHsCount++;    //increment counter
                    }
                    else
                    {
                        //direction = CCW
                        g_Uni.uint8_FsHsCount--;    //decrement counter
                    }
          
                    //verify step mode if full or half
                    if(g_Uni.uint8_Settings & 0x04)
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
                //otherwise load the modulo timer with the maximum value possible for
                //that the interrupt does not execute to much times
                PR2 = 0xFF;
            }
            break;
        
        case (1):
            PR2 = g_Timer2.uint16_LastTime; //load timer with last waiting time
            break; 
      
        default:
            PR2 = g_Timer2.uint16_IntTime;  //load timer with defined time
            break;
    } 
    
    g_Timer2.uint16_Count--;    //decrement the counter by 1 
    T2CONbits.ON = 1;           //enable interrupt module    
}   //end of __IntTimer2Handler


/****************************************************************************/
/*  Purpose of the INT routine:  Manage TIMER3 overflow interrupts          */
/****************************************************************************/
void __ISR(_TIMER_3_VECTOR, IPL1AUTO) __IntTimer3Handler(void)
{
    TMR3 = 0;
    IFS0bits.T3IF = 0;
}

/****************************************************************************/
/*  Purpose of the INT routine:  Manage PWM1 overflow interrupts            */
/****************************************************************************/
void __ISR(_OUTPUT_COMPARE_1_VECTOR, IPL3AUTO) __IntPWM1Handler(void)
{
    IFS0bits.OC1IF = 0;
}
