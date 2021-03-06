/**********************************************************************************************************************

                                            UBD - Unipolar Bipolar Driver

***********************************************************************************************************************
 * File name:               defines.h
 * Creation date:           06.08.2015
 * Main creator:            J. Rebetez
 * Company:                 REFAST GmbH
 *                          Copyright (c) 2015 REFAST GmbH
***********************************************************************************************************************
 * Content overview :       - DEFINES
***********************************************************************************************************************/

/**********************************************************************************************************************
 * Routine:                 DEFINES_H

 * Description:
 * File for all generale defines. 
 * 
 * Creator:                 J. Rebetez
 * Date of creation:        06.08.2015
 * Last modification on:    -
 * Modified by:             - 
 * 
 * Input:                   -
 * Output:                  -
***********************************************************************************************************************/

#ifndef DEFINES_H
#define	DEFINES_H


// Compilation variables
//#define _BIPOLAR
//#define _LIN
//#define _UNIPOLAR
#define _NOT_USED_CMD       // Undefined / Unused commands


// Inputs & Outputs definitions
// Sink / Source Outputs
#define oSinkSource0        PORTAbits.RA7           // Sink / Source output 0
#define oSinkSource1        PORTEbits.RE0           // Sink / Source output 1
#define oSinkSource2        PORTEbits.RE1           // Sink / Source output 2
#define oSinkSource3        PORTGbits.RG14          // Sink / Source output 3
#define oSinkSource4        PORTGbits.RG12          // Sink / Source output 4
#define oSinkSource5        PORTGbits.RG13          // Sink / Source output 5
#define oSinkSource6        PORTEbits.RE2           // Sink / Source output 6
#define oSinkSource7        PORTEbits.RE3           // Sink / Source output 7
#define iSinkSource0        PORTEbits.RE8           // Sink / Source input 0
#define iSinkSource1        PORTFbits.RF0           // Sink / Source input 1
#define iSinkSource2        PORTFbits.RF1           // Sink / Source input 2
#define iSinkSource3        PORTDbits.RD9           // Sink / Source input 3
#define iSinkSource4        PORTGbits.RG1           // Sink / Source input 4
#define iSinkSource5        PORTGbits.RG0           // Sink / Source input 5
#define iSinkSource6        PORTDbits.RD8           // Sink / Source input 6
#define iSinkSource7        PORTAbits.RA6           // Sink / Source input 7
// Bipolar driver
#define iBiDiagSignal       PORTDbits.RD1           // Diag signal of bipolar driver
#define oBiStepSignal       PORTDbits.RD0           // Step signal of bipolar driver
#define oBiDirSignal        PORTDbits.RD2           // DIR signal of bipolar driver
#define oBiResetSignal      PORTEbits.RE4           // RESET signal of bipolar driver
#define oBiStrnSignal       PORTAbits.RA2           // STRn signal of bipolar driver
#define oBiPhCurrCtrl       PORTDbits.RD3           // Phase current control A & B
#define oBiEnaVmot          PORTDbits.RD11          // Enable of the motor supply
#define oBiRelayCoilA       PORTDbits.RD12          // Control relay for coil A
#define oBiRelayCoilB       PORTDbits.RD13          // Control relay for coil B
// Unipolar driver
#define oUniCoilB2          PORTDbits.RD7           // Output to switch on / off coil B2 from unipolar driver
#define oUniCoilB1          PORTDbits.RD6           // Output to switch on / off coil B1 from unipolar driver
#define oUniCoilA2          PORTDbits.RD5           // Output to switch on / off coil A2 from unipolar driver 
#define oUniCoilA1          PORTDbits.RD4           // Output to switch on / off coil A1 from unipolar driver 
// General
#define oVmotOnOff          PORTBbits.RB14          // Output to switch on / off Umot
#define oEnaCoilResMeas     PORTBbits.RB15          // Enable to switch the measure to coil resistor
#define oEnaCurrSource      PORTDbits.RD10          // Enable of the constant current source
#define oTestLed1           PORTEbits.RE5           // Test LED 1
#define oTestLed2           PORTEbits.RE6           // Test LED 2
#define oFrontLedGRN        PORTBbits.RB1           // Front green led
#define oFrontLedRED        PORTBbits.RB0           // Front red led
// LIN driver
#define oEnaVLINSupply      PORTAbits.RA14          // Enable of V LIN supply
#define oEnaLINDriver       PORTAbits.RA5           // Enable of LIN driver
#define oSPModeSignalLIN    PORTAbits.RA4           // SP_MODE signal of LIN driver
#define iNRESSignalLIN      PORTAbits.RA3           // NRES signal of LIN driver
// RF Module
#define oCMDModeSignal      PORTAbits.RA15          // CMD_MODE signal of RF module
// Communication
#define oLIN_RFSwitch       PORTCbits.RC15          // Switch of the communication between LIN driver and RF module
// SD Card
#define iDetectSD           PORTAbits.RA0           // Detection of the SD Card
// USB interface
#define iUSBSupplyDetect    PORTGbits.RG15          // Detection of USB connection
// RTCC Chip
#define iINTSignalRTC       PORTEbits.RE9           // INT signal of RTCC chip
// AD channels
#define adUniICoilA1        3                       // Measure of Icoil A1 (Unipolar)
#define adUniICoilA2        2                       // Measure of Icoil A2 (Unipolar)
#define adUniICoilB1        5                       // Measure of Icoil B1 (Unipolar)
#define adUniICoilB2        4                       // Measure of Icoil B2 (Unipolar)
#define adBiRefSignal       7                       // Measure of voltage reference of Ref signal
#define adBiICoilA          10                      // Measure of Icoil A (bipolar)
#define adBiICoilB          6                       // Measure of Icoil B (bipolar)
#define adMotorCurrent      9                       // Measure of current of the motor
#define adMotorSupply       8                       // Measure of motor supply and coil resistor
#define adBattVoltCtrl      11                      // Measure of the voltage of the backup battery


//error code definition
                            //general errors
#define _OutOfTolSPROD      1               //SPROD internal used, parameters out of range
#define _OutOfTolVmot       16              // Out of tolerance of the Vmot voltage
#define _RTESTINsupply      17              //RTESTIN not possible to switch off the supply under 0.3V
#define _RTESTINnotPossible 18              //RTESTIN not possible with this type of motor
#define _OutOfTolSRUNBIT    19              //SRUNBIT, parameter out of range
#define _OutOfTolSFRQBIT    20              //SFRQBIT, parameter out of range
#define _OutOfTolSYEAR      21              //SYEAR, parameter out of range
#define _OutOfTolSMONTH     22              //SMONTH, parameter out of range
#define _OutOfTolSWDAY      23              //SWDAY, parameter out of range
#define _OutOfTolSDAY       24              //SDAY, parameter out of range
#define _OutOfTolSHRS       25              //SHRS, parameter out of range
#define _OutOfTolSMIN       26              //SMIN, parameter out of range
#define _OutOfTolSSEC       27              //SSEC, parameter out of range
#define _BusCollRTC         28              //General RTC chip, issue during reading temperature
#define _BusCollSROMRROM    29              //SROM,RROM; bus collision error
#define _AckSROMRROM        30              //SROM,RROM; acknowledge error - no answer from EEPROM
#define _OutOfTolSROMRROM   31              //SROM,RROM; received parameters out of tolerance
#define _OutOfTolSSWTYP     32              //SSWTYP; parameter out of range
#define _OutOfTolGPWMVAL    33              //GPWMVAL, GFREQVAL; parameter(s) out of range
#define _OutOfTolGPWMPOS    34              //GPWMPOS; parameter(s) out of range
#define _OutOfTolSPWMLIM    35              //SPWMLIM; parameter(s) out of range
#define _OutOfTolGSWPOS     36              //GSWPOS; parameter(s) out of range
#define _OutOfTolSSWLIM     37              //SSWLIM; received parameters out of tolerance
#define _MotTypSCOILON      38              //SCOILON; try to execute the command with motor type = 'N'
#define _OutOfTolSMCRSTP    39              //SMCRSTP; received parameters out of tolerance 
#define _OutOfTolValPort    40              //SOUT; value to write on the output port out of tolerance
#define _OutOfTolBit        41              //SBIT, CBIT, GBIT, GINPB; bit value out of tolerance
#define _StepModeSSMOD      42              //SSMOD; step mode not defined
#define _OutOfTolSCOILON    43              //SCOILON; received parameters out of tolerance
#define _OutOfTolRUN        44              //RUN; received parameters out of tolerance
#define _MotTypRUN          45              //RUN; try to execute the command with motor type = 'N'
#define _OutOfTolSRDEC      46              //SRDEC; received parameters out of tolerance
#define _OutOfTolSRACC      47              //SRACC; received parameters out of tolerance
#define _UnknownMotTyp      48              //motor type unknown
#define _MotorInRun         49              //driver send not back an information during actuator is in run mode
                            //unipolar errors are the same for matrix errors!
#define _UniSILIM           50              //received parameters not within the tolerance for SILIM
#define _UniPlausiCheck     51              //plausibility check not passed (could be re-used)
#define _UniSMTYP           52              //parameters not within tolerance to switch on unipolar
#define _UniETESTIN         53              //ETESTIN current measure not within the tolerance
                            //bipolar errors
#define _BipSILIM           100             //received parameters not within the tolerance for SILIM
#define _BipSMTYP           101             //parameters not within tolerance for switch on bipolar
#define _BipETESTIN         102             //ETESTIN current measure not within the tolerance
//103 is free
#define _BipPlausiCheck     104             //plausibility check not passed (could be re-used)
#define _BipSBIPPD          105             //SBIPPD parameters not within the tolerance
#define _BipSBIPTSC         106             //SBIPTSC parameter not within the tolerance
#define _BipSBIPOL          107             //SBIPOL parameter not within the tolerance
#define _BipSBIPSTS         108             //SBIPSTS parameter not within the tolerance
#define _BipSBIPPFD         109             //SBIPPFD parameter not within the tolerance
#define _BipSBIPPWM         110             //SBIPPWM parameters not within the tolerance
#define _BipSBIPFRQ         111             //SBIPFRQ parameter not within the tolerance
#define _BipSBIPSLEW        112             //SBIPSLEW parameter not within the tolerance
#define _BipGBIPPWM         113             //GBIPPWM driver is in frequency mode instead of PWM mode
#define _BipGBIPFRQ         114             //GBIPFRQ driver is in PWM mode instead of frequency mode
                            //LIN errors
#define _LinSILIM           150             //received parameters not within the tolerance for SILIM
#define _LinSMTYP           151             //parameters not within tolerance for switch on LIN
#define _LinETESTIN         152             //ETESTIN command is not allowed for LIN application's
#define _LinRUN             153             //RUN command is not allowed for LIN application's
#define _LinSCOILON         154             //SCOILON command is not allowed for LIN application's
#define _OutOfTolSSPDLIN    155             //SSPDLIN, parameter out of range
#define _OutOfTolSTOLIN     156             //STOLIN, parameter out of range
#define _OutOfTolSLIN       157             //SLIN, parameter out of range
#define _LinTO              158             //SLIN (indirect), timeout, no answer from slave
#define _OutOfTolSRESLIN    159             //SRESLIN, parameter out of range
                            //command errors
#define _NotSupAnymore      253             //not supported anymore
#define _NumbRecCharNotOK   254             //number of received characters/parameters not OK
#define _UnknownCmd         255             //unknown or not implemented command


//ID definition for each command
#define _IdNotAllow         0               //do not use this number for a command!
#define _IdSILIM            1               //set current limit and test voltage
#define _IdETESTIN          2               //perform the electrical test on power line
#define _IdSRACC            3               //set acceleration ramp
#define _IdSRDEC            4               //set deceleration ramp
#define _IdRUN              5               //run the motor
#define _IdSPHC             6               //set percentage in half step compensated
#define _IdGVER             7               //read out the device version
#define _IdRAZ              8               //reset all to zero and send back the device version
#define _IdSMTYP            9               //set type of motor
#define _IdGMTYP            10              //get type of motor
#define _IdSTEST            11              //perform a self-test sequence
#define _IdGILIM            12              //get current limit and test voltage
#define _IdGPHC             13              //get percentage in half step compensated
#define _IdSIRUN            14              //-KO-set chopper current in run 
#define _IdGIRUN            15              //-KO-get chopper current in run mode
#define _IdGRACC            16              //get acceleration ramp
#define _IdGRDEC            17              //get deceleration ramp
#define _IdSRUN             18              //-KO- set the running parameters
#define _IdGRUN             19              //get the running parameters
#define _IdBREAK            20              //stop the currently test/run
#define _IdCAL              21              //-KO- starts the calibration of the measure circuit
#define _IdVCAL             22              //-KO- check the calibration with an external voltage source
#define _IdSPRUN            23              //-KO- set protocol in run mode
#define _IdGPRUN            24              //-KO- get protocol in run mode
#define _IdSUMOT            25              //-KO- set motor test voltage
#define _IdGUMOT            26              //-KO- get motor test voltage
#define _IdCHKSC            27              //-KO- check short circuit detected (during last move)
#define _IdMUMOT            28              //measure motor test voltage
#define _IdCHKUMOT          29              //-KO- check the motor test voltage
#define _IdSCOILON          30              //engage coil current
#define _IdSCACC            31              //-KO- set current and ON-delay for acceleration
#define _IdGCACC            32              //-KO- get current and ON-delay for acceleration
#define _IdSCDEC            33              //-KO- set current and OFF-delay for deceleration
#define _IdGCDEC            34              //-KO- get current and OFF-delay for deceleration
#define _IdSSMOD            35              //set step mode
#define _IdGSMOD            36              //get step mode
#define _IdSERUN            37              //-KO- set the E-running parameters with stall detection
#define _IdGERUN            38              //get the E-running parameters
#define _IdERUN             39              //E-run the motor with stall detection and free run
#define _IdGO               40              //start the motor and run until stop command "BREAK"
#define _IdSIMAN            41              //-KO- set chopper current in stop mode
#define _IdGIMAN            42              //-KO- get chopper current in stop mode
#define _IdSBIT             43              //set a bit of the output port
#define _IdCBIT             44              //clear a bit of the output port
#define _IdGBIT             45              //get a bit of the output port
#define _IdSOUT             46              //set the output port
#define _IdGOUT             47              //get the output port
#define _IdGINP             48              //get the input port
#define _IdSMCRSTP          49              //set micro stepping current levels
#define _IdGMCRSTP          50              //get mirco stepping current levels
#define _IdSSWLIM           51              //set position limits for switch or hall-sensor switch
#define _IdGSWLIM           52              //get position limits for switch or hall-sensor switch
#define _IdGSWPOS           53              //get the actually switch-position for switch or hall-sensor
#define _IdSPWMLIM          54              //set PWM 50% DC limits for linear hall-detector
#define _IdGPWMLIM          55              //get PWM 50% DC limits for linear hall-detector
#define _IdGPWMPOS          56              //get PWM 50% DC step postion for linear hall-detector
#define _IdGPWMVAL          57              //get PWM in % duty-cycle for linear hall-detector
#define _IdSSWTYP           58              //set type of switch (switch, hall-detector, linear hall-detector, encoder)
#define _IdGSWTYP           59              //get type of switch (switch, hall-detector, linear hall-detector, encoder)
#define _IdGFRQVAL          60              //get frequency for torque measurement
#define _IdSPOSRUN          61              //-KO- set the P-running parameters with stop on digital input active
#define _IdGPOSRUN          62              //get the P-running parameters
#define _IdPOSRUN           63              //run the motor with stop on digital input active
//new commands:
#define _IdGINPB            64              //get (read) an input bit
#define _IdSROM             65              //store byte into the EEPROM
#define _IdRROM             66              //read out byte from the EEPROM
#define _IdGTEMP            67              //read out the temperature
#define _IdSSEC             68              //set the seconds
#define _IdGSEC             69              //get the seconds
#define _IdSMIN             70              //set the minutes
#define _IdGMIN             71              //get the minutes
#define _IdSHRS             72              //set the hours
#define _IdGHRS             73              //get the hours
#define _IdSDAY             74              //set the day
#define _IdGDAY             75              //get the day
#define _IdSWDAY            76              //set the workday
#define _IdGWDAY            77              //get the workday
#define _IdSMONTH           78              //set the month
#define _IdGMONTH           79              //get the month
#define _IdSYEAR            80              //set the year
#define _IdGYEAR            81              //get the year
#define _IdGTIME            82              //get the time
#define _IdGSTALIN          83              //get the LIN bus driver status
#define _IdSSPDLIN          84              //set the speed of the LIN communication
#define _IdGSPDLIN          85              //get the speed of the LIN communication
#define _IdSTOLIN           86              //set the timeout of the LIN communication
#define _IdGTOLIN           87              //get the timeout of the LIN communication
#define _IdSLIN             88              //send parameters over the LIN bus
#define _IdGBIPSTAS         89              //get bipolar status short
#define _IdGBIPSTAL         90              //get bipolar status long
#define _IdSBIPPD           91              //set bipolar phase decay
#define _IdGBIPPD           92              //get bipolar phase decay
#define _IdSBIPTSC          93              //set bipolar overcurrent fault delay
#define _IdGBIPTSC          94              //get bipolar overcurrent fault delay
#define _IdSBIPOL           95              //set bipolar open load current
#define _IdGBIPOL           96              //get bipolar open load current
#define _IdSBIPSTS          97              //set bipolar stall detection scheme
#define _IdGBIPSTS          98              //get bipolar stall detection scheme
#define _IdSBIPPFD          99              //set bipolar fast decay time
#define _IdGBIPPFD          100             //get bipolar fast decay time
#define _IdSBIPPWM          101             //set bipolar PWM mode
#define _IdGBIPPWM          102             //get bipolar PWM mode
#define _IdSBIPFRQ          103             //set bipolar FRQ mode
#define _IdGBIPFRQ          104             //get bipolar FRQ mode
#define _IdSBIPSLEW         105             //set bipolar slew rate mode
#define _IdGBIPSLEW         106             //get bipolar slew rate mode
#define _IdGBIPSST          107             //get bipolar status stall
#define _IdGVBAK            108             //get the backup voltage of the button cell
#define _IdSRESLIN          109             //set the response mode for the lin bus
#define _IdGRESLIN          110             //get the response mode for the lin bus
#define _IdSFRQBIT          111             //enable the frequency bit
#define _IdGFRQBIT          112             //disable the frequency bit
#define _IdSRUNBIT          113             //enable the run bit
#define _IdGRUNBIT          114             //disable the run bit
#define _IdRTESTIN          115             //resistance test / measure
#define _IdSPROD            116             //Write production information in serial EEPROM
#define _IdSTAT             117             //get status of the SMMA
#define _IdJEprotocol       118             //command is a JE protocol
#define _IDJELIN            119             //number of parameter are correct


#endif	/* DEFINES_H */

