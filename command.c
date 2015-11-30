/**********************************************************************************************************************

                                            UBD - Unipolar Bipolar Driver

***********************************************************************************************************************
 * File name:               command.c
 * Creation date:           03.10.2015
 * Main creator:            Alain Staub
 * Company:                 REFAST GmbH
 *                          Copyright (c) 2015 REFAST GmbH
***********************************************************************************************************************
 * Content overview:        - cmd_init
 *                          - cmd_check
 *                          - cmd_def
 *                          - cmd_SILIM
 *                          - cmd_ETESTIN
 *                          - cmd_SRACC
 *                          - cmd_SRDEC
 *                          - cmd_RUN
 *                          - cmd_SPHC
 *                          - cmd_GVER
 *                          - cmd_RAZ
 *                          - cmd_SMTYP
 *                          - cmd_GMTYP
 *                          - cmd_STEST
 *                          - cmd_GILIM
 *                          - cmd_GPHC
 *                          - cmd_GRACC
 *                          - cmd_GRDEC
 *                          - cmd_GRUN
 *                          - cmd_BREAK
 *                          - cmd_MUMOT
 *                          - cmd_SCOILON
 *                          - cmd_SSMOD
 *                          - cmd_GSMOD
 *                          - cmd_SBIT
 *                          - cmd_CBIT
 *                          - cmd_GBIT
 *                          - cmd_SOUT
 *                          - cmd_GOUT
 *                          - cmd_GINP
 *                          - cmd_SMCRSTP
 *                          - cmd_GMCRSTP
 *                          - cmd_SSWLIM
 *                          - cmd_GSWLIM
 *                          - cmd_GSWPOS
 *                          - cmd_SPWMLIM
 *                          - cmd_GPWMLIM
 *                          - cmd_GPWMPOS
 *                          //new commands:
 *                          - cmd_GINPB
***********************************************************************************************************************/


#include "includes.h"

SCmd g_Cmd;                 //global variables for struct  


/**********************************************************************************************************************
 * Routine:                 cmd_init

 * Description:
 * This routine initialize the basic things.
 * 
 * Creator:                 A. Staub
 * Date of creation:        03.10.2015
 * Last modification on:    -
 * Modified by:             - 
 * 
 * Input:                   -
 * Output:                  -
 * Global variable:         -
***********************************************************************************************************************/
void cmd_init(void)
{
//initialize the command variables    
  g_Cmd.uint8_ParamPos = 0;     //clear the parameter position
  g_Cmd.uint8_LetterPos = 0;    //clear the letter position
  g_Cmd.uint32_Cmd1st4 = 0;     //clear the first 4 letters of the command
  g_Cmd.uint32_Cmd2nd4 = 0;     //clear the second 4 letters of the command
  g_Cmd.uint8_CmdID = 0;        //clear command id
}   //end of cmd_init


/**********************************************************************************************************************
 * Routine:                 cmd_check

 * Description:
 * ...
 * 
 * Creator:                 A. Staub
 * Date of creation:        10.10.2015
 * Last modification on:    -
 * Modified by:             - 
 * 
 * Input:                   -
 * Output:                  -
 * Global variable:         g_Cmd.
 *                              - uint8_ParamPos
 *                              - uint32_TempPara
 *                              - uint8_CmdID
 *                              - uint32_TempPara
 *                              - uint32_Cmd1st4
 *                              - uint32_Cmd2nd4
 *                              - uint8_LetterPos
 *                          g_Param.
 *                              - uint8_ErrCode
 *                          g_UART2rxd.
 *                              - uint16_Rch
 *                              - uint16_Wch
 *                              - uint8_BufEmpty
***********************************************************************************************************************/
void cmd_check(void)
{
    auto unsigned char uint8_RxDBuffChar;       //local byte for the actually received character
    
    uint8_RxDBuffChar = uart2_receivebuffer();  //read out one byte from the receive buffer
  
    //verify the position of the parameter (is it the first, second, etc.)
    switch(g_Cmd.uint8_ParamPos) 
    {
        case (0):   //1. parameter       
            //character = carriage return or a comma?
            if((uint8_RxDBuffChar == 13) || (uint8_RxDBuffChar == 44))
            {
                cmd_def();  //stop here and verify the command
                
                //store the command ID into the temporary register at the place "ParamPos"
                g_Cmd.uint32_TempPara[g_Cmd.uint8_ParamPos] = g_Cmd.uint8_CmdID;
             
                //command not defined?
                if((g_Cmd.uint8_CmdID == _IdNotAllow) && (g_Param.uint8_ErrCode == _UnknownCmd))
                {
                    uart2_SendErrorCode(g_Param.uint8_ErrCode); //then send back the error code
          
                    //reset the variables
                    g_Cmd.uint8_ParamPos = 0;                  
                    g_Cmd.uint32_Cmd1st4 = 0; 
                    g_Cmd.uint32_Cmd2nd4 = 0;              
          
                    //clear the receive buffer
                    g_UART2rxd.uint16_Rch = g_UART2rxd.uint16_Wch;          
                    g_UART2rxd.uint8_BufEmpty = 1;                  
                }
                else
                {
                    g_Cmd.uint8_ParamPos++; //increment the parameter position
                }
        
                g_Cmd.uint8_LetterPos = 0;  //clear the letter position for the next command                 
            }
            else
            {
                switch(g_Cmd.uint8_LetterPos)   //verify the position of the letter
                {
                    case (0): //add 1. letter
                        g_Cmd.uint32_Cmd1st4 = uint8_RxDBuffChar;              
                        break;
          
                    case (1): //add 2. letter
                        g_Cmd.uint32_Cmd1st4 = g_Cmd.uint32_Cmd1st4 << 8;                          
                        g_Cmd.uint32_Cmd1st4 = g_Cmd.uint32_Cmd1st4 | uint8_RxDBuffChar;
                        break;
          
                    case (2): //add 3. letter
                        g_Cmd.uint32_Cmd1st4 = g_Cmd.uint32_Cmd1st4 << 8;                          
                        g_Cmd.uint32_Cmd1st4 = g_Cmd.uint32_Cmd1st4 | uint8_RxDBuffChar; 
                        break;
          
                    case (3): //add 4. letter
                        g_Cmd.uint32_Cmd1st4 = g_Cmd.uint32_Cmd1st4 << 8;                          
                        g_Cmd.uint32_Cmd1st4 = g_Cmd.uint32_Cmd1st4 | uint8_RxDBuffChar;
                        break;
          
                    case (4): //add 5. letter
                        g_Cmd.uint32_Cmd2nd4 = uint8_RxDBuffChar;
                        break;
          
                    case (5): //add 6. letter
                        g_Cmd.uint32_Cmd2nd4 = g_Cmd.uint32_Cmd2nd4 << 8;                          
                        g_Cmd.uint32_Cmd2nd4 = g_Cmd.uint32_Cmd2nd4 | uint8_RxDBuffChar;
                        break;
          
                    case (6): //add 7. letter
                        g_Cmd.uint32_Cmd2nd4 = g_Cmd.uint32_Cmd2nd4 << 8;                          
                        g_Cmd.uint32_Cmd2nd4 = g_Cmd.uint32_Cmd2nd4 | uint8_RxDBuffChar;
                        break;
          
                    case (7): //add 8. letter
                        g_Cmd.uint32_Cmd2nd4 = g_Cmd.uint32_Cmd2nd4 << 8;                          
                        g_Cmd.uint32_Cmd2nd4 = g_Cmd.uint32_Cmd2nd4 | uint8_RxDBuffChar;
                        break;
          
                    default:  //error - command is longer than defined 
                        g_Cmd.uint8_CmdID = _IdNotAllow;        //clear the command ID
                        g_Param.uint8_ErrCode = _UnknownCmd;    //set error code
            
                        //then send back the error code
                        uart2_SendErrorCode(g_Param.uint8_ErrCode); //then send back the error code
          
                        //reset the variables
                        g_Cmd.uint8_ParamPos = 0;                 
                        g_Cmd.uint8_LetterPos = 0;
                        g_Cmd.uint32_Cmd1st4 = 0; 
                        g_Cmd.uint32_Cmd2nd4 = 0;                
          
                        //clear the receive buffer
                        g_UART2rxd.uint16_Rch = g_UART2rxd.uint16_Wch;          
                        g_UART2rxd.uint8_BufEmpty = 1;        
                        break;
                }
                g_Cmd.uint8_LetterPos++;    //increment letter position
            }    
            break;    
    
        default:  //for all other parameters
            //store the actually letter position into the ASCII array position
            g_Funct.uint8_ArrAsciiPos = g_Cmd.uint8_LetterPos;
        
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
                         g_Cmd.uint32_TempPara[g_Cmd.uint8_ParamPos] = funct_AsciiToInt();
                    }
                    else
                    {
                        //otherwise store the character without conversion
                        g_Cmd.uint32_TempPara[g_Cmd.uint8_ParamPos] = g_Funct.uint8_ArrAscii[0];
                    }
                }
                else
                {
                    //otherwise verify if it was a comma (in this case ArrAsciiPos is 255)
                    if(g_Funct.uint8_ArrAsciiPos == 255)
                    {
                        //in this case set all bits 
                        g_Cmd.uint32_TempPara[g_Cmd.uint8_ParamPos] = 0xFFFFFFFF;
                    }
                    else
                    {
                        //otherwise start the conversion
                        g_Cmd.uint32_TempPara[g_Cmd.uint8_ParamPos] = funct_AsciiToInt();
                    }
                }
        
                g_Cmd.uint8_ParamPos++;     //increment the parameter position
                g_Cmd.uint8_LetterPos = 0;  //clear the letter position for the next command                 
            }
            else
            {
                //store the actually letter position into the ASCII array position
                //g_Funct.uint8_ArrAsciiPos = g_Cmd.uint8_LetterPos;
                //store the receive character into the ASCII array
                g_Funct.uint8_ArrAscii[g_Funct.uint8_ArrAsciiPos] = uint8_RxDBuffChar; 
                g_Cmd.uint8_LetterPos++;    //increment letter position
            }
            break;
    }
  
    //character in the buffer was CR and the position of the parameter is not 0
    if((uint8_RxDBuffChar == 13) && (g_Cmd.uint8_ParamPos))
    {
        if(g_Param.uint8_NotPOR)    //device did not see a POR? --> this is new
        {
            switch(g_Cmd.uint8_CmdID)   //execute the needed commando
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
      /*
                case (57):  //command GPWMVAL
                    cmd_GPWMVAL();    //call subroutine
                    break;
      
                case (58):  //command SSWTYP
                    cmd_SSWTYP();     //call subroutine
                    break;
      
                case (59):  //command GSWTYP
                    cmd_GSWTYP();     //call subroutine
                    break;
      
                case (60):  //command GFRQVAL
                    cmd_GFRQVAL();    //call subroutine
                    break;
                */
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
        g_Cmd.uint8_ParamPos = 0;                 
        g_Cmd.uint8_LetterPos = 0; 
        g_Cmd.uint32_Cmd1st4 = 0; 
        g_Cmd.uint32_Cmd2nd4 = 0;
    }
    else
    {
        //do nothing
    }
}   //end of cmd_check


/**********************************************************************************************************************
 * Routine:                 cmd_def

 * Description:
 * This subroutine verifies the received command if it exists and gives them the corresponding ID number. 
 * 
 * Creator:                 A. Staub
 * Date of creation:        10.10.2015
 * Last modification on:    -
 * Modified by:             - 
 * 
 * Input:                   -
 * Output:                  -
 * Global variable:         g_Cmd.
 *                              - uint32_Cmd2nd4
 *                              - uint32_Cmd1st4
 *                              - uint8_CmdID
 *                          g_Param.
 *                              - uint8_ErrCode
***********************************************************************************************************************/
void cmd_def(void)
{
    auto unsigned long uint32_WR;       //local work register
    
    if(g_Cmd.uint32_Cmd2nd4)            //second part not empty?
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
        uint32_WR = g_Cmd.uint32_Cmd1st4;   //otherwise copy the first part into the work register
    }
    
  
    //first verify the commands with 4 letters or less
    switch(uint32_WR)
    {
        case (0x52554E):    //cmd is = RUN
            g_Cmd.uint8_CmdID = _IdRUN;
            break;
      
        case (0x53504843):  //cmd is = SPHC
            g_Cmd.uint8_CmdID = _IdSPHC;
            break;
        
        case (0x47564552):  //cmd is = GVER
            g_Cmd.uint8_CmdID = _IdGVER;
            break;
        
        case (0x52415A):    //cmd is = RAZ
            g_Cmd.uint8_CmdID = _IdRAZ;
            break;
    
        case (0x47504843):  //cmd is = GPHC
            g_Cmd.uint8_CmdID = _IdGPHC;
            break;
    
        case (0x5352554E):  //cmd is = SRUN
            g_Cmd.uint8_CmdID = _IdSRUN;
            break;
    
        case (0x4752554E):  //cmd is = GRUN
            g_Cmd.uint8_CmdID = _IdGRUN;
            break;
      
        case (0x43414C):    //cmd is = CAL
            g_Cmd.uint8_CmdID = _IdCAL;
            break;
    
        case (0x5643414C):  //cmd is = VCAL
            g_Cmd.uint8_CmdID = _IdVCAL;
            break;
    
        case (0x4552554E):  //cmd is = ERUN
            g_Cmd.uint8_CmdID = _IdERUN;
            break;
    
        case (0x474F):      //cmd is = GO
            g_Cmd.uint8_CmdID = _IdGO;
            break;
    
        case (0x53424954):  //cmd is = SBIT
            g_Cmd.uint8_CmdID = _IdSBIT;
            break;
    
        case (0x43424954):  //cmd is = CBIT
            g_Cmd.uint8_CmdID = _IdCBIT;
            break;
    
        case (0x47424954):  //cmd is = GBIT
            g_Cmd.uint8_CmdID = _IdGBIT;
            break;
    
        case (0x534F5554):  //cmd is = SOUT
            g_Cmd.uint8_CmdID = _IdSOUT;
            break;
    
        case (0x474F5554):  //cmd is = GOUT
            g_Cmd.uint8_CmdID = _IdGOUT;
            break;
      
        case (0x47494E50):  //cmd is = GINP
            g_Cmd.uint8_CmdID = _IdGINP;
            break;
        
        default:
            
            //from here - we know that the command does not exists or that
            //is longer then 4 letters. As we can make some groupes with the 
            //the second 4 letters, we verify first this and than again the
            //first 4 letters. 
            
            //to save time, first verify if we have something in the 2nd part
            if(g_Cmd.uint32_Cmd2nd4)
            {
                switch(g_Cmd.uint32_Cmd2nd4)
                {
                    case (0x4D):    //letter M?
                        switch(g_Cmd.uint32_Cmd1st4)
                        {
                            case (0x53494C49):  //cmd is = SILIM
                                g_Cmd.uint8_CmdID = _IdSILIM;
                                break;
            
                            case (0x47494C49):  //cmd is = GILIM
                                g_Cmd.uint8_CmdID = _IdGILIM;
                                break;
            
                            default:  //error - command is not defined
                                g_Cmd.uint8_CmdID = _IdNotAllow;        //clear the command ID
                                g_Param.uint8_ErrCode = _UnknownCmd;    //set error code
                                break;
                        }
                        break;
        
                    case (0x54494E):    //letters TIN?
                        switch(g_Cmd.uint32_Cmd1st4)
                        {
                            case (0x45544553):  //cmd is = ETESTIN
                                g_Cmd.uint8_CmdID = _IdETESTIN;
                                break;
            
                            default:  //error - command is not defined
                                g_Cmd.uint8_CmdID = _IdNotAllow;        //clear the command ID
                                g_Param.uint8_ErrCode = _UnknownCmd;    //set error code
                                break;
                        }
                        break;
        
                    case (0x43):    //letter C?
                        switch(g_Cmd.uint32_Cmd1st4)
                        {
                            case (0x53524143):  //cmd is = SRACC
                                g_Cmd.uint8_CmdID = _IdSRACC;
                                break;
            
                            case (0x53524445):  //cmd is = SRDEC
                                g_Cmd.uint8_CmdID = _IdSRDEC;
                                break;
            
                            case (0x47524143):  //cmd is = GRACC
                                g_Cmd.uint8_CmdID = _IdGRACC;
                                break;
            
                            case (0x47524445):  //cmd is = GRDEC
                                g_Cmd.uint8_CmdID = _IdGRDEC;
                                break;
            
                            case (0x43484B53):  //cmd is = CHKSC
                                g_Cmd.uint8_CmdID = _IdCHKSC;
                                break;
            
                            case (0x53434143):  //cmd is = SCACC
                                g_Cmd.uint8_CmdID = _IdSCACC;
                                break;
              
                            case (0x47434143):  //cmd is = GCACC
                                g_Cmd.uint8_CmdID = _IdGCACC;
                                break;
              
                            case (0x53434445):  //cmd is = SCDEC
                                g_Cmd.uint8_CmdID = _IdSCDEC;
                                break;
            
                            case (0x47434445):  //cmd is = GCDEC
                                g_Cmd.uint8_CmdID = _IdGCDEC;
                                break;
            
                            default:  //error - command is not defined
                                g_Cmd.uint8_CmdID = _IdNotAllow;        //clear the command ID
                                g_Param.uint8_ErrCode = _UnknownCmd;    //set error code
                                break;
                        }
                        break;
          
                    case (0x50):    //letter P?
                        switch(g_Cmd.uint32_Cmd1st4)
                        {
                            case (0x534D5459):  //cmd is = SMTYP
                                g_Cmd.uint8_CmdID = _IdSMTYP;
                                break;
            
                            case (0x474D5459):  //cmd is = GMTYP
                                g_Cmd.uint8_CmdID = _IdGMTYP;
                                break;
            
                            default:  //error - command is not defined
                                g_Cmd.uint8_CmdID = _IdNotAllow;        //clear the command ID
                                g_Param.uint8_ErrCode = _UnknownCmd;    //set error code
                                break;
                        }
                        break;
          
                    case (0x54):    //letter T?
                        switch(g_Cmd.uint32_Cmd1st4)
                        {
                            case (0x53544553):  //cmd is = STEST
                                g_Cmd.uint8_CmdID = _IdSTEST;
                                break;
            
                            case (0x53554D4F):  //cmd is = SUMOT
                                g_Cmd.uint8_CmdID = _IdSUMOT;
                                break;
            
                            case (0x47554D4F):  //cmd is = GUMOT
                                g_Cmd.uint8_CmdID = _IdGUMOT;
                                break;
            
                            case (0x4D554D4F):  //cmd is = MUMOT
                                g_Cmd.uint8_CmdID = _IdMUMOT;
                                break;
            
                            default:  //error - command is not defined
                                g_Cmd.uint8_CmdID = _IdNotAllow;        //clear the command ID
                                g_Param.uint8_ErrCode = _UnknownCmd;    //set error code
                                break;
                        }
                        break;
          
                    case (0x4E):    //letter N?
                        switch(g_Cmd.uint32_Cmd1st4)
                        {
                            case (0x53495255):  //cmd is = SIRUN
                                g_Cmd.uint8_CmdID = _IdSIRUN;
                                break;
            
                            case (0x47495255):  //cmd is = GIRUN
                                g_Cmd.uint8_CmdID = _IdGIRUN;
                                break;
            
                            case (0x53505255):  //cmd is = SPRUN
                                g_Cmd.uint8_CmdID = _IdSPRUN;
                                break;
            
                            case (0x47505255):  //cmd is = GPRUN
                                g_Cmd.uint8_CmdID = _IdGPRUN;
                                break;
            
                            case (0x53455255):  //cmd is = SERUN
                                g_Cmd.uint8_CmdID = _IdSERUN;
                                break;
            
                            case (0x47455255):  //cmd is = GERUN
                                g_Cmd.uint8_CmdID = _IdGERUN;
                                break;
              
                            case (0x53494D41):  //cmd is = SIMAN
                                g_Cmd.uint8_CmdID = _IdSIMAN;
                                break;
              
                            case (0x47494D41):  //cmd is = GIMAN
                                g_Cmd.uint8_CmdID = _IdGIMAN;
                                break;
            
                            default:  //error - command is not defined
                                g_Cmd.uint8_CmdID = _IdNotAllow;        //clear the command ID
                                g_Param.uint8_ErrCode = _UnknownCmd;    //set error code
                                break;
                        }
                        break;
          
                    case (0x4B):    //letter K?
                        switch(g_Cmd.uint32_Cmd1st4)
                        {
                            case (0x42524541):  //cmd is = BREAK
                                g_Cmd.uint8_CmdID = _IdBREAK;
                                break;
            
                            default:  //error - command is not defined
                                g_Cmd.uint8_CmdID = _IdNotAllow;        //clear the command ID
                                g_Param.uint8_ErrCode = _UnknownCmd;    //set error code
                                break;
                        }
                        break;
          
                    case (0x4D4F54):    //letters MOT?
                        switch(g_Cmd.uint32_Cmd1st4)
                        {
                            case (0x43484B55):  //cmd is = CHKUMOT
                                g_Cmd.uint8_CmdID = _IdCHKUMOT;
                                break;
            
                            default:  //error - command is not defined
                                g_Cmd.uint8_CmdID = _IdNotAllow;        //clear the command ID
                                g_Param.uint8_ErrCode = _UnknownCmd;    //set error code
                                break;
                        }
                        break;
          
                    case (0x4C4F4E):    //letters LON?
                        switch(g_Cmd.uint32_Cmd1st4)
                        {
                            case (0x53434F49):  //cmd is = SCOILON
                                g_Cmd.uint8_CmdID = _IdSCOILON;
                                break;
            
                            default:  //error - command is not defined
                                g_Cmd.uint8_CmdID = _IdNotAllow;        //clear the command ID
                                g_Param.uint8_ErrCode = _UnknownCmd;    //set error code
                                break;
                        }
                        break;
          
                    case (0x44):    //letter D?
                        switch(g_Cmd.uint32_Cmd1st4)
                        {
                            case (0x53534D4F):  //cmd is = SSMOD
                                g_Cmd.uint8_CmdID = _IdSSMOD;
                                break;
            
                            case (0x47534D4F):  //cmd is = GSMOD
                                g_Cmd.uint8_CmdID = _IdGSMOD;
                                break;
            
                            default:  //error - command is not defined
                                g_Cmd.uint8_CmdID = _IdNotAllow;      //clear the command ID
                                g_Param.uint8_ErrCode = _UnknownCmd;  //set error code
                                break;
                        }
                        break;
          
                    case (0x535450):    //letters STP?
                        switch(g_Cmd.uint32_Cmd1st4)
                        {
                            case (0x534D4352):  //cmd is = SMCRSTP
                                g_Cmd.uint8_CmdID = _IdSMCRSTP;
                                break;
            
                            case (0x474D4352):  //cmd is = GMCRSTP
                                g_Cmd.uint8_CmdID = _IdGMCRSTP;
                                break;
                            
                            default:  //error - command is not defined
                                g_Cmd.uint8_CmdID = _IdNotAllow;        //clear the command ID
                                g_Param.uint8_ErrCode = _UnknownCmd;    //set error code
                                break;
                        }
                        break;
          
                    case (0x494D):  //letters IM?
                        switch(g_Cmd.uint32_Cmd1st4)
                        {
                            case (0x5353574C):  //cmd is = SSWLIM
                                g_Cmd.uint8_CmdID = _IdSSWLIM;
                                break;
            
                            case (0x4753574C):  //cmd is = GSWLIM
                                g_Cmd.uint8_CmdID = _IdGSWLIM;
                                break;
            
                            default:  //error - command is not defined
                                g_Cmd.uint8_CmdID = _IdNotAllow;        //clear the command ID
                                g_Param.uint8_ErrCode = _UnknownCmd;    //set error code
                                break;
                        }
                        break;
          
                    case (0x4F53):  //letters OS?
                        switch(g_Cmd.uint32_Cmd1st4)
                        {
                            case (0x47535750):  //cmd is = GSWPOS
                                g_Cmd.uint8_CmdID = _IdGSWPOS;
                                break;
            
                            default:  //error - command is not defined
                                g_Cmd.uint8_CmdID = _IdNotAllow;        //clear the command ID
                                g_Param.uint8_ErrCode = _UnknownCmd;    //set error code
                                break;
                        }
                        break;
          
                    case (0x4C494D):    //letters LIM?
                        switch(g_Cmd.uint32_Cmd1st4)
                        {
                            case (0x5350574D):  //cmd is = SPWMLIM
                                g_Cmd.uint8_CmdID = _IdSPWMLIM;
                                break;
            
                            case (0x4750574D):  //cmd is = GPWMLIM
                                g_Cmd.uint8_CmdID = _IdGPWMLIM;
                                break;
            
                            default:  //error - command is not defined
                                g_Cmd.uint8_CmdID = _IdNotAllow;        //clear the command ID
                                g_Param.uint8_ErrCode = _UnknownCmd;    //set error code
                                break;
                        }
                        break;
          
                    case (0x504F53):    //letters POS?
                        switch(g_Cmd.uint32_Cmd1st4)
                        {
                            case (0x4750574D):  //cmd is = GPWMPOS
                                g_Cmd.uint8_CmdID = _IdGPWMPOS;
                                break;
            
                            default:  //error - command is not defined
                                g_Cmd.uint8_CmdID = _IdNotAllow;        //clear the command ID
                                g_Param.uint8_ErrCode = _UnknownCmd;    //set error code
                                break;
                        }
                        break;
          
                    case (0x56414C):    //letters VAL?
                        switch(g_Cmd.uint32_Cmd1st4)
                        {
                            case (0x4750574D):  //cmd is = GPWMVAL
                                g_Cmd.uint8_CmdID = _IdGPWMVAL;
                                break;
              
                            case (0x47465251):  //cmd is GFRQVAL
                                g_Cmd.uint8_CmdID = _IdGFRQVAL;
                                break;
            
                            default:  //error - command is not defined
                                g_Cmd.uint8_CmdID = _IdNotAllow;        //clear the command ID
                                g_Param.uint8_ErrCode = _UnknownCmd;    //set error code
                                break;
                        }
                        break;
          
                    case (0x5950):  //letters YP?
                        switch(g_Cmd.uint32_Cmd1st4)
                        {
                            case (0x53535754):  //cmd is = SSWTYP
                                g_Cmd.uint8_CmdID = _IdSSWTYP;
                                break;
            
                            case (0x47535754):  //cmd is = GSWTYP
                                g_Cmd.uint8_CmdID = _IdGSWTYP;
                                break;
            
                            default:  //error - command is not defined
                                g_Cmd.uint8_CmdID = _IdNotAllow;        //clear the command ID
                                g_Param.uint8_ErrCode = _UnknownCmd;    //set error code
                                break;
                        }
                        break;
          
                    case (0x52554E):    //letters RUN?
                        switch(g_Cmd.uint32_Cmd1st4)
                        {
                            case (0x53504F53):  //cmd is = SPOSRUN
                                g_Cmd.uint8_CmdID = _IdSPOSRUN;
                                break;
            
                            case (0x47504F53):  //cmd is = GPOSRUN
                                g_Cmd.uint8_CmdID = _IdGPOSRUN;
                                break;
            
                            default:  //error - command is not defined
                                g_Cmd.uint8_CmdID = _IdNotAllow;        //clear the command ID
                                g_Param.uint8_ErrCode = _UnknownCmd;    //set error code
                                break;
                        }
                        break;
          
                    case (0x554E):  //letters UN?
                        switch(g_Cmd.uint32_Cmd1st4)
                        {
                            case (0x504F5352):  //cmd is = POSRUN
                                g_Cmd.uint8_CmdID = _IdPOSRUN;
                                break;
            
                            default:  //error - command is not defined
                                g_Cmd.uint8_CmdID = _IdNotAllow;        //clear the command ID
                                g_Param.uint8_ErrCode = _UnknownCmd;    //set error code
                                break;
                        }
                        break;
                        
                    case (0x42):    //letter B?
                        switch(g_Cmd.uint32_Cmd1st4)
                        {
                            case (0x47494E50):  //cmd is = GINPB
                                g_Cmd.uint8_CmdID = _IdGINPB;
                                break;
                                
                            default:    //error - command is not defined
                                g_Cmd.uint8_CmdID = _IdNotAllow;        //clear the command ID
                                g_Param.uint8_ErrCode = _UnknownCmd;    //set error code
                                break;
                        }
                        break;
        
                    default:  //error - command is not defined
                        g_Cmd.uint8_CmdID = _IdNotAllow;        //clear the command ID
                        g_Param.uint8_ErrCode = _UnknownCmd;    //set error code
                        break;
                }
            }
            else
            {
                //error - command is not defined
                g_Cmd.uint8_CmdID = _IdNotAllow;        //clear the command ID
                g_Param.uint8_ErrCode = _UnknownCmd;    //set error code
            }
            
            break;
    }
}   //end of cmd_def


/**********************************************************************************************************************
 * Routine:                 cmd_SILIM

 * Description:
 * Only allowed if the motor is not in run. Verify each parameter if it is within the tolerance and the number of 
 * received parameters are correct. If true then store the parameters. If something is wrong then send back the error. 
 * In the case where the main supply is switched off (uint8_MotTyp = 'N') or the application is set up for unipolar
 * actuators, we only verify if the parameters are within the tolerance. 
 * If true then we store this parameters, if false we send back a feedback to the customer.
 * In the case where the main supply is switched on for bipolar or LIN, we first verify if the parameters
 * are within the application tolerance. If not we send back the corresponding error. 
 * 
 * Creator:                 A. Staub
 * Date of creation:        15.10.2015
 * Last modification on:    -
 * Modified by:             - 
 * 
 * Input:                   -
 * Output:                  -
 * Global variable:         g_Uni.
 *                              - uint8_Settings
 *                          g_Param.
 *                              - uint8_MotTyp
 *                              - uint32_TempPara
 *                              - uint8_ErrCode
 *                          g_Cmd.
 *                              - uint8_ParamPos
***********************************************************************************************************************/
void cmd_SILIM(void)
{
    auto unsigned char uint8_Result = 0;        //local work byte for the result 
  
    if(g_Cmd.uint8_ParamPos == 6)   //number of received characters OK?
    {
        if(g_Uni.uint8_Settings & 0x01)             //is unipolar motor in run mode?
        {
            g_Param.uint8_ErrCode = _MotorInRun;        //set error code
            uart2_SendErrorCode(g_Param.uint8_ErrCode); //call subroutine
        }
        else
        {
            uint8_Result = funct_CheckCmdSILIM();   //call subroutine
            
            if(uint8_Result == 5)   //each parameter within the tolerance?
            {
                //then store the parameters and send back the OK
                g_Param.uint16_Imin = g_Cmd.uint32_TempPara[1] & 0xFFFF;
                g_Param.uint16_Imax = g_Cmd.uint32_TempPara[2] & 0xFFFF;
                g_Param.uint16_Umot = g_Cmd.uint32_TempPara[3] & 0xFFFF;
                g_Param.uint16_SCiLevel = g_Cmd.uint32_TempPara[4] & 0xFFFF;
                g_Param.uint8_SCtLevel = g_Cmd.uint32_TempPara[5] & 0xFF;
      
                uart2_sendbuffer('E');          //first the letter E
                uart2_sendbuffer(13);           //with CR at the end
            }
            else
            {
                uart2_SendErrorCode(g_Param.uint8_ErrCode); //call subroutine
            }  
        }
    }
    else
    {
        g_Param.uint8_ErrCode = _NumbRecCharNotOK;  //set error code
        uart2_SendErrorCode(g_Param.uint8_ErrCode); //call subroutine
    }
}   //end of cmd_SILIM


/**********************************************************************************************************************
 * Routine:                 cmd_ETESTIN

 * Description:
 * Only allowed if the motor is not in run. Send back the:
 * I coil A1, I coil A2, I coil B1, I coil B2, Umot 
 * 
 * Creator:                 A. Staub
 * Date of creation:        19.10.2015
 * Last modification on:    -
 * Modified by:             - 
 * 
 * Input:                   -
 * Output:                  -
 * Global variable:         g_Uni.
 *                              - uint8_Settings
 *                          g_Param.
 *                              - uint8_ErrCode
***********************************************************************************************************************/
void cmd_ETESTIN(void)
{
    if(g_Cmd.uint8_ParamPos == 1)   //number of received characters OK?
    {
        if(g_Uni.uint8_Settings & 0x01)             //is unipolar motor in run mode?
        {
            g_Param.uint8_ErrCode = _MotorInRun;        //set error code
            uart2_SendErrorCode(g_Param.uint8_ErrCode); //call subroutine
        }
        else
        {
            if(g_Param.uint8_MotTyp == 'L')         //motor type = Lin?
            {
                g_Param.uint8_ErrCode = _LinETESTIN;        //set error code
                uart2_SendErrorCode(g_Param.uint8_ErrCode); //call subroutine            
            }
            else if(g_Param.uint8_MotTyp == 'B')    //motor type = Bipolar?
            {
                //starts the measure
            
                //convert the result
            
                //verify if within tolerance
            
                //send back the result
                g_Param.uint8_ErrCode = _BipETESTIN;        //set error code
                uart2_SendErrorCode(g_Param.uint8_ErrCode); //call subroutine
            }
            else if((g_Param.uint8_MotTyp == 'U') || (g_Param.uint8_MotTyp == 'M'))    //motor type = Unipolar or matrix?
            {
                //starts the measure
            
                //convert the result
            
                //verify if within tolerance
            
                //send back the result
                g_Param.uint8_ErrCode = _UniETESTIN;        //set error code
                uart2_SendErrorCode(g_Param.uint8_ErrCode); //call subroutine
            }
            else
            {
                g_Param.uint8_ErrCode = _UnknownMotTyp;        //set error code
                uart2_SendErrorCode(g_Param.uint8_ErrCode); //call subroutine
            }
        }      
    }
    else
    {
        g_Param.uint8_ErrCode = _NumbRecCharNotOK;  //set error code
        uart2_SendErrorCode(g_Param.uint8_ErrCode); //call subroutine
    }
}   //end of cmd_ETESTIN


/**********************************************************************************************************************
 * Routine:                 cmd_SRACC

 * Description:
 * Only allowed if the motor is not in run. First verify if the number of received characters is inside the range.
 * If not, then send back the error.
 * Second verify if the number of received characters is odd-numbered. Because to have a valide result, there must be 
 * always a number of steps with a frequency together and with the command ID at the beginning the size will be
 * 3,6,9,12,15,18,21,24,27,30,33,...,...,...,...,...101 
 * Otherwise send back the error.
 * 
 * Third verify each received parameter with the tolerance until:
 * - all parameters are verified
 * - or the parameter was a comma which is signalized per 0xFFFFFFFF (32-bit)
 * Update the number of received parameters with the number of verified parameters, to not count anymore the parameters 
 * with a comma. Note 1 comma is enough to stop storing the parameters inside the arrays. 
 * So you cannot send SRACC,1,200,2,300,,4,400<CR> 
 * This will accept only 1,200 and 2,300.  
 * 
 * Fourth verify if the result is exactly the number of parameters -1, to be sure that each parameter is within 
 * the tolerance. -1 because we do not count the command ID with. Otherwise send back the error code.   
 * 
 * At the end, first erase the acceleration array. store the new parameters inside, and send back the OK.
 * 
 * Mod. 29.11.2015:
 * The part with the comma check "0xFFFFFFFF" does not really work like JE would at the beginning.
 * I deleted this from the subroutine because this makes not sence to use. Ask me if you need more
 * informations. Now the customer has always to send a pair of parameters (number of steps with a frequency).
 * 
 * Creator:                 A. Staub
 * Date of creation:        19.10.2015
 * Last modification on:    29.11.2015
 * Modified by:             A. Staub
 * 
 * Input:                   -
 * Output:                  -
 * Global variable:         g_Uni.
 *                              - uint8_Settings 
 *                          g_Param.
 *                              - uint8_ErrCode
 *                              - uint8_ParamPos
 *                              - uint32_TempPara
 *                              - uint16_AccNumbStep
 *                              - uint16_AccFreq
 *                              - uint8_AccNumbRamp
***********************************************************************************************************************/
void cmd_SRACC(void)
{
    auto unsigned char uint8_Result = 0;    //local work byte for the result
    auto unsigned char uint8_WB1;           //local work byte for the loops
    auto unsigned char uint8_WB2;           //local work byte to store the results into the array  
  
    if((g_Cmd.uint8_ParamPos > 1) && (g_Cmd.uint8_ParamPos < 102))  //number of received characters inside the range?
    {
        if(g_Cmd.uint8_ParamPos % 2)    //verify if the number of received parameters is odd-numbered (a pair) 
        {
            if(g_Uni.uint8_Settings & 0x01)         //is motor in run mode?
            {
                g_Param.uint8_ErrCode = _MotorInRun;        //set error code
                uart2_SendErrorCode(g_Param.uint8_ErrCode); //call subroutine
            }
            else
            {
                //ToDo:
                //verify each received parameter with the tolerance (start with the 2nd parameter)
                //Until:
                //the local work byte has the same size as number of received characters
                uint8_WB1 = 1;
                do
                {
                    uint8_Result += funct_CheckTol(g_Cmd.uint32_TempPara[uint8_WB1],_AccNumbStepMin,_AccNumbStepMax);
                    uint8_WB1++;                      //increment with 1 to take the next parameter
                    uint8_Result += funct_CheckTol(g_Cmd.uint32_TempPara[uint8_WB1],_AccFreqMin,_AccFreqMax);
                    uint8_WB1++;                      //increment wiht 1 to take the next parameter
                }
                while(uint8_WB1 < g_Cmd.uint8_ParamPos);
      
                //each parameter within the tolerance (uint8_WB1 - 1, because the 1st parameter is the cmd ID)?
                if(uint8_Result == (uint8_WB1-1))
                {
                    //first erase both arrays
                    for(uint8_WB1=0; uint8_WB1<50; uint8_WB1++)
                    {
                        g_Param.uint16_AccNumbStep[uint8_WB1] = 0;
                        g_Param.uint16_AccFreq[uint8_WB1] = 0;
                    }
                    //then store each parameter on the right place (start with the 2nd parameter)
                    uint8_WB1 = 1;
                    uint8_WB2 = 0;
                    do
                    {
                        g_Param.uint16_AccNumbStep[uint8_WB2] = g_Cmd.uint32_TempPara[uint8_WB1] & 0xFFFF;
                        uint8_WB1++;        //increment with 1 to take the next parameter
                        g_Param.uint16_AccFreq[uint8_WB2] = g_Cmd.uint32_TempPara[uint8_WB1] & 0xFFFF;
                        uint8_WB1++;        //increment with 1 to take the next parameter
                        uint8_WB2++;        //increment with 1 to store it on the right position of the array
                    }
                    while(uint8_WB1 < g_Cmd.uint8_ParamPos);
        
                    //to know how many ramps are stored in the array copy the result in the follow variable
                    g_Param.uint8_AccNumbRamp = uint8_WB2;  
        
                    //send back the OK
                    uart2_sendbuffer('E');            //first the letter E
                    uart2_sendbuffer(13);             //then the CR at the end
                }
                //otherwise send back the error code
                else
                {
                    g_Param.uint8_ErrCode = _OutOfTolSRACC;     //set error code
                    uart2_SendErrorCode(g_Param.uint8_ErrCode); //call subroutine
                }
            }        
        }
        else    //otherwise send back the error code
        {
            g_Param.uint8_ErrCode = _NumbRecCharNotOK;  //set error code
            uart2_SendErrorCode(g_Param.uint8_ErrCode); //call subroutine
        }
    }
    else
    {
        g_Param.uint8_ErrCode = _NumbRecCharNotOK;  //set error code
        uart2_SendErrorCode(g_Param.uint8_ErrCode); //call subroutine
    }
}   //end of cmd_SRACC


/**********************************************************************************************************************
 * Routine:                 cmd_SRDEC

 * Description:
 * Only allowed if the motor is not in run. First verify if the number of received characters is inside the range.
 * If not, then send back the error.
 * Second verify if the number of received characters is odd-numbered. Because to have a valide result, there must be 
 * always a number of steps with a frequency together and with the command ID at the beginning the size will be
 * 3,6,9,12,15,18,21,24,27,30,33,...,...,...,...,...101 
 * Otherwise send back the error.
 * 
 * Third verify each received parameter with the tolerance until:
 * - all parameters are verified
 * - or the parameter was a comma which is signalized per 0xFFFFFFFF (32-bit)
 * Update the number of received parameters with the number of verified parameters, to not count anymore the parameters 
 * with a comma. Note 1 comma is enough to stop storing the parameters inside the arrays. 
 * So you cannot send SRDEC,1,200,2,300,,4,400<CR> 
 * This will accept only 1,200 and 2,300.  
 * 
 * Fourth verify if the result is exactly the number of parameters -1, to be sure that each parameter is within 
 * the tolerance. -1 because we do not count the command ID with. Otherwise send back the error code.   
 * 
 * At the end, first erase the acceleration array. store the new parameters inside, and send back the OK.
 * 
 * Mod. 29.11.2015:
 * The part with the comma check "0xFFFFFFFF" does not really work like JE would at the beginning.
 * I deleted this from the subroutine because this makes not sence to use. Ask me if you need more
 * informations. Now the customer has always to send a pair of parameters (number of steps with a frequency).
 * 
 * Creator:                 A. Staub
 * Date of creation:        20.10.2015
 * Last modification on:    29.11.2015
 * Modified by:             A. Staub
 * 
 * Input:                   -
 * Output:                  -
 * Global variable:         g_Uni.
 *                              - uint8_Settings 
 *                          g_Param.
 *                              - uint8_ErrCode
 *                              - uint8_ParamPos
 *                              - uint32_TempPara
 *                              - uint16_DecNumbStep
 *                              - uint16_DecFreq
 *                              - uint8_DecNumbRamp
***********************************************************************************************************************/
void cmd_SRDEC(void)
{
    auto unsigned char uint8_Result = 0;    //local work byte for the result
    auto unsigned char uint8_WB1;           //local work byte for the loops
    auto unsigned char uint8_WB2;           //local work byte to store the results into the array  
    
    if((g_Cmd.uint8_ParamPos > 1) && (g_Cmd.uint8_ParamPos < 102))    //number of received characters OK?
    {
        if(g_Cmd.uint8_ParamPos % 2)    //verify if the number of received parameters is odd-numbered (a pair)
        {
            if(g_Uni.uint8_Settings & 0x01)     //is motor in run mode?
            {
                g_Param.uint8_ErrCode = _MotorInRun;        //set error code
                uart2_SendErrorCode(g_Param.uint8_ErrCode); //call subroutine
            }
            else
            {
                //ToDo:
                //verify each received parameter with the tolerance (start with the 2nd parameter)
                //Until:
                //the local work byte has the same size as number of received characters
                uint8_WB1 = 1;
                do
                {
                    uint8_Result += funct_CheckTol(g_Cmd.uint32_TempPara[uint8_WB1],_DecNumbStepMin,_DecNumbStepMax);
                    uint8_WB1++;                      //increment with 1 to take the next parameter
                    uint8_Result += funct_CheckTol(g_Cmd.uint32_TempPara[uint8_WB1],_DecFreqMin,_DecFreqMax);
                    uint8_WB1++;                      //increment wiht 1 to take the next parameter
                }
                while(uint8_WB1 < g_Cmd.uint8_ParamPos);
      
                //each parameter within the tolerance (uint8_WB1 - 1, because the 1st parameter is the cmd ID)?
                if(uint8_Result == (uint8_WB1-1))
                {
                    //first erase both arrays
                    for(uint8_WB1=0; uint8_WB1<50; uint8_WB1++)
                    {
                        g_Param.uint16_DecNumbStep[uint8_WB1] = 0;
                        g_Param.uint16_DecFreq[uint8_WB1] = 0;
                    }
                    //then store each parameter on the right place (start with the 2nd parameter)
                    uint8_WB1 = 1;
                    uint8_WB2 = 0;
                    do
                    {
                        g_Param.uint16_DecNumbStep[uint8_WB2] = g_Cmd.uint32_TempPara[uint8_WB1] & 0xFFFF;
                        uint8_WB1++;            //increment with 1 to take the next parameter
                        g_Param.uint16_DecFreq[uint8_WB2] = g_Cmd.uint32_TempPara[uint8_WB1] & 0xFFFF;
                        uint8_WB1++;            //increment with 1 to take the next parameter
                        uint8_WB2++;            //increment with 1 to store it on the right position of the array
                    }
                    while(uint8_WB1 < g_Cmd.uint8_ParamPos);
        
                    //to know how many ramps are stored in the array copy the result in the follow variable
                    g_Param.uint8_DecNumbRamp = uint8_WB2; 
        
                    //send back the OK
                    uart2_sendbuffer('E');            //first the letter E
                    uart2_sendbuffer(13);             //then the CR at the end
                }
                //otherwise send back the error code
                else
                {
                    g_Param.uint8_ErrCode = _OutOfTolSRDEC;     //set error code
                    uart2_SendErrorCode(g_Param.uint8_ErrCode); //call subroutine
                }
            }       
        } 
        else    //otherwise send back the error code
        {
            g_Param.uint8_ErrCode = _NumbRecCharNotOK;  //set error code
            uart2_SendErrorCode(g_Param.uint8_ErrCode); //call subroutine
        }
    }
    else
    {
        g_Param.uint8_ErrCode = _NumbRecCharNotOK;  //set error code
        uart2_SendErrorCode(g_Param.uint8_ErrCode); //call subroutine
    }
}   //end of cmd_SRDEC


/**********************************************************************************************************************
 * Routine:                 cmd_RUN

 * Description:
 * This command can only be executed if the motor is not in run, this is the first verification. 
 * Then it verifies if the number of received parameters is correct, otherwise we send back an error code. 
 * This command can only be used for unipolar or bipolar actuators, so if the motor type is LIN or N we send
 * back the corresponding error code. 
 * 
 * Note:
 * Not every parameter inside this command is used for unipolar / matrix actuators. Some are for the bipolar 
 * actuator, but the customer want to use the same command. 
 * 
 * Unipolar / Matrix:
 * Once all parameters are verified we store every received parameter into the corresponding register or 
 * set the corresponding bit for the actuator move. If some definitions are not OK then we send back an error. 
 * All parameters are verified before but if for example the customer wants to execute micro stepping with an 
 * unipolar driver this is still not possible. 
 * The follow parameter are not used for these type of actuators:
 * - BipRunI
 * - BipRunI
 * - BipHoldI
 * - BipAccI
 * - BipDecI
 * Note: 
 * The 'ack' will be not send back inside this routine, because this is a pre-verification. Only if the 
 * 'plausibility check' from the source code inside 'unipolar.c' is verified and ok, we send back the 'ack'. 
 * A second 'ack' will be send back if the 'ack' bit is set true and the actuator reached his position. 
 * 
 * Bipolar:
 * ...
 * 
 * Creator:                 A. Staub
 * Date of creation:        22.10.2015
 * Last modification on:    -
 * Modified by:             - 
 * 
 * Input:                   -
 * Output:                  -
 * Global variable:         g_Uni.
 *                              - uint8_Settings
 *                              - uint32_GoalPos
 *                              - uint16_RunFreq
 *                              - uint16_SwOnLastTime
 *                              - uint16_SwOnCount
 *                              - uint16_SwOffLastTime
 *                              - uint16_SwOffCount
 *                              - uint16_LastTime
 *                              - uint16_Count
 *                          g_Timer2.
 *                              - uint16_IntTime
 *                          g_Param.
 *                              - uint8_ErrCode
 *                              - uint8_StepMode 
 *                              - uint32_StepCount
 *                              - uint16_RunFreq 
 *                              - uint8_Direction 
 *                              - uint8_CoilState 
 *                              - uint16_BipRunI 
 *                              - uint16_BipHoldI 
 *                              - uint8_SelectRamp 
 *                              - uint16_BipAccI
 *                              - uint16_BipDecI 
 *                              - uint16_AccOnDelay
 *                              - uint16_DecOffDelay 
 *                              - uint8_Acknowledge 
 *                          g_Cmd.
 *                              - uint8_ParamPos
 *                              - uint32_TempPara
***********************************************************************************************************************/
void cmd_RUN(void)
{
    auto unsigned char uint8_Result = 0;    //local work byte for the result 
    
    if(g_Cmd.uint8_ParamPos == 15)  //number of received characters OK?
    {
        if(g_Uni.uint8_Settings & 0x01)         //is motor in run mode?
        {
            g_Param.uint8_ErrCode = _MotorInRun;        //set error code
            uart2_SendErrorCode(g_Param.uint8_ErrCode); //call subroutine
        }
        else
        {
            uint8_Result += funct_CheckTol(g_Cmd.uint32_TempPara[2],_StepModeUniMatMin,_StepModeUniMatMax);
            uint8_Result += funct_CheckTol(g_Cmd.uint32_TempPara[3],_StepCountMin,_StepCountMax);
            uint8_Result += funct_CheckTol(g_Cmd.uint32_TempPara[4],_RunFreqMin,_RunFreqMax);      
            uint8_Result += funct_CheckTol(g_Cmd.uint32_TempPara[5],_DirMin,_DirMax);
            uint8_Result += funct_CheckTol(g_Cmd.uint32_TempPara[6],_CoilStateMin,_CoilStateMax);
            uint8_Result += funct_CheckTol(g_Cmd.uint32_TempPara[7],_BipRunIMin,_BipRunIMax);
            uint8_Result += funct_CheckTol(g_Cmd.uint32_TempPara[8],_BipHoldIMin,_BipHoldIMax);
            uint8_Result += funct_CheckTol(g_Cmd.uint32_TempPara[9],_SelectRampMin,_SelectRampMax);
            uint8_Result += funct_CheckTol(g_Cmd.uint32_TempPara[10],_BipAccIMin,_BipAccIMax);
            uint8_Result += funct_CheckTol(g_Cmd.uint32_TempPara[11],_BipDecIMin,_BipDecIMax);
            uint8_Result += funct_CheckTol(g_Cmd.uint32_TempPara[12],_AccOnDelayMin,_AccOnDelayMax);
            uint8_Result += funct_CheckTol(g_Cmd.uint32_TempPara[13],_DecOffDelayMin,_DecOffDelayMax);
            uint8_Result += funct_CheckTol(g_Cmd.uint32_TempPara[14],_AckMin,_AckMax);
            
            if(uint8_Result == 13)      //each parameter within the tolerance?
            {
                //then store the parameters
                g_Param.uint8_StepMode = g_Cmd.uint32_TempPara[2] & 0xFF;
                g_Param.uint32_StepCount = g_Cmd.uint32_TempPara[3] & 0xFFFFFFFF;
                g_Param.uint16_RunFreq = g_Cmd.uint32_TempPara[4] & 0xFFFF;
                g_Param.uint8_Direction = g_Cmd.uint32_TempPara[5] & 0xFF;
                g_Param.uint8_CoilState = g_Cmd.uint32_TempPara[6] & 0xFF;
                g_Param.uint16_BipRunI = g_Cmd.uint32_TempPara[7] & 0xFFFF;
                g_Param.uint16_BipHoldI = g_Cmd.uint32_TempPara[8] & 0xFFFF;
                g_Param.uint8_SelectRamp = g_Cmd.uint32_TempPara[9] & 0xFF;
                g_Param.uint16_BipAccI = g_Cmd.uint32_TempPara[10] & 0xFFFF;
                g_Param.uint16_BipDecI = g_Cmd.uint32_TempPara[11] & 0xFFFF;
                g_Param.uint16_AccOnDelay = g_Cmd.uint32_TempPara[12] & 0xFFFF;
                g_Param.uint16_DecOffDelay = g_Cmd.uint32_TempPara[13] & 0xFFFF;
                g_Param.uint8_Acknowledge = g_Cmd.uint32_TempPara[14] & 0xFF;
                
                //before continue verify type of motor to know what is to do!
                if(g_Cmd.uint32_TempPara[1] == 'N') //type of motor = None?
                {
                    g_Param.uint8_ErrCode = _MotTypRUN;         //set error code
                    uart2_SendErrorCode(g_Param.uint8_ErrCode); //call subroutine
                }
                else if(g_Cmd.uint32_TempPara[1] == 'L')    //type of motor = Lin?
                {
                    g_Param.uint8_ErrCode = _LinRUN;            //set error code
                    uart2_SendErrorCode(g_Param.uint8_ErrCode); //call subroutine                
                }
                else if(g_Cmd.uint32_TempPara[1] == 'B')    //type of motor = Bipolar?
                {
                    //to decide what is to do when motor type changes
                    g_Param.uint8_MotTyp = g_Cmd.uint32_TempPara[1] & 0xFF;
                }
                //type of motor = unipolar or matrix?
                else if((g_Cmd.uint32_TempPara[1] == 'U') || (g_Cmd.uint32_TempPara[1] == 'M'))
                {
                    //to decide what is to do when motor type changes
                    g_Param.uint8_MotTyp = g_Cmd.uint32_TempPara[1] & 0xFF;    
                           
                    g_Uni.uint8_Settings |= 0x01;   //enable run bit
          
                    if(g_Param.uint8_StepMode == 0) //which step mode?
                    {
                        g_Uni.uint8_Settings &= 0xFB;   //enable full step
                        g_Uni.uint8_Settings &= 0xBF;   //one phase ON
                    }
                    else if(g_Param.uint8_StepMode == 1)
                    {
                        g_Uni.uint8_Settings &= 0xFB;   //enable full step
                        g_Uni.uint8_Settings |= 0x40;   //two phase ON
                    }
                    else if(g_Param.uint8_StepMode == 2)
                    {
                        g_Uni.uint8_Settings |= 0x04;   //enable half step
                    }
                    else
                    {
                        //error - definition is half step compensated or micro step send error later
                        g_Uni.uint8_Settings = 0;   //erase settings
                    }
          
                    g_Uni.uint32_GoalPos = g_Param.uint32_StepCount;    //store step count into goal position         
                    g_Uni.uint16_RunFreq = g_Param.uint16_RunFreq;      //store the run frequency
                 
                    if(g_Param.uint8_Direction)     //define direction
                    {
                        g_Uni.uint8_Settings &= 0xEF;   //enable CCW
                    }
                    else
                    {
                        g_Uni.uint8_Settings |= 0x10;   //enable CW
                    }
          
                    if(g_Param.uint8_CoilState)     //define coil state
                    {
                        g_Uni.uint8_Settings |= 0x08;   //enable coil current
                    }
                    else
                    {
                        g_Uni.uint8_Settings &= 0xF7;   //disable coil current
                    }
          
                    if(g_Param.uint8_SelectRamp == 0)   //ramp disabled?
                    {
                        g_Uni.uint8_Settings &= 0xFD;   //disable ramp function
                    }
                    else if(g_Param.uint8_SelectRamp == 1)  //only acceleration?
                    {
                        g_Uni.uint8_Settings |= 0x02;   //enable ramp function
                        g_Uni.uint8_Settings &= 0xDF;   //only acceleration
                    }
                    else if(g_Param.uint8_SelectRamp == 2)  //both ramps active?
                    {
                        g_Uni.uint8_Settings |= 0x02;   //enable ramp function
                        g_Uni.uint8_Settings |= 0x20;   //both ramps acceleration and decelration
                    }
                    else
                    {
                        //error - definition is unknown send error later
                        g_Uni.uint8_Settings = 0;   //erase settings
                    }
          
                    //define switch ON time - first convert and then store it
                    funct_msToTimer2(g_Param.uint16_AccOnDelay,g_Timer2.uint16_IntTime);
                    g_Uni.uint16_SwOnLastTime = g_Uni.uint16_LastTime;
                    g_Uni.uint16_SwOnCount = g_Uni.uint16_Count;
          
                    //define switch OFF time - first convert and then store it
                    funct_msToTimer2(g_Param.uint16_DecOffDelay,g_Timer2.uint16_IntTime);
                    g_Uni.uint16_SwOffLastTime = g_Uni.uint16_LastTime;
                    g_Uni.uint16_SwOffCount = g_Uni.uint16_Count;
          
                    if(g_Uni.uint8_Settings)    //verify if settings are 0, this means there was something wrong
                    {
                        //nothing wrong, nothing to do
                    }
                    else
                    {
                        g_Param.uint8_ErrCode = _OutOfTolRUN;       //set error code
                        uart2_SendErrorCode(g_Param.uint8_ErrCode); //call subroutine
                    }      
                }
                else
                {
                    g_Param.uint8_ErrCode = _UnknownMotTyp;     //set error code
                    uart2_SendErrorCode(g_Param.uint8_ErrCode); //call subroutine
                }
            }
            else
            {
                g_Param.uint8_ErrCode = _OutOfTolRUN;       //set error code
                uart2_SendErrorCode(g_Param.uint8_ErrCode); //call subroutine
            }
        }      
    }
    else
    {
        g_Param.uint8_ErrCode = _NumbRecCharNotOK;  //set error code
        uart2_SendErrorCode(g_Param.uint8_ErrCode); //call subroutine
    }
}   //end of cmd_RUN


/**********************************************************************************************************************
 * Routine:                 cmd_SPHC

 * Description:
 * Used for bipolar actuators to set the current percentage used in half step mode current compensated.
 * 
 * Creator:                 A. Staub
 * Date of creation:        25.10.2015
 * Last modification on:    -
 * Modified by:             - 
 * 
 * Input:                   -
 * Output:                  -
 * Global variable:         g_Cmd.
 *                              - uint8_ParamPos
 *                              - uint32_TempPara
 *                          g_Param.
 *                              - uint8_BipIPct
 *                              - uint8_ErrCode
***********************************************************************************************************************/
void cmd_SPHC(void)
{
    auto unsigned char uint8_Result = 0;    //local work byte for the result 
    
    if(g_Cmd.uint8_ParamPos == 2)   //number of received characters OK?
    {
        //then verify the received parameters
        uint8_Result += funct_CheckTol(g_Cmd.uint32_TempPara[1],_BipIPctMin,_BipIPctMax);
    
        if(uint8_Result == 1)         //each parameter within the tolerance?
        {   
            //then store the parameters and send back the OK 
            g_Param.uint8_BipIPct = g_Cmd.uint32_TempPara[1] & 0xFF;      
            uart2_sendbuffer('E');
            uart2_sendbuffer(13);
        }
        else
        { 
            g_Param.uint8_ErrCode = _BipSPHC;           //set error code
            uart2_SendErrorCode(g_Param.uint8_ErrCode); //call subroutine
        }
    }
    else
    {
        g_Param.uint8_ErrCode = _NumbRecCharNotOK;  //set error code
        uart2_SendErrorCode(g_Param.uint8_ErrCode); //call subroutine
    }
}   //end of cmd_SPHC


/**********************************************************************************************************************
 * Routine:                 cmd_GVER

 * Description:
 * This subroutine sends back the information of the REFAST device.
 * Mod. 29.11.2015:
 * I modified the subroutine that the customer can read out the version even if the actuator
 * is executing steps. 
 * 
 * Creator:                 A. Staub
 * Date of creation:        10.10.2015
 * Last modification on:    29.11.2015
 * Modified by:             A. Staub
 * 
 * Input:                   -
 * Output:                  -
 * Global variable:         g_Uni.
 *                              - uint8_Settings
 *                          g_Param.
 *                              - uint8_ErrCode
***********************************************************************************************************************/
void cmd_GVER(void)
{
    if(g_Cmd.uint8_ParamPos == 1)   //number of received characters OK?
    {
        funct_LoadDeviceInfo();     //call subroutine      
    }
    else
    {
        g_Param.uint8_ErrCode = _NumbRecCharNotOK;  //set error code
        uart2_SendErrorCode(g_Param.uint8_ErrCode); //call subroutine
    }
}   //end of cmd_GVER


/**********************************************************************************************************************
 * Routine:                 cmd_RAZ

 * Description:
 * This subroutine reset all to default value, switch off the outputs and sends back the information 
 * of the REFAST device.
 * 
 * Creator:                 A. Staub
 * Date of creation:        19.10.2015
 * Last modification on:    -
 * Modified by:             - 
 * 
 * Input:                   -
 * Output:                  -
 * Global variable:         g_Uni.
 *                              - uint8_Settings
 *                          g_Param.
 *                              - uint8_ErrCode
***********************************************************************************************************************/
void cmd_RAZ(void)
{
    if(g_Cmd.uint8_ParamPos == 1)   //number of received characters OK?
    {
        if(g_Uni.uint8_Settings & 0x01)                 //is motor in run mode?
        {
            g_Param.uint8_ErrCode = _MotorInRun;        //set error code
            uart2_SendErrorCode(g_Param.uint8_ErrCode); //call subroutine
        }
        else
        {
            //send back a first quittance
            uart2_sendbuffer('E');
            uart2_sendbuffer(13);
        
            //set all back to the default value
        
            //switch off the output's, etc.
        
            //send back the device information's
            funct_LoadDeviceInfo();                     //call subroutine      
        }      
    }
    else
    {
        g_Param.uint8_ErrCode = _NumbRecCharNotOK;  //set error code
        uart2_SendErrorCode(g_Param.uint8_ErrCode); //call subroutine
    }
}   //end of cmd_RAZ


/**********************************************************************************************************************
 * Routine:                 cmd_SMTYP

 * Description:
 * Only allowed if the motor is not in run. Before the system changes to another driver (switching on the
 * output/supply) it verifies if the set limits are within the tolerances. If one parameter/limit is not
 * within the tolerance, the system changes nothing and send back the corresponding error. 
 * If all is correct, it sends back an E<CR>. 
 * 
 * Creator:                 A. Staub
 * Date of creation:        19.10.2015
 * Last modification on:    -
 * Modified by:             - 
 * 
 * Input:                   -
 * Output:                  -
 * Global variable:         g_Uni.
 *                              - uint8_Settings
 *                          g_Param.
 *                              - uint8_MotTyp
 *                              - uint32_TempPara
 *                              - uint8_ErrCode
 *                          g_Cmd.
 *                              - uint8_ParamPos
***********************************************************************************************************************/
void cmd_SMTYP(void)
{
    auto unsigned char uint8_Result = 0;        //local work byte for the result 
  
    if(g_Cmd.uint8_ParamPos == 2)   //number of received characters OK?
    {
        if(g_Uni.uint8_Settings & 0x01)             //is unipolar motor in run mode?
        {
            g_Param.uint8_ErrCode = _MotorInRun;        //set error code
            uart2_SendErrorCode(g_Param.uint8_ErrCode); //call subroutine
        }
        else
        {
            uint8_Result = funct_CheckCmdSMTYP();   //call subroutine
            
            if(uint8_Result == 5)   //each parameter within the tolerance?
            {
                uart2_sendbuffer('E');          //first the letter E
                uart2_sendbuffer(13);           //with CR at the end
            }
            else
            {
                uart2_SendErrorCode(g_Param.uint8_ErrCode); //call subroutine
            }                  
        }      
    }
    else
    {
        g_Param.uint8_ErrCode = _NumbRecCharNotOK;  //set error code
        uart2_SendErrorCode(g_Param.uint8_ErrCode); //call subroutine
    }
}   //end of cmd_SMTYP


/**********************************************************************************************************************
 * Routine:                 cmd_GMTYP

 * Description:
 * Sends back the currently set motor type. 
 * 
 * Creator:                 A. Staub
 * Date of creation:        19.10.2015
 * Last modification on:    -
 * Modified by:             - 
 * 
 * Input:                   -
 * Output:                  -
 * Global variable:         g_Uni.
 *                              - uint8_Settings
 *                          g_Param.
 *                              - uint8_MotTyp
 *                              - uint32_TempPara
 *                              - uint8_ErrCode
 *                          g_Cmd.
 *                              - uint8_ParamPos
***********************************************************************************************************************/
void cmd_GMTYP(void)
{
    if(g_Cmd.uint8_ParamPos == 1)   //number of received characters OK?
    {
        uart2_sendbuffer('E');                    //first the letter E
        uart2_sendbuffer(',');                    //then the comma
        uart2_sendbuffer(g_Param.uint8_MotTyp);   //add the motor type
        uart2_sendbuffer(13);                     //add the CR at the end      
    }
    else
    {
        g_Param.uint8_ErrCode = _NumbRecCharNotOK;  //set error code
        uart2_SendErrorCode(g_Param.uint8_ErrCode); //call subroutine
    }
}   //end of cmd_GMTYP


/**********************************************************************************************************************
 * Routine:                 cmd_STEST

 * Description:
 * Perform a self-test sequence only if possible if there is nothing activated, running, etc. 
 * 
 * Creator:                 A. Staub
 * Date of creation:        25.10.2015
 * Last modification on:    -
 * Modified by:             - 
 * 
 * Input:                   -
 * Output:                  -
 * Global variable:         -
***********************************************************************************************************************/
void cmd_STEST(void)
{  
    if(g_Cmd.uint8_ParamPos == 1)   //number of received characters OK?
    {
        //to define!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    
        uart2_sendbuffer('E');                      //first the letter E
        uart2_sendbuffer(13);                       //add the CR at the end      
    }
    else
    {
        g_Param.uint8_ErrCode = _NumbRecCharNotOK;  //set error code
        uart2_SendErrorCode(g_Param.uint8_ErrCode); //call subroutine
    }
}   //end of cmd_STEST


/**********************************************************************************************************************
 * Routine:                 cmd_GILIM

 * Description:
 * Sends back the currently set limits.  
 * Mod. 29.11.2015:
 * I changed the routine for that the customer can read out the set limits even if the actuator
 * is executing a move. 
 * 
 * Creator:                 A. Staub
 * Date of creation:        25.10.2015
 * Last modification on:    29.11.2015
 * Modified by:             A. Staub
 * 
 * Input:                   -
 * Output:                  -
 * Global variable:         g_Uni
 *                              - uint8_Settings
 *                          g_Param.
 *                              - uint8_ErrCode
 *                              - uint16_Imin
 *                              - uint16_Imax
 *                              - uint16_Umot
 *                              - uint16_SCiLevel
 *                              - uint8_SCtLevel
***********************************************************************************************************************/
void cmd_GILIM(void)
{
    if(g_Cmd.uint8_ParamPos == 1)   //number of received characters OK?
    {
        //send back the needed informations
        uart2_sendbuffer('E');                  //first the letter E
        uart2_sendbuffer(',');                  //add the comma
  
        //convert the minimal ON current into an ascii and store it into the send buffer
        funct_IntToAscii(g_Param.uint16_Imin,_Active);
        uart2_sendbuffer(',');                  //add the comma
  
        //convert the maximal ON current into an ascii and store it into the send buffer
        funct_IntToAscii(g_Param.uint16_Imax,_Active);
        uart2_sendbuffer(',');                  //add the comma
  
        //convert the test voltage into an ascii and store it into the send buffer
        funct_IntToAscii(g_Param.uint16_Umot,_Active);
        uart2_sendbuffer(',');                  //add the comma
  
        //convert the short-circuit current limit into an ascii and store it into the send buffer
        funct_IntToAscii(g_Param.uint16_SCiLevel,_Active);
        uart2_sendbuffer(',');                  //add the comma
  
        //convert the short-circuit wait time into an ascii and store it into the send buffer
        funct_IntToAscii(g_Param.uint8_SCtLevel,_Active);
        uart2_sendbuffer(13);                   //add the CR at the end      
    }
    else
    {
        g_Param.uint8_ErrCode = _NumbRecCharNotOK;  //set error code
        uart2_SendErrorCode(g_Param.uint8_ErrCode); //call subroutine
    }   
}   //end of cmd_GILIM


/**********************************************************************************************************************
 * Routine:                 cmd_GPHC

 * Description:
 * Sends back the percentage of the current for bipolar actuators in half step mode current compensated if there is 
 * nothing activated, running, etc. 
 * 
 * Creator:                 A. Staub
 * Date of creation:        25.10.2015
 * Last modification on:    -
 * Modified by:             - 
 * 
 * Input:                   -
 * Output:                  -
 * Global variable:         g_Uni
 *                              - uint8_Settings
 *                          g_Param.
 *                              - uint8_ErrCode
 *                              - uint8_BipIPct
***********************************************************************************************************************/
void cmd_GPHC(void)
{  
    if(g_Cmd.uint8_ParamPos == 1)   //number of received characters OK?
    {
        if(g_Uni.uint8_Settings & 0x01) //is motor in run mode?
        {
            g_Param.uint8_ErrCode = _MotorInRun;        //set error code
            uart2_SendErrorCode(g_Param.uint8_ErrCode); //call subroutine
        }
        else
        {
            //send back the needed informations
            uart2_sendbuffer('E');                  //first the letter E
            uart2_sendbuffer(',');                  //add the comma
  
            //convert the percentage from the half step compensation into an ascii and store it into the send buffer
            funct_IntToAscii(g_Param.uint8_BipIPct,_Active);
            uart2_sendbuffer(13);                   //add the CR at the end
        }      
    }
    else
    {
        g_Param.uint8_ErrCode = _NumbRecCharNotOK;  //set error code
        uart2_SendErrorCode(g_Param.uint8_ErrCode); //call subroutine
    }
}   //end of cmd_GPHC


/**********************************************************************************************************************
 * Routine:                 cmd_GRACC

 * Description:
 * Sends back the acceleration ramp if there is nothing activated, running, etc. 
 * This can be a long protocol to send back - maximal 50 times number of steps and 50 times the corresponding frequency.  
 * Note - the send buffer needs a minimum size of:
 * - uint16_AccNumbStep max character length = 5 x 50 different steps = 250
 * - uint16_AccFreq max character lenght = 4 x 50 different frequency = 200
 * - Each parameter separated by a comma = 100 x 1 character = 100
 * - Started with the 'E' and ended with 'CR' = 2
 * total size = 552 bytes.  
 * 
 * Creator:                 A. Staub
 * Date of creation:        25.10.2015
 * Last modification on:    -
 * Modified by:             - 
 * 
 * Input:                   -
 * Output:                  -
 * Global variable:         g_Uni
 *                              - uint8_Settings
 *                          g_Param.
 *                              - uint8_ErrCode
 *                              - uint16_AccNumbStep
 *                              - uint16_AccFreq
***********************************************************************************************************************/
void cmd_GRACC(void)
{  
    auto unsigned char uint8_WB;              //local work byte
    
    if(g_Cmd.uint8_ParamPos == 1)   //number of received characters OK?
    {
        if(g_Uni.uint8_Settings & 0x01) //is motor in run mode?
        {
            g_Param.uint8_ErrCode = _MotorInRun;        //set error code
            uart2_SendErrorCode(g_Param.uint8_ErrCode); //call subroutine
        }
        else
        {
            //send back the needed informations
            uart2_sendbuffer('E');                      //first the letter E
           
            for(uint8_WB=0; uint8_WB<50; uint8_WB++)    //read out both arrays
            {
                //is one of the next parameter = 0?
                if((g_Param.uint16_AccNumbStep[uint8_WB] == 0) || (g_Param.uint16_AccFreq[uint8_WB] == 0))
                {
                    uint8_WB = 50;                      //then leave the loop
                }
                else    //otherwise send the parameter back
                {
                    uart2_sendbuffer(',');              //add the comma
    
                    //convert the number of steps into an ascii and store it into the send buffer
                    funct_IntToAscii(g_Param.uint16_AccNumbStep[uint8_WB],_Active);
                    uart2_sendbuffer(',');              //add the comma
    
                    //convert the frequency into an ascii and store it into the send buffer
                    funct_IntToAscii(g_Param.uint16_AccFreq[uint8_WB],_Active);        
                }
            }
            uart2_sendbuffer(13);                       //add the CR at the end       
        }      
    }
    else
    {
        g_Param.uint8_ErrCode = _NumbRecCharNotOK;  //set error code
        uart2_SendErrorCode(g_Param.uint8_ErrCode); //call subroutine
    }
}   //end of cmd_GRACC


/**********************************************************************************************************************
 * Routine:                 cmd_GRDEC

 * Description:
 * Sends back the deceleration ramp if there is nothing activated, running, etc. 
 * This can be a long protocol to send back - maximal 50 times number of steps and 50 times the corresponding frequency.  
 * Note - the send buffer needs a minimum size of:
 * - uint16_DecNumbStep max character length = 5 x 50 different steps = 250
 * - uint16_DecFreq max character lenght = 4 x 50 different frequency = 200
 * - Each parameter separated by a comma = 100 x 1 character = 100
 * - Started with the 'E' and ended with 'CR' = 2
 * total size = 552 bytes.  
 * 
 * Creator:                 A. Staub
 * Date of creation:        25.10.2015
 * Last modification on:    -
 * Modified by:             - 
 * 
 * Input:                   -
 * Output:                  -
 * Global variable:         g_Uni
 *                              - uint8_Settings
 *                          g_Param.
 *                              - uint8_ErrCode
 *                              - uint16_DecNumbStep
 *                              - uint16_DecFreq
***********************************************************************************************************************/
void cmd_GRDEC(void)
{  
    auto unsigned char uint8_WB;              //local work byte
    
    if(g_Cmd.uint8_ParamPos == 1)   //number of received characters OK?
    {
        if(g_Uni.uint8_Settings & 0x01) //is motor in run mode?
        {
            g_Param.uint8_ErrCode = _MotorInRun;        //set error code
            uart2_SendErrorCode(g_Param.uint8_ErrCode); //call subroutine
        }
        else
        {
            //send back the needed informations
            uart2_sendbuffer('E');                      //first the letter E
           
            for(uint8_WB=0; uint8_WB<50; uint8_WB++)    //read out both arrays
            {
                //is one of the next parameter = 0?
                if((g_Param.uint16_DecNumbStep[uint8_WB] == 0) || (g_Param.uint16_DecFreq[uint8_WB] == 0))
                {
                    uint8_WB = 50;                      //then leave the loop
                }
                else    //otherwise send the parameter back
                {
                    uart2_sendbuffer(',');              //add the comma
    
                    //convert the number of steps into an ascii and store it into the send buffer
                    funct_IntToAscii(g_Param.uint16_DecNumbStep[uint8_WB],_Active);
                    uart2_sendbuffer(',');              //add the comma
    
                    //convert the frequency into an ascii and store it into the send buffer
                    funct_IntToAscii(g_Param.uint16_DecFreq[uint8_WB],_Active);        
                }
            }
            uart2_sendbuffer(13);                       //add the CR at the end       
        }      
    }
    else
    {
        g_Param.uint8_ErrCode = _NumbRecCharNotOK;  //set error code
        uart2_SendErrorCode(g_Param.uint8_ErrCode); //call subroutine
    }
}   //end of cmd_GRDEC


/**********************************************************************************************************************
 * Routine:                 cmd_GRUN

 * Description:
 * Send back the run parameter like:
 * motor type, step mode, step count, speed, direction, coil state, Irun, Ihold, selected ramp, ACC I chopper,
 * DEC I chopper, ACC ON delay, DEC OFF delay, protocol
 * 
 * Creator:                 A. Staub
 * Date of creation:        26.10.2015
 * Last modification on:    -
 * Modified by:             - 
 * 
 * Input:                   -
 * Output:                  -
 * Global variable:         g_Param.
 *                              - uint8_MotTyp
 *                              - uint8_StepMode
 *                              - uint32_StepCount
 *                              - uint16_RunFreq
 *                              - uint8_Direction
 *                              - uint8_CoilState
 *                              - uint16_BipRunI
 *                              - uint16_BipHoldI
 *                              - uint8_SelectRamp
 *                              - uint16_BipAccI
 *                              - uint16_BipDecI
 *                              - uint16_AccOnDelay
 *                              - uint16_DecOffDelay
 *                              - uint8_Acknowledge
***********************************************************************************************************************/
void cmd_GRUN(void)
{  
    if(g_Cmd.uint8_ParamPos == 1)   //number of received characters OK?
    {
        //send back the needed informations
        uart2_sendbuffer('E');                      //first the letter E
        uart2_sendbuffer(',');                      //add the comma
        uart2_sendbuffer(g_Param.uint8_MotTyp);     //add the motor type
        uart2_sendbuffer(',');                      //add the comma
  
        //convert the step mode of the actuator into an ascii and store it into the send buffer
        funct_IntToAscii(g_Param.uint8_StepMode,_Active);
        uart2_sendbuffer(',');                      //add the comma
  
        //convert the step count of the actuator into an ascii and store it into the send buffer
        funct_IntToAscii(g_Param.uint32_StepCount,_Active);
        uart2_sendbuffer(',');                      //add the comma 
  
        //convert the speed into an ascii and store it into the send buffer
        funct_IntToAscii(g_Param.uint16_RunFreq,_Active);
        uart2_sendbuffer(',');                      //add the comma
  
        //convert the direction into an ascii and store it into the send buffer
        funct_IntToAscii(g_Param.uint8_Direction,_Active);
        uart2_sendbuffer(',');                      //add the comma
  
        //convert the coil state into an ascii and store it into the send buffer
        funct_IntToAscii(g_Param.uint8_CoilState,_Active);
        uart2_sendbuffer(',');                      //add the comma
  
        //convert the current during move into an ascii and store it into the send buffer
        funct_IntToAscii(g_Param.uint16_BipRunI,_Active);
        uart2_sendbuffer(',');                      //add the comma
  
        //convert the "holding" current into an ascii and store it into the send buffer
        funct_IntToAscii(g_Param.uint16_BipHoldI,_Active);
        uart2_sendbuffer(',');                      //add the comma
  
        //convert the selected ramp into an ascii and store it into the send buffer
        funct_IntToAscii(g_Param.uint8_SelectRamp,_Active);
        uart2_sendbuffer(',');                      //add the comma
  
        //convert the curremt for acceleration into an ascii and store it into the send buffer
        funct_IntToAscii(g_Param.uint16_BipAccI,_Active);
        uart2_sendbuffer(',');                      //add the comma
  
        //convert the curremt for deceleration into an ascii and store it into the send buffer
        funct_IntToAscii(g_Param.uint16_BipDecI,_Active);
        uart2_sendbuffer(',');                      //add the comma
 
        //convert the time into an ascii and store it into the send buffer
        funct_IntToAscii(g_Param.uint16_AccOnDelay,_Active);
        uart2_sendbuffer(',');                      //add the comma
  
        //convert the time into an ascii and store it into the send buffer
        funct_IntToAscii(g_Param.uint16_DecOffDelay,_Active);
        uart2_sendbuffer(',');                      //add the comma
  
        //convert the protocol quittance into an ascii and store it into the send buffer
        funct_IntToAscii(g_Param.uint8_Acknowledge,_Active);
        uart2_sendbuffer(13);                       //add the CR at the end      
    }
    else
    {
        g_Param.uint8_ErrCode = _NumbRecCharNotOK;  //set error code
        uart2_SendErrorCode(g_Param.uint8_ErrCode); //call subroutine
    }
}   //end of cmd_GRUN


/**********************************************************************************************************************
 * Routine:                 cmd_BREAK

 * Description:
 * Stops immediately all running systems!!!
 * 
 * Creator:                 A. Staub
 * Date of creation:        26.10.2015
 * Last modification on:    -
 * Modified by:             - 
 * 
 * Input:                   -
 * Output:                  -
 * Global variable:         -
***********************************************************************************************************************/
void cmd_BREAK(void)
{  
    if(g_Cmd.uint8_ParamPos == 1)   //number of received characters OK?
    {
        //add here the conditions!!!!!
        
        //send back the needed informations
        uart2_sendbuffer('E');                  //first the letter E
        uart2_sendbuffer(13);                   //add the CR at the end
        
    }
    else
    {
        g_Param.uint8_ErrCode = _NumbRecCharNotOK;  //set error code
        uart2_SendErrorCode(g_Param.uint8_ErrCode); //call subroutine
    }    
}   //end of cmd_BREAK


/**********************************************************************************************************************
 * Routine:                 cmd_MUMOT

 * Description:
 * Send back the measured entry voltage Ue/Umot. 
 * 
 * Creator:                 A. Staub
 * Date of creation:        26.10.2015
 * Last modification on:    -
 * Modified by:             - 
 * 
 * Input:                   -
 * Output:                  -
 * Global variable:         g_Param.
 *                              - uint16_Ue
***********************************************************************************************************************/
void cmd_MUMOT(void)
{  
    if(g_Cmd.uint8_ParamPos == 1)   //number of received characters OK?
    {
        //measure here the voltage or take the last measured parameter and send it back!
    
        //send back the needed informations
        uart2_sendbuffer('E');                          //first the letter E
        uart2_sendbuffer(',');                          //add the comma
        funct_IntToAscii(g_Param.uint16_Ue,_Active);    //add the voltage
        uart2_sendbuffer(13);                           //add the CR at the end      
    }
    else
    {
        g_Param.uint8_ErrCode = _NumbRecCharNotOK;  //set error code
        uart2_SendErrorCode(g_Param.uint8_ErrCode); //call subroutine
    }
}   //end of cmd_MUMOT


/**********************************************************************************************************************
 * Routine:                 cmd_SCOILON

 * Description:
 * This command can be used to switch on / off manually the coil's. 
 * 
 * Creator:                 A. Staub
 * Date of creation:        26.10.2015
 * Last modification on:    -
 * Modified by:             - 
 * 
 * Input:                   -
 * Output:                  -
 * Global variable:         g_Uni.
 *                              - uint8_Settings
 *                          g_Cmd.
 *                              - uint8_ParamPos
 *                              - uint32_TempPara
 *                              - uint8_ErrCode
***********************************************************************************************************************/
void cmd_SCOILON(void)
{  
    auto unsigned char uint8_Result = 0;    //local work byte
    auto unsigned char uint8_WB;            //local work byte
    
    if(g_Cmd.uint8_ParamPos == 6)   //number of received characters OK?
    {
        if(g_Uni.uint8_Settings & 0x01) //is motor in run mode?
        {
            g_Param.uint8_ErrCode = _MotorInRun;        //set error code
            uart2_SendErrorCode(g_Param.uint8_ErrCode); //call subroutine
        }
        else
        {
            //verify the limits if they are inside the tolerance
            uint8_WB = g_Cmd.uint32_TempPara[1];
            if((uint8_WB == 'N') || (uint8_WB == 'B') || (uint8_WB == 'M') || (uint8_WB == 'U'))
            {
                uint8_Result++;
            }
            else
            {
                //do nothing
            }
            uint8_Result += funct_CheckTol(g_Cmd.uint32_TempPara[2],0,1);
            uint8_Result += funct_CheckTol(g_Cmd.uint32_TempPara[3],0,1);      
            uint8_Result += funct_CheckTol(g_Cmd.uint32_TempPara[4],0,1);
            uint8_Result += funct_CheckTol(g_Cmd.uint32_TempPara[5],0,1);
            
            if(uint8_Result == 5)   //each parameter within the tolerance?
            {              
                //before continue verify type of motor to know what is to do!
                if(g_Cmd.uint32_TempPara[1] == 'N') //type of motor = None?
                {
                    g_Param.uint8_ErrCode = _MotTypSCOILON;     //set error code
                    uart2_SendErrorCode(g_Param.uint8_ErrCode); //call subroutine
                }
                else if(g_Cmd.uint32_TempPara[1] == 'L')    //type of motor = Lin?
                {
                    g_Param.uint8_ErrCode = _LinSCOILON;        //set error code
                    uart2_SendErrorCode(g_Param.uint8_ErrCode); //call subroutine                
                }
                else if(g_Cmd.uint32_TempPara[1] == 'B')    //type of motor = Bipolar?
                {
                    //switch off/on the inputs here
                }
                //type of motor = unipolar or matrix?
                else if((g_Cmd.uint32_TempPara[1] == 'U') || (g_Cmd.uint32_TempPara[1] == 'M'))
                {
                    //switch off/on the inputs here
                }
                else
                {
                    g_Param.uint8_ErrCode = _UnknownMotTyp;     //set error code
                    uart2_SendErrorCode(g_Param.uint8_ErrCode); //call subroutine
                }
            }  
            else
            {
                g_Param.uint8_ErrCode = _OutOfTolSCOILON;   //set error code
                uart2_SendErrorCode(g_Param.uint8_ErrCode); //call subroutine
            }
        }      
    }
    else
    {
        g_Param.uint8_ErrCode = _NumbRecCharNotOK;  //set error code
        uart2_SendErrorCode(g_Param.uint8_ErrCode); //call subroutine
    }
}   //end of cmd_SCOILON


/**********************************************************************************************************************
 * Routine:                 cmd_SSMOD

 * Description:
 * This command can be used to define the step mode for the application. 
 * 
 * Creator:                 A. Staub
 * Date of creation:        28.11.2015
 * Last modification on:    -
 * Modified by:             - 
 * 
 * Input:                   -
 * Output:                  -
 * Global variable:         g_Param.
 *                              - uint8_StepMode
 *                              - uint8_ErrCode
 *                          g_Uni.
 *                              - uint8_Settings
 *                          g_Cmd.
 *                              - uint8_ParamPos
 *                              - uint32_TempPara
***********************************************************************************************************************/
void cmd_SSMOD(void)
{
    if(g_Cmd.uint8_ParamPos == 2)   //number of received characters OK?
    {
        if(g_Uni.uint8_Settings & 0x01)             //is unipolar motor in run mode?
        {
            g_Param.uint8_ErrCode = _MotorInRun;        //set error code
            uart2_SendErrorCode(g_Param.uint8_ErrCode); //call subroutine
        }
        else
        {
            switch(g_Cmd.uint32_TempPara[1])    //verify mode of step
            {
                case (0):       //full step one phase on
                    g_Param.uint8_StepMode = 0;
                    g_Uni.uint8_Settings &= 0xFB;   //enable full step
                    g_Uni.uint8_Settings &= 0xBF;   //one phase ON for unipolar
                    //send back the needed informations
                    uart2_sendbuffer('E');                  //first the letter E
                    uart2_sendbuffer(13);                   //add the CR at the end
                    break;
                    
                case (1):       //full step two phase on
                    g_Param.uint8_StepMode = 1;
                    g_Uni.uint8_Settings &= 0xFB;   //enable full step
                    g_Uni.uint8_Settings |= 0x40;   //two phase ON for unipolar
                    //send back the needed informations
                    uart2_sendbuffer('E');                  //first the letter E
                    uart2_sendbuffer(13);                   //add the CR at the end
                    break;
                    
                case (2):       //half step
                    g_Param.uint8_StepMode = 2;
                    g_Uni.uint8_Settings |= 0x04;   //enable half step for unipolar
                    //send back the needed informations
                    uart2_sendbuffer('E');                  //first the letter E
                    uart2_sendbuffer(13);                   //add the CR at the end
                    break;
                    
                case (3):       //half step current compensated
                    g_Param.uint8_StepMode = 3;
                    //send back the needed informations
                    uart2_sendbuffer('E');                  //first the letter E
                    uart2_sendbuffer(13);                   //add the CR at the end
                    break;
                    
                case (4):       //micro stepping (quarter)
                    g_Param.uint8_StepMode = 4;
                    //send back the needed informations
                    uart2_sendbuffer('E');                  //first the letter E
                    uart2_sendbuffer(13);                   //add the CR at the end
                    break;
                    
                case (5):       //micro stepping (sixteenth)
                    g_Param.uint8_StepMode = 5;
                    //send back the needed informations
                    uart2_sendbuffer('E');                  //first the letter E
                    uart2_sendbuffer(13);                   //add the CR at the end
                    break;
                    
                default:        //not defined - error
                    g_Param.uint8_ErrCode = _StepModeSSMOD;     //set error code
                    uart2_SendErrorCode(g_Param.uint8_ErrCode); //call subroutine
                    break;
            }
        }      
    }
    else
    {
        g_Param.uint8_ErrCode = _NumbRecCharNotOK;  //set error code
        uart2_SendErrorCode(g_Param.uint8_ErrCode); //call subroutine
    }
}   //end of cmd_SSMOD


/**********************************************************************************************************************
 * Routine:                 cmd_GSMOD

 * Description:
 * This command send back the current step mode 
 * 
 * Creator:                 A. Staub
 * Date of creation:        28.11.2015
 * Last modification on:    -
 * Modified by:             - 
 * 
 * Input:                   -
 * Output:                  -
 * Global variable:         g_Param.
 *                              - uint8_StepMode
***********************************************************************************************************************/
void cmd_GSMOD(void)
{
    if(g_Cmd.uint8_ParamPos == 1)   //number of received characters OK?
    {
        uart2_sendbuffer('E');                    //first the letter E
        uart2_sendbuffer(',');                    //then the comma
        funct_IntToAscii(g_Param.uint8_StepMode,_Active); //add the step mode
        uart2_sendbuffer(13);                     //add the CR at the end      
    }
    else
    {
        g_Param.uint8_ErrCode = _NumbRecCharNotOK;  //set error code
        uart2_SendErrorCode(g_Param.uint8_ErrCode); //call subroutine
    }
}   //end of cmd_GSMOD


/**********************************************************************************************************************
 * Routine:                 cmd_SBIT

 * Description:
 * Verify the received parameters of this command, if all parameters are within the tolerance.
 * If all is correct, then set the corresponding bit / output 
 * 
 * Creator:                 A. Staub
 * Date of creation:        28.11.2015
 * Last modification on:    -
 * Modified by:             - 
 * 
 * Input:                   -
 * Output:                  -
 * Global variable:         g_Cmd.
 *                              - uint8_ParamPos
 *                              - uint32_TempPara
 *                          g_Param.
 *                              - uint8_ErrCode
***********************************************************************************************************************/
void cmd_SBIT(void)
{
    auto unsigned char uint8_Result = 0;    //local work byte
    
    if(g_Cmd.uint8_ParamPos == 2)           //number of received characters OK?
    {
        //verify the limits if they are inside the tolerance
        uint8_Result += funct_CheckTol(g_Cmd.uint32_TempPara[1],_BitMin,_BitMax);
        
        if(uint8_Result == 1)   //each parameter within the tolerance?
        {
            //set the corresponding bit / output by call the subroutine
            funct_OutputHandler(_SetBit,(g_Cmd.uint32_TempPara[1] & 0xFF));
                   
            //send back the needed informations
            uart2_sendbuffer('E');                  //first the letter E
            uart2_sendbuffer(13);                   //add the CR at the end
        }
        else
        {
            g_Param.uint8_ErrCode = _OutOfTolBit;       //set error code
            uart2_SendErrorCode(g_Param.uint8_ErrCode); //call subroutine
        }
    }
    else
    {
        g_Param.uint8_ErrCode = _NumbRecCharNotOK;  //set error code
        uart2_SendErrorCode(g_Param.uint8_ErrCode); //call subroutine
    }  
}   //end of cmd_SBIT


/**********************************************************************************************************************
 * Routine:                 cmd_CBIT

 * Description:
 * Verify the received parameters of this command, if all parameters are within the tolerance.
 * If all is correct, then clear the corresponding bit / output 
 * 
 * Creator:                 A. Staub
 * Date of creation:        28.11.2015
 * Last modification on:    -
 * Modified by:             - 
 * 
 * Input:                   -
 * Output:                  -
 * Global variable:         g_Cmd.
 *                              - uint8_ParamPos
 *                              - uint32_TempPara
 *                          g_Param.
 *                              - uint8_ErrCode
***********************************************************************************************************************/
void cmd_CBIT(void)
{
    auto unsigned char uint8_Result = 0;    //local work byte
    
    if(g_Cmd.uint8_ParamPos == 2)           //number of received characters OK?
    {
        //verify the limits if they are inside the tolerance
        uint8_Result += funct_CheckTol(g_Cmd.uint32_TempPara[1],_BitMin,_BitMax);
        
        if(uint8_Result == 1)   //each parameter within the tolerance?
        {
            //set the corresponding bit / output by call the subroutine
            funct_OutputHandler(_ClrBit,(g_Cmd.uint32_TempPara[1] & 0xFF));
                   
            //send back the needed informations
            uart2_sendbuffer('E');                  //first the letter E
            uart2_sendbuffer(13);                   //add the CR at the end
        }
        else
        {
            g_Param.uint8_ErrCode = _OutOfTolBit;       //set error code
            uart2_SendErrorCode(g_Param.uint8_ErrCode); //call subroutine
        }
    }
    else
    {
        g_Param.uint8_ErrCode = _NumbRecCharNotOK;  //set error code
        uart2_SendErrorCode(g_Param.uint8_ErrCode); //call subroutine
    }  
}   //end of cmd_CBIT


/**********************************************************************************************************************
 * Routine:                 cmd_GBIT

 * Description:
 * Verify the received parameters of this command, if all parameters are within the tolerance.
 * If all is correct, then send back the corresponding bit / output 
 * 
 * Creator:                 A. Staub
 * Date of creation:        28.11.2015
 * Last modification on:    -
 * Modified by:             - 
 * 
 * Input:                   -
 * Output:                  -
 * Global variable:         g_Cmd.
 *                              - uint8_ParamPos
 *                              - uint32_TempPara
 *                          g_Param.
 *                              - uint8_ErrCode
***********************************************************************************************************************/
void cmd_GBIT(void)
{
    auto unsigned char uint8_Result = 0;    //local work byte
    auto unsigned char uint8_value;         //local work byte
    
    if(g_Cmd.uint8_ParamPos == 2)           //number of received characters OK?
    {
        //verify the limits if they are inside the tolerance
        uint8_Result += funct_CheckTol(g_Cmd.uint32_TempPara[1],_BitMin,_BitMax);
        
        if(uint8_Result == 1)   //each parameter within the tolerance?
        {
            //get the corresponding bit / output by call the subroutine
            uint8_value = funct_IOhandler(_GetBit,_Output,(g_Cmd.uint32_TempPara[1] & 0xFF));
                   
            //send back the needed informations
            uart2_sendbuffer('E');                  //first the letter E
            uart2_sendbuffer(',');                  //then the comma
            funct_IntToAscii(uint8_value,_Active);  //add the value
            uart2_sendbuffer(13);                   //add the CR at the end
        }
        else
        {
            g_Param.uint8_ErrCode = _OutOfTolBit;       //set error code
            uart2_SendErrorCode(g_Param.uint8_ErrCode); //call subroutine
        }
    }
    else
    {
        g_Param.uint8_ErrCode = _NumbRecCharNotOK;  //set error code
        uart2_SendErrorCode(g_Param.uint8_ErrCode); //call subroutine
    }  
}   //end of cmd_GBIT


/**********************************************************************************************************************
 * Routine:                 cmd_SOUT

 * Description:
 * Verify the received parameters of this command, if all parameters are within the tolerance.
 * If all is correct, then set the output port
 * 
 * Creator:                 A. Staub
 * Date of creation:        28.11.2015
 * Last modification on:    -
 * Modified by:             - 
 * 
 * Input:                   -
 * Output:                  -
 * Global variable:         g_Cmd.
 *                              - uint8_ParamPos
 *                              - uint32_TempPara
 *                          g_Param.
 *                              - uint8_ErrCode
***********************************************************************************************************************/
void cmd_SOUT(void)
{
    auto unsigned char uint8_Result = 0;    //local work byte
    
    if(g_Cmd.uint8_ParamPos == 2)           //number of received characters OK?
    {
        //verify the limits if they are inside the tolerance
        uint8_Result += funct_CheckTol(g_Cmd.uint32_TempPara[1],_ValPortMin,_ValPortMax);
        
        if(uint8_Result == 1)   //each parameter within the tolerance?
        {
            //set the corresponding bit / output by call the subroutine
            funct_OutputHandler(_SetPort,(g_Cmd.uint32_TempPara[1] & 0xFF));
                   
            //send back the needed informations
            uart2_sendbuffer('E');                  //first the letter E
            uart2_sendbuffer(13);                   //add the CR at the end
        }
        else
        {
            g_Param.uint8_ErrCode = _OutOfTolValPort;   //set error code
            uart2_SendErrorCode(g_Param.uint8_ErrCode); //call subroutine
        }
    }
    else
    {
        g_Param.uint8_ErrCode = _NumbRecCharNotOK;  //set error code
        uart2_SendErrorCode(g_Param.uint8_ErrCode); //call subroutine
    }  
}   //end of cmd_SOUT


/**********************************************************************************************************************
 * Routine:                 cmd_GOUT

 * Description:
 * Verify the received parameters of this command, if all parameters are within the tolerance.
 * If all is correct, then send back the state of the output port 
 * 
 * Creator:                 A. Staub
 * Date of creation:        28.11.2015
 * Last modification on:    -
 * Modified by:             - 
 * 
 * Input:                   -
 * Output:                  -
 * Global variable:         g_Cmd.
 *                              - uint8_ParamPos
 *                              - uint32_TempPara
 *                          g_Param.
 *                              - uint8_ErrCode
***********************************************************************************************************************/
void cmd_GOUT(void)
{
    auto unsigned char uint8_value;         //local work byte
    
    if(g_Cmd.uint8_ParamPos == 1)   //number of received characters OK?
    {
        //get the output port by call the subroutine
        uint8_value = funct_IOhandler(_GetPort,_Output,(g_Cmd.uint32_TempPara[1] & 0xFF));
                   
        //send back the needed informations
        uart2_sendbuffer('E');                  //first the letter E
        uart2_sendbuffer(',');                  //then the comma
        funct_IntToAscii(uint8_value,_Active);  //add the value
        uart2_sendbuffer(13);                   //add the CR at the end      
    }
    else
    {
        g_Param.uint8_ErrCode = _NumbRecCharNotOK;  //set error code
        uart2_SendErrorCode(g_Param.uint8_ErrCode); //call subroutine
    } 
}   //end of cmd_GOUT


/**********************************************************************************************************************
 * Routine:                 cmd_GINP

 * Description:
 * Verify the received parameters of this command, if all parameters are within the tolerance.
 * If all is correct, then send back the state of the input port 
 * 
 * Creator:                 A. Staub
 * Date of creation:        28.11.2015
 * Last modification on:    -
 * Modified by:             - 
 * 
 * Input:                   -
 * Output:                  -
 * Global variable:         g_Cmd.
 *                              - uint8_ParamPos
 *                              - uint32_TempPara
 *                          g_Param.
 *                              - uint8_ErrCode
***********************************************************************************************************************/
void cmd_GINP(void)
{
    auto unsigned char uint8_value;         //local work byte
    
    if(g_Cmd.uint8_ParamPos == 1)   //number of received characters OK?
    {
        //get the output port by call the subroutine
        uint8_value = funct_IOhandler(_GetPort,_Input,(g_Cmd.uint32_TempPara[1] & 0xFF));
                   
        //send back the needed informations
        uart2_sendbuffer('E');                  //first the letter E
        uart2_sendbuffer(',');                  //then the comma
        funct_IntToAscii(uint8_value,_Active);  //add the value
        uart2_sendbuffer(13);                   //add the CR at the end      
    }
    else
    {
        g_Param.uint8_ErrCode = _NumbRecCharNotOK;  //set error code
        uart2_SendErrorCode(g_Param.uint8_ErrCode); //call subroutine
    }
}   //end of cmd_GINP


/**********************************************************************************************************************
 * Routine:                 cmd_SMCRSTP

 * Description:
 * Only allowed if the motor is not in run. First verify if the number of received parameters is inside the range.
 * If not, then send back the error code.
 * 
 * Second verify if the number of received parameters is 5, 9, 17, because to have a valide result, 
 * there must be always a current level for the microsteppint in 4th, 8th or 16th be. 
 * 5,9 or 17 because the command ID is counted as well. 
 * 
 * Third verify each received parameter with the toelrance until all parameters are verified. 
 * Update the number of received parameters with the number of verified parameters. 
 * 
 * Fourth verify if the result is exactly the number of parameters -1, to be sure tht each parameter is 
 * within the tolerance. -1 because we don't count the command ID with. 
 * 
 * At the end, first erase the acceleration array, store the new parameters inside and send back the OK.  
 * 
 * Creator:                 A. Staub
 * Date of creation:        29.11.2015
 * Last modification on:    -
 * Modified by:             - 
 * 
 * Input:                   -
 * Output:                  -
 * Global variable:         g_Cmd.
 *                              - uint8_ParamPos
 *                              - uint32_TempPara
 *                          g_Uni.
 *                              - uint8_Settings
 *                          g_Param.
 *                              - uint8_ErrCode
 *                              - uint16_BipILevel
***********************************************************************************************************************/
void cmd_SMCRSTP(void)
{
    auto unsigned char uint8_Result = 0;    //local work byte for the result
    auto unsigned char uint8_WB1;           //local work byte for the loops
    auto unsigned char uint8_WB2;           //local work byte to store the results into the array
    
    if((g_Cmd.uint8_ParamPos > 1) && (g_Cmd.uint8_ParamPos < 18))   //number of received characters OK?
    {
        //number of received characters are a pair of 4th, 8th or 17th (+1 with command ID)
        if((g_Cmd.uint8_ParamPos == 5) || (g_Cmd.uint8_ParamPos == 9) || (g_Cmd.uint8_ParamPos == 17))
        {
            if(g_Uni.uint8_Settings & 0x01) //is motor in run mode?
            {
                g_Param.uint8_ErrCode = _MotorInRun;        //set error code
                uart2_SendErrorCode(g_Param.uint8_ErrCode);  //call subroutine
            }
            else
            {
                //ToDo: 
                //verify each received parameter with the tolerance (start with the 2nd paramter)
                //Until:
                //the local work byte has the same size as number of received characters
                uint8_WB1 = 1;
                do
                {
                    uint8_Result += funct_CheckTol(g_Cmd.uint32_TempPara[uint8_WB1],_BipILevelMin,_BipILevelMax);
                    uint8_WB1++;    //increment with 1 to take the next parameter
                }
                while(uint8_WB1 < g_Cmd.uint8_ParamPos);
                
                //each paramter within the tolerance (uint8_WB1 - 1, because the 1st parameter is the cmd ID)?
                if(uint8_Result == (uint8_WB1-1))
                {
                    //first erase the array
                    for(uint8_WB1=0; uint8_WB1<16; uint8_WB1++)
                    {
                        g_Param.uint16_BipILevel[uint8_WB1] = 0;
                    }
                    
                    //then store each parameter on the right place (start with the 2nd paramter)
                    uint8_WB1 = 1;
                    uint8_WB2 = 0;
                    do
                    {
                        g_Param.uint16_BipILevel[uint8_WB2] = g_Cmd.uint32_TempPara[uint8_WB1] & 0xFFFF;
                        uint8_WB1++;    //increment with 1 to take the next paramter
                        uint8_WB2++;    //increment with 1 to store it on the right position of the array
                    }
                    while(uint8_WB1 < g_Cmd.uint8_ParamPos);
                    
                    //send back the OK
                    uart2_sendbuffer('E');            //first the letter E
                    uart2_sendbuffer(13);             //then the CR at the end
                }
                else
                {
                    g_Param.uint8_ErrCode = _OutOfTolSMCRSTP;   //set error code
                    uart2_SendErrorCode(g_Param.uint8_ErrCode); //call subroutine
                }
            }
        }
        else
        {
            g_Param.uint8_ErrCode = _NumbRecCharNotOK;  //set error code
            uart2_SendErrorCode(g_Param.uint8_ErrCode); //call subroutine
        }
    }  
    else
    {
        g_Param.uint8_ErrCode = _NumbRecCharNotOK;  //set error code
        uart2_SendErrorCode(g_Param.uint8_ErrCode); //call subroutine
    }
}   // end of cmd_SMCRSTP


/**********************************************************************************************************************
 * Routine:                 cmd_GMCRSTP

 * Description:
 * Only allowed if the motor is not in run. Send back the current level for the micro stepping.  
 * 
 * Creator:                 A. Staub
 * Date of creation:        29.11.2015
 * Last modification on:    -
 * Modified by:             - 
 * 
 * Input:                   -
 * Output:                  -
 * Global variable:         g_Cmd.
 *                              - uint8_ParamPos
 *                              - uint32_TempPara
 *                          g_Uni.
 *                              - uint8_Settings
 *                          g_Param.
 *                              - uint8_ErrCode
 *                              - uint16_BipILevel
***********************************************************************************************************************/
void cmd_GMCRSTP(void)
{
    auto unsigned char uint8_WB;    //local work byte
    
    if(g_Cmd.uint8_ParamPos == 1)   //number of received characters OK?
    {
        if(g_Uni.uint8_Settings & 0x01) //is motor in run mode?
        {
            g_Param.uint8_ErrCode = _MotorInRun;        //set error code
            uart2_SendErrorCode(g_Param.uint8_ErrCode);  //call subroutine
        }
        else 
        {
            uart2_sendbuffer('E');          //first the letter E
            
            //read out the array
            for(uint8_WB=0; uint8_WB<16; uint8_WB++)
            {
                if(g_Param.uint16_BipILevel[uint8_WB] == 0) //is the parameter = 0?
                {
                    uint8_WB = 16;  //leave the for loop
                }
                else    //otherwise send the parameter back
                {
                    uart2_sendbuffer(',');  //add the comma
                    //convert the parameter and store it into the send buffer
                    funct_IntToAscii(g_Param.uint16_BipILevel[uint8_WB],_Active);
                }
            }
            
            uart2_sendbuffer(13);           //add the <CR> at the end
        }
    }
    else
    {
        g_Param.uint8_ErrCode = _NumbRecCharNotOK;  //set error code
        uart2_SendErrorCode(g_Param.uint8_ErrCode); //call subroutine
    }
}   //end of cmd_GMCRSTP cmd_SSWLIM


/**********************************************************************************************************************
 * Routine:                 cmd_SSWLIM

 * Description:
 * Defines the position limits for switch or hall-sensor switch. 
 * 
 * Creator:                 A. Staub
 * Date of creation:        30.11.2015
 * Last modification on:    -
 * Modified by:             - 
 * 
 * Input:                   -
 * Output:                  -
 * Global variable:         g_Cmd.
 *                              - uint8_ParamPos
 *                              - uint32_TempPara
 *                          g_Param.
 *                              - uint32_Sw1min
 *                              - uint32_Sw1max
 *                              - uint32_Sw2min
 *                              - uint32_Sw2max
 *                              - uint8_ErrCode
 *                              - uint16_BipILevel
***********************************************************************************************************************/
void cmd_SSWLIM(void)
{
    auto unsigned char uint8_Result = 0;    //local work byte
    
    if(g_Cmd.uint8_ParamPos == 5)   //number of received characters OK?
    {
        uint8_Result += funct_CheckTol(g_Cmd.uint32_TempPara[1],_SW1min,_SW1max);
        uint8_Result += funct_CheckTol(g_Cmd.uint32_TempPara[2],_SW1min,_SW1max);
        uint8_Result += funct_CheckTol(g_Cmd.uint32_TempPara[3],_SW2min,_SW2max);
        uint8_Result += funct_CheckTol(g_Cmd.uint32_TempPara[4],_SW2min,_SW2max);
        
        if(uint8_Result == 4)   //received parameters are within the tolerance?
        {
            g_Param.uint32_Sw1min = g_Cmd.uint32_TempPara[1];
            g_Param.uint32_Sw1max = g_Cmd.uint32_TempPara[2];
            g_Param.uint32_Sw2min = g_Cmd.uint32_TempPara[3];
            g_Param.uint32_Sw2max = g_Cmd.uint32_TempPara[4];
            
            //send back the needed informations
            uart2_sendbuffer('E');                  //first the letter E
            uart2_sendbuffer(13);                   //add the CR at the end
        }
        else
        {
            g_Param.uint8_ErrCode = _OutOfTolSSWLIM;  //set error code
            uart2_SendErrorCode(g_Param.uint8_ErrCode); //call subroutine
        }   
    }
    else
    {
        g_Param.uint8_ErrCode = _NumbRecCharNotOK;  //set error code
        uart2_SendErrorCode(g_Param.uint8_ErrCode); //call subroutine
    }
}   //end of cmd_SSWLIM 

/**********************************************************************************************************************
 * Routine:                 cmd_GSWLIM

 * Description:
 * Get the position limits for switch or hall-sensor switch. 
 * 
 * Creator:                 J. Rebetez
 * Date of creation:        30.11.2015
 * Last modification on:    -
 * Modified by:             - 
 * 
 * Input:                   -
 * Output:                  -
***********************************************************************************************************************/
void cmd_GSWLIM(void)
{    
    if(g_Cmd.uint8_ParamPos == 1)   //number of received characters OK?
    {
        uart2_sendbuffer('E');          //first the letter E
            
        uart2_sendbuffer(',');  //add the comma
        //convert the parameter and store it into the send buffer
        funct_IntToAscii(g_Param.uint32_Sw1min,_Active);
        uart2_sendbuffer(',');  //add the comma
        funct_IntToAscii(g_Param.uint32_Sw1max,_Active);
        uart2_sendbuffer(',');  //add the comma
        funct_IntToAscii(g_Param.uint32_Sw2min,_Active);
        uart2_sendbuffer(',');  //add the comma
        funct_IntToAscii(g_Param.uint32_Sw2max,_Active);
        uart2_sendbuffer(13);   //add the CR at the end
    } 
    else
    {
        g_Param.uint8_ErrCode = _NumbRecCharNotOK;  //set error code
        uart2_SendErrorCode(g_Param.uint8_ErrCode); //call subroutine
    }
}   //end of cmd_GSWLIM 

/**********************************************************************************************************************
 * Routine:                 cmd_GSWPOS

 * Description:
 * Get the currently switch-position for switch or hall-sensor. 
 * 
 * Creator:                 J. Rebetez
 * Date of creation:        30.11.2015
 * Last modification on:    -
 * Modified by:             - 
 * 
 * Input:                   -
 * Output:                  -
***********************************************************************************************************************/
void cmd_GSWPOS(void)
{    auto unsigned char uint8_Result = 0;
    if(g_Cmd.uint8_ParamPos == 1)   //number of received characters OK?
    {
        uint8_Result += funct_CheckTol(g_Param.uint32_Sw1pos,g_Param.uint32_Sw1min,g_Param.uint32_Sw1max);
        uint8_Result += funct_CheckTol(g_Param.uint32_Sw2pos,g_Param.uint32_Sw2min,g_Param.uint32_Sw2max);
        if(uint8_Result == 2)
        {
            uart2_sendbuffer('E');          //first the letter E
        }
        else
        {
            uart2_sendbuffer('X');  //add the comma
            g_Param.uint8_ErrCode = _OutOfTolGSWPOS;  //set error code
            uart2_sendbuffer(',');  //add the comma
            funct_IntToAscii(g_Param.uint8_ErrCode,_Active);
        }   
        uart2_sendbuffer(',');  //add the comma
        //convert the parameter and store it into the send buffer
        funct_IntToAscii(g_Param.uint32_Sw1pos,_Active);
        uart2_sendbuffer(',');  //add the comma
        funct_IntToAscii(g_Param.uint32_Sw2pos,_Active);
        uart2_sendbuffer(13);   //add the CR at the end
    } 
    else
    {
        g_Param.uint8_ErrCode = _NumbRecCharNotOK;  //set error code
        uart2_SendErrorCode(g_Param.uint8_ErrCode); //call subroutine
    }
}   //end of cmd_GSWPOS 

/**********************************************************************************************************************
 * Routine:                 cmd_SPWMLIM

 * Description:
 * Set PWM limits for linear hall-detector
 * 
 * Creator:                 J. Rebetez
 * Date of creation:        30.11.2015
 * Last modification on:    -
 * Modified by:             - 
 * 
 * Input:                   -
 * Output:                  -
***********************************************************************************************************************/
void cmd_SPWMLIM(void)
{
    auto unsigned char uint8_Result = 0;    //local work byte
    
    if(g_Cmd.uint8_ParamPos == 3)   //number of received characters OK?
    {
        uint8_Result += funct_CheckTol(g_Cmd.uint32_TempPara[1],_SWpwmMin,_SWpwmMax);
        uint8_Result += funct_CheckTol(g_Cmd.uint32_TempPara[2],_SWpwmMin,_SWpwmMax);
        
        if(uint8_Result == 2)   //received parameters are within the tolerance?
        {
            g_Param.uint32_SwPWMmin = g_Cmd.uint32_TempPara[1];
            g_Param.uint32_SwPWMmax = g_Cmd.uint32_TempPara[2];
            
            //send back the needed informations
            uart2_sendbuffer('E');                  //first the letter E
            uart2_sendbuffer(13);                   //add the CR at the end
        }
        else
        {
            g_Param.uint8_ErrCode = _OutOfTolSPWMLIM;  //set error code
            uart2_SendErrorCode(g_Param.uint8_ErrCode); //call subroutine
        }   
    }
    else
    {
        g_Param.uint8_ErrCode = _NumbRecCharNotOK;  //set error code
        uart2_SendErrorCode(g_Param.uint8_ErrCode); //call subroutine
    }
}   //end of cmd_SPWMLIM 

/**********************************************************************************************************************
 * Routine:                 cmd_GPWMLIM

 * Description:
 * Get PWM limits for linear hall-detector. 
 * 
 * Creator:                 J. Rebetez
 * Date of creation:        30.11.2015
 * Last modification on:    -
 * Modified by:             - 
 * 
 * Input:                   -
 * Output:                  -
***********************************************************************************************************************/
void cmd_GPWMLIM(void)
{    
    if(g_Cmd.uint8_ParamPos == 1)   //number of received characters OK?
    {
        uart2_sendbuffer('E');          //first the letter E
            
        uart2_sendbuffer(',');  //add the comma
        //convert the parameter and store it into the send buffer
        funct_IntToAscii(g_Param.uint32_SwPWMmin,_Active);
        uart2_sendbuffer(',');  //add the comma
        funct_IntToAscii(g_Param.uint32_SwPWMmax,_Active);
        uart2_sendbuffer(13);   //add the CR at the end
    } 
    else
    {
        g_Param.uint8_ErrCode = _NumbRecCharNotOK;  //set error code
        uart2_SendErrorCode(g_Param.uint8_ErrCode); //call subroutine
    }
}   //end of cmd_GPWMLIM

/**********************************************************************************************************************
 * Routine:                 cmd_GPWMPOS

 * Description:
 * Get position for linear hall-detector. 
 * 
 * Creator:                 J. Rebetez
 * Date of creation:        30.11.2015
 * Last modification on:    -
 * Modified by:             - 
 * 
 * Input:                   -
 * Output:                  -
***********************************************************************************************************************/
void cmd_GPWMPOS(void)
{    
    auto unsigned char uint8_Result = 0;
    
    if(g_Cmd.uint8_ParamPos == 1)   //number of received characters OK?
    {
        uint8_Result += funct_CheckTol(g_Param.uint32_SwPWMpos,g_Param.uint32_SwPWMmin,g_Param.uint32_SwPWMmax);
        uint8_Result += funct_CheckTol(g_Param.uint16_SwPWMval,470,530);
        if(uint8_Result == 2)
        {
            uart2_sendbuffer('E');          //first the letter E
        }
        else
        {
            uart2_sendbuffer('X');  //add the comma
            g_Param.uint8_ErrCode = _OutOfTolGPWMPOS;  //set error code
            uart2_sendbuffer(',');  //add the comma
            funct_IntToAscii(g_Param.uint8_ErrCode,_Active);
        }   
        uart2_sendbuffer(',');  //add the comma
        //convert the parameter and store it into the send buffer
        funct_IntToAscii(g_Param.uint32_SwPWMpos,_Active);
        uart2_sendbuffer(',');  //add the comma
        funct_IntToAscii(g_Param.uint16_SwPWMval,_Inactive);
        do{
            g_Funct.uint8_ArrAsciiPos--;
            uart2_sendbuffer(g_Funct.uint8_ArrAscii[g_Funct.uint8_ArrAsciiPos]);
            if(g_Funct.uint8_ArrAsciiPos == 1)
            {
                uart2_sendbuffer('.');
            }
        }while(g_Funct.uint8_ArrAsciiPos);
        uart2_sendbuffer(13);   //add the CR at the end
    } 
    else
    {
        g_Param.uint8_ErrCode = _NumbRecCharNotOK;  //set error code
        uart2_SendErrorCode(g_Param.uint8_ErrCode); //call subroutine
    }
}   //end of cmd_GPWMLIM
















/**********************************************************************************************************************
 * Routine:                 cmd_GINPB

 * Description:
 * Verify the received parameters of this command, if all parameters are within the tolerance.
 * If all is correct, then send back the state of the input bit 
 * 
 * Creator:                 A. Staub
 * Date of creation:        29.11.2015
 * Last modification on:    -
 * Modified by:             - 
 * 
 * Input:                   -
 * Output:                  -
***********************************************************************************************************************/
void cmd_GINPB(void)
{
    auto unsigned char uint8_Result = 0;    //local work byte
    auto unsigned char uint8_value;         //local work byte
    
    if(g_Cmd.uint8_ParamPos == 2)           //number of received characters OK?
    {
        //verify the limits if they are inside the tolerance
        uint8_Result += funct_CheckTol(g_Cmd.uint32_TempPara[1],_BitMin,_BitMax);
        
        if(uint8_Result == 1)   //each parameter within the tolerance?
        {
            //get the corresponding bit / output by call the subroutine
            uint8_value = funct_IOhandler(_GetBit,_Input,(g_Cmd.uint32_TempPara[1] & 0xFF));
                   
            //send back the needed informations
            uart2_sendbuffer('E');                  //first the letter E
            uart2_sendbuffer(',');                  //then the comma
            funct_IntToAscii(uint8_value,_Active);  //add the value
            uart2_sendbuffer(13);                   //add the CR at the end
        }
        else
        {
            g_Param.uint8_ErrCode = _OutOfTolBit;       //set error code
            uart2_SendErrorCode(g_Param.uint8_ErrCode); //call subroutine
        }
    }
    else
    {
        g_Param.uint8_ErrCode = _NumbRecCharNotOK;  //set error code
        uart2_SendErrorCode(g_Param.uint8_ErrCode); //call subroutine
    } 
}   //end of cmd_GINPB