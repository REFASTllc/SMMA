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
 * Global variable:         -
***********************************************************************************************************************/

#ifndef DEFINES_H
#define	DEFINES_H

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

#define oVmotOnOff          PORTEbits.RE4           //output to switch on / off Umot
#define oUniCoilB2          PORTEbits.RE3           //output to switch on / off coil B2 from unipolar driver
#define oUniCoilB1          PORTEbits.RE2           //output to switch on / off coil B1 from unipolar driver
#define oUniCoilA2          PORTGbits.RG13          //output to switch on / off coil A2 from unipolar driver 
#define oUniCoilA1          PORTGbits.RG12          //output to switch on / off coil A1 from unipolar driver 



//error code definition
                            //general errors
#define _GlobalLock         0               //driver detected a POR; user configuration not guaranteed
#define _OutOfTolSCOILON    43              //SCOILON; received parameters out of tolerance
#define _OutOfTolRUN        44              //RUN; received parameters out of tolerance
#define _MotTypRUN          45              //RUN; try to execute the command with motor type = 'N'
#define _OutOfTolSRDEC      46              //SRACC; received parameters out of tolerance
#define _OutOfTolSRACC      47              //SRDEC; received parameters out of tolerance
#define _UnknownMotTyp      48              //motor type unknown
#define _MotorInRun         49              //driver send not back an information during actuator is in run mode
                            //unipolar errors are the same for matrix errors!
#define _UniSILIM           50              //unipolar; received parameters not within the tolerance for SILIM
#define _UniPlausiCheck     51              //unipolar; plausibility check not passed
#define _UniSMTYP           52              //unipolar; parameters not within tolerance to switch on unipolar
#define _UniETESTIN         53              //unipolar; ETESTIN current measure not within the tolerance
                            //bipolar errors
#define _BipSILIM           100             //bipolar; received parameters not within the tolerance for SILIM
#define _BipSMTYP           101             //bipolar; parameters not within tolerance for switch on bipolar
#define _BipETESTIN         102             //bipolar; ETESTIN current measure not within the tolerance
#define _BipSPHC            103             //bipolar; SPHC parameter not within the tolerance
                            //LIN errors
#define _LinSILIM           150             //LIN; received parameters not within the tolerance for SILIM
#define _LinSMTYP           151             //LIN; parameters not within tolerance for switch on LIN
#define _LinETESTIN         152             //LIN; ETESTIN command is not allowed for LIN application's
#define _LinRUN             153             //LIN; RUN command is not allowed for LIN application's
#define _LinSCOILON         154             //LIN; SCOILON command is not allowed for LIN application's
                            //command errors
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
#define _IdSSMOD            35              //-KO- set step mode
#define _IdGSMOD            36              //-KO- get step mode
#define _IdSERUN            37              //-KO- set the E-running parameters with stall detection
#define _IdGERUN            38              //get the E-running parameters
#define _IdERUN             39              //run the motor with stall detection and free run
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

#endif	/* DEFINES_H */

