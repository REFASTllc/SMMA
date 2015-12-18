/**********************************************************************************************************************

                                            UBD - Unipolar Bipolar Driver

***********************************************************************************************************************
 * File name:               cmdchk.c
 * Creation date:           05.12.2015
 * Main creator:            Alain Staub
 * Company:                 REFAST GmbH
 *                          Copyright (c) 2015 REFAST GmbH
***********************************************************************************************************************
 * Content overview:        - cmdchk_init
 *                          - cmdchk_check
 *                          - cmdchk_def
***********************************************************************************************************************/


#include "includes.h"

SCmdChk g_CmdChk;           //global variables for struct  
extern SFunct g_Funct;
extern SParam g_Param;
extern SUART2rxd g_UART2rxd;
/**********************************************************************************************************************
 * Routine:                 cmdchk_init

 * Description:
 * This routine initialize the basic things.
 * 
 * Creator:                 A. Staub
 * Date of creation:        05.12.2015
 * Last modification on:    -
 * Modified by:             - 
 * 
 * Input:                   -
 * Output:                  -
***********************************************************************************************************************/
void cmdchk_init(void)
{
//initialize the command variables    
  g_CmdChk.uint8_ParamPos = 0;      //clear the parameter position
  g_CmdChk.uint8_LetterPos = 0;     //clear the letter position
  g_CmdChk.uint32_Cmd1st4 = 0;      //clear the first 4 letters of the command
  g_CmdChk.uint32_Cmd2nd4 = 0;      //clear the second 4 letters of the command
  g_CmdChk.uint8_CmdID = 0;         //clear command id
}   //end of cmdchk_init


/**********************************************************************************************************************
 * Routine:                 cmdchk_check

 * Description:
 * ...
 * 
 * Creator:                 A. Staub
 * Date of creation:        05.12.2015
 * Last modification on:    -
 * Modified by:             - 
 * 
 * Input:                   -
 * Output:                  -
***********************************************************************************************************************/
void cmdchk_check(void)
{
    auto unsigned char uint8_RxDBuffChar;       //local byte for the actually received character
    
    uint8_RxDBuffChar = uart2_receivebuffer();  //read out one byte from the receive buffer
  
    //verify the position of the parameter (is it the first, second, etc.)
    switch(g_CmdChk.uint8_ParamPos) 
    {
        case (0):   //1. parameter       
            //character = carriage return or a comma?
            if((uint8_RxDBuffChar == 13) || (uint8_RxDBuffChar == 44))
            {
                cmdchk_def();   //stop here and verify the command
                
                //store the command ID into the temporary register at the place "ParamPos"
                g_CmdChk.uint32_TempPara[g_CmdChk.uint8_ParamPos] = g_CmdChk.uint8_CmdID;
             
                //command not defined?
                if((g_CmdChk.uint8_CmdID == _IdNotAllow) && (g_Param.uint8_ErrCode == _UnknownCmd))
                {
                    uart2_SendErrorCode(g_Param.uint8_ErrCode); //then send back the error code
          
                    //reset the variables
                    g_CmdChk.uint8_ParamPos = 0;                  
                    g_CmdChk.uint32_Cmd1st4 = 0; 
                    g_CmdChk.uint32_Cmd2nd4 = 0;              
          
                    //clear the receive buffer
                    g_UART2rxd.uint16_Rch = g_UART2rxd.uint16_Wch;          
                    g_UART2rxd.uint8_BufEmpty = 1;                  
                }
                else
                {
                    g_CmdChk.uint8_ParamPos++; //increment the parameter position
                }
        
                g_CmdChk.uint8_LetterPos = 0;  //clear the letter position for the next command                 
            }
            else
            {
                switch(g_CmdChk.uint8_LetterPos)   //verify the position of the letter
                {
                    case (0): //add 1. letter
                        g_CmdChk.uint32_Cmd1st4 = uint8_RxDBuffChar;              
                        break;
          
                    case (1): //add 2. letter
                        g_CmdChk.uint32_Cmd1st4 = g_CmdChk.uint32_Cmd1st4 << 8;                          
                        g_CmdChk.uint32_Cmd1st4 = g_CmdChk.uint32_Cmd1st4 | uint8_RxDBuffChar;
                        break;
          
                    case (2): //add 3. letter
                        g_CmdChk.uint32_Cmd1st4 = g_CmdChk.uint32_Cmd1st4 << 8;                          
                        g_CmdChk.uint32_Cmd1st4 = g_CmdChk.uint32_Cmd1st4 | uint8_RxDBuffChar; 
                        break;
          
                    case (3): //add 4. letter
                        g_CmdChk.uint32_Cmd1st4 = g_CmdChk.uint32_Cmd1st4 << 8;                          
                        g_CmdChk.uint32_Cmd1st4 = g_CmdChk.uint32_Cmd1st4 | uint8_RxDBuffChar;
                        break;
          
                    case (4): //add 5. letter
                        g_CmdChk.uint32_Cmd2nd4 = uint8_RxDBuffChar;
                        break;
          
                    case (5): //add 6. letter
                        g_CmdChk.uint32_Cmd2nd4 = g_CmdChk.uint32_Cmd2nd4 << 8;                          
                        g_CmdChk.uint32_Cmd2nd4 = g_CmdChk.uint32_Cmd2nd4 | uint8_RxDBuffChar;
                        break;
          
                    case (6): //add 7. letter
                        g_CmdChk.uint32_Cmd2nd4 = g_CmdChk.uint32_Cmd2nd4 << 8;                          
                        g_CmdChk.uint32_Cmd2nd4 = g_CmdChk.uint32_Cmd2nd4 | uint8_RxDBuffChar;
                        break;
          
                    case (7): //add 8. letter
                        g_CmdChk.uint32_Cmd2nd4 = g_CmdChk.uint32_Cmd2nd4 << 8;                          
                        g_CmdChk.uint32_Cmd2nd4 = g_CmdChk.uint32_Cmd2nd4 | uint8_RxDBuffChar;
                        break;
          
                    default:  //error - command is longer than defined 
                        g_CmdChk.uint8_CmdID = _IdNotAllow;        //clear the command ID
                        g_Param.uint8_ErrCode = _UnknownCmd;    //set error code
            
                        //then send back the error code
                        uart2_SendErrorCode(g_Param.uint8_ErrCode); //then send back the error code
          
                        //reset the variables
                        g_CmdChk.uint8_ParamPos = 0;                 
                        g_CmdChk.uint8_LetterPos = 0;
                        g_CmdChk.uint32_Cmd1st4 = 0; 
                        g_CmdChk.uint32_Cmd2nd4 = 0;                
          
                        //clear the receive buffer
                        g_UART2rxd.uint16_Rch = g_UART2rxd.uint16_Wch;          
                        g_UART2rxd.uint8_BufEmpty = 1;        
                        break;
                }
                g_CmdChk.uint8_LetterPos++;    //increment letter position
            }    
            break;    
    
        default:  //for all other parameters
            //store the actually letter position into the ASCII array position
            g_Funct.uint8_ArrAsciiPos = g_CmdChk.uint8_LetterPos;
        
            //character = carriage return or a comma?
            if((uint8_RxDBuffChar == 13) || (uint8_RxDBuffChar == 44))
            {
                //decrement the array position to be on the first character
                g_Funct.uint8_ArrAsciiPos--; 
        
                //only one character received?
                if(g_Funct.uint8_ArrAsciiPos == 0)
                {
                    //then verify if it is a number 0-9
                    if((g_Funct.uint8_ArrAscii[0] >= '0') && (g_Funct.uint8_ArrAscii[0] <= '9'))
                    {
                        //start the conversion
                         g_CmdChk.uint32_TempPara[g_CmdChk.uint8_ParamPos] = funct_AsciiToInt();
                    }
                    else
                    {
                        //otherwise store the character without conversion
                        g_CmdChk.uint32_TempPara[g_CmdChk.uint8_ParamPos] = g_Funct.uint8_ArrAscii[0];
                    }
                }
                else
                {
                    //otherwise verify if it was a comma (in this case ArrAsciiPos is 255)
                    if(g_Funct.uint8_ArrAsciiPos == 255)
                    {
                        //in this case set all bits 
                        g_CmdChk.uint32_TempPara[g_CmdChk.uint8_ParamPos] = 0xFFFFFFFF;
                    }
                    else
                    {
                        //otherwise start the conversion
                        g_CmdChk.uint32_TempPara[g_CmdChk.uint8_ParamPos] = funct_AsciiToInt();
                    }
                }
        
                g_CmdChk.uint8_ParamPos++;     //increment the parameter position
                g_CmdChk.uint8_LetterPos = 0;  //clear the letter position for the next command                 
            }
            else
            {
                //store the actually letter position into the ASCII array position
                //g_Funct.uint8_ArrAsciiPos = g_Cmd.uint8_LetterPos;
                //store the receive character into the ASCII array
                g_Funct.uint8_ArrAscii[g_Funct.uint8_ArrAsciiPos] = uint8_RxDBuffChar; 
                g_CmdChk.uint8_LetterPos++;    //increment letter position
            }
            break;
    }
  
    //character in the buffer was CR and the position of the parameter is not 0
    if((uint8_RxDBuffChar == 13) && (g_CmdChk.uint8_ParamPos))
    {
        if(g_Param.uint8_NotPOR)    //device did not see a POR? --> this is new
        {
            switch(g_CmdChk.uint8_CmdID)   //execute the needed commando
            {           
                case (_IdNotAllow):     //command ID is not allowed
                    //do nothing - error has to be send before and uint8_CmdID 
                    //has to be set to 0 before this routine for that the program execute this case.
                    break;
            
                case (_IdSILIM):        //command SILIM
                    cmd_SILIM();        //call subroutine
                    break;
            
                case (_IdETESTIN):      //command ETESTIN
                    cmd_ETESTIN();      //call subroutine
                    break;
            
                case (_IdSRACC):        //command SRACC
                    cmd_SRACC();        //call subroutine
                    break;
            
                case (_IdSRDEC):        //command SRDEC
                    cmd_SRDEC();        //call subroutine
                    break;
                
                case (_IdRUN):          //command RUN
                    cmd_RUN();          //call subroutine
                    break;
                
                case (_IdSPHC):         //command SPHC
                    cmd_SPHC();         //call subroutine
                    break;
                
                case (_IdGVER):         //command GVER
                    cmd_GVER();         //call subroutine
                    break;
            
                case (_IdRAZ):          //command RAZ
                    cmd_RAZ();          //call subroutine
                    break;
            
                case (_IdSMTYP):        //command SMTYP
                    cmd_SMTYP();        //call subroutine
                    break;
            
                case (_IdGMTYP):        //command GMTYP
                    cmd_GMTYP();        //call subroutine
                    break;
                 
                case (_IdSTEST):        //command STEST
                    cmd_STEST();        //call subroutine
                    break;
                    
                case (_IdGILIM):        //command GILIM
                    cmd_GILIM();        //call subroutine
                    break;
                
                case (_IdGPHC):         //command GPHC
                    cmd_GPHC();         //call subroutine
                    break;
                
                case (_IdSIRUN):        //command SIRUN (not used anymore)
                    g_Param.uint8_ErrCode = _UnknownCmd;        //set error code
                    uart2_SendErrorCode(g_Param.uint8_ErrCode); //call subroutine
                    break;
        
                case (_IdGIRUN):        //command GIRUN
                    g_Param.uint8_ErrCode = _UnknownCmd;        //set error code
                    uart2_SendErrorCode(g_Param.uint8_ErrCode); //call subroutine
                    break;
                
                case (_IdGRACC):        //command GRACC
                    cmd_GRACC();        //call subroutine
                    break;
                
                case (_IdGRDEC):        //command GRDEC
                    cmd_GRDEC();        //call subroutine
                    break;
                
                case (_IdSRUN):         //command SRUN (not used anymore)
                    g_Param.uint8_ErrCode = _UnknownCmd;        //set error code
                    uart2_SendErrorCode(g_Param.uint8_ErrCode); //call subroutine
                    break;
                
                case (_IdGRUN):         //command GRUN
                    cmd_GRUN();         //call subroutine
                    break;
                
                case (_IdBREAK):        //command BREAK
                    cmd_BREAK();        //call subroutine
                    break;
                
                case (_IdCAL):          //command CAL (not used anymore)
                    g_Param.uint8_ErrCode = _UnknownCmd;        //set error code
                    uart2_SendErrorCode(g_Param.uint8_ErrCode); //call subroutine
                    break;
            
                case (_IdVCAL):         //command VCAL (not used anymore)
                    g_Param.uint8_ErrCode = _UnknownCmd;        //set error code
                    uart2_SendErrorCode(g_Param.uint8_ErrCode); //call subroutine
                    break;
      
                case (_IdSPRUN):        //command SPRUN (not used anymore)
                    g_Param.uint8_ErrCode = _UnknownCmd;        //set error code
                    uart2_SendErrorCode(g_Param.uint8_ErrCode); //call subroutine
                    break;
            
                case (_IdGPRUN):        //command GPRUN (not used anymore)
                    g_Param.uint8_ErrCode = _UnknownCmd;        //set error code
                    uart2_SendErrorCode(g_Param.uint8_ErrCode); //call subroutine
                    break;
                
                case (_IdSUMOT):        //command SUMOT
                    g_Param.uint8_ErrCode = _UnknownCmd;        //set error code
                    uart2_SendErrorCode(g_Param.uint8_ErrCode); //call subroutine
                    break;
      
                case (_IdGUMOT):        //command GUMOT
                    g_Param.uint8_ErrCode = _UnknownCmd;        //set error code
                    uart2_SendErrorCode(g_Param.uint8_ErrCode); //call subroutine
                    break;  
                
                case (_IdCHKSC):        //command CHKSC (not used anymore)
                    g_Param.uint8_ErrCode = _UnknownCmd;        //set error code
                    uart2_SendErrorCode(g_Param.uint8_ErrCode); //call subroutine
                    break;
                
                case (_IdMUMOT):        //command MUMOT
                    cmd_MUMOT();        //call subroutine
                    break;
                
                case (_IdCHKUMOT):      //command CHKUMOT (not used anymore)
                    g_Param.uint8_ErrCode = _UnknownCmd;        //set error code
                    uart2_SendErrorCode(g_Param.uint8_ErrCode); //call subroutine
                    break;
                    
                case (_IdSCOILON):      //command SCOILON 
                    cmd_SCOILON();      //call subroutine
                    break;
                    
                case (_IdSCACC):        //command SCACC (not used anymore)
                    g_Param.uint8_ErrCode = _UnknownCmd;        //set error code
                    uart2_SendErrorCode(g_Param.uint8_ErrCode); //call subroutine
                    break;
            
                case (_IdGCACC):        //command GCACC (not used anymore)
                    g_Param.uint8_ErrCode = _UnknownCmd;        //set error code
                    uart2_SendErrorCode(g_Param.uint8_ErrCode); //call subroutine
                    break;
            
                case (_IdSCDEC):        //command SCDEC (not used anymore)
                    g_Param.uint8_ErrCode = _UnknownCmd;        //set error code
                    uart2_SendErrorCode(g_Param.uint8_ErrCode); //call subroutine
                    break;
            
                case (_IdGCDEC):        //command GCDEC (not used anymore)
                    g_Param.uint8_ErrCode = _UnknownCmd;        //set error code
                    uart2_SendErrorCode(g_Param.uint8_ErrCode); //call subroutine
                    break;
            
                case (_IdSSMOD):        //command SSMOD
                    cmd_SSMOD();        //call subroutine
                    break;
      
                case (_IdGSMOD):        //command GSMOD
                    cmd_GSMOD();        //call subroutine
                    break;
            
                case (_IdSERUN):        //command SERUN (not used anymore)
                    g_Param.uint8_ErrCode = _UnknownCmd;        //set error code
                    uart2_SendErrorCode(g_Param.uint8_ErrCode); //call subroutine
                    break;
                /*
                case (38):  //command GERUN
                    cmd_GERUN();        //call subroutine
                    break;
                              
                case (39):  //command ERUN
                    cmd_ERUN();         //call subroutine
                    break;
                    
                case (40):  //command GO
                    cmd_GO();           //call subroutine
                    break;
                */    
                case (_IdSIMAN):        //command SIMAN (not used anymore)
                    g_Param.uint8_ErrCode = _UnknownCmd;        //set error code
                    uart2_SendErrorCode(g_Param.uint8_ErrCode); //call subroutine
                    break;
            
                case (_IdGIMAN):        //command GIMAN (not used anymore)
                    g_Param.uint8_ErrCode = _UnknownCmd;        //set error code
                    uart2_SendErrorCode(g_Param.uint8_ErrCode); //call subroutine
                    break;
                
                case (_IdSBIT):         //command SBIT
                    cmd_SBIT();         //call subroutine
                    break;
                
                case (_IdCBIT):         //command CBIT
                    cmd_CBIT();         //call subroutine
                    break;
                
                case (_IdGBIT):         //command GBIT
                    cmd_GBIT();         //call subroutine
                    break;
                
                case (_IdSOUT):         //command SOUT
                    cmd_SOUT();         //call subroutine
                    break;
                
                case (_IdGOUT):         //command GOUT
                    cmd_GOUT();         //call subroutine
                    break;
                
                case (_IdGINP):         //command GINP
                    cmd_GINP();         //call subroutine
                    break;
                
                case (_IdSMCRSTP):      //command SMCRSTP
                    cmd_SMCRSTP();      //call subroutine
                    break;
                
                case (_IdGMCRSTP):      //command GMCRSTP
                    cmd_GMCRSTP();      //call subroutine
                    break;
                
                case (_IdSSWLIM):       //command SSWLIM
                    cmd_SSWLIM();       //call subroutine
                    break;
                
                case (_IdGSWLIM):  //command GSWLIM
                    cmd_GSWLIM();     //call subroutine
                    break;
                
                case (_IdGSWPOS):  //command GSWPOS
                    cmd_GSWPOS();     //call subroutine
                    break;
      
                case (_IdSPWMLIM):  //command SPWMLIM
                    cmd_SPWMLIM();    //call subroutine
                    break;
                
                case (_IdGPWMLIM):  //command GPWMLIM
                    cmd_GPWMLIM();    //call subroutine
                    break;
      
                case (_IdGPWMPOS):  //command GPWMPOS
                    cmd_GPWMPOS();    //call subroutine
                    break;
      
                case (_IdGPWMVAL):  //command GPWMVAL
                    cmd_GPWMVAL();    //call subroutine
                    break;
      
                case (_IdSSWTYP):  //command SSWTYP
                    cmd_SSWTYP();     //call subroutine
                    break;
      
                case (_IdGSWTYP):  //command GSWTYP
                    cmd_GSWTYP();     //call subroutine
                    break;
      
                case (_IdGFRQVAL):  //command GFRQVAL
                    cmd_GFRQVAL();    //call subroutine
                    break;
                
                case (_IdSPOSRUN):      //command SPOSRUN (not used anymore)
                    g_Param.uint8_ErrCode = _UnknownCmd;        //set error code
                    uart2_SendErrorCode(g_Param.uint8_ErrCode); //call subroutine
                    break;
                /*
                case (62):  //command GPOSRUN
                    cmd_GPOSRUN();    //call subroutine
                    break;
                    
                case (63):  //command POSRUN
                    cmd_POSRUN();       //call subroutine
                    break;
                */    
                case (_IdGINPB):        //command GINPB                  
                    cmd_GINPB();        //call subroutine
                    break;
                    
                case (_IdSROM):         //command SROM
                    cmd_SROM();         //call subroutine
                    break;
                    
                case (_IdRROM):         //command RROM
                    cmd_RROM();         //call subroutine
                    break;
                    
                case (_IdGTEMP):        //command GTEMP
                    cmd_GTEMP();        //call subroutine
                    break;
                    
                case (_IdSSEC):         //command SSEC
                    cmd_SSEC();         //call subroutine
                    break;
                    
                case (_IdGSEC):         //commmand GSEC
                    cmd_GSEC();         //call subroutine
                    break;
                    
                case (_IdSMIN):         //command SMIN
                    cmd_SMIN();         //call subroutine
                    break;
                    
                case (_IdGMIN):         //command GMIN
                    cmd_GMIN();         //call subroutine
                    break;
                    
                case (_IdSHRS):         //command SHRS
                    cmd_SHRS();         //call subroutine
                    break;   
                    
                case (_IdGHRS):         //command GHRS
                    cmd_GHRS();         //call subroutine
                    break;
                    
                case (_IdSDAY):         //command SDAY
                    cmd_SDAY();         //call subroutine
                    break;
                    
                case (_IdGDAY):         //command GDAY
                    cmd_GDAY();         //call subroutine
                    break;
                    
                case (_IdSWDAY):        //command SWDAY
                    cmd_SWDAY();        //call subroutine
                    break;
                    
                case (_IdGWDAY):        //command GWDAY
                    cmd_GWDAY();        //call subroutine
                    break;
                    
                case (_IdSMONTH):       //command SMONTH
                    cmd_SMONTH();       //call subroutine
                    break;
                    
                case (_IdGMONTH):       //command GMONTH
                    cmd_GMONTH();       //call subroutine 
                    break;
                    
                case (_IdSYEAR):        //command SYEAR
                    cmd_SYEAR();        //call subroutine
                    break;
                    
                case (_IdGYEAR):        //command GYEAR
                    cmd_GYEAR();        //call subroutine
                    break;
                    
                case (_IdGTIME):        //command GTIME
                    cmd_GTIME();        //call subroutine
                    break;
                    
                case (_IdGSTALIN):      //command GSTALIN
                    cmd_GSTALIN();      //call subroutine
                    break;
                                     
                default:    //command ID does not exist
                    //do nothing - error has to be send before this routine
                    break;
            }
        }
        else
        {
            //send back the information that a POR was detected and set the POR variable to true
            g_Param.uint8_ErrCode = _GlobalLock;        //set error code
            uart2_SendErrorCode(g_Param.uint8_ErrCode); //call subroutine
            
            g_Param.uint8_NotPOR = 1;                   //release the communication
        }
        //reset all variables for the next protocol
        g_CmdChk.uint8_ParamPos = 0;                 
        g_CmdChk.uint8_LetterPos = 0; 
        g_CmdChk.uint32_Cmd1st4 = 0; 
        g_CmdChk.uint32_Cmd2nd4 = 0;
    }
    else
    {
        //do nothing
    }
}   //end of cmdchk_check


/**********************************************************************************************************************
 * Routine:                 cmdchk_def

 * Description:
 * This subroutine verifies the received command if it exists and gives them the corresponding ID number. 
 * 
 * Creator:                 A. Staub
 * Date of creation:        05.12.2015
 * Last modification on:    -
 * Modified by:             - 
 * 
 * Input:                   -
 * Output:                  -
***********************************************************************************************************************/
void cmdchk_def(void)
{
    auto unsigned long int uint32_WR;       //local work register
    
    if(g_CmdChk.uint32_Cmd2nd4)         //second part not empty?
    {
        //then set the local work register to 0, for that the switch case verifies first the second part and 
        //then the first. this is to ensure that we mix not some commands together. for example if we create one time 
        //a command with the name GVERHAR, the switch case would always interpret this as GVER command, as the first 
        //part does match this. So in this case we set the local work register to 0, for that the program first 
        //verifies the part HAR and then the part GVER to have access on this command. 
        uint32_WR = 0;
    }
    else
    {
        uint32_WR = g_CmdChk.uint32_Cmd1st4;   //otherwise copy the first part into the work register
    }
    
  
    //first verify the commands with 4 letters or less
    switch(uint32_WR)
    {
        case (0x52554E):    //cmd is = RUN
            g_CmdChk.uint8_CmdID = _IdRUN;
            break;
      
        case (0x53504843):  //cmd is = SPHC
            g_CmdChk.uint8_CmdID = _IdSPHC;
            break;
        
        case (0x47564552):  //cmd is = GVER
            g_CmdChk.uint8_CmdID = _IdGVER;
            break;
        
        case (0x52415A):    //cmd is = RAZ
            g_CmdChk.uint8_CmdID = _IdRAZ;
            break;
    
        case (0x47504843):  //cmd is = GPHC
            g_CmdChk.uint8_CmdID = _IdGPHC;
            break;
    
        case (0x5352554E):  //cmd is = SRUN
            g_CmdChk.uint8_CmdID = _IdSRUN;
            break;
    
        case (0x4752554E):  //cmd is = GRUN
            g_CmdChk.uint8_CmdID = _IdGRUN;
            break;
      
        case (0x43414C):    //cmd is = CAL
            g_CmdChk.uint8_CmdID = _IdCAL;
            break;
    
        case (0x5643414C):  //cmd is = VCAL
            g_CmdChk.uint8_CmdID = _IdVCAL;
            break;
    
        case (0x4552554E):  //cmd is = ERUN
            g_CmdChk.uint8_CmdID = _IdERUN;
            break;
    
        case (0x474F):      //cmd is = GO
            g_CmdChk.uint8_CmdID = _IdGO;
            break;
    
        case (0x53424954):  //cmd is = SBIT
            g_CmdChk.uint8_CmdID = _IdSBIT;
            break;
    
        case (0x43424954):  //cmd is = CBIT
            g_CmdChk.uint8_CmdID = _IdCBIT;
            break;
    
        case (0x47424954):  //cmd is = GBIT
            g_CmdChk.uint8_CmdID = _IdGBIT;
            break;
    
        case (0x534F5554):  //cmd is = SOUT
            g_CmdChk.uint8_CmdID = _IdSOUT;
            break;
    
        case (0x474F5554):  //cmd is = GOUT
            g_CmdChk.uint8_CmdID = _IdGOUT;
            break;
      
        case (0x47494E50):  //cmd is = GINP
            g_CmdChk.uint8_CmdID = _IdGINP;
            break;
            
        case (0x53524F4D):  //cmd is = SROM
            g_CmdChk.uint8_CmdID = _IdSROM;
            break;
            
        case (0x52524F4D):  //cmd is = RROM
            g_CmdChk.uint8_CmdID = _IdRROM;
            break;
            
        case (0x53534543):  //cmd is = SSEC
            g_CmdChk.uint8_CmdID = _IdSSEC;
            break;
            
        case (0x47534543):  //cmd is = GSEC
            g_CmdChk.uint8_CmdID = _IdGSEC;
            break;
            
        case (0x534D494E):  //cmd is = SMIN
            g_CmdChk.uint8_CmdID = _IdSMIN;
            break;
            
        case (0x474D494E):  //cmd is = GMIN
            g_CmdChk.uint8_CmdID = _IdGMIN;
            break;
            
        case (0x53485253):  //cmd is = SHRS
            g_CmdChk.uint8_CmdID = _IdSHRS;
            break;
            
        case (0x47485253):  //cmd is = GHRS
            g_CmdChk.uint8_CmdID = _IdGHRS;
            break;
            
        case (0x53444159):  //cmd is = SDAY
            g_CmdChk.uint8_CmdID = _IdSDAY;
            break;
            
        case (0x47444159):  //cmd is = GDAY
            g_CmdChk.uint8_CmdID = _IdGDAY;
            break;
        
        default:
            
            //from here - we know that the command does not exists or that
            //is longer then 4 letters. As we can make some groupes with the 
            //the second 4 letters, we verify first this and than again the
            //first 4 letters. 
            
            //to save time, first verify if we have something in the 2nd part
            if(g_CmdChk.uint32_Cmd2nd4)
            {
                switch(g_CmdChk.uint32_Cmd2nd4)
                {
                    case (0x4D):    //letter M?
                        switch(g_CmdChk.uint32_Cmd1st4)
                        {
                            case (0x53494C49):  //cmd is = SILIM
                                g_CmdChk.uint8_CmdID = _IdSILIM;
                                break;
            
                            case (0x47494C49):  //cmd is = GILIM
                                g_CmdChk.uint8_CmdID = _IdGILIM;
                                break;
            
                            default:  //error - command is not defined
                                g_CmdChk.uint8_CmdID = _IdNotAllow;        //clear the command ID
                                g_Param.uint8_ErrCode = _UnknownCmd;    //set error code
                                break;
                        }
                        break;
        
                    case (0x54494E):    //letters TIN?
                        switch(g_CmdChk.uint32_Cmd1st4)
                        {
                            case (0x45544553):  //cmd is = ETESTIN
                                g_CmdChk.uint8_CmdID = _IdETESTIN;
                                break;
            
                            default:  //error - command is not defined
                                g_CmdChk.uint8_CmdID = _IdNotAllow;        //clear the command ID
                                g_Param.uint8_ErrCode = _UnknownCmd;    //set error code
                                break;
                        }
                        break;
        
                    case (0x43):    //letter C?
                        switch(g_CmdChk.uint32_Cmd1st4)
                        {
                            case (0x53524143):  //cmd is = SRACC
                                g_CmdChk.uint8_CmdID = _IdSRACC;
                                break;
            
                            case (0x53524445):  //cmd is = SRDEC
                                g_CmdChk.uint8_CmdID = _IdSRDEC;
                                break;
            
                            case (0x47524143):  //cmd is = GRACC
                                g_CmdChk.uint8_CmdID = _IdGRACC;
                                break;
            
                            case (0x47524445):  //cmd is = GRDEC
                                g_CmdChk.uint8_CmdID = _IdGRDEC;
                                break;
            
                            case (0x43484B53):  //cmd is = CHKSC
                                g_CmdChk.uint8_CmdID = _IdCHKSC;
                                break;
            
                            case (0x53434143):  //cmd is = SCACC
                                g_CmdChk.uint8_CmdID = _IdSCACC;
                                break;
              
                            case (0x47434143):  //cmd is = GCACC
                                g_CmdChk.uint8_CmdID = _IdGCACC;
                                break;
              
                            case (0x53434445):  //cmd is = SCDEC
                                g_CmdChk.uint8_CmdID = _IdSCDEC;
                                break;
            
                            case (0x47434445):  //cmd is = GCDEC
                                g_CmdChk.uint8_CmdID = _IdGCDEC;
                                break;
            
                            default:  //error - command is not defined
                                g_CmdChk.uint8_CmdID = _IdNotAllow;        //clear the command ID
                                g_Param.uint8_ErrCode = _UnknownCmd;    //set error code
                                break;
                        }
                        break;
          
                    case (0x50):    //letter P?
                        switch(g_CmdChk.uint32_Cmd1st4)
                        {
                            case (0x534D5459):  //cmd is = SMTYP
                                g_CmdChk.uint8_CmdID = _IdSMTYP;
                                break;
            
                            case (0x474D5459):  //cmd is = GMTYP
                                g_CmdChk.uint8_CmdID = _IdGMTYP;
                                break;
                                
                            case (0x4754454D):  //cmd is = GTEMP
                                g_CmdChk.uint8_CmdID = _IdGTEMP;
                                break;
            
                            default:  //error - command is not defined
                                g_CmdChk.uint8_CmdID = _IdNotAllow;        //clear the command ID
                                g_Param.uint8_ErrCode = _UnknownCmd;    //set error code
                                break;
                        }
                        break;
          
                    case (0x54):    //letter T?
                        switch(g_CmdChk.uint32_Cmd1st4)
                        {
                            case (0x53544553):  //cmd is = STEST
                                g_CmdChk.uint8_CmdID = _IdSTEST;
                                break;
            
                            case (0x53554D4F):  //cmd is = SUMOT
                                g_CmdChk.uint8_CmdID = _IdSUMOT;
                                break;
            
                            case (0x47554D4F):  //cmd is = GUMOT
                                g_CmdChk.uint8_CmdID = _IdGUMOT;
                                break;
            
                            case (0x4D554D4F):  //cmd is = MUMOT
                                g_CmdChk.uint8_CmdID = _IdMUMOT;
                                break;
            
                            default:  //error - command is not defined
                                g_CmdChk.uint8_CmdID = _IdNotAllow;        //clear the command ID
                                g_Param.uint8_ErrCode = _UnknownCmd;    //set error code
                                break;
                        }
                        break;
          
                    case (0x4E):    //letter N?
                        switch(g_CmdChk.uint32_Cmd1st4)
                        {
                            case (0x53495255):  //cmd is = SIRUN
                                g_CmdChk.uint8_CmdID = _IdSIRUN;
                                break;
            
                            case (0x47495255):  //cmd is = GIRUN
                                g_CmdChk.uint8_CmdID = _IdGIRUN;
                                break;
            
                            case (0x53505255):  //cmd is = SPRUN
                                g_CmdChk.uint8_CmdID = _IdSPRUN;
                                break;
            
                            case (0x47505255):  //cmd is = GPRUN
                                g_CmdChk.uint8_CmdID = _IdGPRUN;
                                break;
            
                            case (0x53455255):  //cmd is = SERUN
                                g_CmdChk.uint8_CmdID = _IdSERUN;
                                break;
            
                            case (0x47455255):  //cmd is = GERUN
                                g_CmdChk.uint8_CmdID = _IdGERUN;
                                break;
              
                            case (0x53494D41):  //cmd is = SIMAN
                                g_CmdChk.uint8_CmdID = _IdSIMAN;
                                break;
              
                            case (0x47494D41):  //cmd is = GIMAN
                                g_CmdChk.uint8_CmdID = _IdGIMAN;
                                break;
            
                            default:  //error - command is not defined
                                g_CmdChk.uint8_CmdID = _IdNotAllow;        //clear the command ID
                                g_Param.uint8_ErrCode = _UnknownCmd;    //set error code
                                break;
                        }
                        break;
          
                    case (0x4B):    //letter K?
                        switch(g_CmdChk.uint32_Cmd1st4)
                        {
                            case (0x42524541):  //cmd is = BREAK
                                g_CmdChk.uint8_CmdID = _IdBREAK;
                                break;
            
                            default:  //error - command is not defined
                                g_CmdChk.uint8_CmdID = _IdNotAllow;        //clear the command ID
                                g_Param.uint8_ErrCode = _UnknownCmd;    //set error code
                                break;
                        }
                        break;
          
                    case (0x4D4F54):    //letters MOT?
                        switch(g_CmdChk.uint32_Cmd1st4)
                        {
                            case (0x43484B55):  //cmd is = CHKUMOT
                                g_CmdChk.uint8_CmdID = _IdCHKUMOT;
                                break;
            
                            default:  //error - command is not defined
                                g_CmdChk.uint8_CmdID = _IdNotAllow;        //clear the command ID
                                g_Param.uint8_ErrCode = _UnknownCmd;    //set error code
                                break;
                        }
                        break;
          
                    case (0x4C4F4E):    //letters LON?
                        switch(g_CmdChk.uint32_Cmd1st4)
                        {
                            case (0x53434F49):  //cmd is = SCOILON
                                g_CmdChk.uint8_CmdID = _IdSCOILON;
                                break;
            
                            default:  //error - command is not defined
                                g_CmdChk.uint8_CmdID = _IdNotAllow;        //clear the command ID
                                g_Param.uint8_ErrCode = _UnknownCmd;    //set error code
                                break;
                        }
                        break;
          
                    case (0x44):    //letter D?
                        switch(g_CmdChk.uint32_Cmd1st4)
                        {
                            case (0x53534D4F):  //cmd is = SSMOD
                                g_CmdChk.uint8_CmdID = _IdSSMOD;
                                break;
            
                            case (0x47534D4F):  //cmd is = GSMOD
                                g_CmdChk.uint8_CmdID = _IdGSMOD;
                                break;
            
                            default:  //error - command is not defined
                                g_CmdChk.uint8_CmdID = _IdNotAllow;      //clear the command ID
                                g_Param.uint8_ErrCode = _UnknownCmd;  //set error code
                                break;
                        }
                        break;
          
                    case (0x535450):    //letters STP?
                        switch(g_CmdChk.uint32_Cmd1st4)
                        {
                            case (0x534D4352):  //cmd is = SMCRSTP
                                g_CmdChk.uint8_CmdID = _IdSMCRSTP;
                                break;
            
                            case (0x474D4352):  //cmd is = GMCRSTP
                                g_CmdChk.uint8_CmdID = _IdGMCRSTP;
                                break;
                            
                            default:  //error - command is not defined
                                g_CmdChk.uint8_CmdID = _IdNotAllow;        //clear the command ID
                                g_Param.uint8_ErrCode = _UnknownCmd;    //set error code
                                break;
                        }
                        break;
          
                    case (0x494D):  //letters IM?
                        switch(g_CmdChk.uint32_Cmd1st4)
                        {
                            case (0x5353574C):  //cmd is = SSWLIM
                                g_CmdChk.uint8_CmdID = _IdSSWLIM;
                                break;
            
                            case (0x4753574C):  //cmd is = GSWLIM
                                g_CmdChk.uint8_CmdID = _IdGSWLIM;
                                break;
            
                            default:  //error - command is not defined
                                g_CmdChk.uint8_CmdID = _IdNotAllow;        //clear the command ID
                                g_Param.uint8_ErrCode = _UnknownCmd;    //set error code
                                break;
                        }
                        break;
          
                    case (0x4F53):  //letters OS?
                        switch(g_CmdChk.uint32_Cmd1st4)
                        {
                            case (0x47535750):  //cmd is = GSWPOS
                                g_CmdChk.uint8_CmdID = _IdGSWPOS;
                                break;
            
                            default:  //error - command is not defined
                                g_CmdChk.uint8_CmdID = _IdNotAllow;        //clear the command ID
                                g_Param.uint8_ErrCode = _UnknownCmd;    //set error code
                                break;
                        }
                        break;
          
                    case (0x4C494D):    //letters LIM?
                        switch(g_CmdChk.uint32_Cmd1st4)
                        {
                            case (0x5350574D):  //cmd is = SPWMLIM
                                g_CmdChk.uint8_CmdID = _IdSPWMLIM;
                                break;
            
                            case (0x4750574D):  //cmd is = GPWMLIM
                                g_CmdChk.uint8_CmdID = _IdGPWMLIM;
                                break;
            
                            default:  //error - command is not defined
                                g_CmdChk.uint8_CmdID = _IdNotAllow;        //clear the command ID
                                g_Param.uint8_ErrCode = _UnknownCmd;    //set error code
                                break;
                        }
                        break;
          
                    case (0x504F53):    //letters POS?
                        switch(g_CmdChk.uint32_Cmd1st4)
                        {
                            case (0x4750574D):  //cmd is = GPWMPOS
                                g_CmdChk.uint8_CmdID = _IdGPWMPOS;
                                break;
            
                            default:  //error - command is not defined
                                g_CmdChk.uint8_CmdID = _IdNotAllow;        //clear the command ID
                                g_Param.uint8_ErrCode = _UnknownCmd;    //set error code
                                break;
                        }
                        break;
          
                    case (0x56414C):    //letters VAL?
                        switch(g_CmdChk.uint32_Cmd1st4)
                        {
                            case (0x4750574D):  //cmd is = GPWMVAL
                                g_CmdChk.uint8_CmdID = _IdGPWMVAL;
                                break;
              
                            case (0x47465251):  //cmd is GFRQVAL
                                g_CmdChk.uint8_CmdID = _IdGFRQVAL;
                                break;
            
                            default:  //error - command is not defined
                                g_CmdChk.uint8_CmdID = _IdNotAllow;        //clear the command ID
                                g_Param.uint8_ErrCode = _UnknownCmd;    //set error code
                                break;
                        }
                        break;
          
                    case (0x5950):  //letters YP?
                        switch(g_CmdChk.uint32_Cmd1st4)
                        {
                            case (0x53535754):  //cmd is = SSWTYP
                                g_CmdChk.uint8_CmdID = _IdSSWTYP;
                                break;
            
                            case (0x47535754):  //cmd is = GSWTYP
                                g_CmdChk.uint8_CmdID = _IdGSWTYP;
                                break;
            
                            default:  //error - command is not defined
                                g_CmdChk.uint8_CmdID = _IdNotAllow;        //clear the command ID
                                g_Param.uint8_ErrCode = _UnknownCmd;    //set error code
                                break;
                        }
                        break;
          
                    case (0x52554E):    //letters RUN?
                        switch(g_CmdChk.uint32_Cmd1st4)
                        {
                            case (0x53504F53):  //cmd is = SPOSRUN
                                g_CmdChk.uint8_CmdID = _IdSPOSRUN;
                                break;
            
                            case (0x47504F53):  //cmd is = GPOSRUN
                                g_CmdChk.uint8_CmdID = _IdGPOSRUN;
                                break;
            
                            default:  //error - command is not defined
                                g_CmdChk.uint8_CmdID = _IdNotAllow;        //clear the command ID
                                g_Param.uint8_ErrCode = _UnknownCmd;    //set error code
                                break;
                        }
                        break;
          
                    case (0x554E):  //letters UN?
                        switch(g_CmdChk.uint32_Cmd1st4)
                        {
                            case (0x504F5352):  //cmd is = POSRUN
                                g_CmdChk.uint8_CmdID = _IdPOSRUN;
                                break;
            
                            default:  //error - command is not defined
                                g_CmdChk.uint8_CmdID = _IdNotAllow;        //clear the command ID
                                g_Param.uint8_ErrCode = _UnknownCmd;    //set error code
                                break;
                        }
                        break;
                        
                    case (0x42):    //letter B?
                        switch(g_CmdChk.uint32_Cmd1st4)
                        {
                            case (0x47494E50):  //cmd is = GINPB
                                g_CmdChk.uint8_CmdID = _IdGINPB;
                                break;
                                
                            default:    //error - command is not defined
                                g_CmdChk.uint8_CmdID = _IdNotAllow;        //clear the command ID
                                g_Param.uint8_ErrCode = _UnknownCmd;    //set error code
                                break;
                        }
                        break;
                        
                    case (0x59):    //letter Y?
                        switch(g_CmdChk.uint32_Cmd1st4)
                        {
                            case (0x53574441):  //cmd is = SWDAY
                                g_CmdChk.uint8_CmdID = _IdSWDAY;
                                break;
                                
                            case (0x47574441):  //cmd is = GWDAY
                                g_CmdChk.uint8_CmdID = _IdGWDAY;
                                break;
                                
                            default:    //error - command is not defined
                                g_CmdChk.uint8_CmdID = _IdNotAllow;        //clear the command ID
                                g_Param.uint8_ErrCode = _UnknownCmd;    //set error code
                                break;
                        }
                        break;
                        
                    case (0x5448):  //letter TH?
                        switch(g_CmdChk.uint32_Cmd1st4)
                        {
                            case (0x534D4F4E):  //cmd is = SMONTH
                                g_CmdChk.uint8_CmdID = _IdSMONTH;
                                break;
                                
                            case (0x474D4F4E):  //cmd is = GMONTH
                                g_CmdChk.uint8_CmdID = _IdGMONTH;
                                break;
                                
                            default:    //error - command is not defined
                                g_CmdChk.uint8_CmdID = _IdNotAllow;        //clear the command ID
                                g_Param.uint8_ErrCode = _UnknownCmd;    //set error code
                                break;
                        }
                        break;
                        
                    case (0x52):    //letter R?
                        switch(g_CmdChk.uint32_Cmd1st4)
                        {
                            case (0x53594541):  //cmd is = SYEAR
                                g_CmdChk.uint8_CmdID = _IdSYEAR;
                                break;
                                
                            case (0x47594541):  //cmd is = GYEAR
                                g_CmdChk.uint8_CmdID = _IdGYEAR;
                                break;
                                
                            default:    //error - command is not defined
                                g_CmdChk.uint8_CmdID = _IdNotAllow;        //clear the command ID
                                g_Param.uint8_ErrCode = _UnknownCmd;    //set error code
                                break;
                        }
                        break;
                        
                    case (0x45):    //letter E?
                        switch(g_CmdChk.uint32_Cmd1st4)
                        {
                            case (0x4754494D):  //cmd is = GTIME
                                g_CmdChk.uint8_CmdID = _IdGTIME;
                                break;
                                
                            default:    //error - command is not defined
                                g_CmdChk.uint8_CmdID = _IdNotAllow;        //clear the command ID
                                g_Param.uint8_ErrCode = _UnknownCmd;    //set error code
                                break;
                        }
                        break;
                        
                    case (0x4C494E):    //letter LIN?
                        switch(g_CmdChk.uint32_Cmd1st4)
                        {
                            case (0x47535441):  //cmd is = GSTALIN
                                g_CmdChk.uint8_CmdID = _IdGSTALIN;
                                break;
                                
                            default:    //error - command is not defined
                                g_CmdChk.uint8_CmdID = _IdNotAllow;        //clear the command ID
                                g_Param.uint8_ErrCode = _UnknownCmd;    //set error code
                                break;
                        }
                        break;
        
                    default:  //error - command is not defined
                        g_CmdChk.uint8_CmdID = _IdNotAllow;        //clear the command ID
                        g_Param.uint8_ErrCode = _UnknownCmd;    //set error code
                        break;
                }
            }
            else
            {
                //error - command is not defined
                g_CmdChk.uint8_CmdID = _IdNotAllow;        //clear the command ID
                g_Param.uint8_ErrCode = _UnknownCmd;    //set error code
            }
            
            break;
    }
}   //end of cmdchk_def