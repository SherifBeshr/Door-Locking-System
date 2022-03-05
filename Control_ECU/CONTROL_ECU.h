/*
 * CONTROL_ECU.h
 * Author: Sherif Beshr
 */

#ifndef CONTROL_ECU_H_
#define CONTROL_ECU_H_

#include "uart.h"
#include "gpio.h"
#include "dc_motor.h"
#include "external_eeprom.h"
#include "std_types.h"
#include "util/delay.h"
#include "twi.h"
#include "common_macros.h"
#include "buzzer.h"


/*********************************************UART MESSAGES**********************************************/

#define CONTROL_ECU_READY 	0x10
#define HMI_ECU_READY		0x11
#define PASS_MATCH			0x12
#define PASS_UNMATCH		0x13
#define MAIN_OPTIONS		0x14
#define TIME_15_SEC			0x15
#define START_TIME_15_SEC	0x16
#define START_TIME_3_SEC	0x17
#define TIME_3_SEC			0x18
#define TIME_60_SEC			0x19
#define START_TIME_60_SEC	0x20

/***********************************************DEFINES************************************************/

#define ERROR				0
#define PASS				1
#define MAX_PASSWORD		15
#define MAX_FAIL_TRIALS		3

/*****************************************FUNCTIONS DECLARATIONS******************************************/

/*-------------------------------------------------------------------------------------------------------
 * [Description]:	Function that receives password form HMI with UART and stores it in array
 *------------------------------------------------------------------------------------------------------*/
void receive_Password(uint8 *password);

/*-------------------------------------------------------------------------------------------------------
 * [Description]:	Function that saves the password in EEPROM that uses I2C communication protocol
 *------------------------------------------------------------------------------------------------------*/
void save_password(uint8 *pass);

/*-------------------------------------------------------------------------------------------------------
 * [Description]:	Function that opens door with motor by rotating motor in clock-wise direction for
 * 					15 seconds, stops it for 3 seconds, then closes door by rotating anti-clock-wise
 * 					for another 15 seconds and finally stops the motor.
 *------------------------------------------------------------------------------------------------------*/
void openDoor();

/*-------------------------------------------------------------------------------------------------------
 * [Description]:	Function that checks if the entered password is correct with the password saved
 * 					in the EEPROM by reading password stored in EEPROM and comparing to received one
 *------------------------------------------------------------------------------------------------------*/
void check_password(uint8 *entered_password, uint8 *Saved_Password);

/*-------------------------------------------------------------------------------------------------------
 * [Description]:	Function that saves the new password in array Pass1 as the new password is received
 * 					with UART from HMI ECU
 *------------------------------------------------------------------------------------------------------*/
void changePassword(uint8 *newPassword, uint8 *savedPass);

/*-------------------------------------------------------------------------------------------------------
 * [Description]:	Function that compares first and second password arrays of first entry system if
 * 					matching return indication that they match and vice versa if doesn't match
 *------------------------------------------------------------------------------------------------------*/
uint8 Pass_Compare(uint8 *pass1, uint8 *pass2);




#endif /* CONTROL_ECU_H_ */
