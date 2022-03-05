/******************************************************************************************************
File Name	: twi.h
Author		: Sherif Beshr
Description : Header file for the TWI(I2C) AVR driver
*******************************************************************************************************/

#ifndef TWI_H_
#define TWI_H_

#include "std_types.h"

/***************************************************************************************************
 *                                		Macro Definitions                                   	   *
 ***************************************************************************************************/

/* I2C Status Bits in the TWSR Register */
#define TWI_START         0x08 /* start has been sent */
#define TWI_REP_START     0x10 /* repeated start */
#define TWI_MT_SLA_W_ACK  0x18 /* Master transmit ( slave address + Write request ) to slave + ACK received from slave. */
#define TWI_MT_SLA_R_ACK  0x40 /* Master transmit ( slave address + Read request ) to slave + ACK received from slave. */
#define TWI_MT_DATA_ACK   0x28 /* Master transmit data and ACK has been received from Slave. */
#define TWI_MR_DATA_ACK   0x50 /* Master received data and send ACK to slave. */
#define TWI_MR_DATA_NACK  0x58 /* Master received data but doesn't send ACK to slave. */

/***************************************************************************************************
 *                                		Types Decelerations                                  	   *
 ***************************************************************************************************/

/*	TWI Pre-scalar	*/
typedef enum
{
	TWI_PRESCALAR_1 = 1, TWI_PRESCALAR_4 = 4, TWI_PRESCALAR_16 = 16, TWI_PRESCALAR_64 = 64
}TWI_Prescalar;

/*	Structure accessed to choose the TWI:
 * 	1- Frequency desired for TWI (in Hz)
 *  2- Address
 *  3- Pre-scalar value to determine the TWI clock speed
 */
typedef struct
{
	uint32			SCL_Frequency;
	uint8			Address;
	TWI_Prescalar 	TWI_Prescalar;
}TWI_ConfigType;

/***************************************************************************************************
 *                                		Function Prototypes                                 	   *
 ***************************************************************************************************/

/*
 * Description :
 * Initialize the TWI with configurable pre-scalar and address.
 */
void TWI_init(const TWI_ConfigType* Config_Ptr);

/*
 * Description :
 * Sends a TWI start bit.
 */
void TWI_start(void);

/*
 * Description :
 * Sends a TWI stop bit.
 */
void TWI_stop(void);

/*
 * Description :
 * Write a byte and receive an ACK from selected slave.
 */
void TWI_writeByte(uint8 data);

/*
 * Description :
 * Read a byte from slave and send ACK.
 */
uint8 TWI_readByteWithACK(void);

/*
 * Description :
 * Read a byte from slave and send NACK.
 */
uint8 TWI_readByteWithNACK(void);

/*
 * Description :
 * Read status.
 */
uint8 TWI_getStatus(void);


#endif /* TWI_H_ */
