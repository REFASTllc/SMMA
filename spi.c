/********************************************************************************************************************/
/*  Name of the file:       spi.c                                                                                   
/*  Purpose of the file:    Definition of functions related to SPI module                                           
/*  Creator:                julien_rebetez                                                                          
/*  Date of creation:       9 septembre 2015									    
/*														    
/*  Last modification on:   -											    
/*  Modified by:            -											    
/*  Version:                -											    
/*														    
/*  List of functions:      InitSPI                                                                                 
/*                          SendDataSPI                                                                             
/* ******************************************************************************************************************/

#include "includes.h" // File which contain all includes files

T_SPI SPI1, SPI2;


/********************************************************************************************************************/
/*  Name of the function:       InitSPI										    
/*  Purpose of the function:    Initialization of SPI module							    
/*  Parameters:													    
/*      IN:                     spix (selection of the SPI module)						    
/*      OUT:                    -										    
/*														    
/*  Used global variables:      -										    
/*														    
/*  Creator:                    julien_rebetez									    
/*  Date of creation:           9 septembre 2015								    
/*														    
/*  Last modified on:           -										    
/*  Modified by:                -										    
/*  Version:                    -										    
/*														    
/*  Remark:                     -										    
/********************************************************************************************************************/
void InitSPI(unsigned char spix)
{
    if(spix == _SPI_1)
    {
        unsigned char i = 0;
        int short tempData;
    //--- Initialization of the SPI struct ---//
        for(i; i < 10; i++)
            SPI1.RxSPIbuffer[i] = 0;
        SPI1.RxIndex = 0;
        SPI1.initialized = 0;
        SPI1.OK = 1;
        SPI1.lastRxWrong = 0;
        SPI1.bufferFull = 0;
        
        oBiStrnSignal = 1;
        
        
        SPI1CON = 0;
        tempData = SPI1BUF;
        IFS0bits.SPI1EIF = 0;
        IFS0bits.SPI1RXIF = 0;
        IFS0bits.SPI1TXIF = 0;
    //--- SPI1CON ---------------------------------------------------------------------------------------------------/
        SPI1CONbits.MSTEN = 1;	    // Master Mode Enable bit
                                    // 1 = Master mode
                                    // 0 = Slave mode

        SPI1CONbits.SIDL = 1;	    // Stop in Idle Mode bit
                                    // 1 = Discontinue operation when CPU enters in Idle mode
                                    // 0 = Continue operation in Idle mode

        SPI1CONbits.DISSDO = 0;	    // Disable SDOx pin bit
                                    // 1 = SDOx pin is not used by the module (pin is controlled
                                    // by associated PORT register)
                                    // 0 = SDOx pin is controlled by the module

                                    // MODE32   MODE16  Communication
                                    // 1	    x	    32-bit
                                    // 0	    1	    16-bit
                                    // 0	    0	    8-bit
#ifdef _SPI_1_8_BITS
        SPI1CONbits.MODE16 = 0;
        SPI1CONbits.MODE32 = 0;
#endif
#ifdef _SPI_1_16_BITS
        SPI1CONbits.MODE16 = 1;
        SPI1CONbits.MODE32 = 0;
#endif
#ifdef _SPI_1_32_BITS
        SPI1CONbits.MODE16 = 0;
        SPI1CONbits.MODE32 = 1;	 
#endif

        SPI1CONbits.SMP = 0;	    // SPI Data Input Sample Phase bit
                                    // Master mode (MSTEN = 1):
                                    // 1 = Input data sampled at end of data output time
                                    // 0 = Input data sampled at middle of data output time
                                    // Slave mode (MSTEN = 0):
                                    // SMP value is ignored when SPI is used in Slave mode.
                                    // The module always uses SMP = 0.

        SPI1CONbits.CKP = 1;	    // Clock Polarity Select bit
                                    // 1 = Idle state for clock is a high level; active state is a low level
                                    // 0 = Idle state for clock is a low level; active state is a high level
        
        SPI1CONbits.CKE = 0;        // SPI Clock Edge Select bit
                                    // 1 = Serial output data changes on transition from active clock state
                                    //	to idle clock state (see CKP bit)
                                    // 0 = Serial output data changes on transition from idle clock state
                                    //	to active clock state (see CKP bit)
                                    // The CKE bit is not used in the Framed SPI mode. The user should program
                                    // this bit to '0' for the Framed SPI mode (FRMEN = 1).

        SPI1CONbits.SSEN = 0;	    // Slave Select Enable (Slave mode) bit
                                    // 1 = /SSx pin used for Slave mode
                                    // 0 = /SSx pin not used for Slave mode, pin controlled by port function.
    //--- SPI1STAT --------------------------------------------------------------------------------------------------/
    // All bits in this register are read-only.
        SPI1STATbits.SPIROV = 0;
    //--- SPI1BUF ---------------------------------------------------------------------------------------------------/
        SPI1BUF = 0;
    //--- SPI1BRG ---------------------------------------------------------------------------------------------------/
        SPI1BRG = 79;
    //--- Signal that SPI 1 is initialized ---//
        SPI1.initialized = 1;
    }
    else if(spix == _SPI_2)
    {
    //--- SPI2CON ---------------------------------------------------------------------------------------------------/
	SPI2CONbits.FRMEN = 0;	    // Framed SPI Support bit
				    // 1 = Framed SPI support is enabled (/SSx pin used as FSYNC input/output)
				    // 0 = Framed SPI support is disabled

        SPI2CONbits.FRMSYNC = 0;    // Frame Sync Pulse Direction Control on /SSx pin bit (Framed SPI mode only)
				    // 1 = Frame sync pulse input (Slave mode)
				    // 0 = Frame sync pulse output (Master mode)

        SPI2CONbits.FRMPOL = 0;	    // Frame Sync Polarity bit (Framed SPI mode only)
				    // 1 = Frame pulse is active-high
				    // 0 = Frame pulse is active-low
        
        SPI2CONbits.SPIFE = 0;	    // Frame Sync Pulse Edge Select bit (Framed SPI mode only)
				    // 1 = Frame synchronization pulse coincides with the first bit clock
				    // 0 = Frame synchronization pulse precedes the first bit clock
        
        SPI2CONbits.ON = 0;	    // SPI Peripheral On bit
				    // 1 = SPI Peripheral is enabled
				    // 0 = SPI Peripheral is disabled
				    // When ON = 1, DISSDO and DISSDI are the only other bits that
				    // can be modified. When using the 1:1 PBCLK
				    // divisor, the user?s software should not read or write the
				    // peripheral?s SFRs in the SYSCLK cycle immediately
				    // following the instruction that clears the module?s ON bit.

        SPI2CONbits.SIDL = 0;	    // Stop in Idle Mode bit
				    // 1 = Discontinue operation when CPU enters in Idle mode
				    // 0 = Continue operation in Idle mode

        SPI2CONbits.DISSDO = 0;	    // Disable SDOx pin bit
				    // 1 = SDOx pin is not used by the module (pin is controlled
				    // by associated PORT register)
				    // 0 = SDOx pin is controlled by the module
        
                    // MODE32   MODE16  Communication
				    // 1	    x	    32-bit
				    // 0	    1	    16-bit
				    // 0	    0	    8-bit
#ifdef _SPI_2_8_BITS
        SPI2CONbits.MODE16 = 0;
        SPI2CONbits.MODE32 = 0;
#endif
#ifdef _SPI_2_16_BITS
        SPI2CONbits.MODE16 = 1;
        SPI2CONbits.MODE32 = 0;
#endif
#ifdef _SPI_2_32_BITS
        SPI2CONbits.MODE16 = 0;
        SPI2CONbits.MODE32 = 1;	 
#endif
        
        SPI2CONbits.SMP = 1;	    // SPI Data Input Sample Phase bit
				    // Master mode (MSTEN = 1):
				    // 1 = Input data sampled at end of data output time
				    // 0 = Input data sampled at middle of data output time
				    // Slave mode (MSTEN = 0):
				    // SMP value is ignored when SPI is used in Slave mode.
				    // The module always uses SMP = 0.

        SPI2CONbits.CKE = 1;        // SPI Clock Edge Select bit
				    // 1 = Serial output data changes on transition from active clock state
				    //	to idle clock state (see CKP bit)
				    // 0 = Serial output data changes on transition from idle clock state
				    //	to active clock state (see CKP bit)
                                    // The CKE bit is not used in the Framed SPI mode. The user should program
				    // this bit to ?0? for the Framed SPI mode (FRMEN = 1).

        SPI2CONbits.SSEN = 1;	    // Slave Select Enable (Slave mode) bit
                                    // 1 = /SSx pin used for Slave mode
                                    // 0 = /SSx pin not used for Slave mode, pin controlled by port function.

        SPI2CONbits.CKP = 0;	    // Clock Polarity Select bit
                                    // 1 = Idle state for clock is a high level; active state is a low level
                                    // 0 = Idle state for clock is a low level; active state is a high level

        SPI2CONbits.MSTEN = 1;	    // Master Mode Enable bit
                                    // 1 = Master mode
                                    // 0 = Slave mode
    //--- SPI2STAT --------------------------------------------------------------------------------------------------/
    // All bits in this register are read-only.
    //--- SPI2BUF ---------------------------------------------------------------------------------------------------/
        SPI2BUF = 0;
    //--- SPI2BRG ---------------------------------------------------------------------------------------------------/
        SPI2BRG = 79;
    }
}
//--- S P I   M a s t e r   M o d e   C l o c k   F r e q u e n c y -------------------------------------------------/
//														    -/
//		Fsck = Fpb / 2 * (SPIxBRG + 1)									    -/
//		SPIxBRG = Fpb / (2 * Fsck) - 1									    -/
//-------------------------------------------------------------------------------------------------------------------/

/********************************************************************************************************************/
/*  Name of the function:       InitSPIInterrupt								    
/*  Purpose of the function:    Initialization of the interrupts of the SPI module				    
/*  Parameters:													    
/*      IN:                     spix (choose of the SPI module)							    
/*				action (enable or disable)                                                          
/*				See .h file for all definitions							    
/*      OUT:                    -										    
/*														    
/*  Used global variables:      -										    
/*														    
/*  Creator:                    julien_rebetez									    
/*  Date of creation:           12 septembre 2015								    
/*														    
/*  Last modified on:           -										    
/*  Modified by:                -										    
/*  Version:                    -										    
/*														    
/*  Remark:                     -										    
/********************************************************************************************************************/
void InitSPIInterrupt(unsigned char spix, unsigned char action)
{
    if(spix == _SPI_1)
    {
        if(action == _ENABLE)
        {
        //--- Error interrupt ---//
            IFS0bits.SPI1EIF = 0;
            IPC5bits.SPI1IP = 4;
            IPC5bits.SPI1IS = 3;
            IEC0bits.SPI1EIE = 1;
        //--- Receive data interrupt ---//
            IFS0bits.SPI1RXIF = 0;
            IEC0bits.SPI1RXIE = 1;
        //--- Transmit buffer empty interrupt ---//
            IFS0bits.SPI1TXIF = 0;
            IEC0bits.SPI1TXIE = 0;
        }
        else
        {
        //--- Error interrupt ---//
            IFS0bits.SPI1EIF = 0;
            IEC0bits.SPI1EIE = 0;
        //--- Receive data interrupt ---//
            IFS0bits.SPI1RXIF = 0;
            IEC0bits.SPI1RXIE = 0;
        //--- Transmit buffer empty interrupt ---//
            IFS0bits.SPI1TXIF = 0;
            IEC0bits.SPI1TXIE = 0;
        }
    }
    if(spix == _SPI_2)
    {
        if(action == _ENABLE)
        {
        //--- Error interrupt ---//
            IFS1bits.SPI2EIF = 0;
            IPC7bits.SPI2IP = 7;
            IPC7bits.SPI2IS = 3;
            IEC1bits.SPI2EIE = 1;
        //--- Receive data interrupt ---//
            IFS1bits.SPI2RXIF = 0;
            IEC1bits.SPI2RXIE = 1;
        //--- Transmit buffer empty interrupt ---//
            IFS1bits.SPI2TXIF = 0;
            IEC1bits.SPI2TXIE = 1;
        }
        else
        {
        //--- Error interrupt ---//
            IFS1bits.SPI2EIF = 0;
            IEC1bits.SPI2EIE = 0;
        //--- Receive data interrupt ---//
            IFS1bits.SPI2RXIF = 0;
            IEC1bits.SPI2RXIE = 0;
        //--- Transmit buffer empty interrupt ---//
            IFS1bits.SPI2TXIF = 0;
            IEC1bits.SPI2TXIE = 0;
        }
    }
}

/********************************************************************************************************************/
/*  Name of the function:       SendDataSPI1									   
/*  Purpose of the function:    Send a register over SPI 1							    
/*  Parameters:													    
/*      IN:                     dataToSend (register to send 8/16/32 bits (see bits MODE16/MODE32 of reg SPIxCON)   
/*                              See .h file for all definitions							    
/*      OUT:                    -										    
/*														    
/*  Used global variables:      -										    
/*														    
/*  Creator:                    julien_rebetez									    
/*  Date of creation:           14 septembre 2015								    
/*														    
/*  Last modified on:           -										    
/*  Modified by:                -										    
/*  Version:                    -										   
/*														    
/*  Remark:                     -										    
/********************************************************************************************************************/
signed char SendOneDataSPI1(unsigned short int dataToSend)
{
    unsigned short int i;
    
    if(!SPI1STATbits.SPIBUSY)
    {
        oBiStrnSignal = 0;
        for(i = 0; i < 1000; i++)
            Nop();
        SPI1BUF = dataToSend;
        while(SPI1STATbits.SPIBUSY);
        for(i = 0; i < 1000; i++)
            Nop();
        oBiStrnSignal = 1;
        return 1;
    }
    else
        return -1;
}

/********************************************************************************************************************/
/*  Name of the function:       SendDataSPI2									   
/*  Purpose of the function:    Send a register over SPI 2							    
/*  Parameters:													    
/*      IN:                     dataToSend (register to send 8/16/32 bits (see bits MODE16/MODE32 of reg SPIxCON)   
/*                              See .h file for all definitions							    
/*      OUT:                    -										    
/*														    
/*  Used global variables:      -										    
/*														    
/*  Creator:                    julien_rebetez									    
/*  Date of creation:           14 septembre 2015								    
/*														    
/*  Last modified on:           -										    
/*  Modified by:                -										    
/*  Version:                    -										   
/*														    
/*  Remark:                     -										    
/********************************************************************************************************************/
void SendOneDataSPI2(unsigned short int dataToSend)
{
//--- Waiting that the SPI peripheral is not busy ---//
	while(SPI2STATbits.SPIBUSY);
	SPI2BUF = dataToSend;
}

/********************************************************************************************************************/
/*  Name of the function:       GetLastDataSPI1									   
/*  Purpose of the function:    Get the last data received on SPI 1							    
/*  Parameters:													    
/*      IN:                     -   						    
/*      OUT:                    data (char, int or long)
/*                              See .h file for all definitions										    
/*														    
/*  Used global variables:      -										    
/*														    
/*  Creator:                    julien_rebetez									    
/*  Date of creation:           09.12.2015								    
/*														    
/*  Last modified on:           -										    
/*  Modified by:                -										    
/*  Version:                    -										   
/*														    
/*  Remark:                     -										    
/********************************************************************************************************************/
#ifdef _SPI_1_32_BITS
unsigned long GetLastDataSPI1(void)
#endif
#ifdef _SPI_1_16_BITS
unsigned short int GetLastDataSPI1(void)
#endif
#ifdef _SPI_1_8_BITS
unsigned char GetLastDataSPI1(void)
#endif
{
    return SPI1.RxSPIbuffer[SPI1.RxIndex - 1];
}