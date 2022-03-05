/******************************************************************************************************
File Name	: dc_motor.h
Author		: Sherif Beshr
Description : header file for the DC Motor driver
*******************************************************************************************************/

#ifndef DC_MOTOR_H_
#define DC_MOTOR_H_

#include "std_types.h"

/***************************************************************************************************
 *                                		Definitions                                  			   *
 ***************************************************************************************************/

#define DC_MOTOR_PORT_ID			PORTD_ID
#define DC_MOTOR_PIN1_ID			PIN6_ID
#define DC_MOTOR_PIN2_ID			PIN7_ID

/***************************************************************************************************
 *                                		Types Declaration                                  	   	   *
 ***************************************************************************************************/

typedef enum {
	CW, ACW, STOP
}DcMotor_State;

/***************************************************************************************************
 *                                		Functions Prototypes                                  	   *
 ***************************************************************************************************/

/*
 * Description :
 * Initialize the DC Motor:
 * 1. Setup the DC Motor pins directions by using the GPIO driver.
 * 2. Initialize the DC Motor to STOP.
 */
void DcMotor_Init(void);

/*
 * Description :
 * Set the DC Motor State and speed:
 * 1. Set the Motor State ( STOP --> PIN1=0 & PIN2=0  ,  CW --> PIN1=0 & PIN2=1 ,  ACW --> PIN1=1 & PIN2=0 )
 * 2. Set the Motor Speed by passing duty cycle to the PWM function.
 * Input: State ( CW or ACW or STOP ) , Speed = duty cycle
 */
void DcMotor_Rotate(DcMotor_State state);


#endif /* DC_MOTOR_H_ */
