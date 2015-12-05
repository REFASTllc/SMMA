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
***********************************************************************************************************************/


#include "includes.h"

Si2c g_i2c;     //global variables for struct 


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