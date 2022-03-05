/*
 * 	HMI ECU
 *  Author: Sherif	Beshr
 */

#include "HMI_ECU.h"

/********************************************GLOBAL VARIABLES*********************************************/

volatile static uint8 g_OpenDoorTick = 0;
volatile static uint8 g_BuzzerTick = 0;
volatile static uint8 g_Timer_Flag = 0;
static uint8 g_FirstTime_flag = 0;
uint8 pass_matching = PASS_UNMATCH;
uint8 g_fail_count = MAX_FAIL_TRIALS;



/*-------------------------------------------------------------------------------------------------------
 * [Description]: Main function that initializes the LCD, Timer, UART and waits for other MCU to be ready
 *------------------------------------------------------------------------------------------------------*/
int main (void)
{
	/* LCD Initialization on PORTB */
	LCD_init();

	/* Timer 1 Configurations to count 15 seconds
	 * time per cycle = 8MHz / 1024 = 128uS
	 * Cycles for 15 seconds = 15 / 0.000128 = 117,188 cycles
	 * 117,188 / 2 = 58594 (On two Cycles with compare mode)
	 */
	Timer_ConfigType Timer1;
	Timer1.Compare_value = 58594;
	Timer1.Start_value = 0;
	Timer1.Timer_Source = TIMER1_PRESCALAR_1024;
	Timer1.Timer_mode = TIMER_COMPARE_MODE;
	Timer1.Timerx_ID = TIMER1_ID;

	/*UART Initialization
	 * 1- Baud Rate : 9600
	 * 2- Data Bits : 8
	 * 3- Parity	: Disable
	 * 4- Stop Bits : 1
	 */
	UART_ConfigType UART_Config;
	UART_Config.BaudRate = Baud_9600;
	UART_Config.DataBits = Data_8;
	UART_Config.ParityMode = Parity_Disable;
	UART_Config.StopBits = StopBits_1;
	UART_init(&UART_Config);

	/*	Waits Until the other MCU is ready to communicate */
	UART_sendByte(HMI_ECU_READY);
	while(UART_receiveByte() != CONTROL_ECU_READY){}

	uint8 mainOptionKey = 0;									/* Variable that holds the mode '+' or '-' */

	SREG |= (1<<7);												/* Enables I-bit for timer */

	/*******************************************SUPER LOOP*******************************************/
	for(;;)
	{
		/* Checks flag if it is the first time to use the system */
		if(g_FirstTime_flag == 0)
		{
			/* Keeps entering new password until it matches first and second time */
			while(pass_matching == PASS_UNMATCH)
			{
				pass_Enter_1();
				pass_Enter_2();
				pass_matching = UART_receiveByte();
				pass_status(pass_matching);
			}
		}


		g_FirstTime_flag = 1;									/* Sets first time use flag to 1 to not enter again */

		/* Variable that holds the mode '+' or '-' */
		mainOptionKey = main_options();
		if(mainOptionKey == '-')								/* Open Door mode */
		{
			openDoorMatch(&Timer1);
		}
		else if(mainOptionKey == '+')							/* Change Password mode */
		{
			changePassword(&Timer1);
		}
	}
}

/*-------------------------------------------------------------------------------------------------------
 * [Description]: Function that displays the keys pressed for first entry
 *------------------------------------------------------------------------------------------------------*/
void pass_Enter_1(void)
{
	LCD_displayStringRowColumn(0,0,"Enter New Pass:  ");
	LCD_displayStringRowColumn(1, 0, "                ");
	LCD_moveCursor(1, 0);
	uint8 key = 0;
	_delay_ms(300);
	while(key != '=')
	{
		/* Waits until keypad is pressed and save number in key variable*/
		key = KEYPAD_getPressedKey();
		/* Sends numbers and the enter key '=' only */
		if( (key>=0 && key<=9) || key == '=')
			UART_sendByte(key);
		/* Displays (*) each time a key is pressed */
		if(key != '=')
		{
			LCD_displayCharacter('*');
		}
		/* Small delay between each key press  to avoid repetition*/
		_delay_ms(250);
	}
}

/*-------------------------------------------------------------------------------------------------------
 * [Description]: Function that displays the keys pressed for second entry
 *------------------------------------------------------------------------------------------------------*/
void pass_Enter_2(void)
{
	uint8 key = 0;
	LCD_displayStringRowColumn(0, 0, "Re-enter Pass:  ");
	LCD_displayStringRowColumn(1, 0, "                ");
	LCD_moveCursor(1, 0);
	while(key != '=')
	{
		/* Waits until keypad is pressed and save number in key variable*/
		key = KEYPAD_getPressedKey();
		/* Sends numbers and the enter key '=' only */
		if( (key>=0 && key<=9) || key == '=')
			UART_sendByte(key);
		/* Displays (*) each time a key is pressed */
		if(key != '=')
			LCD_displayCharacter('*');
		/* Small delay between each key press  to avoid repetition*/
		_delay_ms(250);
	}
}

/*-------------------------------------------------------------------------------------------------------
 * [Description]: Function that displays if two entries matched or no, if yes displays Saving password
 *------------------------------------------------------------------------------------------------------*/
uint8 pass_status(uint8 status)
{
	/* If password doesn't match displays Wrong password on LCD for 1 second and return UNMATCH */
	if(status == PASS_UNMATCH)
	{
		LCD_clearScreen();
		LCD_displayString("WRONG PASS!");
		_delay_ms(1000);
		return PASS_UNMATCH;
	}
	/* If password match displays Correct password and Saving Pass on LCD for 1 second and return MATCH */
	else if(status == PASS_MATCH)
	{
		LCD_clearScreen();
		LCD_displayString("CORRECT PASS");
		LCD_displayStringRowColumn(1, 0, "Saving Pass");
		_delay_ms(1000);
		LCD_clearScreen();
		return PASS_MATCH;
	}
	return PASS_UNMATCH;
}

/*-------------------------------------------------------------------------------------------------------
 * [Description]: Function that displays the main options to select from and keeps looping if
 * a non available option is pressed
 *------------------------------------------------------------------------------------------------------*/
uint8 main_options(void)
{
	uint8 key = 0;
	/* Displays the main options on LCD */
	LCD_clearScreen();
	LCD_displayString("+ : Change PASS ");
	LCD_displayStringRowColumn(1, 0, "- : Open Door   ");
	/* Keeps waiting until an available key is pressed */
	while(key != '+' || key!= '-')
	{
		/* Waits until keypad is pressed and save option in key variable*/
		key = KEYPAD_getPressedKey();
		/* Send key to Control ECU if only available option is pressed*/
		if(key == '+' || key == '-')
		{
			UART_sendByte(key);
			return key;
		}
	}
	return key;
}

/*-------------------------------------------------------------------------------------------------------
 * [Description]: Function that displays Enter password for checking password entry
 *------------------------------------------------------------------------------------------------------*/
void send_password(void)
{
	uint8 key = 0;
	/* Displays Enter PASS on LCD and and sends password with UART to Control ECU*/
	LCD_clearScreen();
	LCD_displayString("Enter PASS");
	_delay_ms(500);
	LCD_moveCursor(1, 0);
	/* Keeps sending password until enter is pressed or in this case '=' */
	while(key != '=')
	{
		key = KEYPAD_getPressedKey();
		/* Only sends available password keys from 0 to 9 or = to act as password end */
		if( (key>=0 && key<=9) || key == '=')
			UART_sendByte(key);
		/* Displays (*) if numbers are pressed as indicator that they are pressed successfully */
		if(key != '=')
		{
			LCD_displayCharacter('*');
		}
		/* Delay between each key press to avoid repetition */
		_delay_ms(250);
	}
	if(key == '=')
	{
		UART_sendByte(key);
	}
}

/*-------------------------------------------------------------------------------------------------------
 * [Description]: Function that displays status of the Door if opening or closing (If password matches),
 * and displays Wrong password (If password doesn't match)
 * then displays ALERT (If Maximum trials exceeded)
 *------------------------------------------------------------------------------------------------------*/
void openDoorMatch(Timer_ConfigType *Timer1)
{
	/* Send to Control ECU that HMI is ready */
	UART_sendByte(HMI_ECU_READY);
	pass_matching = PASS_UNMATCH;
	/* Keeps looping until password matches */
	while(pass_matching == PASS_UNMATCH)
	{
		send_password();											/* Send password function send to Control ECU */
		_delay_ms(300);
		pass_matching = UART_receiveByte();

		if(pass_matching == PASS_UNMATCH)
		{
			/* If password doesn't match decrement the fail times */
			--g_fail_count;
			LCD_clearScreen();
			/* Displays wrong password and the remaining fail times */
			LCD_displayString("Wrong Password");
			LCD_displayStringRowColumn(1, 0, "Trials Remain: ");
			LCD_intgerToString(g_fail_count);
			_delay_ms(2000);
			UART_sendByte(HMI_ECU_READY);							/* Tells MCU2 that MCU1 is ready */
			/* Checks if Max fails reached to display ALERT */
			if(g_fail_count == 0)
			{
				LCD_clearScreen();
				LCD_displayString("Alert Thief!!");
				Timer1->Compare_value = 58594;						/*	Activate Buzzer 1 minute	*/
				/* Starts Timer1 to count for 60 seconds */
				Timer_init(Timer1);
				Timer_setCallBack(TIMER1_ID, Buzzer_fn);
				g_fail_count = MAX_FAIL_TRIALS;						/* Resets Max fail trials counter */
				while(UART_receiveByte() != CONTROL_ECU_READY);
				break;
			}
		}
		else
		{
			LCD_clearScreen();
			LCD_displayString("OPENING...");
			UART_sendByte(HMI_ECU_READY);								/* Tells MCU2 that MCU1 is ready */
			while(UART_receiveByte() != START_TIME_15_SEC);
			/* Starts Timer1 to count for 15 seconds door opening */
			Timer_setCallBack(TIMER1_ID, openDoorTimer1);
			Timer_init(Timer1);
			while(g_Timer_Flag == 0);
			g_Timer_Flag = 1;
		}
	}
}

/*-------------------------------------------------------------------------------------------------------
 * [Description]:	Function to count time fort the door opening, keeping still, and closing
 *------------------------------------------------------------------------------------------------------*/
void openDoorTimer1()
{
	++g_OpenDoorTick;
	if(g_OpenDoorTick == 2)											/* 15 Seconds Passed */
	{
		Timer_SetCompareValue(TIMER1_ID, 23438);					/* Wait 3 Seconds Door Opened*/
		UART_sendByte(TIME_15_SEC);
		while(UART_receiveByte() != CONTROL_ECU_READY);
		LCD_clearScreen();
		LCD_displayString("Door Opened");
	}
	else if(g_OpenDoorTick == 3)
	{
		Timer_SetCompareValue(TIMER1_ID, 58594);					/* Wait 15 Seconds Closing Door */
		UART_sendByte(TIME_3_SEC);
		while(UART_receiveByte() != CONTROL_ECU_READY);
		LCD_clearScreen();
		LCD_displayString("Closing Door...");
	}
	else if(g_OpenDoorTick == 5)
	{
		/* Send that 15 Seconds are counted to Control ECU */
		UART_sendByte(TIME_15_SEC);
		while(UART_receiveByte() != CONTROL_ECU_READY);
		g_OpenDoorTick = 0;										/* Resets ISR count */
		Timer_deinit(TIMER1_ID);								/* Stops timer */
		LCD_clearScreen();
		LCD_displayString("Door Closed");
		g_Timer_Flag = 1;
	}
}

/*-------------------------------------------------------------------------------------------------------
 * [Description]:
 * Function that count 60 seconds then tells CONTROL ECU that it finished count
 *------------------------------------------------------------------------------------------------------*/
void Buzzer_fn()
{
	++g_BuzzerTick;
	/* Displays the main options on LCD */
	if(g_BuzzerTick == 8)
	{
		UART_sendByte(TIME_60_SEC);
	}

}

/*-------------------------------------------------------------------------------------------------------
 * [Description]:
 * Function that sends old password to check. If password match sends new password,
 * if password doesn't match you have MAX_TRIALS to try password again then alert will be displayed
 *------------------------------------------------------------------------------------------------------*/
void changePassword(Timer_ConfigType *Timer1)
{
	UART_sendByte(HMI_ECU_READY);
	pass_matching = PASS_UNMATCH;
	while(pass_matching == PASS_UNMATCH)
	{
		send_password();											/* Send password function send to Control ECU */
		_delay_ms(300);
		pass_matching = UART_receiveByte();

		if(pass_matching == PASS_UNMATCH)
		{
			/* If password doesn't match decrement the fail times */
			--g_fail_count;
			/* Displays wrong password and the remaining fail times */
			LCD_clearScreen();
			LCD_displayString("Wrong Password");
			LCD_displayStringRowColumn(1, 0, "Trials Remain: ");
			LCD_intgerToString(g_fail_count);
			_delay_ms(2000);
			UART_sendByte(HMI_ECU_READY);							/* Tells MCU2 that MCU1 is ready */
			/* Checks if Max fails reached to display ALERT */
			if(g_fail_count == 0)
			{
				LCD_clearScreen();
				LCD_displayString("Alert Thief!!");
				Timer1->Compare_value = 58594;						/*	Activate displaying ALERT 1 minute	*/
				Timer_init(Timer1);									/* Starts Timer1 to count for 60 seconds ALERT*/
				Timer_setCallBack(TIMER1_ID, Buzzer_fn);
				g_fail_count = MAX_FAIL_TRIALS;
				while(UART_receiveByte() != CONTROL_ECU_READY);
				break;
			}
		}
		/* If password matched it sends the New Password to control ECU */
		else
		{
			pass_Enter_1();
		}
	}
}
