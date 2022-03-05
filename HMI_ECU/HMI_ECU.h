/*
 * HMI_ECU.h
 * Author: Sherif Beshr
 */

#ifndef HMI_ECU_H_
#define HMI_ECU_H_


#include "keypad.h"
#include "lcd.h"
#include "uart.h"
#include "timer.h"
#include "std_types.h"
#include "util/delay.h"
#include <avr/io.h>


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

/***********************************************DEFINITIONS************************************************/

#define MAX_FAIL_TRIALS		3


/*****************************************FUNCTIONS DECLARATIONS******************************************/

/* [Description]: Function that displays the keys pressed for first entry */
void pass_Enter_1(void);

/* [Description]: Function that displays the keys pressed for second entry */
void pass_Enter_2(void);

/* [Description]: Function that displays Enter password for checking password entry */
void send_password(void);

/*	[Description]:	Function to count time fort the door opening, keeping still, and closing*/
void openDoorTimer1();

/* [Description]: Function that displays status of the Door if opening or closing (If password matches),
 * and displays Wrong password (If password doesn't match)
 * then displays ALERT (If Maximum trials exceeded)
 */
void openDoorMatch(Timer_ConfigType *Timer1);

/* [Description]: Function that count 60 seconds then tells CONTROL ECU that it finished count	*/
void Buzzer_fn();

/* [Description]: Function that sends old password to check. If password match sends new password,
 * if password doesn't match you have MAX_TRIALS to try password again then alert will be displayed
 */
void changePassword(Timer_ConfigType *Timer1);

/* [Description]: Function that displays if two entries matched or no, if yes displays Saving password */
uint8 pass_status(uint8 status);

/* [Description]: Function that displays the main options to select from and keeps looping if
 * a non available option is pressed */
uint8 main_options(void);


#endif /* HMI_ECU_H_ */
