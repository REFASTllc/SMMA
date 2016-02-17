/********************************************************************************************************************/
/*  Name of the file:       productionInfo.c								    
/*  Purpose of the file:    Definitions of all functions related to production information					    
/*  Creator:                julien_rebetez									    
/*  Date of creation:       February 17, 2016									    
/*														    
/*  Last modification on:   -											    
/*  Modified by:            -											    
/*  Version:                -											    
/*														    
/*  List of functions:      -											    
/* ******************************************************************************************************************/

#include "includes.h" // File which contain all includes files

S_PROD prod, newProdValues;
/********************************************************************************************************************/
/*  Name of the function:       ReadProdInfoFromEEPROM									    
/*  Purpose of the function:    Readout of all production info from external EEPROM	into working memory (RAM)							    
/*  Parameters:													    
/*      IN:                     -										    
/*      OUT:                    -										    
/*														    
/*  Used global variables:      -										    
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
void ReadProdInfoFromEEPROM(void)
{
    unsigned char i = 0;
    // Readout of the serial number
    prod.serialNumber = 0;
    for(i=4;i>0;i--)
    {
        prod.serialNumber <<= 8;
        prod.serialNumber |= ROM24LC256_RdByte(EE_ADDR_H, EE_ADDR_L_SERIAL_NUMBER + i - 1, 1);
    }
    // Readout of the production year
    prod.Year = ROM24LC256_RdByte(EE_ADDR_H, EE_ADDR_L_PROD_YEAR, 1);
    // Readout of the production week
    prod.Week = ROM24LC256_RdByte(EE_ADDR_H, EE_ADDR_L_PROD_WEEK, 1);
    // Readout of the test status
    prod.testStatus = ROM24LC256_RdByte(EE_ADDR_H, EE_ADDR_L_TEST_STATUS, 1);
    // Readout of the FW version
/*    for(i = 0;i<3;i++)
    {
        prod.fwVersion[i] = ROM24LC256_RdByte(EE_ADDR_H, EE_ADDR_L_FW_VERSION + i, 1);
    }*/
    prod.fwVersion[0] = ROM24LC256_RdByte(EE_ADDR_H, EE_ADDR_L_FW_VERSION, 1);
    prod.fwVersion[1] = ROM24LC256_RdByte(EE_ADDR_H, EE_ADDR_L_FW_VERSION + 1, 1);
    prod.fwVersion[2] = ROM24LC256_RdByte(EE_ADDR_H, EE_ADDR_L_FW_VERSION + 2, 1);
    Nop();
    // Formatting of the FW version xyz ==> x.y.z
    prod.fwVersion[5] = '\0';
    prod.fwVersion[4] = prod.fwVersion[2] + 0x30;
    prod.fwVersion[2] = prod.fwVersion[1] + 0x30;
    prod.fwVersion[3] = prod.fwVersion[1] = '.';
    // Readout of the HW version
    prod.hwVersion = ROM24LC256_RdByte(EE_ADDR_H, EE_ADDR_L_HW_VERSION, 1);
}