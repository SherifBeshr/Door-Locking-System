/******************************************************************************************************
File Name	: uart.c
Author		: Sherif Beshr
Description : Source file for the UART AVR driver
*******************************************************************************************************/


#include "uart.h"
#include "common_macros.h"
#include <avr/io.h>				/* To use the UART Registers */

/***************************************************************************************************
 *                                		Function Definitions                                  	   *
 ***************************************************************************************************/

/*
 * Description :
 * Functional responsible for Initializing the UART device by:
 * 1. Setup the Frame format like number of data bits, parity bit type and number of stop bits.
 * 2. Enable the UART.
 * 3. Setup the UART baud rate.
 */
void UART_init(const UART_ConfigType* Config_Ptr)
{
	/* U2X = 1 for double transmission speed */
	SET_BIT(UCSRA,U2X);

	/* RXEN = 1 for Receiver Enable */
	SET_BIT(UCSRB,RXEN);
	/* TXEN = 1 for Transmitter Enable */
	SET_BIT(UCSRB,TXEN);

	/* URSEL = 1 to write on UCSRC shared register*/
	SET_BIT(UCSRC,URSEL);
	UCSRC = (UCSRC & 0xCF) | ((Config_Ptr->ParityMode)<<UPM0);
	UCSRC = (UCSRC & 0xF7) | ((Config_Ptr->StopBits)<<USBS);
	UCSRC = (UCSRC & 0xF9) | ((Config_Ptr->DataBits)<<UCSZ0);

	/* URSEL = 0 to write on UBRRH shared register*/
	CLEAR_BIT(UBRRH,URSEL);
	/* Calculate the UBRR register value */
	uint16 Baudrate_value = (uint16)(((F_CPU/((Config_Ptr->BaudRate) * 8UL)))-1);
	/* First 8 bits from the BAUD_PRESCALE inside UBRRL and last 4 bits in UBRRH*/
	UBRRH = (Baudrate_value>>8);
	UBRRL = Baudrate_value;

}

/*
 * Description :
 * Functional responsible for send byte to another UART device.
 */
void UART_sendByte(const uint8 data)
{
	/*
	 * UDRE flag is set when the TX buffer (UDR) is empty and ready for
	 * transmitting a new byte so wait until this flag is set to one
	 */
	while (BIT_IS_CLEAR(UCSRA,UDRE)){}

	/*
	 * Put the required data in the UDR register and it also clear the UDRE flag as
	 * the UDR register is not empty now
	 */
	UDR = data;

	/************************* Another Method *************************
	UDR = data;
	while(BIT_IS_CLEAR(UCSRA,TXC)){} // Wait until the transmission is complete TXC = 1
	SET_BIT(UCSRA,TXC); // Clear the TXC flag
	*******************************************************************/
}

/*
 * Description :
 * Functional responsible for receive byte from another UART device.
 */
uint8 UART_receiveByte(void)
{
	/* RXC flag is set when the UART receive data so wait until this flag is set to one */
	while (BIT_IS_CLEAR(UCSRA,RXC)){}

	/*
	 * Read the received data from the RX buffer (UDR)
	 * The RXC flag will be cleared after read the data
	 */
	return UDR;
}

/*
 * Description :
 * Send the required string through UART to the other UART device.
 */
void UART_sendString(const uint8 *Str)
{
	uint8 i = 0;
	/* Send the whole string */
	while(Str[i] != '\0')
	{
		UART_sendByte(Str[i]);
		i++;
	}
	/************************* Another Method *************************
	while(*Str != '\0')
	{
		UART_sendByte(*Str);
		Str++;
	}
	*******************************************************************/
}

/*
 * Description :
 * Receive the required string until the '#' symbol through UART from the other UART device.
 */
void UART_receiveString(uint8 *Str)
{
	uint8 i = 0;

	/* Receive the first byte */
	Str[i] = UART_receiveByte();

	/* Receive the whole string until the '#' */
	while(Str[i] != '#')
	{
		i++;
		Str[i] = UART_receiveByte();
	}

	/* After receiving the whole string plus the '#', replace the '#' with '\0' */
	Str[i] = '\0';
}

