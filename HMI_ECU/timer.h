/******************************************************************************************************
File Name	: timer.h
Author		: Sherif Beshr
Description : Header file for the Timer AVR driver
 *******************************************************************************************************/

#ifndef TIMER_H_
#define TIMER_H_

#include "std_types.h"

/***************************************************************************************************
 *                                		Types Decelerations                                  	   *
 ***************************************************************************************************/

/*	Timer Select	*/
typedef enum
{
	TIMER0_ID, TIMER1_ID, TIMER2_ID
}Timer_ID;

/*	Timer Select	*/
typedef enum
{
	TIMER_NORMAL_MODE, TIMER_COMPARE_MODE=2
}Timer_Mode;

/*	Compare Match Mode	*/
typedef enum
{
	TIMERx_COMPARE_NORMAL_NO_OCx, TIMERx_COMPARE_TOGGLE_OCx, TIMERx_COMPARE_CLEAR_OCx, TIMERx_COMPARE_SET_OCx
}Timer_Compare_Match;

/*	Timer Pre-scalar / Source	*/
typedef enum
{
	TIMER0_NO_CLOCK, TIMER0_PRESCALAR_1,  TIMER0_PRESCALAR_8,  TIMER0_PRESCALAR_64,  TIMER0_PRESCALAR_256,\
	TIMER0_PRESCALAR_1024, TIMER0_EXTERNAL_FALLING, TIMER0_EXTERNAL_RISING,\

	TIMER1_NO_CLOCK=0, TIMER1_PRESCALAR_1,  TIMER1_PRESCALAR_8,  TIMER1_PRESCALAR_64,  TIMER1_PRESCALAR_256,\
	TIMER1_PRESCALAR_1024, TIMER1_EXTERNAL_FALLING, TIMER1_EXTERNAL_RISING,\

	TIMER2_NO_CLOCK=0, TIMER2_PRESCALAR_1,  TIMER2_PRESCALAR_8,  TIMER2_PRESCALAR_32,  TIMER2_PRESCALAR_64,\
	TIMER2_PRESCALAR_128, TIMER2_PRESCALAR_256, TIMER2_PRESCALAR_1024,
}Timer_Source;

/*	Structure accessed to choose the Timer:
 * 	1- Timer to initialize from (Timer0/Timer1/Timer2)
 *  2- Timer starting value (TCNTx)
 *  3- Timer mode (Normal / Compare)
 *  4- Timer compare value (OCRx) [ Only for Compare mode ]
 *  5- Timer source (Pre-scalar / No Clock / External Clock [Timer0/Timer1 Only])
 *  6- Timer Compare Match (No OCx, Toggle OCx, Clear OCx, Set OCx)	[Only for compare mode]
 */
typedef struct{
	uint16				Start_value;
	uint16				Compare_value;
	Timer_ID 			Timerx_ID;
	Timer_Mode 			Timer_mode;
	Timer_Source		Timer_Source;
	Timer_Compare_Match	Timer_Compare_Match;
}Timer_ConfigType;

/***************************************************************************************************
 *                                		Function Prototypes                                 	   *
 ***************************************************************************************************/

/*
 * Description :
 * Initialize the Timer with configurable inputs:
 * 1- Timerx_ID: 			Choose from (TIMER0_ID / TIMER1_ID / TIMER2_ID)
 * 2- Start Value: 			0 -> 255 (Timer0/Timer2) and 0 -> 65535 (Timer1)
 * 3- Timer Mode: 			Choose (TIMER_NORMAL_MODE / TIMER_COMPARE_MODE)
 * 4- Compare Value: 		0 -> 255 (Timer0/Timer2) and 0 -> 65535 (Timer1)
 * 5- Timerx Source:		Choose from (No Clock/ Pre-scalar / External Clock)
 * 6- Timer Compare Match:	Choose from (No OCx, Toggle OCx, Clear OCx, Set OCx)	[Only for compare mode]
 */
void Timer_init(const Timer_ConfigType* Config_Ptr);

/*
 * Description :
 * De-Initialize the Timerx for the chosen timer (TIMER0_ID / TIMER1_ID / TIMER2_ID)
 */
void Timer_deinit(Timer_ID timer_ID);

/*
 * Description: Function to set the Call Back function address.
 */
void Timer_setCallBack(Timer_ID timer_ID, void(*a_ptr)(void));

/*
 * Description: Function to set the Initial value of selected timer.
 */
void Timer_SetStartValue(Timer_ID timer_ID, uint16 start_value);

/*
 * Description: Function to set the Compare Value of the selected timer.
 */
void Timer_SetCompareValue(Timer_ID timer_ID, uint16 compare_value);


#endif /* TIMER_H_ */
