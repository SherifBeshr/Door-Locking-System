/******************************************************************************************************
File Name	: uart.h
Author		: Sherif Beshr
Description : Header file for the UART AVR driver
*******************************************************************************************************/

#ifndef UART_H_
#define UART_H_

#include "std_types.h"

/***************************************************************************************************
 *                                		Types Decelerations                                  	   *
 ***************************************************************************************************/

/*	Baud rate that UART will work on	*/
typedef enum
{
	Baud_2400=2400, 	Baud_4800=4800, 	Baud_9600=9600, 	Baud_14400=14400, 	Baud_19200=19200,\
	Baud_28800=28800,	Baud_38400=38400, 	Baud_57600=57600, 	Baud_76800=76800,	Baud_115200=115200,\
	Baud_230400=230400, Baud_250k=250000,	Baud_500k=500000,	Baud_1M=1000000
}UART_BaudRate;

/*	Number of bits to send	*/
typedef enum
{
	Data_5, Data_6, Data_7, Data_8
}UART_DataBits;

/*	Parity checking mode that check for number of '1' in frame */
typedef enum
{
	Parity_Disable, Parity_Even=2, Parity_Odd
}UART_ParityMode;

/*	Number of stop bits sent at the end of the frame	*/
typedef enum
{
	StopBits_1, StopBits_2
}UART_StopBits;

/*	Structure accessed to choose the UART different modes selecting
 *  1- Baud Rate that is the speed of transfer
 *  2- Data bits sent each time
 *  3- Parity mode to be disabled/enabled for checking data is sent correctly
 *  4- Stop bits at the end of each frame ( 1-bit or 2-bits )
 */
typedef struct
{
	UART_BaudRate 	BaudRate;
	UART_DataBits 	DataBits;
	UART_ParityMode ParityMode;
	UART_StopBits	StopBits;
}UART_ConfigType;

/***************************************************************************************************
 *                                		Function Prototypes                                  	   *
 ***************************************************************************************************/

/*
 * Description :
 * Functional responsible for Initializing the UART device by:
 * 1. Setup the Frame format like number of data bits, parity bit type and number of stop bits.
 * 2. Enable the UART.
 * 3. Setup the UART baud rate.
 */
void UART_init(const UART_ConfigType* Config_Ptr);

/*
 * Description :
 * Functional responsible for send byte to another UART device.
 */
void UART_sendByte(const uint8 data);

/*
 * Description :
 * Functional responsible for receive byte from another UART device.
 */
uint8 UART_receiveByte(void);

/*
 * Description :
 * Send the required string through UART to the other UART device.
 */
void UART_sendString(const uint8 *Str);

/*
 * Description :
 * Receive the required string until the '#' symbol through UART from the other UART device.
 */
void UART_receiveString(uint8 *Str); // Receive until #


#endif /* UART_H_ */
