/******************************************************************************************************
File Name	: twi.c
Author		: Sherif Beshr
Description : Source file for the TWI(I2C) AVR driver
 *******************************************************************************************************/


#include "twi.h"
#include "common_macros.h"
#include <avr/io.h>


/***************************************************************************************************
 *                                		Function Prototypes                                 	   *
 ***************************************************************************************************/

/*
 * Description :
 * Initialize the TWI with configurable SCL frequency, pre-scalar and address.
 */
void TWI_init(const TWI_ConfigType* Config_Ptr)
{
	/* Equation to calculate the Bit rate register from frequency and pre-scalar */
	TWBR = ((((F_CPU / Config_Ptr->SCL_Frequency) - 16) / (Config_Ptr->TWI_Prescalar)) / 2 );
	/* Set the configured pre-scalar*/
	uint8 twps_value;
	switch (Config_Ptr->TWI_Prescalar)
	{
	case (1):
		twps_value = 0;		break;
	case (4):
		twps_value = 1;		break;
	case (16):
		twps_value = 2;		break;
	case (64):
		twps_value = 3;		break;
	default :
		twps_value = 0;		break;
	}
	TWSR = (TWSR & 0xFC) | (twps_value<<TWPS0);

	/* Two Wire Bus address my address if any master device want to call me (used in case this MC is a slave device)
       General Call Recognition: Off */
	TWAR = (TWAR & 0x01) | ((Config_Ptr->Address)<<TWA0);
	/* Enable the TWI */
	TWCR = (1<<TWEN);
}

/*
 * Description :
 * Sends a TWI start bit.
 */
void TWI_start(void)
{
	/*
	 * Clear the TWINT flag before sending the start bit TWINT=1
	 * send the start bit by TWSTA=1
	 * Enable TWI Module TWEN=1
	 */
	TWCR = (1<TWINT) | (1<<TWSTA) | (1<<TWEN);

	/* Wait for TWINT flag set in TWCR Register (start bit is send successfully) */
	while(BIT_IS_CLEAR(TWCR,TWINT));
}

/*
 * Description :
 * Sends a TWI stop bit.
 */
void TWI_stop(void)
{
	/*
	 * Clear the TWINT flag before sending the stop bit TWINT=1
	 * send the stop bit by TWSTO=1
	 * Enable TWI Module TWEN=1
	 */
	TWCR = (1<TWINT) | (1<<TWSTO) | (1<<TWEN);
}

/*
 * Description :
 * Write a byte and receive an ACK from selected slave.
 */
void TWI_writeByte(uint8 data)
{
	/* Put data On TWI data Register */
	TWDR = data;

	/*
	 * Clear the TWINT flag before sending the data TWINT=1
	 * Enable TWI Module TWEN=1
	 */
	TWCR = (1 << TWINT) | (1 << TWEN);
	/* Wait for TWINT flag set in TWCR Register(data is send successfully) */
	while(BIT_IS_CLEAR(TWCR,TWINT));
}

/*
 * Description :
 * Read a byte from slave and send ACK.
 */
uint8 TWI_readByteWithACK(void)
{
	/*
	 * Clear the TWINT flag before reading the data TWINT=1
	 * Enable sending ACK after reading or receiving data TWEA=1
	 * Enable TWI Module TWEN=1
	 */
	TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWEA);
	/* Wait for TWINT flag set in TWCR Register (data received successfully) */
	while(BIT_IS_CLEAR(TWCR,TWINT));
	/* Read Data */
	return TWDR;
}

/*
 * Description :
 * Read a byte from slave and send NACK.
 */
uint8 TWI_readByteWithNACK(void)
{
	/*
	 * Clear the TWINT flag before reading the data TWINT=1
	 * Enable TWI Module TWEN=1
	 */
	TWCR = (1 << TWINT) | (1 << TWEN);
	/* Wait for TWINT flag set in TWCR Register (data received successfully) */
	while(BIT_IS_CLEAR(TWCR,TWINT));
	/* Read Data */
	return TWDR;
}

/*
 * Description :
 * Read status.
 */
uint8 TWI_getStatus(void)
{
	uint8 status;
	/* masking to eliminate first 3 bits and get the last 5 bits (status bits) */
	status = (TWSR & 0xF8);
	return status;
}
