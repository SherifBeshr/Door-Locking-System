/******************************************************************************************************
File Name	: external_eeprom.h
Author		: Sherif Beshr
Description : Header file for the External EEPROM Memory
*******************************************************************************************************/

#ifndef EXTERNAL_EEPROM_H_
#define EXTERNAL_EEPROM_H_

#include "std_types.h"

/***************************************************************************************************
 *                                		Macro Definitions                                   	   *
 ***************************************************************************************************/
#define ERROR 			0
#define SUCCESS 		1


/***************************************************************************************************
 *                                		Function Prototypes                                 	   *
 ***************************************************************************************************/

/*
 * Description :
 * Function to write a byte on EEPROM from Address xx.
 */
uint8 EEPROM_writeByte(uint16 u16addr,uint8 u8data);

/*
 * Description :
 * Function to read a byte from EEPROM from Address xx.
 */
uint8 EEPROM_readByte(uint16 u16addr,uint8 *u8data);


#endif /* EXTERNAL_EEPROM_H_ */
