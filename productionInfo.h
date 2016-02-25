/********************************************************************************************************************/
/*  Name of the file:       productionInfo.h                                                              
/*  Purpose of the file:    Headers of all functions related to ...                                         
/*  Creator:                julien_rebetez                                                                      
/*  Date of creation:       February 17, 2016                                                                       
/*                                                                                                                  
/*  Last modification on:   -                                                                                       
/*  Modified by:            -                                                                                       
/*  Version:                -                                                                                       
/*                                                                                                                  
/*  List of functions:      -                                                                                       
/* ******************************************************************************************************************/

#ifndef PRODUCTIONINFO_H
#define	PRODUCTIONINFO_H

typedef struct
{
    long serialNumber;
    unsigned char Year;
    unsigned char Week;
    signed char testStatus;
    unsigned char fwVersion[6];
    unsigned char hwVersion;
} S_PROD;

#define EE_ADDR_H                   0x00
#define EE_ADDR_L_SERIAL_NUMBER1    0x00
#define EE_ADDR_L_SERIAL_NUMBER2    0x01
#define EE_ADDR_L_SERIAL_NUMBER3    0x02
#define EE_ADDR_L_SERIAL_NUMBER4    0x03
#define EE_ADDR_L_PROD_YEAR         0x04
#define EE_ADDR_L_PROD_WEEK         0x05
#define EE_ADDR_L_TEST_STATUS       0x06
#define EE_ADDR_L_FW_VERSION        0x07
#define EE_ADDR_L_HW_VERSION        0x0A




/********************************************************************************************************************/
/*  Purpose of the function:    Header of the function ReadProdInfoFromEEPROM                                                   
/*  Parameters:                                                                                                     
/*      IN:                     -                                                                                   
/*      OUT:                    -                                                                                   
/*                                                                                                                  
/*  Used global variables:      S_PROD prod                                                                                  
/*                                                                                                                  
/*  Creator:                    julien_rebetez                                                                  
/*  Date of creation:           February 17, 2016                                                                   
/*                                                                                                                  
/*  Last modified on:           -                                                                                   
/*  Modified by:                -                                                                                   
/*  Version:                    -                                                                                   
/*                                                                                                                  
/*  Remark:                     -                                                                                   
/********************************************************************************************************************/
void ReadProdInfoFromEEPROM(void);

#endif	/* PRODUCTIONINFO_H */
