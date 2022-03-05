/*
 *	Control ECU
 *  Author: Sherif	Beshr
 */

#include "CONTROL_ECU.h"

/********************************************GLOBAL VARIABLES*********************************************/

static uint8 g_fail_count = MAX_FAIL_TRIALS;
static uint8 g_Passwrod_Status = PASS_UNMATCH;


/*-------------------------------------------------------------------------------------------------------
 * [Description]:	Main function that initializes the BUZZER, UART, I2C and waits for the other MCU
 * 					to be ready
 *------------------------------------------------------------------------------------------------------*/
int main (void)
{

	uint8 pass1[MAX_PASSWORD] = {0};					/* Array to save first entry password */
	uint8 entered_password[MAX_PASSWORD] = {0};			/* Array to save entered passwords to check matching */

	/* Buzzer initialization */
	Buzzer_init();

	/* DC Motor Initialization on PORTD PIN6 & PIN7 */
	DcMotor_Init();

	/* UART Initialization
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


	/* I2C Initialization
	 * 1- I2C Rate  	: 400KHz
	 * 2- I2C Address	: 0x02
	 * 3- Pre-scalar	: No Pre-scalar

	 */
	TWI_ConfigType TWI_Config  = { 400000, 0x02, TWI_PRESCALAR_1};
	TWI_init(&TWI_Config);


	/*	Waits Until the other MCU is ready to communicate */
	while(UART_receiveByte() != HMI_ECU_READY){}
	UART_sendByte(CONTROL_ECU_READY);

	uint8 status = ERROR;
	uint8 key = 0;
	uint8 i;

	/****************************************	SUPER LOOP	****************************************/
	for(;;)
	{
		/* Loop for Max fail trials in New Password mode */
		while(status != PASS)
		{
			/* RESET Strings if doesn't match to avoid errors*/
			for(i=0 ; i<MAX_PASSWORD ; ++i)
			{
				pass1[i] = 0;
				entered_password[i] = 0;
			}

			receive_Password(pass1);				/* Saves First entry password in first array */
			receive_Password(entered_password);				/* Saves Second entry password in second array */
			status = Pass_Compare(pass1,entered_password);		/* Compares first and second password matching*/
		}
		/* Saves Password if entry matches */
		save_password(pass1);

		/* Variable that holds the received mode by UART mode '+' or '-' */
		key = UART_receiveByte();
		if(key == '-')														/* Open Door */
		{
			while(g_Passwrod_Status == PASS_UNMATCH)
			{
				while (UART_receiveByte() != HMI_ECU_READY);
				receive_Password(entered_password);
				check_password(entered_password, pass1);					/* Check Password */
			}
			/* Calls open door function if password match */
			openDoor();
		}
		else if(key == '+')
		{
			while(g_Passwrod_Status == PASS_UNMATCH)
			{
				while (UART_receiveByte() != HMI_ECU_READY);
				receive_Password(entered_password);
				check_password(entered_password, pass1);					/* Check Password */
			}
			/* Calls change password function if password match */
			changePassword(entered_password, pass1);
		}
	}
}


/*-------------------------------------------------------------------------------------------------------
 * [Description]:	Function that receives password form HMI with UART and stores it in array
 *------------------------------------------------------------------------------------------------------*/
void receive_Password(uint8 *password)
{
	uint8 i = 0, flag_pass=0;

	/* Keeps storing password in array passed until key sent is enter (=) */
	while(flag_pass == 0)
	{
		password[i] = UART_receiveByte();
		if(password[i] == '=')
		{
			flag_pass = 1;
			password[i] = '\0';
			++i;
			break;
		}
		++i;
	}
}

/*-------------------------------------------------------------------------------------------------------
 * [Description]:	Function that compares first and second password arrays of first entry system if
 * 					matching return indication that they match and vice versa if doesn't match
 *------------------------------------------------------------------------------------------------------*/
uint8 Pass_Compare(uint8 *pass1, uint8 *entered_password)
{
	uint8 i;

	for(i=0 ; i < MAX_PASSWORD ; ++i)
	{
		if(pass1[i] != entered_password[i])
		{
			UART_sendByte(PASS_UNMATCH);		/* Tells HMI ECU that passwords doesn't match */
			return ERROR;
		}
	}
	UART_sendByte(PASS_MATCH);					/* Tells HMI ECU that passwords match */
	return PASS;
}

/*-------------------------------------------------------------------------------------------------------
 * [Description]:	Function that saves the password in EEPROM that uses I2C communication protocol
 *------------------------------------------------------------------------------------------------------*/
void save_password(uint8 *password)
{
	uint8 i = 0;
	/* Saves password characters until null*/
	while(password[i] != '\0')
	{
		/* Saves Password in the following address and increment address*/
		EEPROM_writeByte((0xF000+i), password[i]);
		++i;
	}
	/*	Saves null as an indication for password end */
	EEPROM_writeByte((0xF000+i), '\0');
}


/*-------------------------------------------------------------------------------------------------------
 * [Description]:	Function that checks if the entered password is correct with the password saved
 * 					in the EEPROM by reading password stored in EEPROM and comparing to received one
 *------------------------------------------------------------------------------------------------------*/
void check_password(uint8 *entered_password, uint8 *Saved_Password)
{
	uint8 i = 0;
	uint8 status = 0;		/* status that indicates if password comparison is matching[0] or not[1] */
	while(Saved_Password[i] != '\0')
	{
		EEPROM_readByte((0x100+i), &Saved_Password[i]);
		if(Saved_Password[i] != entered_password[i])
		{
			status = 1;
			break;											/* If un-match in numbers don't loop to the end */
		}
		else
		{
			status = 0;
		}
		++i;
	}
	/* Check that entered password matches with saved password till the end for example
	 * Saved Pass = 245, entered password 2457 ( Will Match without the next code ).
	 */
	if(Saved_Password[i] != entered_password[i])
			{
				status = 1;		/* Password end doesn't match */
			}
			else
			{
				status = 0;		/* Password end match */
			}


	/* Password end match */
	if(status == 0)
	{
		UART_sendByte(PASS_MATCH);			/* Send to HMI control Match */
		g_Passwrod_Status = PASS_MATCH;		/* Saves status in global variable to stop matching password again */

	}
	else
	{
		UART_sendByte(PASS_UNMATCH);
		g_Passwrod_Status = PASS_UNMATCH;
		--g_fail_count;										/* decrement fail trials if password didn't match */
		if(g_fail_count == 0)
		{
			buzzerOn();										/* Activates buzzer */
			while(UART_receiveByte() != TIME_60_SEC);		/* Waits 60 seconds */
			buzzerOff();									/* De-activates buzzer */
			UART_sendByte(CONTROL_ECU_READY);
			g_fail_count = MAX_FAIL_TRIALS;					/* reset max fail trials */
		}
	}
}

/*-------------------------------------------------------------------------------------------------------
 * [Description]:	Function that opens door with motor by rotating motor in clock-wise direction for
 * 					15 seconds, stops it for 3 seconds, then closes door by rotating anti-clock-wise
 * 					for another 15 seconds and finally stops the motor.
 *------------------------------------------------------------------------------------------------------*/
void openDoor()
{
	DcMotor_Rotate(CW);
	UART_sendByte(START_TIME_15_SEC);
	while(UART_receiveByte() != TIME_15_SEC);
	UART_sendByte(CONTROL_ECU_READY);
	DcMotor_Rotate(STOP);
	while(UART_receiveByte() != TIME_3_SEC);
	UART_sendByte(CONTROL_ECU_READY);
	DcMotor_Rotate(ACW);
	while(UART_receiveByte() != TIME_15_SEC);
	UART_sendByte(CONTROL_ECU_READY);
	DcMotor_Rotate(STOP);
	g_Passwrod_Status = PASS_UNMATCH;
}

/*-------------------------------------------------------------------------------------------------------
 * [Description]:	Function that saves the new password in array Pass1 as the new password is received
 * 					with UART from HMI ECU
 *------------------------------------------------------------------------------------------------------*/
void changePassword(uint8 *newPassword, uint8 *pass1)
{
	uint8 i = 0, flag_pass=0;
	while(flag_pass == 0)
	{
		newPassword[i] = UART_receiveByte();
		if(newPassword[i]>=0 && newPassword[i] <= 9)
		{
			pass1[i-1] = newPassword[i];
		}
		else if(newPassword[i] == '=' && i>0)
		{
			flag_pass = 1;
			pass1[i-1] = '\0';
			++i;
			break;
		}
		++i;
	}
	save_password(pass1);
	g_Passwrod_Status = PASS_UNMATCH;
}
