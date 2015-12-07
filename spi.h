/********************************************************************************************************************/
/*  Name of the file:       spi.h                                                                                   */
/*  Purpose of the file:    Header of functions related to the SPI module                                           */
/*  Creator:                julien_rebetez                                                                          */
/*  Date of creation:       9 septembre 2015, 16:50                                                                 */
/*                                                                                                                  */
/*  Last modification on:   -                                                                                       */
/*  Modified by:            -                                                                                       */
/*  Version:                -                                                                                       */
/*                                                                                                                  */
/*  List of functions:      InitSPI                                                                                 */
/*                          SendDataSPI                                                                             */
/* ******************************************************************************************************************/

#ifndef SPI_H
#define	SPI_H

#define _SPI_1      1
#define _SPI_2      2
#define _SPI_3      3
#define _SPI_4      4

#define _ENABLE     1
#define _DISABLE    0

#define EnableSPI1()   SPI1CONbits.ON = 1;
#define DisableSPI1()  SPI1CONbits.ON = 0;

/*
typedef struct
{
    unsigned char SPI_OK:1;
    unsigned char lastRxWrong:1;
    unsigned char NotUsed:5;
} SPIStatus;
*/
typedef struct
{
    volatile unsigned int RxSPIbuffer[10];
    volatile unsigned char RxIndex;
    unsigned char OK:1;
    unsigned char lastRxWrong:1;
    unsigned char initialized:1;
    unsigned char bufferFull:1;
} T_SPI;

/********************************************************************************************************************/
/*  Purpose of the function:    Header of the function InitSPI                                                      */
/*  Parameters:                                                                                                     */
/*      IN:                     spix (selection of the SPI module)						    */
/*      OUT:                    -                                                                                   */
/*                                                                                                                  */
/*  Used global variables:      -                                                                                   */
/*                                                                                                                  */
/*  Creator:                    julien_rebetez                                                                      */
/*  Date of creation:           9 septembre 2015                                                                    */
/*                                                                                                                  */
/*  Last modified on:           -                                                                                   */
/*  Modified by:                -                                                                                   */
/*  Version:                    -                                                                                   */
/*                                                                                                                  */
/*  Remark:                     -                                                                                   */
/********************************************************************************************************************/
void InitSPI(unsigned char spix);

/********************************************************************************************************************/
/*  Purpose of the function:    Header of the function InitSPIInterrupt                                             */
/*  Parameters:                                                                                                     */
/*      IN:                     spix (choose of the SPI module)							    */
/*				action (enable or disable)                                                          */
/*				See .h file for all definitions							    */
/*      OUT:                    -                                                                                   */
/*                                                                                                                  */
/*  Used global variables:      -                                                                                   */
/*                                                                                                                  */
/*  Creator:                    julien_rebetez                                                                      */
/*  Date of creation:           12 septembre 2015								    */
/*                                                                                                                  */
/*  Last modified on:           -                                                                                   */
/*  Modified by:                -                                                                                   */
/*  Version:                    -                                                                                   */
/*                                                                                                                  */
/*  Remark:                     -                                                                                   */
/********************************************************************************************************************/
void InitSPIInterrupt(unsigned char spix, unsigned char action);

/********************************************************************************************************************/
/*  Purpose of the function:    Header of the function InitSendDataSPIx                                              
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
signed char SendOneDataSPI1(unsigned int dataToSend);
void SendOneDataSPI2(unsigned int dataToSend);

#endif	/* SPI_H */
