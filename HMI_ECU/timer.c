/******************************************************************************************************
File Name	: timer.c
Author		: Sherif Beshr
Description : Source file for the Timer AVR driver
 *******************************************************************************************************/

#include <avr/io.h>
#include <avr/interrupt.h>
#include "timer.h"

/***************************************************************************************************
 *                                		Global Variables                                    	   *
 ***************************************************************************************************/

/* Global variables to hold the address of the call back function in the application */
static volatile void (*g_Timer0_callBackPtr)(void) = NULL_PTR;
static volatile void (*g_Timer1_callBackPtr)(void) = NULL_PTR;
static volatile void (*g_Timer2_callBackPtr)(void) = NULL_PTR;


/***************************************************************************************************
 *                                	Interrupt Service Routine                                      *
 ***************************************************************************************************/

/*	Timer0 callback function for overflow mode*/
ISR(TIMER0_OVF_vect)
{
	if (g_Timer0_callBackPtr != NULL_PTR)
	{
		/* Call the Call Back function in the application after the edge is detected */
		(*g_Timer0_callBackPtr)(); /* call the function using pointer to function g_Timer0_callBackPtr(); */
	}
}

/*	Timer0 callback function for compare mode*/
ISR(TIMER0_COMP_vect)
{
	if (g_Timer0_callBackPtr != NULL_PTR)
	{
		/* Call the Call Back function in the application after the edge is detected */
		(*g_Timer0_callBackPtr)(); /* call the function using pointer to function g_Timer0_callBackPtr(); */
	}
}

/*	Timer1 callback function for overflow mode*/
ISR(TIMER1_OVF_vect)
{
	if (g_Timer1_callBackPtr != NULL_PTR)
	{
		/* Call the Call Back function in the application after the edge is detected */
		(*g_Timer1_callBackPtr)(); /* call the function using pointer to function g_Timer1_callBackPtr(); */
	}
}

/*	Timer1 callback function for compare (A) mode*/
ISR(TIMER1_COMPA_vect)
{
	if (g_Timer1_callBackPtr != NULL_PTR)
	{
		/* Call the Call Back function in the application after the edge is detected */
		(*g_Timer1_callBackPtr)(); /* call the function using pointer to function g_Timer1_callBackPtr(); */
	}
}

/*	Timer1 callback function for compare (B) mode*/
ISR(TIMER1_COMPB_vect)
{
	if (g_Timer1_callBackPtr != NULL_PTR)
	{
		/* Call the Call Back function in the application after the edge is detected */
		(*g_Timer1_callBackPtr)(); /* call the function using pointer to function g_Timer1_callBackPtr(); */
	}
}

/*	Timer2 callback function for overflow mode*/
ISR(TIMER2_OVF_vect)
{
	if (g_Timer2_callBackPtr != NULL_PTR)
	{
		/* Call the Call Back function in the application after the edge is detected */
		(*g_Timer2_callBackPtr)(); /* call the function using pointer to function g_Timer2_callBackPtr(); */
	}
}

/*	Timer2 callback function for compare mode*/
ISR(TIMER2_COMP_vect)
{
	if (g_Timer2_callBackPtr != NULL_PTR)
	{
		/* Call the Call Back function in the application after the edge is detected */
		(*g_Timer2_callBackPtr)(); /* call the function using pointer to function g_Timer2_callBackPtr(); */
	}
}


/***************************************************************************************************
 *                                		Function Definitions                                  	   *
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
void Timer_init(const Timer_ConfigType* Config_Ptr)
{
	switch (Config_Ptr->Timerx_ID)
	{

	/**************************************************************************)*
	 *                                	Timer0                   	   			*
	 ****************************************************************************/
	case (TIMER0_ID):
																		/* FOCx is always set when Timer is not in PWM mode. Clears all register */
																		TCCR0 = (1<<FOC0);

	/* Set the start value */
	TCNT0 = Config_Ptr->Start_value;

	/* Set the pre-scalar or timer source */
	TCCR0 = (TCCR0 & 0xF8) | (Config_Ptr->Timer_Source << CS00);

	/* If timer compare mode is selected set the WGM01 = 1
	 * set the COM0x from compare match mode
	 * set OCRx value*/
	if(Config_Ptr->Timer_mode == TIMER_COMPARE_MODE)
	{
		TCCR0 = (TCCR0 & 0xB7) | (1<<WGM01);			/* Clears WGM00 and Set WGM01*/
		TCCR0 = (TCCR0 & 0xCF) | (Config_Ptr->Timer_Compare_Match << COM00);
		OCR0  = Config_Ptr->Compare_value;
		TIMSK |= (1<<OCIE0);								/* Enable Timer0 compare interrupt */
	}
	else if(Config_Ptr->Timer_mode == TIMER_NORMAL_MODE)
	{
		TIMSK |= (1<<TOIE0);								/* Enable Timer0 overflow interrupt */
	}
	break;

	/**************************************************************************)*
	 *                                	Timer1                   	   			*
	 ****************************************************************************/
	case (TIMER1_ID):
																		/* FOCx is always set when Timer is not in PWM mode. Clears all register */
																		TCCR1A |= (1<<FOC1A) | (1<<FOC1B);

	/* Set the start value */
	TCNT1 = Config_Ptr->Start_value;

	/* Set the pre-scalar or timer source */
	TCCR1B = (TCCR1B & 0xF8) | (Config_Ptr->Timer_Source << CS10);

	/* If timer compare mode is selected set the WGM01 = 1
	 * set the COM0x from compare match mode
	 * set OCRx value*/
	if(Config_Ptr->Timer_mode == TIMER_COMPARE_MODE)
	{
		TCCR1B = (TCCR1B & 0xE7) | (1<<WGM12);				/* Clears WGM13 and Set WGM12 (Mode 4 CTC)*/
		TCCR1A = (TCCR1A & 0x3F) | (Config_Ptr->Timer_Compare_Match << COM1A0);
		OCR1A  = Config_Ptr->Compare_value;
		TIMSK |= (1<<OCIE1A);								/* Enable Timer1 compare interrupt */
	}
	else if(Config_Ptr->Timer_mode == TIMER_NORMAL_MODE)
	{
		TIMSK |= (1<<TOIE1);								/* Enable Timer1 overflow interrupt */
	}
	break;


	/**************************************************************************)*
	 *                                	Timer2                   	   			*
	 ****************************************************************************/
	case (TIMER2_ID):
																		/* FOCx is always set when Timer is not in PWM mode. Clears all register */
																		TCCR2 = (1<<FOC2);

	/* Set the start value */
	TCNT2 = Config_Ptr->Start_value;

	/* Set the pre-scalar or timer source */
	TCCR2 = (TCCR2 & 0xF8) | (Config_Ptr->Timer_Source << CS20);

	/* If timer compare mode is selected set the WGM01 = 1
	 * set the COM0x from compare match mode
	 * set OCRx value*/
	if(Config_Ptr->Timer_mode == TIMER_COMPARE_MODE)
	{
		TCCR2 = (TCCR2 & 0xB7) | (1<<WGM21);			/* Clears WGM20 and Set WGM21*/
		TCCR2 = (TCCR2 & 0xCF) | (Config_Ptr->Timer_Compare_Match << COM20);
		OCR2  = Config_Ptr->Compare_value;
		TIMSK |= (1<<OCIE2);								/* Enable Timer2 compare interrupt */
	}
	else if(Config_Ptr->Timer_mode == TIMER_NORMAL_MODE)
	{
		TIMSK |= (1<<TOIE2);								/* Enable Timer2 overflow interrupt */
	}
	break;
	}
}


/*
 * Description: Function to set the Call Back function address.
 */
void Timer_setCallBack(Timer_ID timer_ID, void(*a_ptr)(void))
{
	if(timer_ID == TIMER0_ID)
	{
		/* Save the address of the Call back function in a global variable of Timer0 */
		g_Timer0_callBackPtr = a_ptr;
	}
	else if(timer_ID == TIMER1_ID)
	{
		/* Save the address of the Call back function in a global variable of Timer1 */
		g_Timer1_callBackPtr = a_ptr;
	}
	else if(timer_ID == TIMER2_ID)
	{
		/* Save the address of the Call back function in a global variable of Timer2 */
		g_Timer2_callBackPtr = a_ptr;
	}
}


/*
 * Description :
 * De-Initialize the Timerx for the chosen timer (TIMER0_ID / TIMER1_ID / TIMER2_ID)
 */
void Timer_deinit(Timer_ID timer_ID)
{
	if(timer_ID == TIMER0_ID)				/* De-initialize Timer0 */
	{
		TCCR0 = 0;
		TCNT1 = 0;
		TIMSK &= ~(1<<TOIE0);
		TIMSK &= ~(1<<OCIE0);
	}

	else if(timer_ID == TIMER1_ID)			/* De-initialize Timer1 */
	{
		TCCR1A = 0;
		TCCR1B = 0;
		TCNT1 = 0;
		TIMSK &= ~(1<<TOIE1);
		TIMSK &= ~(1<<OCIE1A);
		TIMSK &= ~(1<<OCIE1B);
	}

	else if(timer_ID == TIMER2_ID)			/* De-initialize Timer2 */
	{
		TCCR2 = 0;
		TCNT2 = 0;
		TIMSK &= ~(1<<TOIE2);
		TIMSK &= ~(1<<OCIE2);
	}
}

/*
 * Description: Function to set the Initial value of selected timer.
 */
void Timer_SetStartValue(Timer_ID timer_ID, uint16 start_value)
{
	if(timer_ID == TIMER0_ID)			/* Set initial value for Timer0 */
	{
		TCNT0 = start_value;
	}

	else if(timer_ID == TIMER1_ID)			/* Set initial value for Timer1 */
	{
		TCNT1 = start_value;
	}

	else if(timer_ID == TIMER2_ID)			/* Set initial value for Timer2 */
	{
		TCNT2 = start_value;
	}
}

/*
 * Description: Function to set the Compare Value of the selected timer.
 */
void Timer_SetCompareValue(Timer_ID timer_ID, uint16 compare_value)
{
	if(timer_ID == TIMER0_ID)			/* Set compare value for Timer0 */
	{
		OCR0 = compare_value;
	}

	else if(timer_ID == TIMER1_ID)			/* Set compare value for Timer1 */
	{
		OCR1A = compare_value;
	}

	else if(timer_ID == TIMER2_ID)			/* Set compare value for Timer2 */
	{
		OCR2 = compare_value;
	}
}
